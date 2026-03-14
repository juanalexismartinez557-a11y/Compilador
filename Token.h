#pragma once
#include <string> //con esta libreria almacenamos el texto del token

enum class TokenType { // creamos la clase token donde se encuentran todos los identificadores que estaran dentro de nuestro compilador
    // tipos de dato
    INT,
    STRING,
    //estructuras de control
    IF,
    ELSE,
    FOR,
    //identificadores
    IDENTIFIER,
    NUMBER,
    STRING_LITERAL,
    //aritmetica
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    // operadores relacionales
    LESS, 
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    // de igualdad
    EQUAL_EQUAL,
    NOT_EQUAL,
    // fin de linea
    SEMICOLON,
    // simbolos
    HASH, // #
    DOLLAR, // $
    EXCLAMATION, // !
    QUESTION, // ?
    INV_QUESTION, // ¿
    INV_EXCLAMATION, // ¡

    END // fin del programa
};

// estructura para los tokens solos
struct Token {

    TokenType type; // tipo (basicamente son los que pusimos arriba)
    std::string value; // texto original

};

static std::string tokenTypeToString(TokenType type) // con esta funcion en la salida muestra el tipo de token
{
    switch (type)
    {
    case TokenType::INT: return "INT";
    case TokenType::STRING: return "STRING";
    case TokenType::IF: return "IF";
    case TokenType::ELSE: return "ELSE";
    case TokenType::FOR: return "FOR";
    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::NUMBER: return "NUMBER";
    case TokenType::STRING_LITERAL: return "STRING_LITERAL";

    case TokenType::PLUS: return "PLUS";
    case TokenType::MINUS: return "MINUS";
    case TokenType::MULT: return "MULT";
    case TokenType::DIV: return "DIV";
    case TokenType::MOD: return "MOD";

    case TokenType::LESS: return "LESS";
    case TokenType::GREATER: return "GREATER";
    case TokenType::LESS_EQUAL: return "LESS_EQUAL";
    case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";

    case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
    case TokenType::NOT_EQUAL: return "NOT_EQUAL";

    case TokenType::SEMICOLON: return "SEMICOLON";

    case TokenType::HASH: return "HASH";
    case TokenType::DOLLAR: return "DOLLAR";
    case TokenType::EXCLAMATION: return "EXCLAMATION";
    case TokenType::QUESTION: return "QUESTION";

    default: return "UNKNOWN";
    }
}