#include "Semantico.h"

// ════════════════════════════════════════════════════════════════
//  CONSTRUCTOR
// ════════════════════════════════════════════════════════════════
Semantico::Semantico(std::vector<Token> tokens)
{
    this->tokens = tokens;
    this->pos = 0;
    scopeStack.push_back({}); // scope global
}

// ════════════════════════════════════════════════════════════════
//  NAVEGACIÓN  (idéntica al parser)
// ════════════════════════════════════════════════════════════════
Token Semantico::current()
{
    if (pos < (int)tokens.size()) return tokens[pos];
    return Token{ TK_END_OF_FILE, "" };
}

Token Semantico::peek(int offset)
{
    int idx = pos + offset;
    if (idx < (int)tokens.size()) return tokens[idx];
    return Token{ TK_END_OF_FILE, "" };
}

Token Semantico::consume()
{
    Token t = current();
    if (!isAtEnd()) pos++;
    return t;
}

bool Semantico::check(TokenType type) { return current().type == type; }

bool Semantico::match(TokenType type)
{
    if (check(type)) { consume(); return true; }
    return false;
}

bool Semantico::isAtEnd() { return current().type == TK_END_OF_FILE; }

void Semantico::synchronize()
{
    while (!isAtEnd())
    {
        if (check(TK_SEMICOLON)) { consume(); return; }
        if (check(TK_RBRACE))             return;
        if (check(TK_IF) || check(TK_FOR) ||
            check(TK_MOSTRAR) || check(TK_PRINT)) return;
        consume();
    }
}

// ════════════════════════════════════════════════════════════════
//  TABLA DE SÍMBOLOS
// ════════════════════════════════════════════════════════════════

// Abre un nuevo nivel de scope (al entrar a un bloque { })
void Semantico::pushScope()
{
    scopeStack.push_back({});
    log.push_back("Scope abierto  (nivel " +
        std::to_string(scopeStack.size() - 1) + ").");
}

// Cierra el nivel actual y elimina todas sus variables
void Semantico::popScope()
{
    if (scopeStack.empty()) return;
    log.push_back("Scope cerrado  (nivel " +
        std::to_string(scopeStack.size() - 1) + ").");
    scopeStack.pop_back();
}

// Intenta registrar una variable en el scope actual.
// Falla si ya existe en ESE mismo scope (redeclaración).
bool Semantico::declareVariable(const std::string& name, TokenType type)
{
    auto& current_scope = scopeStack.back();
    if (current_scope.find(name) != current_scope.end())
    {
        errors.push_back("ERROR SEMANTICO: la variable '" + name +
            "' ya fue declarada en este bloque.");
        return false;
    }
    current_scope[name] = { type, (int)scopeStack.size() - 1, false };
    log.push_back("Variable '" + name + "' registrada como " +
        tokenTypeToString(type) + ".");
    return true;
}

// Busca la variable desde el scope más interno hacia afuera
bool Semantico::isDeclared(const std::string& name)
{
    for (int i = (int)scopeStack.size() - 1; i >= 0; i--)
        if (scopeStack[i].count(name)) return true;
    return false;
}

// Obtiene el tipo de una variable ya declarada
TokenType Semantico::getType(const std::string& name)
{
    for (int i = (int)scopeStack.size() - 1; i >= 0; i--)
        if (scopeStack[i].count(name))
            return scopeStack[i][name].type;
    return TK_UNKNOWN;
}

// Marca una variable como inicializada (recibió un valor)
void Semantico::markInitialized(const std::string& name)
{
    for (int i = (int)scopeStack.size() - 1; i >= 0; i--)
        if (scopeStack[i].count(name))
        {
            scopeStack[i][name].initialized = true;
            return;
        }
}

// ════════════════════════════════════════════════════════════════
//  PUNTO DE ENTRADA
// ════════════════════════════════════════════════════════════════
void Semantico::analyze()
{
    log.push_back("Inicio del analisis semantico.");
    analyzeProgram();

    if (errors.empty())
        log.push_back("Analisis semantico completado SIN errores.");
    else
        log.push_back("Analisis semantico completado CON " +
            std::to_string(errors.size()) + " error(es).");
}

void Semantico::analyzeProgram()
{
    while (!isAtEnd())
        analyzeStatement();
}

