#pragma once
#include "helper.h"
#include <concepts>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

void mainWork();
void startWork(std::vector<int> &data);
void elseWork(int rank);
std::vector<std::ifstream> openFilesIn(const std::string &folder_name);

template <typename T>
requires(std::is_trivially_copyable_v<T>) std::vector<T> sortUnity(
    const std::vector<T> &data1, const std::vector<T> &data2) {
  auto iter1 = data1.cbegin(), iter2 = data2.cbegin();
  std::vector<T> out(data1.size() + data2.size());
  for (auto iter = out.begin(); iter != out.end(); ++iter) {
    *iter = *iter1 < *iter2 ? *iter1++ : *iter2++;
    std::cout << "C\n";
    if (iter1 == data1.cend()) {
      std::cout << "A\n";
      std::copy(iter2, data2.cend(), iter);
      break;
    }
    if (iter2 == data2.cend()) {
      std::cout << "B\n";
      std::copy(iter1, data2.cend(), iter);
      break;
    }
  }
  for (auto &elem : data)
    std::cout << elem << "\t";
  std::cout << "\n";
}