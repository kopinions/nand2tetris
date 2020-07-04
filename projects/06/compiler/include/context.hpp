#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <list>
#include <optional>
#include <string>

class symbol {
public:
  symbol(std::string name, int address) : _name(name), _address(address) {}

private:
  std::string _name;
  int _address;
};
class context {
public:
  context() : _symbols() { _symbols.push_back(symbol("R1", 1)); };
  virtual ~context() = default;
  std::optional<symbol> find(std::string name) { return std::nullopt; };
  virtual void add(std::string name) {}
  virtual void add(std::string name, int address) {}

  virtual void update(std::string name, int address){

  };

private:
  std::list<symbol> _symbols;
};
#endif // CONTEXT_HPP
