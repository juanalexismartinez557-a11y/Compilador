#pragma once
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Lexer.h"
#include <msclr/marshal_cppstd.h>
#include <string>

namespace CompiladorWinForms
{
    using namespace System;
    using namespace System::Windows::Forms;

    public ref class FormPrincipal : public Form
    {
    private:
        TextBox^ editor;
        TextBox^ salida;
        Button^ analizar;

    public:
        FormPrincipal()
        {
            this->Width = 900;
            this->Height = 500;
            this->Text = "Compilador - Analizador Léxico";

            editor = gcnew TextBox();
            editor->Multiline = true;
            editor->Width = 350;
            editor->Height = 400;
            editor->Left = 10;
            editor->Top = 10;
            editor->ScrollBars = ScrollBars::Vertical;

            salida = gcnew TextBox();
            salida->Multiline = true;
            salida->Width = 350;
            salida->Height = 400;
            salida->Left = 520;
            salida->Top = 10;
            salida->ScrollBars = ScrollBars::Vertical;

            analizar = gcnew Button();
            analizar->Text = "Analizar Léxico";
            analizar->Left = 380;
            analizar->Top = 200;
            analizar->Width = 120;
            analizar->Height = 35;

            analizar->Click += gcnew EventHandler(this, &FormPrincipal::Analizar);

            Controls->Add(editor);
            Controls->Add(salida);
            Controls->Add(analizar);
        }

    private:
        void Analizar(Object^ sender, EventArgs^ e)
        {
            salida->Clear();

            std::string codigo = msclr::interop::marshal_as<std::string>(editor->Text);
            Lexer lexer(codigo);

            while (true)
            {
                Token t = lexer.getNextToken();

                if (t.type == TK_END_OF_FILE)
                    break;

                std::string tipo = tokenTypeToString(t.type);
                std::string linea = "TOKEN: " + t.value + "  |  TIPO: " + tipo;

                salida->AppendText(gcnew String(linea.c_str()));
                salida->AppendText("\r\n");
            }

            salida->AppendText("\r\n--- PROCESO DEL ANALISIS ---\r\n");

            for (size_t i = 0; i < lexer.log.size(); i++)
            {
                salida->AppendText(gcnew String(lexer.log[i].c_str()));
                salida->AppendText("\r\n");
            }
        }
    };
}