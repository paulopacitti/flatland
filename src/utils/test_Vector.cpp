#include "./Vector.hpp"
#include <cstddef>
#include <iostream>

void printVector(Vector<float> &v) {
  for (size_t i = 0; i < v.getSize(); i++) {
    std::cout << "items[" << i << "] = " << v[i] << std::endl;
  }
}

int main() {
  Vector<float> v;
  v.pushBack(1);
  v.pushBack(2);
  v.pushBack(3);
  v.pushBack(10);
  v.pushBack(25);
  v.pushBack(50);
  v.pushBack(75);
  v.pushBack(80);
  v.pushBack(90);
  v.pushBack(97);
  v.pushBack(98);
  v.pushBack(99);
  printVector(v);

  float pop = v.popBack();
  std::cout << "pop = " << pop << std::endl;

  v.erase(1);
  printVector(v);

  // Vector<float> w;
  // v = w;

  std::cout << "size = " << v.getSize() << ", capacity = " << v.getCapacity()
            << std::endl;

  v.insert(123, 102);
  printVector(v);
  return 0;
}