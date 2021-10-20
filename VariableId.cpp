#include "VariableId.h"
#include "PseudocodeSynIdConv.h"

/******************************************************************************/

std::string EquivalentPythonVar(VariableId vId)
{
	std::string v;
	if(vId.variableDataType == VariableId::pseudocodeDataTypes::Int)
	{
		v = "int(0)";
	}
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Real)
	{
		v = "float(0)";
	}
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::String)
	{
		v = "str(\"\")";
	}
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Char)
	{
		v = "str(\"\")";
	}
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Bool)
	{
		v = "bool(0)";
	}
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::None)
	{
		std::cout << "EquivalentPythonVar(): Datatype \"None\" detected\n";
		v = "\0";
	}
	else
	{
		std::cout << "EquivalentPythonVar(): Else condition reached";
		v = "\0";
	}

	return v;
}

bool IsVariable(std::string str)
{
	for(int i = 0; i < str.length(); i++)
	{
		if(!isalnum(str[i]) || str[i] != '_')
		{
			return false;
		}
	}
	return true;
}

void VariableId::PrintDetails(void)
{
	//"NDL" stands for "name" "datatype" and "lineno" respectively
	std::cout << "(N/D/L) \""
	<< variableName << "\":"
	<< variableDataType << ":"
	<< lineNumber << "\n";
	//std::cout << "Variable name: " << variableName << "\n";
	//std::cout << "Variable datatype: " << variableDataType << "\n";
	//std::cout << "Line number where encountered: " << lineNumber << "\n";
}

std::string DeclaredVariableName(std::string line)
{
	if(line.find("DECLARE") != std::string::npos)
	{
		line.erase
		(
			line.find(":") - 1
		);
		line.replace
		(
			line.find("DECLARE "),
			strlen("DECLARE "),
			""
		);
		return line;
	}
	else
	{
		return NO_VAR;
	}
}

VariableId::pseudocodeDataTypes DeclaredVarDataType(const std::string line)
{
	if(line.find("INTEGER") != std::string::npos)
	{
		return VariableId::pseudocodeDataTypes::Int;
	}
	else if(line.find("REAL") != std::string::npos)
	{
		return VariableId::pseudocodeDataTypes::Real;
	}
	else if(line.find("STRING") != std::string::npos)
	{
		return VariableId::pseudocodeDataTypes::String;
	}
	else if(line.find("CHAR") != std::string::npos)
	{
		return VariableId::pseudocodeDataTypes::Char;
	}
	else if(line.find("BOOL") != std::string::npos)
	{
		return VariableId::pseudocodeDataTypes::Bool;
	}
	else
	{
		return VariableId::pseudocodeDataTypes::None;
	}
}

void DetectAddDeclaredVariable
(
	std::vector<VariableId> &varIdGrp,
	std::string line,
	int lineNo
)
{
	std::string varName = DeclaredVariableName(line);
	VariableId::pseudocodeDataTypes dataType = DeclaredVarDataType(line);
	VariableId temp(varName, dataType, lineNo);

	varIdGrp.insert(varIdGrp.begin(), temp);
}

/* Python variable naming rules
*
* A variable name must start with a letter or the underscore character.
* A variable name cannot start with a number.
* A variable name can only contain alpha-numeric characters and underscores
* Variable names are case-sensitive
* The reserved words(keywords) cannot be used naming the variable.
*/

bool StringContainsCharInSet
(
	std::string &originalLine,
	const std::string &charSet
)
{
	int i = 0;
	int j = 0;
	while(i < originalLine.length())
	{
		while(j < charSet.length())
		{
			if(originalLine[i] == charSet[j])
			{
				return true;
			}
			else
			{
				j++;
			}
		}
		j = 0;
		i++;
	}
	return false;
}

int IndexOfCharInSet(std::string &originalLine, const std::string &charSet)
{
	int i = 0;
	int j = 0;
	while(i < originalLine.length())
	{
		while(j < charSet.length())
		{
			if(originalLine[i] == charSet[j])
			{
				return i;
			}
			else
			{
				j++;
			}
		}
		j = 0;
		i++;
	}
	return -1;
}

