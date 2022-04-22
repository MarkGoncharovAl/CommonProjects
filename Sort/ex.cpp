#include <iostream>

int main() {
  int mas[5] = {0, 1, 2, 3, 4};
  int *ptr = mas;
  std::cout << *ptr++;
  std::cout << *ptr++;
}