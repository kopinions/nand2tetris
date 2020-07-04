test_headers=include_directories('./include')
testfiles = files(
  'tokenizer.cpp',
  'ast.cpp',
  'visitor.cpp',
  'first_pass_visitor_test.cpp')


test('tests', executable(
  'tests',
  testfiles,
  include_directories : [inc_src, inc_headers, test_headers],
  dependencies : [
    gmock_dep,
    gtest_main_dep
  ]
))
