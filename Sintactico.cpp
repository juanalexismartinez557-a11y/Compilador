#include "Sintactico.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
    this->pos = 0;
}

Token Parser::current()
{
    if (pos < (int)tokens.size())
        return tokens[pos];
    return Token{ TK_END_OF_FILE, "" };
}

Token Parser::peek(int offset)
{
    int idx = pos + offset;
    if (idx < (int)tokens.size())
        return tokens[idx];
    return Token{ TK_END_OF_FILE, "" };
}

Token Parser::consume()
{
    Token t = current();
    if (!isAtEnd()) pos++;
    return t;
}

bool Parser::check(TokenType type)
{
    return current().type == type;
}

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        consume();
        return true;
    }
    return false;
}

bool Parser::expect(TokenType type, const std::string& errorMsg)
{
    if (check(type))
    {
        consume();
        return true;
    }
    errors.push_back("ERROR SINTACTICO: " + errorMsg +
        " (se encontro '" + current().value + "')");
    return false;
}

bool Parser::isAtEnd()
{
    return current().type == TK_END_OF_FILE;
}

// Avanza hasta un punto seguro para seguir parseando tras un error
void Parser::synchronize()
{
    while (!isAtEnd())
    {
        if (check(TK_SEMICOLON)) { consume(); return; }
        if (check(TK_RBRACE))             return;
        if (check(TK_IF) ||
            check(TK_FOR) ||
            check(TK_INICIO) ||
            check(TK_MOSTRAR)) return;
        consume();
    }
}

void Parser::parse()
{
    log.push_back("Inicio del analisis sintactico.");
    parseProgram();
    if (errors.empty())
        log.push_back("Analisis sintactico completado SIN errores.");
    else
        log.push_back("Analisis sintactico completado CON " +
            std::to_string(errors.size()) + " error(es).");
}

void Parser::parseProgram()
{
    while (!isAtEnd())
        parseStatement();
}

void Parser::parseStatement()
{
    Token t = current();

    // Declaración de variable: string | double
    if (t.type == TK_STRING || t.type == TK_DOUBLE)
    {
        log.push_back("Declaracion de variable detectada: " + t.value);
        parseVarDecl(t.type);
        return;
    }

    // if
    if (t.type == TK_IF)
    {
        log.push_back("Sentencia if detectada.");
        parseIfStmt();
        return;
    }

    // for
    if (t.type == TK_FOR)
    {
        log.push_back("Sentencia for detectada.");
        parseForStmt();
        return;
    }

    // mostrar / print
    if (t.type == TK_MOSTRAR || t.type == TK_PRINT)
    {
        log.push_back("Sentencia mostrar/print detectada.");
        parseMostrarStmt();
        return;
    }

    // Bloque { }
    if (t.type == TK_LBRACE)
    {
        log.push_back("Bloque detectado.");
        parseBlock();
        return;
    }

    // Asignación: IDENTIFIER = expr ;
    if (t.type == TK_IDENTIFIER)
    {
        // Comprobamos que el siguiente sea '='
        if (peek(1).type == TK_ASSIGN)
        {
            log.push_back("Asignacion detectada: " + t.value);
            parseAssignment();
            return;
        }

        // Si no es asignación, podría ser una llamada o expresión suelta
        errors.push_back("ERROR SINTACTICO: se esperaba '=' despues de '" +
            t.value + "' (se encontro '" + peek(1).value + "')");
        synchronize();
        return;
    }

    // Token inesperado
    errors.push_back("ERROR SINTACTICO: sentencia invalida, token inesperado '" +
        t.value + "'");
    consume();
    synchronize();
}

// varDecl → TYPE IDENTIFIER (= expr)? ;
void Parser::parseVarDecl(TokenType typeToken)
{
    consume(); // consume el tipo (string / double)

    if (!expect(TK_IDENTIFIER, "se esperaba un identificador despues del tipo"))
    {
        synchronize();
        return;
    }

    std::string varName = tokens[pos - 1].value;

    if (match(TK_ASSIGN))
    {
        log.push_back("Inicializacion de variable '" + varName + "'.");
        parseExpr();
    }

    if (!expect(TK_SEMICOLON, "se esperaba ';' al final de la declaracion de '" + varName + "'"))
        synchronize();
    else
        log.push_back("Declaracion de '" + varName + "' valida.");
}

// assignment → IDENTIFIER = expr ;
void Parser::parseAssignment()
{
    std::string varName = current().value;
    consume();              // IDENTIFIER
    consume();              // =
    parseExpr();

    if (!expect(TK_SEMICOLON, "se esperaba ';' al final de la asignacion a '" + varName + "'"))
        synchronize();
    else
        log.push_back("Asignacion a '" + varName + "' valida.");
}

// ifStmt → if ( expr ) block (else block)?
void Parser::parseIfStmt()
{
    consume(); // if

    if (!expect(TK_LPAREN, "se esperaba '(' despues de 'if'"))
    {
        synchronize(); return;
    }

    parseExpr();

    if (!expect(TK_RPAREN, "se esperaba ')' despues de la condicion del if"))
    {
        synchronize(); return;
    }

    if (!check(TK_LBRACE))
    {
        errors.push_back("ERROR SINTACTICO: se esperaba '{' para el cuerpo del if"
            " (se encontro '" + current().value + "')");
        synchronize();
        return;
    }
    parseBlock();

    if (match(TK_ELSE))
    {
        log.push_back("Rama else detectada.");
        if (!check(TK_LBRACE))
        {
            errors.push_back("ERROR SINTACTICO: se esperaba '{' para el cuerpo del else"
                " (se encontro '" + current().value + "')");
            synchronize();
            return;
        }
        parseBlock();
    }

    log.push_back("Sentencia if valida.");
}

