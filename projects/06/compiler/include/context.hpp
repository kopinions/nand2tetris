#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <list>
#include <map>
#include <optional>
#include <string>

class context {
public:
  context() : _symbols(), _variable_it(16){};
  virtual ~context() = default;
  std::optional<int> defined(std::string name) {
    try {
      return std::make_optional<int>(_symbols.at(name));
    } catch (std::exception ignored) {
      return std::nullopt;
    }
  };
  virtual void define(std::string name, int location = -1) {
    if (location == -1) {
      _symbols[name] = ++_variable_it;
    } else {
      _symbols[name] = location;
    }
  }

private:
  std::map<std::string, int> _symbols;
  int _variable_it;
};
#endif // CONTEXT_HPP
