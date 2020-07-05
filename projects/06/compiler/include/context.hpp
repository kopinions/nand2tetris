#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <string>

class context {
public:
  context() : _symbols(), _variable_it(16), _freed_variable_location(), _preserved() {
    for (auto i = 0; i < 16; i++) {
      _symbols["R" + std::to_string(i)] = i;
    }
    _symbols["SCREEN"] = 16384;
    _symbols["KBD"] = 24576;
    _symbols["SP"] = 0;
    _symbols["LCL"] = 1;
    _symbols["ARG"] = 2;
    _symbols["THIS"] = 3;
    _symbols["THAT"] = 4;

    for (auto i = 0; i < 16; i++) {
      _preserved.push_back("R" + std::to_string(i));
    }
    _preserved.push_back("SCREEN");
    _preserved.push_back("KBD");
    _preserved.push_back("SP");
    _preserved.push_back("LCL");
    _preserved.push_back("ARG");
    _preserved.push_back("THIS");
    _preserved.push_back("THAT");
  };
  virtual ~context() = default;
  std::optional<int> defined(std::string name) {
    try {
      return std::make_optional<int>(_symbols.at(name));
    } catch (std::exception ignored) {
      return std::nullopt;
    }
  };
  virtual void define(std::string name, int location = -1) {
    if (std::find(_preserved.begin(), _preserved.end(), name) != _preserved.end()) {
      return;
    }

    if (location == -1) {
      if (_freed_variable_location.empty()) {
        _symbols[name] = _variable_it++;
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
  std::vector<std::string> _preserved;
};

#endif // CONTEXT_HPP
