project(
  'assembler',
  ['c', 'cpp'],
  version : '0.0.1',
  license : 'MIT',
  meson_version : '>= 0.5.0',
  default_options : ['buildtype=debug', 'b_ndebug=if-release', 'c_std=c11', 'cpp_std=c++17']
)

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

subdir('include')

gtest_proj = subproject('gtest')
gmock_dep = gtest_proj.get_variable('gmock_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
subdir('src')
subdir('test')