// ════════════════════════════════════════════════════════════════
//  SENTENCIAS
// ════════════════════════════════════════════════════════════════
void Semantico::analyzeStatement()
{
    Token t = current();

    if (t.type == TK_STRING || t.type == TK_DOUBLE)
    {
        analyzeVarDecl(t.type);
        return;
    }
    if (t.type == TK_IF) { analyzeIfStmt();      return; }
    if (t.type == TK_FOR) { analyzeForStmt();     return; }
    if (t.type == TK_MOSTRAR || t.type == TK_PRINT) { analyzeMostrarStmt(); return; }
    if (t.type == TK_LBRACE) { analyzeBlock();       return; }

    if (t.type == TK_IDENTIFIER)
    {
        // ── x = expr ─────────────────────────────────────────────
        if (peek(1).type == TK_ASSIGN)
        {
            analyzeAssignment();
            return;
        }

        errors.push_back("ERROR SEMANTICO: sentencia invalida con '" +
            t.value + "'.");
        synchronize();
        return;
    }

    // Token inesperado: consumir y seguir
    consume();
    synchronize();
}

// ── Declaración: string/double IDENTIFIER (= expr)? ; ────────────
void Semantico::analyzeVarDecl(TokenType typeToken)
{
    consume(); // consume el tipo

    if (!check(TK_IDENTIFIER)) { synchronize(); return; }
    std::string varName = current().value;
    consume();

    declareVariable(varName, typeToken);

    if (match(TK_ASSIGN))
    {
        TokenType exprType = analyzeExpr();

        if (exprType != TK_UNKNOWN && exprType != typeToken)
        {
            std::string esperado = (typeToken == TK_STRING) ? "string" : "double";
            std::string recibido = (exprType == TK_STRING) ? "string" : "double";
            errors.push_back("ERROR SEMANTICO: tipo incompatible al inicializar '"
                + varName + "' — se esperaba " + esperado
                + " pero la expresion es " + recibido + ".");
        }
        else
        {
            markInitialized(varName);
            log.push_back("Variable '" + varName + "' inicializada correctamente.");
        }
    }

    match(TK_SEMICOLON);
}

// ── Asignación: IDENTIFIER = expr ; ──────────────────────────────
void Semantico::analyzeAssignment()
{
    std::string varName = current().value;
    consume(); // IDENTIFIER
    consume(); // =

    if (!isDeclared(varName))
    {
        errors.push_back("ERROR SEMANTICO: variable '" + varName +
            "' usada sin declarar.");
        synchronize();
        return;
    }

    TokenType varType = getType(varName);
    TokenType exprType = analyzeExpr();

    if (exprType != TK_UNKNOWN && exprType != varType)
    {
        std::string vt = (varType == TK_STRING) ? "string" : "double";
        std::string et = (exprType == TK_STRING) ? "string" : "double";
        errors.push_back("ERROR SEMANTICO: no se puede asignar un valor " + et +
            " a la variable '" + varName + "' que es " + vt + ".");
    }
    else
    {
        markInitialized(varName);
        log.push_back("Asignacion a '" + varName + "' verificada.");
    }

    match(TK_SEMICOLON);
}

// ── if ( expr ) block (else block)? ──────────────────────────────
void Semantico::analyzeIfStmt()
{
    consume(); // if
    match(TK_LPAREN);
    analyzeExpr();
    match(TK_RPAREN);

    if (check(TK_LBRACE)) analyzeBlock();

    if (match(TK_ELSE))
        if (check(TK_LBRACE)) analyzeBlock();

    log.push_back("Sentencia if verificada.");
}

// ── for ( init  cond ;  incr ) block ─────────────────────────────
void Semantico::analyzeForStmt()
{
    consume(); // for
    match(TK_LPAREN);

    // Inicialización
    Token t = current();
    if (t.type == TK_STRING || t.type == TK_DOUBLE)
        analyzeVarDecl(t.type);
    else if (t.type == TK_IDENTIFIER && peek(1).type == TK_ASSIGN)
        analyzeAssignment();
    else
        synchronize();

    // Condición
    analyzeExpr();
    match(TK_SEMICOLON);

    // Incremento: IDENTIFIER = expr
    if (check(TK_IDENTIFIER))
    {
        std::string varName = current().value;
        consume();
        match(TK_ASSIGN);

        if (!isDeclared(varName))
            errors.push_back("ERROR SEMANTICO: variable '" + varName +
                "' no declarada en el incremento del for.");

        analyzeExpr();
    }

    match(TK_RPAREN);
    if (check(TK_LBRACE)) analyzeBlock();

    log.push_back("Sentencia for verificada.");
}

// ── mostrar/print ( expr ) ; ─────────────────────────────────────
void Semantico::analyzeMostrarStmt()
{
    consume(); // mostrar / print
    match(TK_LPAREN);
    analyzeExpr(); // acepta cualquier tipo
    match(TK_RPAREN);
    match(TK_SEMICOLON);
    log.push_back("Sentencia mostrar/print verificada.");
}

