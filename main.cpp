#include "FormPrincipal.h" // llamamos el form

using namespace System;
using namespace System::Windows::Forms;

[STAThread] // este es el punto de entrada del programa

int main(array<String^>^ args) 
{

	Application::EnableVisualStyles(); // esto es para que el programa tenga el estilo visual de windows
	Application::SetCompatibleTextRenderingDefault(false); // esto es para que el texto se vea bien en el programa

	CompiladorWinForms::FormPrincipal form; // se crea la instancia del formulario principal

	Application::Run(gcnew CompiladorWinForms::FormPrincipal()); // se ejecuta el formulario principal

	return 0;
}