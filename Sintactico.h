#pragma once
#include <vector>
#include <string>
#include "Token.h"

class Parser
{
public:
    std::vector<std::string> errors;
    std::vector<std::string> log;

    Parser(std::vector<Token> tokens);
    void parse();

private:
    std::vector<Token> tokens;
    int pos;

    Token current();
    Token peek(int offset = 0);
    Token consume();
    bool check(TokenType type);
    bool match(TokenType type);
    bool expect(TokenType type, const std::string& errorMsg);
    bool isAtEnd();
    void synchronize();

    void parseProgram();
    void parseStatement();
    void parseVarDecl(TokenType typeToken);
    void parseAssignment();
    void parseIfStmt();
    void parseForStmt();
    void parseMostrarStmt();
    void parseBlock();
    void parseExpr();
    void parseComparison();
    void parseTerm();
    void parseFactor();
    void parseUnary();
    void parsePrimary();
};