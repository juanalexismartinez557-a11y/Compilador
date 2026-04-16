#pragma once
#include <vector>
#include <string>
#include <map>
#include "Token.h"

struct SymbolInfo {
    TokenType type;        // TK_STRING o TK_DOUBLE
    bool      initialized; // si ya recibió un valor
};

class Semantico {
public:
    std::vector<std::string> errors;
    std::vector<std::string> log;

    Semantico(std::vector<Token> tokens);
    void analyze();

private:
    std::vector<Token> tokens;
    int pos;

    // Tabla de símbolos plana — todas las variables viven aquí
    std::map<std::string, SymbolInfo> symbols;

    // ── Navegación ────────────────────────────────────────────────
    Token current();
    Token peek(int offset = 0);
    Token consume();
    bool  check(TokenType type);
    bool  match(TokenType type);
    bool  isAtEnd();
    void  synchronize();

    // ── Tabla de símbolos ─────────────────────────────────────────
    bool      declareVariable(const std::string& name, TokenType type);
    bool      isDeclared(const std::string& name);
    TokenType getType(const std::string& name);
    void      markInitialized(const std::string& name);

    // ── Sentencias ────────────────────────────────────────────────
    void analyzeProgram();
    void analyzeStatement();
    void analyzeVarDecl(TokenType typeToken);
    void analyzeAssignment();
    void analyzeIfStmt();
    void analyzeForStmt();
    void analyzeMostrarStmt();
    void analyzeBlock();

    // ── Expresiones ───────────────────────────────────────────────
    TokenType analyzeExpr();
    TokenType analyzeComparison();
    TokenType analyzeTerm();
    TokenType analyzeFactor();
    TokenType analyzeUnary();
    TokenType analyzePrimary();
};