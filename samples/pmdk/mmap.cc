// A C style example from man page, with some C++ style modifications.
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>


#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    goto error_exit;                                                           \
  } while (0)

int main(int argc, char *argv[]) {
  char *addr = nullptr;
  int fd;
  struct stat sb;
  off_t offset, pa_offset;
  size_t length;
  size_t mapped_length = 0;
  int rc = EXIT_FAILURE;

  if (argc < 3 || argc > 4) {
    fprintf(stderr, "%s file offset [length]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  fd = open(argv[1], O_RDWR);
  if (fd == -1) {
    handle_error("open");
  }

  /* To obtain file size */
  if (fstat(fd, &sb) == -1) {
    handle_error("fstat");
  }

  offset = atoi(argv[2]);
  /* offset for mmap() must be page aligned */
  pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
  assert(pa_offset <= offset);

  if (argc == 4) {
    length = atoi(argv[3]);
  } else {
    if (sb.st_size <= offset) {
      fprintf(stderr, "offset: %lu is past end of file: %lu\n", offset,
            sb.st_size);
      goto error_exit;
    }

    length = sb.st_size - offset;
  }

  if (offset + length > (size_t)sb.st_size) {
    fprintf(stderr, "offset: %lu is past end of file: %lu\n", offset,
            sb.st_size);
  }

  mapped_length = length + offset - pa_offset;
  addr = (char *)mmap(nullptr, mapped_length, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, pa_offset);
  if (addr == MAP_FAILED) {
    handle_error("mmap");
  }

  {
    char data[] = "-A";
    // Update the last byte to 'A'.
    if (pa_offset < (off_t) sb.st_size) {
      size_t off = sb.st_size - pa_offset - 1;
      data[0] = *(addr + off);
      if (isupper(data[0])) {
        data[0] = tolower(data[0]);
      } else if (islower(data[0])) {
        data[0] = toupper(data[0]);
      } else {
        data[0] = data[1];
      }

      memcpy(addr + sb.st_size - pa_offset - 1, data, 1);
    }
  }

  if (msync(addr, mapped_length, MS_SYNC) < 0) {
    handle_error("msync");
  }

  if (offset + length > (size_t) sb.st_size) {
    // Oops, file size is smaller than requested.
    // fallocate it and re-map it.
    if (fallocate(fd, 0, sb.st_size, offset + length - sb.st_size) < 0) {
     handle_error("fallocate");
    }

    size_t new_offset = sb.st_size;
    size_t new_pa_offset = new_offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
    size_t new_mapped_length = mapped_length - (new_pa_offset - pa_offset);
    char *new_addr = (char *) mmap(addr + new_pa_offset - pa_offset, new_mapped_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, new_pa_offset);
    if (new_addr == MAP_FAILED) {
      handle_error("mmap again");
    }

    char data[] = "Z";
    memcpy(new_addr + new_mapped_length - 1, data, 1);

    if (msync(new_addr, new_mapped_length, MS_SYNC) < 0) {
      handle_error("msync again");
    }
  }

  rc = EXIT_SUCCESS;

error_exit:

  fprintf(stderr, "rc: %d\n", rc);

  if ((addr != nullptr) && munmap(addr, mapped_length) < 0) {
    perror("munmap");
    exit(rc);
  }

  if (fd != -1) {
    close(fd);
  }

  exit(rc);
}
