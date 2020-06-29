tokenizer = executable(
  'tokenizer',
  'tokenizer.cpp',
  include_directories : [inc_src, inc_headers],
  dependencies : [
    gtest_dep,
    gmock_dep
  ]
)

ast = executable(
  'ast',
  'ast.cpp',
  include_directories : [inc_src, inc_headers],
  dependencies : [
    gtest_dep,
    gmock_dep
  ]
)

test('tokenizer', tokenizer)
test('ast', ast)
