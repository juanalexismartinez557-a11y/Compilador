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