// forStmt → for ( varDecl|assignment  expr ;  IDENTIFIER = expr ) block
void Parser::parseForStmt()
{
    consume(); // for

    if (!expect(TK_LPAREN, "se esperaba '(' despues de 'for'"))
    {
        synchronize(); return;
    }

    // Inicializacion
    Token t = current();
    if (t.type == TK_STRING || t.type == TK_DOUBLE)
    {
        // declaración con ; ya la consume parseVarDecl
        parseVarDecl(t.type);
    }
    else if (t.type == TK_IDENTIFIER && peek(1).type == TK_ASSIGN)
    {
        parseAssignment(); // consume el ;
    }
    else
    {
        errors.push_back("ERROR SINTACTICO: se esperaba inicializacion en el for"
            " (se encontro '" + t.value + "')");
        synchronize();
        return;
    }

    // Condicion
    parseExpr();
    if (!expect(TK_SEMICOLON, "se esperaba ';' despues de la condicion del for"))
    {
        synchronize(); return;
    }

    // Incremento/decremento: IDENTIFIER = expr  (sin ; al final, lo cierra el ')')
    if (!check(TK_IDENTIFIER))
    {
        errors.push_back("ERROR SINTACTICO: se esperaba identificador en el incremento del for"
            " (se encontro '" + current().value + "')");
        synchronize();
        return;
    }
    consume(); // IDENTIFIER
    if (!expect(TK_ASSIGN, "se esperaba '=' en el incremento del for"))
    {
        synchronize(); return;
    }
    parseExpr();

    if (!expect(TK_RPAREN, "se esperaba ')' para cerrar el for"))
    {
        synchronize(); return;
    }

    if (!check(TK_LBRACE))
    {
        errors.push_back("ERROR SINTACTICO: se esperaba '{' para el cuerpo del for"
            " (se encontro '" + current().value + "')");
        synchronize();
        return;
    }
    parseBlock();
    log.push_back("Sentencia for valida.");
}

// mostrarStmt → (mostrar|print) ( expr ) ;
void Parser::parseMostrarStmt()
{
    consume(); // mostrar / print

    if (!expect(TK_LPAREN, "se esperaba '(' despues de mostrar/print"))
    {
        synchronize(); return;
    }

    parseExpr();

    if (!expect(TK_RPAREN, "se esperaba ')' despues del argumento de mostrar/print"))
    {
        synchronize(); return;
    }

    if (!expect(TK_SEMICOLON, "se esperaba ';' al final de mostrar/print"))
        synchronize();
    else
        log.push_back("Sentencia mostrar/print valida.");
}

// block → { statement* }
void Parser::parseBlock()
{
    consume(); // {
    while (!isAtEnd() && !check(TK_RBRACE))
        parseStatement();

    if (!expect(TK_RBRACE, "se esperaba '}' para cerrar el bloque"))
        synchronize();
    else
        log.push_back("Bloque valido.");
}

// expr → comparison ((== | !=) comparison)*
void Parser::parseExpr()
{
    parseComparison();
    while (check(TK_EQUAL_EQUAL) || check(TK_NOT_EQUAL))
    {
        consume();
        parseComparison();
    }
}

// comparison → term ((< | > | <= | >=) term)*
void Parser::parseComparison()
{
    parseTerm();
    while (check(TK_LESS) || check(TK_GREATER) ||
        check(TK_LESS_EQUAL) || check(TK_GREATER_EQUAL))
    {
        consume();
        parseTerm();
    }
}

// term → factor ((+ | -) factor)*
void Parser::parseTerm()
{
    parseFactor();
    while (check(TK_PLUS) || check(TK_MINUS))
    {
        consume();
        parseFactor();
    }
}

// factor → unary ((* | /) unary)*
void Parser::parseFactor()
{
    parseUnary();
    while (check(TK_MULT) || check(TK_DIV))
    {
        consume();
        parseUnary();
    }
}

// unary → ! unary | primary
void Parser::parseUnary()
{
    if (check(TK_EXCLAMATION) || check(TK_MINUS))
    {
        consume();
        parseUnary();
        return;
    }
    parsePrimary();
}

// primary → NUMBER | STRING_LITERAL | CHAR_LITERAL | IDENTIFIER | ( expr )
void Parser::parsePrimary()
{
    Token t = current();

    if (t.type == TK_NUMBER ||
        t.type == TK_STRING_LITERAL ||
        t.type == TK_CHAR_LITERAL ||
        t.type == TK_IDENTIFIER)
    {
        consume();
        return;
    }

    if (t.type == TK_LPAREN)
    {
        consume();
        parseExpr();
        if (!expect(TK_RPAREN, "se esperaba ')' para cerrar la expresion"))
            synchronize();
        return;
    }

    errors.push_back("ERROR SINTACTICO: se esperaba un valor o expresion"
        " (se encontro '" + t.value + "')");
    consume();
}