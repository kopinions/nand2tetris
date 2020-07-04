test_headers=include_directories('./include')
tokenizer = executable(
  'tokenizer',
  'tokenizer.cpp',
  include_directories : [inc_src, inc_headers, test_headers],
  dependencies : [
    gtest_dep,
    gmock_dep
  ]
)

ast = executable(
  'ast',
  'ast.cpp',
  include_directories : [inc_src, inc_headers, test_headers],
  dependencies : [
    gtest_dep,
    gmock_dep
  ]
)

visitor = executable(
  'visitor',
  'visitor.cpp',
  include_directories : [inc_src, inc_headers, test_headers],
  dependencies : [
    gtest_dep,
    gmock_dep
  ]
)

test('tokenizer', tokenizer)
test('ast', ast)
test('visitor', visitor)
