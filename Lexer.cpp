#include "Lexer.h"
#include <cctype>

Lexer::Lexer(std::string text)
{
    input = text;
    pos = 0;
}

Token Lexer::getNextToken()
{
    while (pos < (int)input.length())
    {
        char c = input[pos];

        if (isspace((unsigned char)c))
        {
            log.push_back("Espacio en blanco encontrado, se ignora.");
            pos++;
            continue;
        }

        if (isdigit((unsigned char)c))
        {
            std::string number;
            bool hasDot = false;

            while (pos < (int)input.length())
            {
                if (isdigit((unsigned char)input[pos]))
                {
                    number += input[pos];
                    pos++;
                }
                else if (input[pos] == '.' && !hasDot)
                {
                    hasDot = true;
                    number += input[pos];
                    pos++;
                }
                else
                {
                    break;
                }
            }

            log.push_back("Número encontrado: " + number);
            return Token{ TK_NUMBER, number };
        }

        if (isalpha((unsigned char)c) || c == '_')
        {
            std::string id;

            while (pos < (int)input.length() &&
                (isalnum((unsigned char)input[pos]) || input[pos] == '_'))
            {
                id += input[pos];
                pos++;
            }

            log.push_back("Identificador o palabra reservada encontrada: " + id);

            if (id == "int" || id == "INT") return Token{ TK_INT, id };
            if (id == "string" || id == "STRING") return Token{ TK_STRING, id };
            if (id == "float" || id == "FLOAT") return Token{ TK_FLOAT, id };
            if (id == "bool" || id == "BOOL") return Token{ TK_BOOL, id };
            if (id == "double" || id == "DOUBLE") return Token{ TK_DOUBLE, id };
            if (id == "char" || id == "CHAR") return Token{ TK_CHAR, id };
            if (id == "long" || id == "LONG") return Token{ TK_LONG, id };
            if (id == "void" || id == "VOID") return Token{ TK_VOID, id };

            if (id == "entero" || id == "ENTERO") return Token{ TK_ENTERO, id };
            if (id == "cadena" || id == "CADENA") return Token{ TK_CADENA, id };
            if (id == "decimal" || id == "DECIMAL") return Token{ TK_DECIMAL, id };

            if (id == "if" || id == "IF") return Token{ TK_IF, id };
            if (id == "else" || id == "ELSE") return Token{ TK_ELSE, id };
            if (id == "for" || id == "FOR") return Token{ TK_FOR, id };
            if (id == "while" || id == "WHILE") return Token{ TK_WHILE, id };

            if (id == "inicio" || id == "INICIO") return Token{ TK_INICIO, id };
            if (id == "mostrar" || id == "MOSTRAR") return Token{ TK_MOSTRAR, id };
            if (id == "leer" || id == "LEER") return Token{ TK_LEER, id };
            if (id == "print" || id == "PRINT") return Token{ TK_PRINT, id };

            return Token{ TK_IDENTIFIER, id };
        }

        if (c == '"')
        {
            pos++;
            std::string str;

            while (pos < (int)input.length() && input[pos] != '"')
            {
                str += input[pos];
                pos++;
            }

            if (pos < (int)input.length() && input[pos] == '"')
            {
                pos++;
                log.push_back("Cadena encontrada: " + str);
                return Token{ TK_STRING_LITERAL, str };
            }

            log.push_back("Error: cadena no cerrada.");
            return Token{ TK_UNKNOWN, str };
        }

        if (c == '\'')
        {
            pos++;
            std::string ch;

            while (pos < (int)input.length() && input[pos] != '\'')
            {
                ch += input[pos];
                pos++;
            }

            if (pos < (int)input.length() && input[pos] == '\'')
            {
                pos++;
                if (ch.length() == 1)
                {
                    log.push_back("Carácter encontrado: " + ch);
                    return Token{ TK_CHAR_LITERAL, ch };
                }

                log.push_back("Error: literal de carácter inválido: " + ch);
                return Token{ TK_UNKNOWN, ch };
            }

            log.push_back("Error: carácter no cerrado.");
            return Token{ TK_UNKNOWN, ch };
        }

        switch (c)
        {
        case '+': pos++; return Token{ TK_PLUS, "+" };
        case '-': pos++; return Token{ TK_MINUS, "-" };
        case '*': pos++; return Token{ TK_MULT, "*" };
        case '/': pos++; return Token{ TK_DIV, "/" };
        case '%': pos++; return Token{ TK_MOD, "%" };

        case '<':
            pos++;
            if (pos < (int)input.length() && input[pos] == '=')
            {
                pos++;
                return Token{ TK_LESS_EQUAL, "<=" };
            }
            return Token{ TK_LESS, "<" };

        case '>':
            pos++;
            if (pos < (int)input.length() && input[pos] == '=')
            {
                pos++;
                return Token{ TK_GREATER_EQUAL, ">=" };
            }
            return Token{ TK_GREATER, ">" };

        case '=':
            pos++;
            if (pos < (int)input.length() && input[pos] == '=')
            {
                pos++;
                return Token{ TK_EQUAL_EQUAL, "==" };
            }
            return Token{ TK_ASSIGN, "=" };

        case '!':
            pos++;
            if (pos < (int)input.length() && input[pos] == '=')
            {
                pos++;
                return Token{ TK_NOT_EQUAL, "!=" };
            }
            return Token{ TK_EXCLAMATION, "!" };

        case ';': pos++; return Token{ TK_SEMICOLON, ";" };
        case ',': pos++; return Token{ TK_COMMA, "," };
        case '(': pos++; return Token{ TK_LPAREN, "(" };
        case ')': pos++; return Token{ TK_RPAREN, ")" };
        case '{': pos++; return Token{ TK_LBRACE, "{" };
        case '}': pos++; return Token{ TK_RBRACE, "}" };
        case '#': pos++; return Token{ TK_HASH, "#" };
        case '$': pos++; return Token{ TK_DOLLAR, "$" };
        case '?': pos++; return Token{ TK_QUESTION, "?" };
        case '&': pos++; return Token{ TK_AMPERSAND, "&" };
        case '@': pos++; return Token{ TK_AT, "@" };

        default:
        {
            std::string unknown(1, c);
            log.push_back("Token desconocido encontrado: " + unknown);
            pos++;
            return Token{ TK_UNKNOWN, unknown };
        }
        }
    }

    return Token{ TK_END_OF_FILE, "" };
}