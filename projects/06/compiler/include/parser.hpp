#ifndef PARSER_HPP
#define PARSER_HPP
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
            iter++; // eat jump
            nodes.push_back(std::make_unique<cnode>(cnode()));
          }
        }
      }
    }
    return nodes;
  };
};
#endif // PARSER_HPP
