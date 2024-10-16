#include "lexer.hpp"
#include "parser.hpp"
#include "runtime.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

std::string readf(const char* path) {
  std::ifstream f(path);
  f.seekg(0, std::ios::end);
  size_t size = f.tellg();
  std::string s(size, ' ');
  f.seekg(0);
  f.read(&s[0], size);
  return s;
}

int main(int argc, char** argv) {

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " path/to/file.ram" << std::endl;
    exit(1);
  }

  std::string program = readf(argv[1]);

  Lexer lexer(program);
  Parser parser(lexer);

  AST ast = parser.Parse();
  RunTime rt(ast);

  rt.Run();
  ast.Dump("graph.gv");

  return 0;

}