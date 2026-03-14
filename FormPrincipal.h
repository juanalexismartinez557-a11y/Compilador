#pragma once
// agrego librerias necesarias para el funcionamiento del programa 
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>
#include "Lexer.h" // incluimos el analizador lexico que creamos
#include <msclr/marshal_cppstd.h> // se usa para convertir entre tipos de cadena de C++ y .NET

namespace CompiladorWinForms { // creamos el espacio de nombres para nuestro programa

    //estos son para usar todo lo de forms
    using namespace System;
    using namespace System::Windows::Forms;

    public ref class FormPrincipal : public Form // clase principal
    {

    private:

        TextBox^ editor; // aqui escribiremos l codigo
		TextBox^ salida; // aqui se mostrara el resultado del analisis lexico
		Button^ analizar; // con este inicia el analizador lexico

    public:

		FormPrincipal() // constructor del formulario
        {
            // le damos un tamaño
            this->Width = 900;
            this->Height = 500;

			editor = gcnew TextBox(); // este es el editor de texto donde se escribira el codigo
			editor->Multiline = true; // podemos escribir en varias lineas
            editor->Width = 350;
            editor->Height = 400;
			// le damos una posicion dentro del formulario
            editor->Left = 10; 
            editor->Top = 10;

			salida = gcnew TextBox(); // aqui sale el resultado del analisis lexico
            salida->Multiline = true;
            salida->Width = 350;
            salida->Height = 400;
            salida->Left = 520; // posicion
            salida->Top = 10;

            analizar = gcnew Button(); // boton
			analizar->Text = "Analizar Léxico"; // texto del boton
            analizar->Left = 380;
            analizar->Top = 200;

            analizar->Click += gcnew EventHandler(this, &FormPrincipal::Analizar); // basicamente llama a la funcion Analizar cuando el boton se preciona

			// agregamos los controles al formulario
            Controls->Add(editor);
            Controls->Add(salida);
            Controls->Add(analizar);
        }

    private:

		void Analizar(Object^ sender, EventArgs^ e) // esta es la funcion que se llama cuando se preciona el boton
        {

			salida->Clear(); // limpiamos el resultado anterior

			std::string codigo = msclr::interop::marshal_as<std::string>(editor->Text); // convertimos el texto del editor a una cadena de C++ usando marshal_as

			Lexer lexer(codigo); // iniciamos el analizador lexico con el codigo que escribimos

			while (true) // se ejecuta hasta llegar al final del codigo
            {
                Token t = lexer.getNextToken();

                if (t.type == TokenType::END)
                    break;

                std::string tipo = tokenTypeToString(t.type); 

				// aqui se muestra el resultado del analisis lexico en la salida, mostrando el valor del token y su tipo
                std::string linea = 
                    "TOKEN: " + t.value +
                    "  |  TIPO: " + tipo;

                salida->AppendText(gcnew String(linea.c_str())); // mostramos el valor del token en la salida
				salida->AppendText("\r\n"); // salto dr linea
            }
			salida->AppendText("\r\n--- PROCESO DEL ANALISIS ---\r\n"); // se muestra el proceso del analisis lexico, mostrando los mensajes que se guardaron en el log del lexer

            for (auto& s : lexer.log) // se muestra cada mensaje del log en la salida
            {
                salida->AppendText(gcnew String(s.c_str())); // mostramos el mensaje
                salida->AppendText("\r\n"); // salto de linea
            }
        }
    };
}