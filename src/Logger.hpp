#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>

class Logger {
public:
  static void info(const std::string &message);
  static void error(const std::string &message);
};

#endif