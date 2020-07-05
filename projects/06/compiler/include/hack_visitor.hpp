#ifndef HACK_VISITOR_HPP
#define HACK_VISITOR_HPP
#include "ast.hpp"
#include "context.hpp"
#include "reporter.hpp"
#include "visitor.hpp"
#include <memory>

class hack_visitor : public visitor, public std::enable_shared_from_this<hack_visitor> {
public:
  hack_visitor(std::shared_ptr<reporter> reporter, std::shared_ptr<context> ctx)
      : _reporter(reporter), _ctx(ctx), _destinations(), _jumps(), _computes(), _expr("") {
    _destinations["M"] = "001";
    _destinations["D"] = "010";
    _destinations["MD"] = "011";
    _destinations["A"] = "100";
    _destinations["AM"] = "101";
    _destinations["AD"] = "110";
    _destinations["AMD"] = "111";

    _jumps["JGT"] = "001";
    _jumps["JEQ"] = "010";
    _jumps["JGE"] = "011";
    _jumps["JLT"] = "100";
    _jumps["JNE"] = "101";
    _jumps["JLE"] = "110";
    _jumps["JMP"] = "111";

    _computes["0"] = "0101010";
    _computes["1"] = "0111111";
    _computes["-1"] = "0111010";
    _computes["D"] = "0001100";
    _computes["A"] = "0110000";
    _computes["!D"] = "0001101";
    _computes["!A"] = "0110001";
    _computes["-D"] = "0001111";
    _computes["-A"] = "0110011";
    _computes["D+1"] = "0011111";
    _computes["A+1"] = "0110111";
    _computes["D-1"] = "0001110";
    _computes["A-1"] = "0110010";
    _computes["D+A"] = "0000010";
    _computes["D-A"] = "0010011";
    _computes["A-D"] = "0000111";
    _computes["D&A"] = "0000000";
    _computes["D|A"] = "0010101";
    _computes["M"] = "1110000";
    _computes["!M"] = "1110001";
    _computes["-M"] = "1110011";
    _computes["M+1"] = "1110111";
    _computes["M-1"] = "1110010";
    _computes["D+M"] = "1000010";
    _computes["D-M"] = "1010011";
    _computes["M-D"] = "1000111";
    _computes["D&M"] = "1000000";
    _computes["D|M"] = "1010101";
  };

  virtual void visit(anode *n) {
    if (n->address() != -1) {
      _reporter->report("0" + binary(n->address()));
      return;
    }

    auto def = _ctx->defined(n->symbol());
    if (def != std::nullopt) {
      _reporter->report("0" + binary(*def));
    } else {
      _ctx->define(n->symbol());
      _reporter->report("0" + binary(*(_ctx->defined(n->symbol()))));
    }
  };

  virtual void visit(cnode *c) {
    auto dest = c->dest().empty() ? "000" : _destinations.at(c->dest());
    auto jmp = c->jmp().empty() ? "000" : _jumps.at(c->jmp());

    c->e()->accept(shared_from_this());
    _reporter->report("111" + _expr + dest + jmp);
  };

  virtual void visit(constant *c) { _expr = _computes.at(std::to_string(c->number())); };

  virtual void visit(binary *b) { _expr = _computes.at(b->exp()); };

  virtual void visit(unary *u) { _expr = _computes.at(u->exp()); };

  virtual void visit(label *){

  };

  virtual void visit(expression *){

  };

  virtual void visit(std::shared_ptr<context>){

  };

private:
  std::shared_ptr<reporter> _reporter;
  std::shared_ptr<context> _ctx;
  std::string binary(int d) {
    std::string bin;
    for (int i = 0; i < 15; i++) {
      auto k = d >> i;
      if (k & 1) {
        bin = "1" + bin;
      } else {
        bin = "0" + bin;
      }
    }
    return bin;
  }
  std::map<std::string, std::string> _destinations;
  std::map<std::string, std::string> _jumps;
  std::map<std::string, std::string> _computes;
  std::string _expr;
};
#endif // HACK_VISITOR_HPP
