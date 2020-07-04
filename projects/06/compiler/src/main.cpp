#include "ast.hpp"
#include "context.hpp"
#include "first_pass_visitor.hpp"
#include "hack_visitor.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include <fstream>
#include <string>

int main(int argc, char **argv) {
  std::ifstream infile(argv[1]);
  std::string input((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
  tokenizer to;
  std::list<token> tokens = to.tokenize(input);
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);

  auto ctx = std::make_shared<context>();
  auto first_pass = std::make_shared<first_pass_visitor>(ctx);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(first_pass);
  }

  auto reporter = std::make_shared<file_reporter>();
  auto v = std::make_shared<hack_visitor>(reporter, ctx);
  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(v);
  }
}