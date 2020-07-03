#ifndef PARSER_HPP
#define PARSER_HPP
#include "ast.hpp"
#include "token.hpp"
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <string>

template <typename F, typename T> class iparser {
public:
  virtual std::optional<std::unique_ptr<T>> parse(typename std::list<F>::iterator &) = 0;
  virtual ~iparser() = default;
};

class aparser : public iparser<token, node> {
public:
  virtual std::optional<std::unique_ptr<node>> parse(std::list<token>::iterator &iter) {
    if (iter->type() == token::type::at &&
        (std::next(iter, 1)->type() == token::type::number || std::next(iter, 1)->type() == token::type::symbol)) {
      iter++; // eat at
      auto res = iter->type() == token::type::number ? std::make_unique<anode>(anode(std::stoi(iter->value())))
                                                     : std::make_unique<anode>(anode(iter->value()));
      iter++; // eat symbol/number
      return std::make_optional<std::unique_ptr<node>>(std::move(res));
    }

    return std::nullopt;
  };
};

class unary_expression_parser : public iparser<token, node> {
public:
  virtual std::optional<std::unique_ptr<node>> parse(std::list<token>::iterator &iter) {
    if (!iter->is_operator() && iter->type() != token::type::number) {
      return std::nullopt;
    }

    if (iter->is_operator()) {
      if (std::next(iter, 1)->type() != token::type::symbol && std::next(iter, 1)->type() != token::type::number) {
        return std::nullopt;
      }

      if (std::next(iter, 2)->type() != token::type::semicolon) {
        return std::nullopt;
      }

      token op = *iter;
      iter++; // eat op
      token operand = *iter;
      iter++; // eat symbol/number
      iter++; // eat semicolon
      token jmp = *iter;
      auto result = std::make_unique<cnode>(cnode(expression(), iter->value()));
      iter++; // eat jump
      return std::make_optional<std::unique_ptr<node>>(std::move(result));
    }

    if (iter->type() == token::type::number) {
      if (std::next(iter, 1)->type() != token::type::semicolon || std::next(iter, 2)->type() != token::type::symbol) {
        return std::nullopt;
      }

      token number = *iter;
      iter++; // eat number
      iter++; // eat semicolon
      token jmp = *iter;
      auto result = std::make_unique<cnode>(cnode(expression(), iter->value()));
      iter++; // eat jump
      return std::make_optional<std::unique_ptr<node>>(std::move(result));
    }

    return std::nullopt;
  };
};

class parser {
public:
  std::list<std::unique_ptr<node>> parse(std::list<token> tokens) {
    std::list<std::unique_ptr<node>> nodes;
    for (auto iter = tokens.begin(); iter != tokens.end();) {
      aparser a;
      auto parsed = a.parse(iter);
      // TODO changed to whether the iter advanced
      if (parsed != std::nullopt) {
        nodes.push_back(std::move(*parsed));
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
              nodes.push_back(std::make_unique<cnode>(cnode(first.value(), expression(), iter->value())));
              iter++; // eat jump
            } else {
              nodes.push_back(std::make_unique<cnode>(cnode(first.value(), expression())));
            }
            continue;
          } else {
            auto compute = *iter;
            iter++; // eat first compute
          }

          if (iter->type() == token::type::semicolon) {
            iter++; // eat semicolon
            nodes.push_back(std::make_unique<cnode>(cnode(first.value(), expression(), iter->value())));
            iter++; // eat jump
          } else if (is_operator(*iter)) {
            token op = *iter;
            iter++; // eat op
            token operand = *iter;
            nodes.push_back(std::make_unique<cnode>(cnode(first.value(), expression())));
            iter++; // eat operand
          } else if (iter->type() == token::type::number) {
            // D=1
            nodes.push_back(std::make_unique<cnode>(cnode(first.value(), expression())));
            iter++; // eat number
          } else {
            nodes.push_back(std::make_unique<cnode>(cnode(first.value(), expression())));
          }
        } else if (iter->type() == token::type::semicolon) {
          iter++; // eat ;
          nodes.push_back(std::make_unique<cnode>(cnode(expression(), iter->value())));
          iter++; // eat jump
        } else if (is_operator(*iter)) {
          token op = *iter;
          iter++; // eat op
          token rhs = *iter;

          iter++; // eat rhs

          if (iter->type() == token::type::semicolon) {
            iter++; // eat ;
            nodes.push_back(std::make_unique<cnode>(cnode(expression(), iter->value())));
            iter++; // eat jump
          }
        }
        continue;
      }

      unary_expression_parser unary_ep;
      auto unary_expression_parsed = unary_ep.parse(iter);
      if (unary_expression_parsed != std::nullopt) {
        nodes.push_back(std::move(*unary_expression_parsed));
        continue;
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
