#pragma once
#include <string>

enum TokenType
{
    TK_STRING,
    TK_DOUBLE,

    TK_IF,
    TK_ELSE,
    TK_FOR,

    TK_INICIO,
    TK_MOSTRAR,
    TK_LEER,
    TK_PRINT,
    TK_PRUEBA,

    TK_IDENTIFIER,
    TK_NUMBER,
    TK_STRING_LITERAL,
    TK_CHAR_LITERAL,

    TK_SUMA,
    TK_INCREMENTO,
    TK_MINUS,
    TK_MULT,
    TK_DIV,
    TK_MOD,

    TK_LESS,
    TK_GREATER,
    TK_LESS_EQUAL,
    TK_GREATER_EQUAL,

    TK_EQUAL_EQUAL,
    TK_NOT_EQUAL,

    TK_ASSIGN,

    TK_SEMICOLON,
    TK_COMMA,

    TK_LPAREN,
    TK_RPAREN,
    TK_LBRACE,
    TK_RBRACE,

    TK_HASH,
    TK_DOLLAR,
    TK_EXCLAMATION,
    TK_QUESTION,
    TK_AMPERSAND,
    TK_AT,

    TK_UNKNOWN,

    TK_END_OF_FILE
};

struct Token
{
    TokenType type;
    std::string value;
};

inline std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TK_STRING:         return "STRING";
    case TK_DOUBLE:         return "DOUBLE";
    case TK_IF:             return "IF";
    case TK_ELSE:           return "ELSE";
    case TK_FOR:            return "FOR";
    case TK_INICIO:         return "INICIO";
    case TK_MOSTRAR:        return "MOSTRAR";
    case TK_LEER:           return "LEER";
    case TK_PRINT:          return "PRINT";
    case TK_PRUEBA:          return "PRUEBA";
    case TK_IDENTIFIER:     return "IDENTIFIER";
    case TK_NUMBER:         return "NUMBER";
    case TK_STRING_LITERAL: return "STRING_LITERAL";
    case TK_CHAR_LITERAL:   return "CHAR_LITERAL";
    case TK_SUMA:           return "SUMA";
    case TK_INCREMENTO:     return "INCREMENTO";
    case TK_MINUS:          return "MINUS";
    case TK_MULT:           return "MULT";
    case TK_DIV:            return "DIV";
    case TK_MOD:            return "MOD";
    case TK_LESS:           return "LESS";
    case TK_GREATER:        return "GREATER";
    case TK_LESS_EQUAL:     return "LESS_EQUAL";
    case TK_GREATER_EQUAL:  return "GREATER_EQUAL";
    case TK_EQUAL_EQUAL:    return "EQUAL_EQUAL";
    case TK_NOT_EQUAL:      return "NOT_EQUAL";
    case TK_ASSIGN:         return "ASSIGN";
    case TK_SEMICOLON:      return "SEMICOLON";
    case TK_COMMA:          return "COMMA";
    case TK_LPAREN:         return "LPAREN";
    case TK_RPAREN:         return "RPAREN";
    case TK_LBRACE:         return "LBRACE";
    case TK_RBRACE:         return "RBRACE";
    case TK_HASH:           return "HASH";
    case TK_DOLLAR:         return "DOLLAR";
    case TK_EXCLAMATION:    return "EXCLAMATION";
    case TK_QUESTION:       return "QUESTION";
    case TK_AMPERSAND:      return "AMPERSAND";
    case TK_AT:             return "AT";
    case TK_UNKNOWN:        return "UNKNOWN";
    case TK_END_OF_FILE:    return "END_OF_FILE";
    default:                return "UNKNOWN";
    }
}