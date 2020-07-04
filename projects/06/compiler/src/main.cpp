#include "ast.hpp"
#include "hack_visitor.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include "context.hpp"

int main(int argc, char **argv) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@111");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  auto ctx = std::make_shared<context>();
  hack_visitor visitor(std::make_shared<file_reporter>(), ctx);
  visitor.visit(ctx);
}