#pragma once
#include <vector>
#include <string>
#include "Token.h"

class Lexer
{
private:
    std::string input;
    int pos;

public:
    std::vector<std::string> log;

    Lexer(std::string text);
    Token getNextToken();
};