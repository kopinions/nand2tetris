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

class const_expression_parser : public iparser<token, node> {
public:
  virtual std::optional<std::unique_ptr<node>> parse(std::list<token>::iterator &iter) {
    if (iter->type() == token::type::hyphen && std::next(iter, 1)->type() == token::type::number) {
      iter++; // eat hyphen
      auto result = std::make_unique<number>(number(0 - std::stoi(iter->value())));
      iter++; // eat number
      return std::make_optional<std::unique_ptr<node>>(std::move(result));
    } else if (iter->type() == token::type::number) {
      auto result = std::make_unique<number>(number(std::stoi(iter->value())));
      iter++; // eat number
      return std::make_optional<std::unique_ptr<node>>(std::move(result));
    } else {
      return std::nullopt;
    }
  }
};

class unary_expression_parser : public iparser<token, node> {
public:
  virtual std::optional<std::unique_ptr<node>> parse(std::list<token>::iterator &iter) {
    if (iter->is_operator() && std::next(iter, 1)->type() == token::type::symbol) {
      token op = *iter;
      iter++; // eat operator
      auto result = std::make_unique<unary>(unary(op.value(), iter->value()));
      iter++; // eat symbol
      return std::make_optional<std::unique_ptr<node>>(std::move(result));
    } else if (iter->type() == token::type::symbol) {
      auto result = std::make_unique<unary>(unary("", iter->value()));
      iter++; // eat symbol
      return std::make_optional<std::unique_ptr<node>>(std::move(result));
    } else {
      return std::nullopt;
    }
  }
};

class binary_expression_parser : public iparser<token, node> {
  virtual std::optional<std::unique_ptr<node>> parse(std::list<token>::iterator &iter) {
    if (iter->type() == token::type::symbol && std::next(iter, 1)->is_operator() &&
        (std::next(iter, 2)->type() == token::type::symbol || std::next(iter, 2)->type() == token::type::number)) {
      token operand1 = *iter;
      iter++; // eat operand1
      token op = *iter;
      iter++; // eat op
      auto result = std::make_unique<binary>(binary(operand1.value(), op.value(), iter->value()));
      iter++; // eat operand2
      return std::make_optional<std::unique_ptr<node>>(std::move(result));
    } else {
      return std::nullopt;
    }
  }
};

class expression_parser : public iparser<token, node> {
public:
  expression_parser() : _parsers() {
    _parsers.push_back(new const_expression_parser());
    _parsers.push_back(new binary_expression_parser());
    _parsers.push_back(new unary_expression_parser());
  }

  virtual std::optional<std::unique_ptr<node>> parse(std::list<token>::iterator &iter) {
    for (auto pit = _parsers.begin(); pit != _parsers.end(); ++pit) {
      auto parsed = (*pit)->parse(iter);
      if (parsed != std::nullopt) {
        return parsed;
      }
    }
    return std::nullopt;
  }

private:
  std::list<iparser<token, node> *> _parsers;
};

class unary_c_instruction : public iparser<token, node> {
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
          expression_parser ep;
          auto exp = ep.parse(iter);
          if (exp == std::nullopt) {
            throw std::runtime_error("error");
          }
          if (iter->type() == token::type::semicolon) {
            iter++; // eat semicolon
            nodes.push_back(std::make_unique<cnode>(cnode(first.value(), expression(), iter->value())));
            iter++; // eat jump
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

      unary_c_instruction unary_c;
      auto unary_c_pasted = unary_c.parse(iter);
      if (unary_c_pasted != std::nullopt) {
        nodes.push_back(std::move(*unary_c_pasted));
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
