#pragma once
#include <string>

enum TokenType

    //ACABAR EL LEXICO
{
    // tipos de dato estándar
    TK_INT,    //Quitar
    TK_STRING,  ///// Dejar
    TK_FLOAT,    //Quitar
    TK_BOOL,     //Quitar
    TK_DOUBLE,    ///// Dejar
    TK_CHAR,     //Quitar
    TK_LONG,     //Quitar
    TK_VOID,    //Quitar

    // tipos de dato personalizados   
    TK_ENTERO,//Quitar
    TK_CADENA,//Quitar
    TK_DECIMAL,//Quitar

    // estructuras de control
    TK_IF,  //dejar
    TK_ELSE, //dejar
    TK_FOR, //dejar
    TK_WHILE,    /////Quitar este 

    // palabras reservadas creadas por ti  
    TK_INICIO,    ///Dejar
    TK_MOSTRAR,///Dejar
    TK_LEER,///Dejar
    TK_PRINT,///Dejar

    // identificadores y literales  
    TK_IDENTIFIER,//Quitar
    TK_NUMBER,      //Quitar
    TK_STRING_LITERAL,//Quitar
    TK_CHAR_LITERAL,//Quitar

    // operadores aritméticos  
    TK_PLUS,///Dejar
    TK_MINUS,///Dejar
    TK_MULT,///Dejar
    TK_DIV,///Dejar
    TK_MOD,///Dejar

    // operadores relacionales   
    TK_LESS,///Dejar
    TK_GREATER,///Dejar
    TK_LESS_EQUAL,///Dejar
    TK_GREATER_EQUAL,///Dejar

    // operadores de igualdad  
    TK_EQUAL_EQUAL,///Dejar
    TK_NOT_EQUAL,///Dejar

    // asignación   
    TK_ASSIGN,//Quitar

    // separadores   
    TK_SEMICOLON,//Quitar
    TK_COMMA,//Quitar

    // agrupadores 
    TK_LPAREN,//Quitar
    TK_RPAREN,//Quitar
    TK_LBRACE,//Quitar
    TK_RBRACE,//Quitar

    // símbolos especiales   
    TK_HASH,///Dejar
    TK_DOLLAR,///Dejar
    TK_EXCLAMATION,///Dejar
    TK_QUESTION,///Dejar
    TK_AMPERSAND,///Dejar
    TK_AT,///Dejar

    // token desconocido    
    TK_UNKNOWN,///Dejar

    // fin de archivo   
    TK_END_OF_FILE///Dejar
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
	case TK_INT: return "INT";  //Quitar
	case TK_STRING: return "STRING"; ///// Dejar
	case TK_FLOAT: return "FLOAT"; ///Quitar
	case TK_BOOL: return "BOOL"; //Quitar
	case TK_DOUBLE: return "DOUBLE"; ///Dejar
	case TK_CHAR: return "CHAR"; // Quitar
	case TK_LONG: return "LONG"; //Quitar
	case TK_VOID: return "VOID"; //Quitar

	case TK_ENTERO: return "ENTERO"; //Quitar
	case TK_CADENA: return "CADENA"; //Quitar
	case TK_DECIMAL: return "DECIMAL"; ///Quitar

    case TK_IF: return "IF";
    case TK_ELSE: return "ELSE";
    case TK_FOR: return "FOR";
    case TK_WHILE: return "WHILE";

    case TK_INICIO: return "INICIO";
    case TK_MOSTRAR: return "MOSTRAR";
    case TK_LEER: return "LEER";
    case TK_PRINT: return "PRINT";

    case TK_IDENTIFIER: return "IDENTIFIER";
    case TK_NUMBER: return "NUMBER";
    case TK_STRING_LITERAL: return "STRING_LITERAL";
    case TK_CHAR_LITERAL: return "CHAR_LITERAL";

    case TK_PLUS: return "PLUS";
    case TK_MINUS: return "MINUS";
    case TK_MULT: return "MULT";
    case TK_DIV: return "DIV";
    case TK_MOD: return "MOD";

    case TK_LESS: return "LESS";
    case TK_GREATER: return "GREATER";
    case TK_LESS_EQUAL: return "LESS_EQUAL";
    case TK_GREATER_EQUAL: return "GREATER_EQUAL";

    case TK_EQUAL_EQUAL: return "EQUAL_EQUAL";
    case TK_NOT_EQUAL: return "NOT_EQUAL";

    case TK_ASSIGN: return "ASSIGN";

    case TK_SEMICOLON: return "SEMICOLON";
    case TK_COMMA: return "COMMA";

    case TK_LPAREN: return "LPAREN";
    case TK_RPAREN: return "RPAREN";
    case TK_LBRACE: return "LBRACE";
    case TK_RBRACE: return "RBRACE";

    case TK_HASH: return "HASH";
    case TK_DOLLAR: return "DOLLAR";
    case TK_EXCLAMATION: return "EXCLAMATION";
    case TK_QUESTION: return "QUESTION";
    case TK_AMPERSAND: return "AMPERSAND";
    case TK_AT: return "AT";

    case TK_UNKNOWN: return "UNKNOWN";
    case TK_END_OF_FILE: return "END_OF_FILE";

    default: return "UNKNOWN";
    }
}