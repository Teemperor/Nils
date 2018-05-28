#ifndef NILS_PASSUTILS_H
#define NILS_PASSUTILS_H

#include <utility>
#include <vector>
#include <string>

struct Token {
  enum class Type {
    Identifier,
    Number,
    Whitespace,
    Other
  };
  Type TType;
  std::string Content;
  Token() = default;
  Token(Type T, std::string Content) : TType(T), Content(std::move(Content)) {
  }
  bool operator==(const Token& Other) const {
    return TType == Other.TType && Content == Other.Content;
  }
};

class PassUtils {

public:
  static std::vector<Token> tokenize(const std::string &S);
  static std::string tokenToStr(const std::vector<Token> &T);
};


#endif //NILS_PASSUTILS_H
