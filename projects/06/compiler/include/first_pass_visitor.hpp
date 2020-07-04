#ifndef HACK_VISITOR_HPP
#define HACK_VISITOR_HPP
#include "ast.hpp"
#include "context.hpp"
#include "visitor.hpp"
#include <iostream>
#include <memory>

class first_pass_visitor : public visitor {
public:
  first_pass_visitor(std::shared_ptr<context> ctx) : _ctx(ctx), _pc(0){};

  virtual void visit(anode *n) {
    _ctx->define(n->symbol());
    _pc++;
  };

  virtual void visit(cnode *){ _pc++; };

  virtual void visit(number *){

  };

  virtual void visit(label *l) {
    if (_ctx->defined(l->name()) == std::nullopt) {
      _ctx->define(l->name(), _pc);
    } else if (*(_ctx->defined(l->name())) == -1) {
      _ctx->define(l->name(), _pc);
    } else {
      std::cerr << "error" << std::endl;
    }
  };

  virtual void visit(expression *){

  };

  virtual void visit(std::shared_ptr<context>){

  };

private:
  std::shared_ptr<context> _ctx;
  int _pc;
};
#endif // HACK_VISITOR_HPP
