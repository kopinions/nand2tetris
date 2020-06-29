#include "ast.hpp"
#include "hack_visitor.hpp"
#include "parser.hpp"
#include "pass.hpp"
#include "tokenizer.hpp"

int main(int argc, char **argv) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@111");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);

  functional_pass first(
      [](std::shared_ptr<context> ctx) -> bool { return true; });
  functional_pass second(
      [](std::shared_ptr<context> ctx) -> bool { return true; });
  auto ctx = std::make_shared<context>(std::move(nodes));
  first.run(ctx);
  second.run(ctx);

  hack_visitor visitor(std::make_shared<reporter>());
  visitor.visit(ctx);
}