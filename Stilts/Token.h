#ifndef TOKEN_INCLUDE
#define TOKEN_INCLUDE
#include "Utils/Common.h"

enum TokenType {
  WhiteSpace,
  PreProcessorToken,
  Identifier,
  StringLiteral,
  CharLiteral,
  Operator,
  Keyword
};

struct Token {
  TokenType type;
};

#endif // TOKEN_INCLUDE