#pragma once
#include <vector>
#include <string>
#include <map>
#include "Token.h"

// Información que se guarda por cada variable en la tabla de símbolos
struct SymbolInfo {
    TokenType type;       // TK_STRING o TK_DOUBLE
    int       scopeLevel; // en qué nivel de anidamiento fue declarada
    bool      initialized;// si ya recibió un valor
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

    // Tabla de símbolos: pila de scopes, cada uno es un mapa nombre→info
    std::vector<std::map<std::string, SymbolInfo>> scopeStack;

    // ── Navegación (igual que el parser) ──────────────────────────
    Token current();
    Token peek(int offset = 0);
    Token consume();
    bool  check(TokenType type);
    bool  match(TokenType type);
    bool  isAtEnd();
    void  synchronize();

    // ── Tabla de símbolos ─────────────────────────────────────────
    void      pushScope();
    void      popScope();
    bool      declareVariable(const std::string& name, TokenType type);
    bool      isDeclared(const std::string& name);
    TokenType getType(const std::string& name);
    void      markInitialized(const std::string& name);

    // ── Análisis de sentencias ────────────────────────────────────
    void analyzeProgram();
    void analyzeStatement();
    void analyzeVarDecl(TokenType typeToken);
    void analyzeAssignment();
    void analyzeIfStmt();
    void analyzeForStmt();
    void analyzeMostrarStmt();
    void analyzeBlock();

    // ── Análisis de expresiones (devuelven el tipo resultante) ────
    TokenType analyzeExpr();
    TokenType analyzeComparison();
    TokenType analyzeTerm();
    TokenType analyzeFactor();
    TokenType analyzeUnary();
    TokenType analyzePrimary();
};