/*
 * https://devblogs.microsoft.com/oldnewthing/20191107-00/?p=103071
 * https://devblogs.microsoft.com/oldnewthing/20191106-00/?p=103066
 */
#include <iostream>

void lambda1() {
  auto lambda = [](int v) { return v + 2; };
  std::cout << lambda(100) << ", " << lambda(1000) << std::endl;
}

void lambda2() {
  auto lambda = [](auto p)
  {
    if constexpr (std::is_same_v<decltype(p), int>) {
     return p + 1;
    } else {
     return "oops";
    }
  };

  auto result1 = lambda(123); // result1 is 124
  auto result2 = lambda('x'); // result2 is "oops"
  std::cout << result1 << ", " << result2 << std::endl;
}

void lambda3() {
  struct add_tax_t {};
  constexpr add_tax_t add_tax;

  struct apply_discount_t {};
  constexpr apply_discount_t apply_discount;

  auto total = 0.0;
  auto lambda = [total](auto op, auto value) mutable
  {
    using Op = decltype(op);
    if constexpr (std::is_same_v<Op, add_tax_t>) {
     total += total * value; // value is the tax rate
     return total;
    } else if constexpr (std::is_same_v<Op, apply_discount_t>) {
     total -= std::max(value, total); // value is the discount
     return total;
    } else {
     static_assert(!sizeof(Op*), "Don't know what you are asking me to do.");
    }
  };

  auto result1 = lambda(apply_discount, 5.00); // apply $5 discount
  auto result2 = lambda(add_tax, 0.10); // add 10% tax
  std::cout << result1 << ", " << result2 << std::endl;
}

void lambda4() {
  auto total =  0.0;
  auto lambda = [total](auto... args) mutable
  {
    using ArgsT = std::tuple<decltype(args)...>;
    if constexpr (std::is_same_v<ArgsT, std::tuple<int, int>>) {
     // two integers = add to total
     auto [a, b] = ArgsT(args...);
     total += a + b;
    } else if constexpr (std::is_same_v<ArgsT, std::tuple<>>) {
     // no parameters = print
     std::cout << total << std::endl;
    } else {
     static_assert(!sizeof(ArgsT*), "Don't know what you are asking me to do.");
    }
  };

  lambda(100, 1);
  // Print the total variable in outer scope: 0.
  std::cout << total << std::endl;
  // Print the total variable in inner scope: 101.
  lambda();
}

int main(void) {
  lambda1();
  lambda2();
  lambda3();
  lambda4();

  return 0;
}
