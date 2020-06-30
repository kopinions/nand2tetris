#ifndef PARSER_HPP
#define PARSER_HPP
#include "ast.hpp"
#include "token.hpp"
#include <iostream>
#include <list>
#include <memory>
#include <string>

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

        if (iter->type() == token::type::assign) {
          iter++; // eat =
          if (is_operator(*iter)) {
            // compute expression is unary expression
            token op = *iter;
            iter++;
            token operand = *iter;
            iter++; // eat operand

            if (iter->type() == token::type::semicolon) {
              nodes.push_back(std::make_unique<cnode>(
                  cnode(first.value(), expression(), iter->value())));
              iter++; // eat jump
            } else {
              nodes.push_back(
                  std::make_unique<cnode>(cnode(first.value(), expression())));
            }
            continue;
          } else {
            auto compute = *iter;
            iter++; // eat first compute
          }

          if (iter->type() == token::type::semicolon) {
            iter++; // eat semicolon
            nodes.push_back(std::make_unique<cnode>(
                cnode(first.value(), expression(), iter->value())));
            iter++; // eat jump
          } else if (is_operator(*iter)) {
            token op = *iter;
            iter++; // eat op
            token operand = *iter;
            nodes.push_back(
                std::make_unique<cnode>(cnode(first.value(), expression())));
            iter++; // eat operand
          } else if (iter->type() == token::type::number) {
            // D=1
            nodes.push_back(
                std::make_unique<cnode>(cnode(first.value(), expression())));
            iter++; // eat number
          } else {
            nodes.push_back(
                std::make_unique<cnode>(cnode(first.value(), expression())));
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
            iter++; // eat ;
            nodes.push_back(
                std::make_unique<cnode>(cnode(expression(), iter->value())));
            iter++; // eat jump
          }
        }
        continue;
      }

      if (is_operator(*iter)) {
        token op = *iter;
        iter++; // eat op
        if (iter->type() != token::type::symbol &&
            iter->type() != token::type::number) {
          throw std::runtime_error("Expect symbol but found " +
                                   std::to_string(iter->type()));
        }
        token operand = *iter;
        iter++; // eat symbol/number

        if (iter->type() != token::type::semicolon) {
          throw std::runtime_error("Expect semicolon but found " +
                                   std::to_string(iter->type()));
        }
        iter++; // eat semicolon
        token jmp = *iter;
        nodes.push_back(
            std::make_unique<cnode>(cnode(expression(), iter->value())));
        iter++; // eat jump
        continue;
      }

      if (iter->type() == token::type::number) {
        token number = *iter;
        iter++; // eat number
        if (iter->type() != token::type::semicolon) {
          throw std::runtime_error("Expect semicolon but found " +
                                   std::to_string(iter->type()));
        }
        iter++; // eat semicolon
        if (iter->type() != token::type::symbol) {
          throw std::runtime_error("Expect symbol but found " +
                                   std::to_string(iter->type()));
        }

        token jmp = *iter;
        nodes.push_back(
            std::make_unique<cnode>(cnode(expression(), iter->value())));
        iter++; // eat jump
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
