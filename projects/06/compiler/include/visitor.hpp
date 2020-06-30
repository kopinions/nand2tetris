#ifndef VISITOR_HPP
#define VISITOR_HPP
#include <memory>
class context;
class anode;
class cnode;
class expression;

class visitor {
public:
  virtual ~visitor() = default;
  virtual void visit(anode *) = 0;
  virtual void visit(cnode *) = 0;
  virtual void visit(expression *) = 0;
  virtual void visit(std::shared_ptr<context>) = 0;
};

#endif // VISITOR_HPP
