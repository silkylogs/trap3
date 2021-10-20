/*******************************************************************************
TODO:
-	Debug VariableEnclosingPass()
-	Move concerned functions in VariableId.cpp and main.cpp to another file
	dedicated for string utilities
-	fix the incrementing condition of EncapsulatePythonVars
-	As per the new paradigm, do the operator replacement pass

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

void MakeDeclaredVarIds(std::vector<VariableId> &declaredVarIds)
{
	int lineNo = 1;
	std::string tempLine;

	OpenInputFile(srcFileName, sourceFile);
	while(std::getline(sourceFile, tempLine))
	{
		if(tempLine.find("DECLARE") != std::string::npos)
		{
			DetectAddDeclaredVariable(declaredVarIds, tempLine, lineNo);
		}
		//declaredVarIds.front().PrintDetails();
		lineNo++;
	}
	CloseInputFile(sourceFile);
	return;
}

void MakeDetectedIds(std::vector<VariableId> &detectedIds)
{
	std::string tempLine;
	int lineNo = 1;

	OpenInputFile(srcFileName, sourceFile);
	while(std::getline(sourceFile, tempLine))
	{
		//std::cout << "processing line " << lineNo << "\n";
		std::vector<VariableId> temp;

		//associate line number with each VariableId
		temp = DetectNamesInLine(tempLine);
		for(int i = 0; i < temp.size(); i++)
		{
			temp[i].lineNumber = lineNo;
			//std::cout << "details of var inserted:\n";
			//temp[i].PrintDetails();
		}

		//append the batch to detectedIds
		detectedIds.insert
		(
			detectedIds.end(),
			temp.begin(),
			temp.end()
		);

		lineNo++;
	}
	CloseInputFile(sourceFile);
	return;
}


//this generates the true list of variable ids
//note: this may fail if number of detected IDs < declared IDs
//hence check who is bigger before deciding to assign
void MakeUtilisedVarIds
(
	const std::vector<VariableId> &declaredVarIds,
	const std::vector<VariableId> &detectedIds,
	std::vector<VariableId> &utilisedVarIds
)
{
	for(int j = 0; j < detectedIds.size(); j++)
	{
		VariableId tVarId;

		//look up each declaredVarId variable name
		//find how many times it has been used in the script
		//for each time it has been used, put a duplicate one in utilisedVarIds
		//the exception being the line number
		for(int i = 0; i < declaredVarIds.size(); i++)
		{
			if(declaredVarIds[i].variableName == detectedIds[j].variableName)
			{
				tVarId.variableName = declaredVarIds[i].variableName;
				tVarId.variableDataType = declaredVarIds[i].variableDataType;
				tVarId.lineNumber = detectedIds[j].lineNumber;

				utilisedVarIds.insert(utilisedVarIds.begin(), tVarId);

				//std::cout << "inserted var in utilisedVarIds with details: ";
				//tVarId.PrintDetails();
			}
		}
	}

	return;
}

void ExtractLineNoFromVarIdVector
(
	std::vector<int> &lineNoVector,
	const std::vector<VariableId> &varIdVector
)
{
	for(int i = 0; i < varIdVector.size(); i++)
	{
		lineNoVector.insert
		(
			lineNoVector.begin(),
			varIdVector[i].lineNumber
		);
		//std::cout << "inserted line " << varIdVector[i].lineNumber << "\n";
	}
	std::sort(lineNoVector.begin(), lineNoVector.end());
	return;
}

void AppendPythonDeclaredDatatype
(
	const std::vector<VariableId> &declaredVarIds,
	const std::vector<int> &decVarLineNos
)
{
	std::vector<std::string> tempOutputFileLines;

	ReadTxtFileContentToVector(tempOutputFileLines, outFile, outFileName);
	int decVarLineIndex = 0;
	for(int lineIndex = 0; lineIndex < tempOutputFileLines.size(); lineIndex++)
	{
		if((lineIndex + 1) == decVarLineNos[decVarLineIndex])
		{
			std::string vNameTemp = tempOutputFileLines[lineIndex];

			//strip the variable name from the statement
			RemoveLeadingIndent(vNameTemp);
			RemoveUnwantedCharsFromLine(vNameTemp, "=");
			ExcessSpaceRemover(vNameTemp);
			vNameTemp.pop_back();

			//search for the matching VariableID
			for(int dVarInd = 0; dVarInd < declaredVarIds.size(); dVarInd++)
			{
				//Append the datatype
				if(declaredVarIds[dVarInd].variableName == vNameTemp)
				{
					tempOutputFileLines[lineIndex] +=
					EquivalentPythonVar(declaredVarIds[dVarInd]);
					break;
				}
			}

			decVarLineIndex++;
		}

		//std::cout << "[" << lineIndex + 1 << "]: "
		//<< tempOutputFileLines[lineIndex] << "\n";
	}
	WriteStringVectorToFile(tempOutputFileLines, outFile, outFileName);
	return;
}

void EncapsulatePythonVars
(
	const std::vector<VariableId> &utilisedVarIds,
	const std::vector<int> &utilVarLineNos
)
{
	/*
	Algorithm executed each line specified in utilVarLineNos:

	1. strip indentation
	2. strip unwanted characters
	3. strip excess spaces
	4. get names into a vector
	5. store those pythonNames into a vector of VariableIDs
	(with their lineNumbers pointing to string index)

	6. for each of the pythonNames:
		1. compare if pythonName == any element in utilisedVarIds
		2. if yes then go to the index of the name in the tempOutputFileLine and
		encapsulate as nessecary
		3. else continue
	*/

	std::vector<std::string> tempOutputFileLines;
	int varLineNoIndex = 0;
	
	for(int i = 0; i < utilVarLineNos.size(); i++)
	{
		std::cout << utilVarLineNos[i] << "\n";
	}

	ReadTxtFileContentToVector(tempOutputFileLines, outFile, outFileName);

	for(int lineNo = 1; lineNo <= tempOutputFileLines.size(); lineNo++)
	{
		std::cout << "comparing " << lineNo
		<< " and " << utilVarLineNos[varLineNoIndex] << "\n";
		
		if(lineNo == utilVarLineNos[varLineNoIndex])
		{
			std::string tempLine = tempOutputFileLines[lineNo];

			RemoveLeadingIndent(tempLine);
			RemoveUnwantedCharsFromLine(tempLine, unwanted_chars);
			ExcessSpaceRemover(tempLine);
			std::cout << "processed line: " << tempLine << "\n";

			//just because of the repeating elements in utilVarLineNos
			bool isNextLineNoEqualToLast =
			utilVarLineNos[varLineNoIndex] ==
			utilVarLineNos[varLineNoIndex + 1];
			if(isNextLineNoEqualToLast)
			{
				while(isNextLineNoEqualToLast)
				{
					varLineNoIndex++;
					bool isNextLineNoEqualToLast =
					utilVarLineNos[varLineNoIndex] ==
					utilVarLineNos[varLineNoIndex + 1];
				}
			}
			else
			{
				varLineNoIndex++;
				continue;
			}
		}

	}

	WriteStringVectorToFile(tempOutputFileLines, outFile, outFileName);

	return;
}

