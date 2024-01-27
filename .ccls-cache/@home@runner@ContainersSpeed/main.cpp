#include <algorithm>
#include <chrono>
#include <deque>
#include <iostream>
#include <list>
#include <unordered_set>
#include <vector>

using namespace std::chrono;

template <typename Func> double measureTimeNanos(Func func, int iterations) {
  double totalTime = 0;
  for (int i = 0; i < iterations; ++i) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    totalTime += duration_cast<nanoseconds>(end - start).count();
  }
  return totalTime / iterations;
}

int main() {
  const int size = 100000;
  const int iterations = 10000; // количество повторов для каждого теста
  std::vector<int> vectorTest(size, 42);
  std::deque<int> dequeTest(size, 42);
  std::list<int> listTest(size, 42);
  std::unordered_set<int> unorderedSetTest;

  // Тестирование доступа по индексу
  auto vectorAccessTime = measureTimeNanos(
      [&]() {
        volatile int dummy = 0; // Избегаем оптимизации
        for (int i = 0; i < size; ++i) {
          dummy += vectorTest[i];
        }
      },
      iterations);

  auto dequeAccessTime = measureTimeNanos(
      [&]() {
        volatile int dummy = 0; // Избегаем оптимизации
        for (int i = 0; i < size; ++i) {
          dummy += dequeTest[i];
        }
      },
      iterations);

  // Тестирование добавления элементов в начало
  auto vectorInsertTime = measureTimeNanos(
      [&]() {
        for (int i = 0; i < 1000; ++i) {
          vectorTest.insert(vectorTest.begin(), i);
        }
      },
      iterations);

  auto dequeInsertTime = measureTimeNanos(
      [&]() {
        for (int i = 0; i < 1000; ++i) {
          dequeTest.push_front(i);
        }
      },
      iterations);

  auto listInsertTime = measureTimeNanos(
      [&]() {
        for (int i = 0; i < 1000; ++i) {
          listTest.push_front(i);
        }
      },
      iterations);

  // Тестирование поиска элемента
  for (int i = 0; i < size; ++i) {
    unorderedSetTest.insert(i);
  }

  auto vectorSearchTime = measureTimeNanos(
      [&]() {
        for (int i = 0; i < size; ++i) {
          std::find(vectorTest.begin(), vectorTest.end(), i);
        }
      },
      iterations);

  auto unorderedSetSearchTime = measureTimeNanos(
      [&]() {
        for (int i = 0; i < size; ++i) {
          unorderedSetTest.find(i);
        }
      },
      iterations);

  std::cout << "Access Time (ns): \n"
            << "Vector: " << vectorAccessTime << "\n"
            << "Deque: " << dequeAccessTime << "\n\n";

  std::cout << "Insert Time at Beginning (ns): \n"
            << "Vector: " << vectorInsertTime << "\n"
            << "Deque: " << dequeInsertTime << "\n"
            << "List: " << listInsertTime << "\n\n";

  std::cout << "Search Time (ns): \n"
            << "Vector: " << vectorSearchTime << "\n"
            << "Unordered Set: " << unorderedSetSearchTime << "\n";

  return 0;
}
