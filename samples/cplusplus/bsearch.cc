//
// Improve the performance of binary search.
// 1. optimize for the unmatched search.
// 2. avoid duplicated compares.
// 3. avoid branch prediction.
//
// See http://databasearchitects.blogspot.de/2015/09/trying-to-speed-up-binary-search.html
// for more interesting variants.
#include <sys/time.h>
#include <cassert>
#include <iostream>

typedef int64_t Value;

inline int64_t get_usec(struct timeval v) {
  return ((int64_t) v.tv_sec * 1000 * 1000 + v.tv_usec);
}
  
void generate(Value *&data, int64_t n) {
  assert(data == nullptr);
  assert(n > 0);

  data = new Value[n];
  assert(data != nullptr);

  for (int64_t i = 0; i < n; i++) {
    data[i] = Value(i);
  }
}

void sort(Value *data, int64_t n) {
  assert(data != nullptr);
  assert(n > 0);

  // qsort(data, n, sizeof(Value), nullptr);
  for (int64_t i = 0; i < n - 1; i++) {
    assert(data[i] <= data[i + 1]);
  }
}

inline int64_t bsearch_basic(Value *data, int64_t n, Value v) {
  assert(data != nullptr);
  assert(n > 0);
  int64_t low = 0, high = n;
  int64_t mid;
  while (low < high) {
    mid = (low + high ) / 2;
    if (v < data[mid]) {
      high = mid;
    } else if (v > data[mid]) {
      low = mid + 1;
    } else {
      return mid;
    }
  }

  return -1;
}

inline int64_t bsearch_quickcmp(Value *data, int64_t n, Value v) {
  assert(data != nullptr);
  assert(n > 0);
  int64_t low = 0, high = n;
  int64_t mid;
  while (low < high) {
    mid = (low + high ) / 2;
    int64_t cmp = v - data[mid];
    if (cmp < 0) {
      high = mid;
    } else if (cmp > 0) {
      low = mid + 1;
    } else {
      return mid;
    }
  }

  return -1;
}

typedef int64_t (*func)(Value *, int64_t, Value);

inline Value get_min(Value *data, int64_t n) {
  return data[0];
}

inline Value get_max(Value *data, int64_t n) {
  return data[n - 1];
}

void search(Value *data, int64_t n, func ptr) {
  assert(data != nullptr);
  assert(n > 0);
  assert(ptr != nullptr);

  struct timeval start, end;
  gettimeofday(&start, nullptr);

  for (int64_t i = 0; i < n; i++) {
    Value v = i;
    // fast path for unmatched search
    if (v < get_min(data, n) || v > get_max(data, n)) {
      ;
    } else {
      int64_t pos = ptr(data, n, v);

#if 1
      if (pos >= 0 && pos < n) {
        if (data[pos] != v) {
          std::cout << "found=yes, pos: " << pos << " v: " << v << " data[pos]: " << data[pos] << std::endl;
          assert(false);
        }
      } else {
        std::cout << "found=no, pos: " << pos << " v: " << v << std::endl;
        assert(false);
      }
#endif // 0
    }
  }

  gettimeofday(&end, nullptr);
  int64_t usec = get_usec(end) - get_usec(start);
  std::cout << usec / 1000 << " ms, and " << usec % 1000 << " micro seconds" << std::endl;
}

void run(Value *data, int64_t n, func ptr) {
  generate(data, n);
  sort(data, n);
  search(data, n, ptr);
}

int main(void) {
  Value *data = nullptr;
  run(data, 1024, bsearch_basic);
  run(data, 1024 * 1024, bsearch_basic);
  //
  run(data, 1024, bsearch_quickcmp);
  run(data, 1024 * 1024, bsearch_quickcmp);
  return 0;
}
