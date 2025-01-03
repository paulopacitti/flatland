#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

const size_t DEFAULT_CAPACITY = 10;
template <typename T> class Vector {
private:
  size_t m_size;
  size_t m_capacity;
  T *m_elements;

public:
  Vector() {
    m_size = 0;
    m_capacity = DEFAULT_CAPACITY;
    m_elements = new T[DEFAULT_CAPACITY];
  }
  Vector(size_t n, const T &value) {
    m_size = n;
    m_capacity = n * 2;
    m_elements = new T[m_capacity];
    for (size_t i = 0; i < m_size; i++) {
      m_elements[i] = value;
    }
  }
  Vector(const Vector &v) {
    m_size = v.getSize();
    m_capacity = v.getCapacity();
    m_elements = new T[m_capacity];
    for (size_t i = 0; i < m_size; i++) {
      m_elements[i] = v[i];
    }
  }
  ~Vector() { delete[] m_elements; }

  /**
   * Returns the size of the vector.
   */
  size_t getSize() const { return m_size; }
  /**
   * Returns the capacity of the vector.
   */
  size_t getCapacity() const { return m_capacity; }

  /**
   * Adds a new object to the end of the vector.
   */
  void pushBack(const T &object) {
    if (m_size == m_capacity) {
      m_capacity *= 2;
      T *newArray = new T[m_capacity];
      for (size_t i = 0; i < m_size; i++) {
        newArray[i] = m_elements[i];
      }
      delete[] m_elements;
      m_elements = newArray;
    }
    m_elements[m_size] = object;
    m_size++;
  }

  /**
   * Removes the last element from the vector and returns it.
   */
  T popBack() {
    if (m_size > 0) {
      T pop = m_elements[m_size - 1];
      m_size--;

      return pop;
    } else {
      std::cerr << "Error: vector is empty, it's not possible to pop elements, "
                   "popBack() failed."
                << std::endl;
      exit(1);
    }
  }

  /**
   * Inserts a value at the specified index in the vector. If the index is
   * greater than the current size of the vector or less than 0, an
   * `std::out_of_range` exception is thrown.
   */
  void insert(size_t index, T value) {
    if (index > m_size) {
      throw std::out_of_range("Insert index out of bounds");
    }
    if (m_size == m_capacity) {
      m_capacity *= 2;
      T *newArray = new T[m_capacity];
      for (size_t i = 0; i < m_size; i++) {
        newArray[i] = m_elements[i];
      }
      delete[] m_elements;
      m_elements = newArray;
    }

    for (size_t i = m_size; i > index; i--) {
      m_elements[i] = m_elements[i - 1];
    }

    m_elements[index] = value;
    m_size++;
  }

  /**
   * Removes the element at the specified index from the vector. This function
   * shifts all elements after the specified index one position to the left,
   * effectively erasing the element at the given index. If the index is greater
   * than the current size of the vector or less than 0, an
   * `std::out_of_range` exception is thrown.
   */
  void erase(size_t index) {
    if (index > m_size) {
      throw std::out_of_range("Insert index out of bounds");
    }
    for (size_t i = index; i < m_size - 1; i++) {
      m_elements[i] = m_elements[i + 1];
    }
    m_size--;
  }

  /**
   * Clears the vector by setting its size to zero.
   */
  void clear() { m_size = 0; }

  T &operator[](size_t index) {
    if (index >= m_size) {
      throw std::out_of_range("Index out of bounds");
    }
    return m_elements[index];
  }

  const T &operator[](size_t index) const {
    if (index >= m_size) {
      throw std::out_of_range("Index out of bounds");
    }
    return m_elements[index];
  }

  Vector &operator=(const Vector &v) {
    delete[] m_elements;
    m_capacity = v.getCapacity();
    m_elements = new T[m_capacity];

    for (size_t i = 0; i < v.getSize(); i++) {
      m_elements[i] = v[i];
    }
    m_size = v.getSize();

    return *this;
  }
};

#endif
