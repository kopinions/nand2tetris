#ifndef PASS_HPP
#define PASS_HPP
#include <memory>

class context {
public:
  context(std::list<std::unique_ptr<node>> nodes){

  };
};
class pass {
public:
  virtual ~pass() = default;
  virtual bool applicable(std::shared_ptr<context> ctx) = 0;
  virtual bool run(std::shared_ptr<context> ctx) = 0;
};

class functional_pass : public pass {
public:
  functional_pass(std::function<bool(std::shared_ptr<context>)> fp) : _fp(fp) {}
  virtual bool applicable(std::shared_ptr<context> ctx) { return true; }
  virtual bool run(std::shared_ptr<context> ctx) { return this->_fp(ctx); };

private:
  std::function<bool(std::shared_ptr<context>)> _fp;
};
#endif // PASS_HPP
