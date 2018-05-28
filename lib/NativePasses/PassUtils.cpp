#include "PassUtils.h"

#include <sstream>

namespace {
struct Stream {
  std::string Str;
  std::size_t Index = 0;
  char pop() { return Str.at(Index++); }
  char peek() const { return Str.at(Index); }
  bool eof() const { return Index >= Str.size(); }
};
} // namespace

// Utility methods.
static bool isChar(int c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool isNum(int c) { return c >= '0' && c <= '9'; }

static bool isWhitespace(int c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
static bool isAlphaNumUnderscore(int c) {
  return isNum(c) || isChar(c) || c == '_';
}

std::vector<Token> PassUtils::tokenize(const std::string &S) {
  std::vector<Token> Result;

  Stream Input;
  Input.Str = S;

  char c;
  while (!Input.eof()) {
    Token::Type Type;
    std::stringstream CurrentTok;

    c = Input.peek();
    if (isChar(c)) {
      Type = Token::Type::Identifier;
      while (isAlphaNumUnderscore(c)) {
        CurrentTok << Input.pop();
        if (Input.eof())
          break;
        c = Input.peek();
      }
      Result.push_back({Type, CurrentTok.str()});
    } else if (isNum(c)) {
      Type = Token::Type::Number;
      while (isNum(c)) {
        CurrentTok << Input.pop();
        if (Input.eof())
          break;
        c = Input.peek();
      }
      Result.push_back({Type, CurrentTok.str()});
    } else if (isWhitespace(c)) {
      Type = Token::Type::Whitespace;
      while (isWhitespace(c)) {
        CurrentTok << Input.pop();
        if (Input.eof())
          break;
        c = Input.peek();
      }
      Result.push_back({Type, CurrentTok.str()});
    } else {
      Type = Token::Type::Other;
      CurrentTok << Input.pop();
      Result.push_back({Type, CurrentTok.str()});
    }
  }

  return Result;
}

std::string PassUtils::tokenToStr(const std::vector<Token> &Tokens) {
  std::stringstream Output;
  for (const Token &T : Tokens) {
    Output << T.Content;
  }
  return Output.str();
}
