#ifndef HACK_VISITOR_HPP
#define HACK_VISITOR_HPP
#include "ast.hpp"
#include "context.hpp"
#include "visitor.hpp"
#include <memory>

class first_pass_visitor : public visitor {
public:
  first_pass_visitor(std::shared_ptr<context> ctx) : _ctx(ctx){};

  virtual void visit(anode *n) { _ctx->add(n->symbol()); };

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
  std::shared_ptr<context> _ctx;
};
#endif // HACK_VISITOR_HPP
