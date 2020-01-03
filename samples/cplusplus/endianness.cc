// c++20
// https://en.cppreference.com/w/cpp/header/bit
// $ ./a.out
// little: 1234
// big: 4321
// native: 1234
//
#include <bit>
#include <iostream>

int main(void) {
  std::cout << "little: " << (int) std::endian::little << std::endl;
  std::cout << "big: " << (int) std::endian::big << std::endl;
  std::cout << "native: " << (int) std::endian::native << std::endl;
  return 0;
}

