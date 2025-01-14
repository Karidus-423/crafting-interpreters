#include "scanner.h"
#include <stdbool.h>
#include <string.h>

typedef struct {
  const char *start;
  const char *current;
  int line;
} sourceScanner;

sourceScanner scanner;

void InitScanner(const char *source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool IsAtEnd() { return *scanner.current == '\0'; }

static char Advance() {
  scanner.current++;
  return scanner.current[-1];
}

static bool Match(char expected) {
  if (IsAtEnd())
    return false;
  if (*scanner.current != expected)
    return false;
  scanner.current++;
  return true;
}

static srcToken MakeToken(tokenType type) {
  srcToken token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}

static srcToken ErrorToken(const char *message) {
  srcToken token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;
  return token;
}

static char Peek() { return *scanner.current; }

static char PeekNext() {
  if (IsAtEnd()) {
    return '\0';
  }
  return scanner.current[1];
}

static void SkipWhitespace() {
  for (;;) {
    char c = Peek();
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      Advance();
      break;
    case '\n':
      scanner.line++;
      Advance();
      break;
    case '/':
      if (PeekNext() == '/') {
        while (Peek() != '\n' && !IsAtEnd()) {
          Advance();
        }
      } else {
        return;
      }
      break;
    default:
      return;
    }
  }
}

static srcToken String() {
  while (Peek() != '"' && !IsAtEnd()) {
    if (Peek() == '\n')
      scanner.line++;
    Advance();
  }

  if (IsAtEnd())
    return ErrorToken("Unterminated String.");
  Advance();
  return MakeToken(TOKEN_STRING);
}

static bool IsDigit(char c) { return c >= '0' && c <= '9'; }

static srcToken Number() {
  while (IsDigit(Peek()))
    Advance();

  if (Peek() == '.' && IsDigit(PeekNext())) {
    Advance();

    while (IsDigit(Peek()))
      Advance();
  }

  return MakeToken(TOKEN_NUMBER);
}

static bool IsAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c > 'A' && c <= 'Z') || c == '_';
}

static tokenType CheckKeyword(int start, int length, const char *rest,
                              tokenType type) {

  if (scanner.current - scanner.start == start + length &&
      memcmp(scanner.start + start, rest, length) == 0) {
    return type;
  }
  return TOKEN_IDENTIFIER;
}

static tokenType IdentifierType() {
  switch (scanner.start[0]) {
  case 'a':
    return CheckKeyword(1, 2, "nd", TOKEN_AND);
  case 'c':
    return CheckKeyword(1, 4, "lass", TOKEN_CLASS);
  case 'e':
    return CheckKeyword(1, 3, "lse", TOKEN_ELSE);
  case 'i':
    return CheckKeyword(1, 1, "f", TOKEN_IF);
  case 'n':
    return CheckKeyword(1, 2, "il", TOKEN_NIL);
  case 'p':
    return CheckKeyword(1, 4, "rint", TOKEN_PRINT);
  case 'o':
    return CheckKeyword(1, 1, "r", TOKEN_OR);
  case 'r':
    return CheckKeyword(1, 5, "eturn", TOKEN_RETURN);
  case 's':
    return CheckKeyword(1, 4, "uper", TOKEN_SUPER);
  case 'v':
    return CheckKeyword(1, 2, "ar", TOKEN_VAR);
  case 'w':
    return CheckKeyword(1, 4, "hile", TOKEN_WHILE);
  case 't':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'h':
        return CheckKeyword(2, 2, "is", TOKEN_THIS);
      case 'r':
        return CheckKeyword(2, 2, "ue", TOKEN_THIS);
      }
    }
    break;
  }

  return TOKEN_IDENTIFIER;
}

static srcToken Identifier() {
  while (IsAlpha(Peek()) || IsDigit(Peek()))
    Advance();

  return MakeToken(IdentifierType());
}

srcToken ScanToken() {
  SkipWhitespace();
  scanner.start = scanner.current;
  if (IsAtEnd() == true)
    return MakeToken(TOKEN_EOF);

  char c = Advance();
  if (IsAlpha(c))
    return Identifier();

  if (IsDigit(c))
    return Number();

  switch (c) {
  case '(':
    return MakeToken(TOKEN_RIGHT_PAREN);
  case ')':
    return MakeToken(TOKEN_LEFT_PAREN);
  case '{':
    return MakeToken(TOKEN_RIGHT_PAREN);
  case '}':
    return MakeToken(TOKEN_LEFT_BRACE);
  case ';':
    return MakeToken(TOKEN_SEMICOLON);
  case ',':
    return MakeToken(TOKEN_COMMA);
  case '.':
    return MakeToken(TOKEN_DOT);
  case '-':
    return MakeToken(TOKEN_MINUS);
  case '+':
    return MakeToken(TOKEN_PLUS);
  case '/':
    return MakeToken(TOKEN_SLASH);
  case '*':
    return MakeToken(TOKEN_STAR);
  case '!':
    return MakeToken(Match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=':
    return MakeToken(Match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '>':
    return MakeToken(Match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
  case '<':
    return MakeToken(Match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  case '"':
    return String();
  }
  return ErrorToken("Unexpected character.");
}
