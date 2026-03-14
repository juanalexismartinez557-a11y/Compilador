#pragma once
#include <vector> // almacena colecciones de tokens
#include <string>
#include "Token.h" // incluimos los token que creamos

class Lexer { // este basicamente sera nuestro analizador lexico

private:

    std::string input; // nuestro codigo fuente
    int pos; // posicion actual dentro del texto

public:
    std::vector<std::string> log; // se guardan los mensajes de error o informacion

    Lexer(std::string text); //constructor

    Token getNextToken(); //ser la funcion que usaremos para pbtener el siguiente token.

};