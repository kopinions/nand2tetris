#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <list>
#include <map>
#include <optional>
#include <string>
#include <queue>

class context {
public:
  context() : _symbols(), _variable_it(16), _freed_variable_location(){};
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
      if (_freed_variable_location.empty()) {
        _symbols[name] = ++_variable_it;
      } else {
        _symbols[name] = _freed_variable_location.front();
        _freed_variable_location.pop();
      }
    } else {
      if (_symbols.find(name) != _symbols.end()) {
        _freed_variable_location.push(_symbols[name]);
      }
      _symbols[name] = location;
    }
  }

private:
  std::map<std::string, int> _symbols;
  int _variable_it;
  std::queue<int> _freed_variable_location;
};
#endif // CONTEXT_HPP
