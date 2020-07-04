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
      return std::make_optional<std::unique_ptr<anode>>(std::move(res));
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

class cparser : public iparser<token, node> {
public:
  virtual std::optional<std::unique_ptr<node>> parse(std::list<token>::iterator &iter) {
    if (iter->type() == token::type::symbol && std::next(iter, 1)->type() == token::type::assign) {
      token first = *iter;
      iter++; // eat symbol
      iter++; // eat =
      expression_parser ep;
      auto exp = ep.parse(iter);
      if (exp == std::nullopt) {
        return std::nullopt;
      }
      if (iter->type() == token::type::semicolon) {
        iter++; // eat semicolon
        auto result = std::make_unique<cnode>(cnode(first.value(), expression(), iter->value()));
        iter++; // eat jump
        return std::make_optional<std::unique_ptr<node>>(std::move(result));
      } else {
        auto result = std::make_unique<cnode>(cnode(first.value(), expression(), iter->value()));
        return std::make_optional<std::unique_ptr<node>>(std::move(result));
      }
    } else {
      expression_parser ep;
      auto exp = ep.parse(iter);
      if (exp == std::nullopt) {
        return std::nullopt;
      }
      if (iter->type() == token::type::semicolon) {
        iter++; // eat semicolon
        auto result = std::make_unique<cnode>(cnode(expression(), iter->value()));
        iter++; // eat jump
        return std::make_optional<std::unique_ptr<node>>(std::move(result));
      } else {
        auto result = std::make_unique<cnode>(cnode(expression()));
        return std::make_optional<std::unique_ptr<node>>(std::move(result));
      }
    }
  }
};

class label_parser : public iparser<token, node> {
public:
  virtual std::optional<std::unique_ptr<node>> parse(std::list<token>::iterator &iter) {
    if (iter->type() == token::type::lparen && std::next(iter, 1)->type() == token::type::symbol &&
        std::next(iter, 2)->type() == token::type::rparen) {
      iter++; // eat (
      auto result = std::make_unique<label>(label(iter->value()));
      iter++; // eat symbol
      iter++; // eat )
      return std::make_optional<std::unique_ptr<node>>(std::move(result));
    }
    return std::nullopt;
  }
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

      cparser c;
      auto cparsed = c.parse(iter);
      if (cparsed != std::nullopt) {
        nodes.push_back(std::move(*cparsed));
        continue;
      }

      label_parser l;
      auto lparsed = l.parse(iter);
      if (lparsed != std::nullopt) {
        nodes.push_back(std::move(*lparsed));
        continue;
      }
    }
    return nodes;
  }
};
#endif // PARSER_HPP
