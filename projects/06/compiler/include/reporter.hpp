#ifndef REPORTER_HPP
#define REPORTER_HPP
#include <iostream>

class reporter {
  void report(std::string message) { std::cout << message << std::endl; }
};
#endif // REPORTER_HPP
