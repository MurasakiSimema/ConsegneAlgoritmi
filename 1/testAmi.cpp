#include <iostream>

int main() {
  long num1 = 58973;
  long num2 = 6097;

  long num1_2 = num1 * 100000 + num2;

  std::cout << num1_2 << std::endl;

  int newNum1 = num1_2 / 100000;
  int newNum2 = num1_2 % 100000;

  std::cout << newNum1 << " " << newNum2 << std::endl;
}