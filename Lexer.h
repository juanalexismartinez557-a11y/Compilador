#pragma once
#include <vector> // almacena colecciones de tokens
#include <string>
#include "Token.h" // incluimos los token que creamos

class Lexer { // este basicamente sera nuestro analizador lexico

private:

    std::string input; // nuestro codigo fuente
    int pos; // posicion actual dentro del texto

    char current(); // devolvera el caracter que se esta leyendo en el momento

public:

    Lexer(std::string text); //constructor

    Token getNextToken(); //ser la funcion que usaremos para pbtener el siguiente token.

};