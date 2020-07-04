#ifndef MOCK_REPORTER_HPP
#define MOCK_REPORTER_HPP

#include "reporter.hpp"
#include "gmock/gmock.h"
#include <string>

class mock_context : public context {
public:
  MOCK_METHOD(void, add, (std::string));
};

#endif // MOCK_REPORTER_HPP
