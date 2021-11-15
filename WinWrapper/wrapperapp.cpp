/*
	This is a wrapper application for the windows command line version for trap3.
	In other words, this application is recommended for users who are not
	familiar with launching applications on the command line and prefer
	using something which even vaugely resembles a menu
*/

#include <iostream>
#include <conio.h>

std::string inFileName = "source.txt";
std::string outFileName = "output.py";

void PrintOptions()
{
	std::cout << "1. Set input filename (chosen filename: " << inFileName << ")\n";
	std::cout << "2. Set output filename (chosen filename: " << outFileName << ")\n";
	std::cout << "3. Convert input pseudocode file to output python file\n";
	std::cout << "4. List files in current directory\n";
	std::cout << "5. Exit\n";
	return;
}

void OptionHandler(std::string &option)
{
	if(option == "1")
	{
		std::cout << "Enter input filename: ";
		std::cin >> inFileName;
	}
	else if(option == "2")
	{
		std::cout << "Enter output filename: ";
		std::cin >> outFileName;
	}
	else if(option == "3")
	{
		std::cout << "Executing trap3.exe with chosen parameters...\n";
		std::string command = "trap3.exe " + inFileName + " " + outFileName;
		system(command.c_str());
	}
	else if(option == "4")
	{
		system("dir /B");
	}
	else if(option == "5")
	{
		exit(0);
	}
	else
	{
		std::cout << "Invalid option\n";
	}
	return;
}

void Menu()
{
	std::string option = "NULL";

	std::cout << "Trap3 Cambridge pseudocode to Python transpiler\n\n\n";
	PrintOptions();
	printf("\n");

	std::cout << "Choose an option: ";
	std::cin >> option;
	OptionHandler(option);
}

int main()
{
	while(1)
	{
		Menu();
		std::cout << "\n\n";
	}

	return 0;
}
