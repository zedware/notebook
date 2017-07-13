/**
 * Performance tests.
 *
 * $ make -f Makefile perf_example && rm -rf /tmp/rocksdb_perf_example && ./perf_example 1000 0
 *
 * 1. Reduce the cost of read line and parse line.
 * 2. Use batch write.
 * 3. Turn off concurrent write may helps a little?
 * 4. Insert hint doesn't help?
 */
#include <cassert>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
#include "rocksdb/slice_transform.h"

using namespace rocksdb;
using namespace std;

std::string kDBPath = "/tmp/rocksdb_perf_example";
int batch_size = 0;
bool with_hint = false;

class TestPrefixExtractor : public SliceTransform {
 public:
  virtual const char* Name() const override { return "TestPrefixExtractor"; }

  virtual Slice Transform(const Slice& key) const override {
    const char* p = separator(key);
    if (p == nullptr) {
      assert (false);
      return Slice();
    }
    return Slice(key.data(), p - key.data() + 1);
  }

  virtual bool InDomain(const Slice& key) const override {
    return separator(key) != nullptr;
  }

  virtual bool InRange(const Slice& key) const override { return false; }

 private:
  const char* separator(const Slice& key) const {
    return reinterpret_cast<const char*>(key.data() + sizeof(int));
      }
};

#define check(s) {\
  if (!s.ok()) {\
    cout << s.ToString() << endl;\
  }\
}

void testPutIntIntKey() {
  ifstream file("./lineitem-scale1g.tbl");
  if (!file.is_open()) {
    cout << "Open failed." << endl;
  }

  DB *db;
  Options options;
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();
  // create the DB if it's not already present
  options.create_if_missing = true;
  // 
  if (with_hint) {
    options.memtable_insert_with_hint_prefix_extractor.reset(new TestPrefixExtractor());
    options.allow_concurrent_memtable_write = false;
  }

  // open DB
  Status s = DB::Open(options, kDBPath, &db);
  check(s);

  WriteOptions write_options;

  char key[sizeof(int) * 2];
  char value[sizeof(int)];

  struct timeval start;
  gettimeofday(&start, nullptr);

  int row = 0;
  string line;
  char delim = '|';
  WriteBatch *batch = new WriteBatch();
  while (file.good()) {

    getline(file, line);

    string next;
    int column = 0;
    bool found = false;
    int k1, k2;
    for (string::const_iterator it = line.begin(); it != line.end(); it++) {
      if (*it == delim) {
        if (column == 0) {
          k1 = atoi(next.c_str());
        } else if (column == 3) {
          k2 = atoi(next.c_str());
          found = true;
        }
        ++column;
        next.clear();
      } else {
        next += *it;
      }
    }

    // cout << row << "[0]" << result[0].c_str() << endl;
    // cout << row << "[3]" << result[3].c_str() << endl;

    if (found) {
      memcpy(key, &k1, sizeof(int));
      memcpy(key + sizeof(int), &k2, sizeof(int));

      row++;
      memcpy(value, &row, sizeof(int));

      if (batch_size == 0) {
        s = db->Put(write_options, key, value);
      } else {
        if (row % batch_size == 0) {
          batch->Put(key, value);
          s = db->Write(write_options, batch);
          batch = new WriteBatch();
        }
      }
      check(s);
    }
  }

  if (batch_size > 0 && row % batch_size != 0) {
    s = db->Write(write_options, batch);
    check(s);
  }

  struct timeval stop;
  gettimeofday(&stop, nullptr);
  long ms = (stop.tv_sec * 1000 * 1000 + stop.tv_usec
      - (start.tv_sec * 1000 * 1000 + start.tv_usec)) / 1000 + 1;
  long seconds = ms / 1000;

  string stats;
  db->GetProperty("rocksdb.stats", &stats);
  cout << stats << endl;

  cout << "Total time=>" << ms << "ms";
  if (seconds > 0) {
    cout << ", " << row / seconds << " rps.";
  }
  cout << endl;

  delete db;
  file.close();
}

void usage() {
  cout << "Usage: ./perf_example <batchSize> <insertHint>" << endl;
  cout << endl;
  exit(1);
}

// TODO: gflags or getopt
int main(int argc, char *argv[]) {
  if (argc != 3) {
    usage();
  }

  batch_size = atoi(argv[1]);
  if (batch_size < 0 || batch_size > 1000 * 1000 * 1000) {
    cout << "invalid batch size:" << batch_size << endl;
    exit(1);
  }

  int insert_hint = atoi(argv[2]);
  if (insert_hint == 1 || insert_hint == 0) {
    with_hint = insert_hint == 1 ? true : false;
  } else {
    cout << "invalid insert hint:" << insert_hint << endl;
    exit(1);
  }

  testPutIntIntKey();
  return 0;
}
