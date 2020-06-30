#ifndef PARSER_HPP
#define PARSER_HPP
#include "ast.hpp"
#include "token.hpp"
#include <iostream>
#include <list>
#include <memory>

class parser {
public:
  std::list<std::unique_ptr<node>> parse(std::list<token> tokens) {
    std::list<std::unique_ptr<node>> nodes;
    for (auto iter = tokens.begin(); iter != tokens.end();) {
      if (iter->type() == token::type::at) {
        iter++; // eat at
        if (iter->type() == token::type::number) {
          nodes.push_back(
              std::make_unique<anode>(anode(std::stoi(iter->value()))));
        } else {
          nodes.push_back(std::make_unique<anode>(anode(iter->value())));
        }
        iter++; // eat symbol or number
        continue;
      }

      if (iter->type() == token::type::symbol) {
        token first = *iter;
        iter++; // eat symbol
        std::cout << iter->type() << std::endl;

        if (iter->type() == token::type::assign) {
          std::cout << iter->type() << std::endl;
          iter++; // eat =
          auto compute = *iter;
          std::cout << iter->type() << std::endl;
          iter++; // TODO eat compute by expression
          std::cout << iter->type() << std::endl;
          if (iter->type() == token::type::semicolon) {
            iter++; // eat semicolon
            nodes.push_back(std::make_unique<cnode>(
                cnode(first.value(), expression(), iter->value())));
            iter++; // eat jump
          }
        } else if (iter->type() == token::type::semicolon) {
          iter++; // eat ;
          nodes.push_back(
              std::make_unique<cnode>(cnode(expression(), iter->value())));
          iter++; // eat jump
        } else if (is_operator(*iter)) {
          token op = *iter;
          iter++; // eat op
          token rhs = *iter;

          iter++; // eat rhs

          if (iter->type() == token::type::semicolon) {
	    iter++; //eat ;
            nodes.push_back(
                std::make_unique<cnode>(cnode(expression(), iter->value())));
	    iter++; // eat jump
          }
        }
      }
    }
    return nodes;
  }

private:
  bool is_operator(token t) {
    switch (t.type()) {
    case token::type::plus:
    case token::type::hyphen:
    case token::type::ampersand:
    case token::type::vbar:
    case token::type::exclamation:
      return true;
    default:
      return false;
    }
  }
};
#endif // PARSER_HPP
