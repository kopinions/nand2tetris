#ifndef HACK_VISITOR_HPP
#define HACK_VISITOR_HPP
#include "ast.hpp"
#include "reporter.hpp"
#include "visitor.hpp"
#include <memory>

class hack_visitor : public visitor {
public:
  hack_visitor(std::shared_ptr<reporter> reporter) : _reporter(reporter){};

  virtual void visit(anode *n) { _reporter->report("0" + binary(n->address())); };

  virtual void visit(cnode *){

  };

  virtual void visit(number *){

  };

  virtual void visit(label *){

  };

  virtual void visit(expression *){

  };

  virtual void visit(std::shared_ptr<context>){

  };

private:
  std::shared_ptr<reporter> _reporter;
  std::string binary(int d) {
    std::string bin;
    for (int i = 0; i < 14; i++) {
      auto k = d >> i;
      if (k & 1) {
        bin = "1" + bin;
      } else {
        bin = "0" + bin;
      }
    }
    return bin;
  }
};
#endif // HACK_VISITOR_HPP