void VariableEnclosingPass()
{
	std::vector<VariableId> declaredVarIds;	//declared variables
	std::vector<VariableId> detectedIds;	//names detected in file
	std::vector<VariableId> utilisedVarIds;	//the correct variables
	std::string tempLine;
	int lineNo = 1;

	//ID pass
	//make a list of variables (and their types) which were declared		(v1)
	MakeDeclaredVarIds(declaredVarIds);

	//make a list of variables and lineNos which were used in each lines	(v2)
	MakeDetectedIds(detectedIds);

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

	AppendPythonDeclaredDatatype(declaredVarIds, decVarLineNos);

	/*
	for each variable, using the contents of utilisedVarIds, go to each line and
	encapsulate each utilised variable with its type
	*/
	std::vector<int> utilVarLineNos;
	ExtractLineNoFromVarIdVector(utilVarLineNos, utilisedVarIds);
	
	EncapsulatePythonVars(utilisedVarIds, utilVarLineNos);


	std::cout << "\n";
	std::cout << "Variable enclosing complete.\n";
	std::cout << "\n";
}

/******************************************************************************/

void OperatorReplacementPass()
{
	OpenBothFiles();

	CloseBothFiles();

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
	ArgControl(argc, argv);

	//erase output file first to prevent trailing
	EraseFileContents(outFileName, outFile);

	PseudocodeSyntaxConversionPass();
	std::cout << "\n";
	VariableEnclosingPass();

	/*
	do a "press enter to continue mechanic" when there are zero arguments
	say, incase someone actually uses this like some
	generic double click to run windows application

	or make a wrapper applicaton for that which solely focuses on the UI
	side of things, for this program is getting too bloated as is
	*/
	std::cout << "\n\n\n";
	std::cout << "Complete translation successful.";// Press Enter to exit: ";
	//std::cin.ignore();

	return 0;
}




/******************************************************************************/
//  					"hall" of old code backup							  //
/******************************************************************************/

/*
OpenBothFiles();
	std::cout << "\n";

	std::string sourceLine;
	int lineIndex = 0;
	while(std::getline(sourceFile, sourceLine))
	{
		std::string outLine = sourceLine;
		std::cout << "[" << lineIndex + 1 << "]: " << sourceLine << "\n";

		//classify statement
		OutLineStats olc;
		olc.AttemptLineClassify(outLine);

		//store variable details (if needed)
		varInfo[lineIndex].RecordDeclaredVar(olc, outLine);
		std::cout << "VarName: " <<
		varInfo[lineIndex].ExtractFirstVarName(olc, outLine) << "\n";
		//std::cout << "Datatype: " << varInfo[lineIndex].dataType << "\n";

		//comment replacement (best to offload it to a function later)
		if(outLine.find("//") != std::string::npos)
		{
			outLine.replace(outLine.find("//"), strlen("//"), "#");
		}

		/*
			if statement contains a variable,
			but isnt a declaration statment,
			then enclose it with its type
		/*

		//modify statement according to classification
		ConvertLine(olc, varInfo[lineIndex], outLine);
		std::cout << "Converted: " << outLine << "\n";

		//write modified line to file
		outFile << outLine + "\n";

		lineIndex++;
	}
*/
