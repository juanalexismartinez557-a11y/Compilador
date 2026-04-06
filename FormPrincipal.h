#pragma once
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Lexer.h"
#include "Sintactico.h"
#include <msclr/marshal_cppstd.h>
#include <string>
#include <vector>

namespace CompiladorWinForms
{
    using namespace System;
    using namespace System::Windows::Forms;

    public ref class FormPrincipal : public Form
    {
    private:
        TextBox^ editor;
        TextBox^ salidaLexico;
        TextBox^ salidaSintactico;
        Button^ btnLexico;
        Button^ btnSintactico;

    public:
        FormPrincipal()
        {
            this->Width = 1100;
            this->Height = 560;
            this->Text = "Compilador - Analizador Lexico y Sintactico";

            editor = gcnew TextBox();
            editor->Multiline = true;
            editor->Width = 320;
            editor->Height = 460;
            editor->Left = 10;
            editor->Top = 10;
            editor->ScrollBars = ScrollBars::Vertical;
            editor->Font = gcnew System::Drawing::Font("Consolas", 10);

            salidaLexico = gcnew TextBox();
            salidaLexico->Multiline = true;
            salidaLexico->Width = 320;
            salidaLexico->Height = 460;
            salidaLexico->Left = 380;
            salidaLexico->Top = 10;
            salidaLexico->ScrollBars = ScrollBars::Vertical;
            salidaLexico->ReadOnly = true;
            salidaLexico->Font = gcnew System::Drawing::Font("Consolas", 9);

            salidaSintactico = gcnew TextBox();
            salidaSintactico->Multiline = true;
            salidaSintactico->Width = 320;
            salidaSintactico->Height = 460;
            salidaSintactico->Left = 750;
            salidaSintactico->Top = 10;
            salidaSintactico->ScrollBars = ScrollBars::Vertical;
            salidaSintactico->ReadOnly = true;
            salidaSintactico->Font = gcnew System::Drawing::Font("Consolas", 9);

            btnLexico = gcnew Button();
            btnLexico->Text = "Analizar Lexico";
            btnLexico->Left = 340;
            btnLexico->Top = 200;
            btnLexico->Width = 30;
            btnLexico->Height = 35;
            btnLexico->Click += gcnew EventHandler(this, &FormPrincipal::AnalizarLexico);

            btnSintactico = gcnew Button();
            btnSintactico->Text = "Analizar Sint.";
            btnSintactico->Left = 710;
            btnSintactico->Top = 200;
            btnSintactico->Width = 30;
            btnSintactico->Height = 35;
            btnSintactico->Click += gcnew EventHandler(this, &FormPrincipal::AnalizarSintactico);

            Controls->Add(editor);
            Controls->Add(salidaLexico);
            Controls->Add(salidaSintactico);
            Controls->Add(btnLexico);
            Controls->Add(btnSintactico);
        }

    private:
        std::vector<Token> obtenerTokens()
        {
            std::string codigo =
                msclr::interop::marshal_as<std::string>(editor->Text);
            Lexer lexer(codigo);
            std::vector<Token> tokens;
            while (true)
            {
                Token t = lexer.getNextToken();
                if (t.type == TK_END_OF_FILE) break;
                tokens.push_back(t);
            }
            return tokens;
        }

        void AnalizarLexico(Object^ sender, EventArgs^ e)
        {
            salidaLexico->Clear();
            std::string codigo =
                msclr::interop::marshal_as<std::string>(editor->Text);
            Lexer lexer(codigo);

            while (true)
            {
                Token t = lexer.getNextToken();
                if (t.type == TK_END_OF_FILE) break;

                std::string tipo = tokenTypeToString(t.type);
                std::string linea = "TOKEN: " + t.value + "  |  TIPO: " + tipo;
                salidaLexico->AppendText(gcnew String(linea.c_str()));
                salidaLexico->AppendText("\r\n");
            }

            salidaLexico->AppendText("\r\n--- PROCESO DEL ANALISIS ---\r\n");
            for (size_t i = 0; i < lexer.log.size(); i++)
            {
                salidaLexico->AppendText(gcnew String(lexer.log[i].c_str()));
                salidaLexico->AppendText("\r\n");
            }
        }

        void AnalizarSintactico(Object^ sender, EventArgs^ e)
        {
            salidaSintactico->Clear();
            std::vector<Token> tokens = obtenerTokens();

            Parser parser(tokens);
            parser.parse();

            if (parser.errors.empty())
            {
                salidaSintactico->AppendText("=== RESULTADO: SINTAXIS CORRECTA ===\r\n\r\n");
            }
            else
            {
                salidaSintactico->AppendText("=== RESULTADO: ERRORES ENCONTRADOS ===\r\n\r\n");
                for (size_t i = 0; i < parser.errors.size(); i++)
                {
                    salidaSintactico->AppendText(
                        gcnew String(parser.errors[i].c_str()));
                    salidaSintactico->AppendText("\r\n");
                }
                salidaSintactico->AppendText("\r\n");
            }

            salidaSintactico->AppendText("--- PROCESO DEL ANALISIS ---\r\n");
            for (size_t i = 0; i < parser.log.size(); i++)
            {
                salidaSintactico->AppendText(
                    gcnew String(parser.log[i].c_str()));
                salidaSintactico->AppendText("\r\n");
            }
        }
    };
}