// ── { statement* } ───────────────────────────────────────────────
void Semantico::analyzeBlock()
{
    pushScope();
    consume(); // {
    while (!isAtEnd() && !check(TK_RBRACE))
        analyzeStatement();
    match(TK_RBRACE);
    popScope();
    log.push_back("Bloque verificado.");
}

// ════════════════════════════════════════════════════════════════
//  EXPRESIONES  —  cada método devuelve el tipo del resultado
// ════════════════════════════════════════════════════════════════

// Nivel 0: == y !=
TokenType Semantico::analyzeExpr()
{
    TokenType left = analyzeComparison();

    while (check(TK_EQUAL_EQUAL) || check(TK_NOT_EQUAL))
    {
        consume();
        TokenType right = analyzeComparison();

        if (left != TK_UNKNOWN &&
            right != TK_UNKNOWN &&
            left != right)
            errors.push_back("ERROR SEMANTICO: comparacion de igualdad "
                "entre tipos distintos (string y double).");

        left = TK_DOUBLE; // el resultado de == o != es siempre un booleano (double)
    }
    return left;
}

// Nivel 1: < > <= >=
TokenType Semantico::analyzeComparison()
{
    TokenType left = analyzeTerm();

    while (check(TK_LESS) || check(TK_GREATER) ||
        check(TK_LESS_EQUAL) || check(TK_GREATER_EQUAL))
    {
        consume();
        TokenType right = analyzeTerm();

        if (left == TK_STRING || right == TK_STRING)
            errors.push_back("ERROR SEMANTICO: los operadores '<' '>' '<=' '>=' "
                "no se pueden usar con strings.");

        left = TK_DOUBLE;
    }
    return left;
}

// Nivel 2: + y -
TokenType Semantico::analyzeTerm()
{
    TokenType left = analyzeFactor();

    while (check(TK_SUMA) || check(TK_MINUS))
    {
        Token op = current();
        consume();
        TokenType right = analyzeFactor();

        if (op.type == TK_MINUS &&
            (left == TK_STRING || right == TK_STRING))
        {
            errors.push_back("ERROR SEMANTICO: el operador '-' "
                "no se puede usar con strings.");
            left = TK_UNKNOWN;
        }
        else if (op.type == TK_SUMA &&
            left != TK_UNKNOWN &&
            right != TK_UNKNOWN &&
            left != right)
        {
            errors.push_back("ERROR SEMANTICO: no se puede sumar "
                "un string y un double.");
            left = TK_UNKNOWN;
        }
        else
        {
            // string + string → string (concatenación)
            // double + double → double
            left = (left == TK_STRING && right == TK_STRING)
                ? TK_STRING : TK_DOUBLE;
        }
    }
    return left;
}

// Nivel 3: * y /
TokenType Semantico::analyzeFactor()
{
    TokenType left = analyzeUnary();

    while (check(TK_MULT) || check(TK_DIV))
    {
        consume();
        TokenType right = analyzeUnary();

        if (left == TK_STRING || right == TK_STRING)
            errors.push_back("ERROR SEMANTICO: '*' y '/' "
                "no se pueden usar con strings.");

        left = TK_DOUBLE;
    }
    return left;
}

// Nivel 4: ! y - (unario)
TokenType Semantico::analyzeUnary()
{
    if (check(TK_EXCLAMATION) || check(TK_MINUS))
    {
        Token op = current();
        consume();
        TokenType t = analyzeUnary();

        if (op.type == TK_MINUS && t == TK_STRING)
            errors.push_back("ERROR SEMANTICO: el operador '-' unario "
                "no se puede aplicar a un string.");

        return TK_DOUBLE;
    }
    return analyzePrimary();
}

// Nivel 5: valores atómicos
TokenType Semantico::analyzePrimary()
{
    Token t = current();

    if (t.type == TK_NUMBER) { consume(); return TK_DOUBLE; }
    if (t.type == TK_STRING_LITERAL) { consume(); return TK_STRING; }
    if (t.type == TK_CHAR_LITERAL) { consume(); return TK_STRING; }

    if (t.type == TK_IDENTIFIER)
    {
        consume();
        if (!isDeclared(t.value))
        {
            errors.push_back("ERROR SEMANTICO: variable '" + t.value +
                "' usada sin declarar.");
            return TK_UNKNOWN;
        }
        return getType(t.value);
    }

    if (t.type == TK_LPAREN)
    {
        consume();
        TokenType inner = analyzeExpr();
        match(TK_RPAREN);
        return inner;
    }

    if (!isAtEnd()) consume();
    return TK_UNKNOWN;
}