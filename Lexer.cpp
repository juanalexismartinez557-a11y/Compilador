#include "Lexer.h"
#include <cctype> // nos permite analizar caracteres.

Lexer::Lexer(std::string text) { // constructor

    input = text;
    pos = 0;

}

char Lexer::current() { // devuelve el caracter actual del texto

    if (pos >= input.length())
        return '\0'; // esto es para cuando acabe de revisar todo

    return input[pos];

}

Token Lexer::getNextToken() { // aqui se bisca el token que va a seguir 

    while (pos < input.length()) { // basicamente buscamos hasta que termine de revisar el texto

        char c = current(); // guardamos el caracter 

        if (isspace(c)) { // aqui hace que se ignoren los espacion (parte importante del analizador lexico)
            pos++; // hacemos que avance 
            continue;
        }

        if (isdigit(c)) { // aqui revisamos si el caracter es un numero

            std::string number;

            while (isdigit(current())) { // con este while hacemos que si hay varios digitos juntos los ponga dentro del mismo token
                number += current(); // le asignamos el valor
                pos++;
            }

            return { TokenType::NUMBER, number }; // lo metemos como NUMBER de los tipos de token que creamos
        }

        if (isalpha(c)) { //  con esto revisamos identificadores y palabras reservadad. Porque puede ser tanto una variable como una palabra reservada.

            std::string id; // el acumulador

            while (isalnum(current())) { // lee letras y numeros
                id += current();
                pos++;
            }

            // aqui revisamos si son palabras reservadas (son las que ya registramos
            // por simplicidad solamente acepta si es todo mayuscula o todo minuscula
            if (id == "int" || id == "INT") return {TokenType::INT, id};
            if (id == "string" || id == "STRING") return {TokenType::STRING, id};
            if (id == "if" || id == "IF") return {TokenType::IF, id};
            if (id == "else" || id == "ELSE") return {TokenType::ELSE, id};
            if (id == "for" || id == "FOR") return {TokenType::FOR, id};

            return { TokenType::IDENTIFIER, id };
        }

		switch (c) { // revisa operadores y simbolos (basicamente lo que no es letra ni numero)
        
        // nuevamente revsamos los que agregamos en tokens
        case '+': pos++; return { TokenType::PLUS, "+" };
        case '-': pos++; return { TokenType::MINUS, "-" };
        case '*': pos++; return { TokenType::MULT, "*" };
        case '/': pos++; return { TokenType::DIV, "/" };
        case '%': pos++; return { TokenType::MOD, "%" };

		case '<': // aqui revisamos si es < o <=
            pos++;
            if (current() == '=') { pos++; return { TokenType::LESS_EQUAL,"<=" }; }
            return { TokenType::LESS,"<" };

		case '>': // rwevisamos si es > o >=
            pos++;
            if (current() == '=') { pos++; return { TokenType::GREATER_EQUAL,">=" }; }
            return { TokenType::GREATER,">" };

		case '=': // revisamos si es = o ==
            pos++;
            if (current() == '=') { pos++; return { TokenType::EQUAL_EQUAL,"==" }; }
            break;

		case '!': // revisamos si es ! o !=
            pos++;
            if (current() == '=') { pos++; return { TokenType::NOT_EQUAL,"!=" }; }
            return { TokenType::EXCLAMATION,"!" };

		case ';': pos++; return { TokenType::SEMICOLON,";" }; // fin de linea
        
				// simbolos que agregamos
        case '#': pos++; return { TokenType::HASH,"#" };
        case '$': pos++; return { TokenType::DOLLAR,"$" };
        case '?': pos++; return { TokenType::QUESTION,"?" };

		default: // si no es nada de lo que reconocemos, simplemente lo ignoramos y avanzamos
            pos++;
            break;
        }

    }

    return { TokenType::END,"" }; // con esto revisamos si ya revisamos todo
}