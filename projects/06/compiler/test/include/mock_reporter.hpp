#ifndef MOCK_REPORTER_HPP
#define MOCK_REPORTER_HPP

#include "reporter.hpp"
#include "gmock/gmock.h"
#include <string>

class mock_reporter : public reporter {
public:
  MOCK_METHOD(void, report, (std::string), (override));
};

#endif // MOCK_REPORTER_HPP
