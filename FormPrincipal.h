#pragma once
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Lexer.h"
#include "Sintactico.h"
#include "Semantico.h"

#include <msclr/marshal_cppstd.h>
#include <string>
#include <vector>

namespace CompiladorWinForms
{
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class FormPrincipal : public Form
    {
    private:
        // TextBoxes
        TextBox^ editor;
        TextBox^ salidaLexico;
        TextBox^ salidaSintactico;
        TextBox^ salidaSemantico;

        // Botones
        Button^ btnTodos;
        Button^ btnLexico;
        Button^ btnSintactico;
        Button^ btnSemantico;

    public:
        FormPrincipal()
        {
            this->Width = 1100;
            this->Height = 650;
            this->Text = "Compilador - Analizador Completo";

            int panelW = 520;
            int panelH = 260;

            // ================= PANEL CODIGO =================
            Panel^ panelCodigo = gcnew Panel();
            panelCodigo->SetBounds(10, 10, panelW, panelH);

            btnTodos = gcnew Button();
            btnTodos->Text = "Todos los análisis";
            btnTodos->Dock = DockStyle::Top;
            btnTodos->Height = 30;
            btnTodos->Click += gcnew EventHandler(this, &FormPrincipal::AnalizarTodo);

            editor = gcnew TextBox();
            editor->Multiline = true;
            editor->Dock = DockStyle::Fill;
            editor->Font = gcnew System::Drawing::Font("Consolas", 10);
            editor->ScrollBars = ScrollBars::Vertical;

            panelCodigo->Controls->Add(editor);
            panelCodigo->Controls->Add(btnTodos);

            // ================= PANEL LEXICO =================
            Panel^ panelLexico = gcnew Panel();
            panelLexico->SetBounds(550, 10, panelW, panelH);

            btnLexico = gcnew Button();
            btnLexico->Text = "Análisis Léxico";
            btnLexico->Dock = DockStyle::Top;
            btnLexico->Height = 30;
            btnLexico->Click += gcnew EventHandler(this, &FormPrincipal::AnalizarLexico);

            salidaLexico = gcnew TextBox();
            salidaLexico->Multiline = true;
            salidaLexico->Dock = DockStyle::Fill;
            salidaLexico->ReadOnly = true;
            salidaLexico->Font = gcnew System::Drawing::Font("Consolas", 9);
            salidaLexico->ScrollBars = ScrollBars::Vertical;

            panelLexico->Controls->Add(salidaLexico);
            panelLexico->Controls->Add(btnLexico);

            // ================= PANEL SINTACTICO =================
            Panel^ panelSintactico = gcnew Panel();
            panelSintactico->SetBounds(10, 290, panelW, panelH);

            btnSintactico = gcnew Button();
            btnSintactico->Text = "Análisis Sintáctico";
            btnSintactico->Dock = DockStyle::Top;
            btnSintactico->Height = 30;
            btnSintactico->Click += gcnew EventHandler(this, &FormPrincipal::AnalizarSintactico);

            salidaSintactico = gcnew TextBox();
            salidaSintactico->Multiline = true;
            salidaSintactico->Dock = DockStyle::Fill;
            salidaSintactico->ReadOnly = true;
            salidaSintactico->Font = gcnew System::Drawing::Font("Consolas", 9);
            salidaSintactico->ScrollBars = ScrollBars::Vertical;

            panelSintactico->Controls->Add(salidaSintactico);
            panelSintactico->Controls->Add(btnSintactico);

            // ================= PANEL SEMANTICO =================
            Panel^ panelSemantico = gcnew Panel();
            panelSemantico->SetBounds(550, 290, panelW, panelH);

            btnSemantico = gcnew Button();
            btnSemantico->Text = "Análisis Semántico";
            btnSemantico->Dock = DockStyle::Top;
            btnSemantico->Height = 30;
            btnSemantico->Click += gcnew EventHandler(this, &FormPrincipal::AnalizarSemantico);

            salidaSemantico = gcnew TextBox();
            salidaSemantico->Multiline = true;
            salidaSemantico->Dock = DockStyle::Fill;
            salidaSemantico->ReadOnly = true;
            salidaSemantico->Font = gcnew System::Drawing::Font("Consolas", 9);
            salidaSemantico->ScrollBars = ScrollBars::Vertical;

            panelSemantico->Controls->Add(salidaSemantico);
            panelSemantico->Controls->Add(btnSemantico);

            // Agregar todo al form
            Controls->Add(panelCodigo);
            Controls->Add(panelLexico);
            Controls->Add(panelSintactico);
            Controls->Add(panelSemantico);
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

        // ================= LEXICO =================
        void AnalizarLexico(Object^ sender, EventArgs^ e)
        {
            salidaLexico->Clear();

            std::string codigo =
                msclr::interop::marshal_as<std::string>(editor->Text);

            Lexer lexer(codigo);

            // TOKENS
            while (true)
            {
                Token t = lexer.getNextToken();
                if (t.type == TK_END_OF_FILE) break;

                std::string tipo = tokenTypeToString(t.type);
                std::string linea = "TOKEN: " + t.value + " | TIPO: " + tipo;

                salidaLexico->AppendText(gcnew String(linea.c_str()) + "\r\n");
            }

            // LOGS
            salidaLexico->AppendText("\r\n--- PROCESO DEL ANALISIS ---\r\n");

            for (size_t i = 0; i < lexer.log.size(); i++)
            {
                salidaLexico->AppendText(
                    gcnew String(lexer.log[i].c_str()) + "\r\n");
            }
        }

        // ================= SINTACTICO =================
        void AnalizarSintactico(Object^ sender, EventArgs^ e)
        {
            salidaSintactico->Clear();

            std::vector<Token> tokens = obtenerTokens();
            Parser parser(tokens);
            parser.parse();

            if (parser.errors.empty())
            {
                salidaSintactico->AppendText("=== SINTAXIS CORRECTA ===\r\n\r\n");
            }
            else
            {
                salidaSintactico->AppendText("=== ERRORES ===\r\n\r\n");

                for (auto& err : parser.errors)
                {
                    salidaSintactico->AppendText(
                        gcnew String(err.c_str()) + "\r\n");
                }
                salidaSintactico->AppendText("\r\n");
            }

            // LOGS
            salidaSintactico->AppendText("--- PROCESO ---\r\n");

            for (auto& l : parser.log)
            {
                salidaSintactico->AppendText(
                    gcnew String(l.c_str()) + "\r\n");
            }
        }

        // ================= SEMANTICO =================
        void AnalizarSemantico(Object^ sender, EventArgs^ e)
        {
            salidaSemantico->Clear();

            std::vector<Token> tokens = obtenerTokens();

            Parser parser(tokens);
            parser.parse();

            if (!parser.errors.empty())
            {
                salidaSemantico->AppendText(
                    "Error: no se puede ejecutar el análisis semántico\n");
                salidaSemantico->AppendText(
                    "Corrige primero el sintáctico.\n");
                return;
            }

            Semantico sem(tokens);
            sem.analyze();

            // LOGS
            salidaSemantico->AppendText("--- PROCESO ---\r\n");

            for (auto& l : sem.log)
            {
                salidaSemantico->AppendText(
                    gcnew String(l.c_str()) + "\r\n");
            }

            // ERRORES SEMÁNTICOS
            if (!sem.errors.empty())
            {
                salidaSemantico->AppendText("\r\n--- ERRORES ---\r\n");

                for (auto& e : sem.errors)
                {
                    salidaSemantico->AppendText(
                        gcnew String(e.c_str()) + "\r\n");
                }
            }
            else
            {
                salidaSemantico->AppendText(
                    "\r\n=== SEMANTICA CORRECTA ===\r\n");
            }
        }

        // ================= TODOS =================
        void AnalizarTodo(Object^ sender, EventArgs^ e)
        {
            AnalizarLexico(nullptr, nullptr);
            AnalizarSintactico(nullptr, nullptr);
            AnalizarSemantico(nullptr, nullptr);
        }
    };
}