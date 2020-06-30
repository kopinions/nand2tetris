#ifndef HACK_VISITOR_HPP
#define HACK_VISITOR_HPP
#include "visitor.hpp"
#include "reporter.hpp"
#include <memory>

class hack_visitor : public visitor {
public:
  hack_visitor(std::shared_ptr<reporter> reporter){

  };

  virtual void visit(anode *){

  };
  virtual void visit(cnode *){

  };
 
  virtual void visit(expression *){

  };
  virtual void visit(std::shared_ptr<context>){

  };

private:
  std::shared_ptr<reporter> _reporter;
};
#endif // HACK_VISITOR_HPP