std::vector<VariableId> DetectNamesInLine(std::string line)
{
	std::vector<VariableId> namesUsedInLine;
	OutLineStats lineType;

	lineType.DetectMode(line);

	if(lineType.mode == OutLineStats::lineMode::Assign)
	{
		//std::cout << "Parsing assign statement\n";
		//std::cout << "Line: " << line << "\n";

		//replace the following with space (" "):
		//brackets, signs as well as the "<-" sign:: ()+-*^/<>[]"',
		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		/*
		std::cout << "Line after removing unwanted chars: \"" << line << "\"\n";
		*/

		//remove indent and extra spaces
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		ExcessSpaceRemover(line);
		
		/*
		std::cout << "Line after removing space and indent: \"" <<line<< "\"\n";
		*/

		//read each name and store them into namesUsedInLine
		//hint:
		//	go through each character
		//	check wether alphanumeric or underscore
		//	if yes then add to a temp string
		//	else add string to variableid then add to vector
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::TxtOut)
	{
		//std::cout << "Parsing output statement\n";
		//std::cout << "Line: " << line << "\n";

		line.replace(line.find("OUTPUT"), strlen("OUTPUT"), "");

		//recursively remove anything between qoutes
		while(0 == RemoveTextEnclosedByChar(line, '\"')) {;}
		//std::cout << "Line after removing qoutes:" << line << "\n";

		//remove commas
		RemoveUnwantedCharsFromLine(line, ",");

		/*this one is for that one person who surrounds strings with single
		//qoutes... nah they can suffer
		flag = 0;
		while(flag == 0)
		{
			RemoveTextEnclosedByChar(line, '\'');
		}*/

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::Input)
	{
		//std::cout << "Parsing input statement\n";
		//std::cout << "Line: " << line << "\n";

		//removing the input as well as the commas would leave a list of vars
		line.replace(line.find("INPUT"), strlen("INPUT"), "");

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		//std::cout << "Line after processing: " << line << "\n";

		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::WhileLoopStart)
	{
		//std::cout << "Parsing while loop start\n";
		//std::cout << "Line: " << line << "\n";

		line.replace(line.find("WHILE"), strlen("WHILE"), "");
		line.replace(line.find("DO"), strlen("DO"), "");

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::ForLoopStart)
	{
		//std::cout << "Parsing for loop start\n";
		//std::cout << "Line: " << line << "\n";

		line.replace(line.find("FOR"), strlen("FOR"), "");
		line.replace(line.find("<-"), strlen("<-"), "");
		line.replace(line.find("TO"), strlen("TO"), "");

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::IfStart)
	{
		//std::cout << "Parsing IF start\n";
		//std::cout << "Line: " << line << "\n";

		line.replace(line.find("IF"), strlen("IF"), "");

		//for that one person who insists on using "THEN"
		if(line.find("THEN") != std::string::npos)
		{
			line.replace(line.find("THEN"), strlen("THEN"), "");
		}

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::ElseIfKeyword)
	{
		//std::cout << "Parsing ELSE IF start\n";
		//std::cout << "Line: " << line << "\n";

		line.replace(line.find("ELSE"), strlen("ELSE"), "");
		line.replace(line.find("IF"), strlen("IF"), "");

		//for that one person who insists on using "THEN"
		if(line.find("THEN") != std::string::npos)
		{
			line.replace(line.find("THEN"), strlen("THEN"), "");
		}

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	return namesUsedInLine;
}

void RemoveUnwantedCharsFromLine
(
	std::string &line,
	const std::string &unwantedChars
)
{
	int i = 0;
	while(StringContainsCharInSet(line, unwanted_chars))
	{
		i = IndexOfCharInSet(line, unwanted_chars);
		if(i < 0)
		{
			std::cout << "Error from RemoveUnwantedCharsFromLine():\n";
			std::cout << "DetectNamesInLine(): " <<
			"IndexOfCharInSet() returned negative value\n";
		}
		else
		{
			line.replace(i, 1, " ");
		}
	}
}

void RemoveLeadingIndent(std::string &line)
{
	while(line[0] == '\t' || line[0] == ' ')
	{
		line.erase(0,1);
	}
	return;
}

void ExcessSpaceRemover(std::string &line)
{
	//checks wether there are any spaces at all to begin with
	if(line.find(" ") == std::string::npos)
	{
		return;
	}

	for(int i = 0; i < line.length(); i++)
	{
		//find space position
		if(line[i] != ' ')
		{
			continue;
		}
		else
		{
			//erase the next character if its a space
			while(line[i+1] == ' ')
			{
				line.erase(i + 1, 1);
			}
		}
	}
	return;
}

//issue: doesnt store the last variable in the string
//solution: either add a space at the end or mod the else if
void StoreNameListIntoVariableVector
(
	std::string &varString,
	std::vector<VariableId> &varVector
)
{
	std::string tempString;
	VariableId tempVarId;

	//hack to make the function parse the last name in the string
	varString += ' ';

	for(int i = 0; i < varString.length(); i++)
	{
		if(isalnum(varString[i]) || varString[i] == '_')
		{
			tempString += varString[i];
		}
		else if(varString[i] == ' ')
		{
			tempVarId.variableName = tempString;

			//std::cout << "Placing name: \"" << tempString << "\"\n";
			varVector.emplace(varVector.begin(), tempVarId);

			tempString.erase();
		}
		else
		{
			std::cout << "VariableId.cpp/StoreNameListIntoVariableVector(): " <<
			"Warning: else conditon reached\n";
			std::cout << "Attempting to parse string \"" << varString << "\"\n";
			std::cout << "Error at char \'" << varString[i] << "\'\n";
			std::cout << "Character index " << i << "\n";
		}
	}
}

int RemoveTextEnclosedByChar(std::string &text, const char delimiter)
{
	size_t startMarker = 0;
	size_t endMarker = 0;

	//check for first occurence of string
	for(int i = 0; i < text.length(); i++)
	{
		if(text[i] == delimiter)
		{
			startMarker = i;
			break;
		}
		else if(i >= text.length() - 1)
		{
			return -1;
		}
	}

	//check for next occurence in string
	for(int i = startMarker + 1; i < text.length(); i++)
	{
		if(text[i] == delimiter)
		{
			endMarker = i + 1;
			break;
		}
		else if(i >= text.length() - 1)
		{
			return -1;
		}
	}

	//removing text between delimiters
	text.replace(startMarker, endMarker, "\0");

	return 0;
}


/******************************************************************************/
