#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <list>
#include <map>
#include <optional>
#include <string>

class context {
public:
  context() : _symbols(){};
  virtual ~context() = default;
  std::optional<int> defined(std::string name) {
    try {
      return std::make_optional<int>(_symbols.at(name));
    } catch (std::exception ignored) {
      return std::nullopt;
    }
  };
  virtual void define(std::string name, int location = -1) { _symbols[name] = location; }

private:
  std::map<std::string, int> _symbols;
};
#endif // CONTEXT_HPP
