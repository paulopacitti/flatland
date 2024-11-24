/*
 * toy logger implementation using std::cout. Not fast enough ot be used in the
 * engine
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include <vector>

enum LogType { INFO, WARNING, ERROR };

struct LogEntry {
  LogType type;
  std::string message;
};

class Logger {
public:
  static std::vector<LogEntry> m_messages;

  static void info(const std::string &message);
  static void warning(const std::string &message);
  static void error(const std::string &message);
};

#endif