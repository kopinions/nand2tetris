#ifndef REPORTER_HPP
#define REPORTER_HPP
#include <iostream>

class reporter {
public:
  virtual void report(std::string) = 0;
  virtual ~reporter() = default;
};

class file_reporter : public reporter {
public:
  virtual void report(std::string message){

  };
};
#endif // REPORTER_HPP
