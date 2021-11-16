/*******************************************************************************
TODO:
-	Fix array declaration
-	Write some actual documentation for this language

program function
- pass 1: converting semantics to pseudocode
- pass 2: enclosing variables
- pass 3: replacing operators
*******************************************************************************/
#include "global_declarations.h"
#include "FileUtils.h"
#include "PseudocodeSynIdConv.h"
#include "VariableId.h"

#include <algorithm> //for VariablEnclosingPass()

char srcFileName[256] = "source.txt";
char outFileName[256] = "output.py";

std::fstream sourceFile;
std::fstream outFile;

/******************************************************************************/
//misc functions

void ArgControl(int argc, char *argv[])
{
	if(argc > 3)
	{
		std::cout << "Excessive arguements detected, maximum 2 allowed\n";
		exit(1);
	}
	else if(argc == 3)
	{
		printf("Input and output files detected\n");
		strcpy(srcFileName, argv[1]);
		strcpy(outFileName, argv[2]);
	}
	else if(argc == 2)
	{
		printf("Input file detected, searching for %s...\n", argv[1]);
		strcpy(srcFileName, argv[1]);
		return;
	}
	else if(argc == 1) 
	{
		std::cout << "No arguements detected\n" <<
		"Searching for source.txt...\n";
		return;
	}
	else
	{
		std::cout << "main.cpp/ArgControl(): " << 
		"How did you reach the else condition?\n";
		exit(1);
	}
}

void OpenBothFiles()
{
	OpenInputFile(srcFileName, sourceFile);
	OpenOutputFile(outFileName, outFile);
	return;
}
void CloseBothFiles()
{
	sourceFile.close();
	outFile.close();
	return;
}

/******************************************************************************/

void PseudocodeSyntaxConversionPass()
{
	OpenBothFiles();
	std::cout << "\n";

	std::string sourceLine;
	int lineIndex = 0;
	while(std::getline(sourceFile, sourceLine))
	{
		std::string outLine = sourceLine;
		//std::cout << "[" << lineIndex + 1 << "]: " << outLine << "\n";

		//classify statement
		OutLineStats olc;
		olc.AttemptLineClassify(outLine);

		//modify statement according to classification
		PseudocodeSynConv(olc, outLine);
		//std::cout << "Converted: " << outLine << "\n";

		//write modified line to file
		outFile << outLine + "\n";

		lineIndex++;
	}
	CloseBothFiles();

	std::cout << "\n";
	std::cout << "Pseudocode semantics translation complete.\n";
	std::cout << "\n";
}

/******************************************************************************/

void VariableEnclosingPass()
{
	std::vector<VariableId> declaredVarIds;	//declared variables
	std::vector<VariableId> detectedIds;	//names detected in file
	std::vector<VariableId> utilisedVarIds;	//the correct variables
	std::string tempLine;

	MakeDeclaredVarIds(declaredVarIds, srcFileName, sourceFile);
	MakeDetectedIds(detectedIds, srcFileName, sourceFile);

	/*
	at this point in time;
	declaredVarIds contains correct datatypes and correct variable names
	detectedIds contains correct line numbers and variable names (w/ junk)
	utilisedVarIds should therefore contain correct datatypes, lineNos and names
	*/

	MakeUtilisedVarIds(declaredVarIds, detectedIds, utilisedVarIds);

	//Write passes
	std::vector<int> decVarLineNos;
	ExtractLineNoFromVarIdVector(decVarLineNos, declaredVarIds);

	AppendPythonDeclaredDatatype(declaredVarIds, decVarLineNos, outFileName, outFile);

	std::cout << "\n";
	std::cout << "Variable enclosing complete.\n";
	std::cout << "\n";
}

/******************************************************************************/

void OperatorReplacementPass()
{
	/*
	substitutions in the out file:
	= == (only when unindented statement has if or elif)
	<> !=
	AND and
	OR or
	NOT not
	*/

	std::vector<std::string> outputLines;
	ReadTxtFileContentToVector(outputLines, outFile, outFileName);

	for(size_t i = 0; i < outputLines.size(); i++)
	{
		std::string moddedLine = outputLines[i];

		if
		(
			moddedLine.find("if") != std::string::npos ||
			moddedLine.find("elif") != std::string::npos
		)
		{
			if(moddedLine.find("=") != std::string::npos)
			{
				moddedLine.replace(moddedLine.find("="), sizeof("="), "== ");
			}
		}
		if(moddedLine.find("<>") != std::string::npos)
		{
			moddedLine.replace(moddedLine.find("<>"), sizeof("<>"), "!= ");
		}
		if(moddedLine.find("AND") != std::string::npos)
		{
			moddedLine.replace(moddedLine.find("AND"), sizeof("AND"), "and ");
		}
		if(moddedLine.find("OR") != std::string::npos)
		{
			moddedLine.replace(moddedLine.find("OR"), sizeof("OR"), "or ");
		}
		if(moddedLine.find("NOT") != std::string::npos)
		{
			moddedLine.replace(moddedLine.find("NOT"), sizeof("NOT"), "not ");
		}

		outputLines[i] = moddedLine;
	}

	WriteStringVectorToFile(outputLines, outFile, outFileName);

	std::cout << "\n";
	std::cout << "Operator replacement complete.\n";
	std::cout << "\n";
}

/******************************************************************************/

//its only job is to comment out marked lines in the out file
void LineCommenter()
{
	//OpenOutputFile();

	//CloseOutputFile();

	std::cout << "\n";
	std::cout << "Commented out errorneous lines.\n";
	std::cout << "\n";
}

/******************************************************************************/

int main(int argc, char *argv[])
{
	std::cout << "Trap3 cambridge pseudocode to python translator\n" <<
	"Warning: translation may not be error-proof, " <<
	"so please do proofread the output if you find any errors\n";

	ArgControl(argc, argv);

	//erase output file first to prevent trailing
	EraseFileContents(outFileName, outFile);

	PseudocodeSyntaxConversionPass();
	std::cout << "\n";
	VariableEnclosingPass();
	std::cout << "\n";
	OperatorReplacementPass();

	/*
	do a "press enter to continue mechanic" when there are zero arguments
	say, incase someone actually uses this like some
	generic double click to run windows application

	or make a wrapper applicaton for that which solely focuses on the UI
	side of things, for this program is getting too bloated as is
	*/
	std::cout << "\n\n\n";
	std::cout << "Complete translation successful.\n";


	/*
	do a "press enter to continue mechanic" when there are zero arguments
	say, incase someone actually uses this like some
	generic double click to run windows application
	*/
	if(argc == 1)
	{
		std::cout << "Press Enter to exit\n";
		std::cin.ignore();
	}

	return 0;
}
