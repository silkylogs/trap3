#include "VariableId.h"
#include "PseudocodeSynIdConv.h"
#include <algorithm>

void VariableId::PrintDetails(void){
	//"N/DT/LN" stands for "name" "datatype" and "lineno" respectively
	std::cout << "(N/DT/LN)\""
	<< variableName << "\":"
	<< variableDataType << ":"
	<< lineNumber;
	//std::cout << "Variable name: " << variableName << "\n";
	//std::cout << "Variable datatype: " << variableDataType << "\n";
	//std::cout << "Line number where encountered: " << lineNumber << "\n";
}

void PrintVariableIdVector(std::vector<VariableId> &v){
	std::cout << "{";
	for(size_t i = 0; i < v.size()-1; i++){
		v[i].PrintDetails(); std::cout << ", ";
	}
	v[v.size()-1].PrintDetails(); std::cout << "}\n";
}

/**************************************************************************************************/

std::string EquivalentPythonVar(VariableId vId){
	std::string v;
	if(vId.variableDataType == VariableId::pseudocodeDataTypes::Int)         { v = "int(0)"; }
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Real)   { v = "float(0)"; }
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::String) { v = "str(\"\")"; }
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Char)   { v = "str(\"\")"; }
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Bool)   { v = "bool(0)"; }

	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Int_A)   { v = "[int(0)]"; }
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Real_A)  { v = "[float(0)]"; }
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::String_A){ v = "[str(\"\")]"; }
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Char_A)  { v = "[str(\"\")]"; }
	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::Bool_A)  { v = "[bool(0)]"; }

	else if(vId.variableDataType == VariableId::pseudocodeDataTypes::None){
		std::cout << "EquivalentPythonVar(): Datatype \"None\" detected\n";
		v = "\0";
	}
	else{
		std::cout << "EquivalentPythonVar(): Else condition reached";
		v = "\0";
	}

	return v;
}

bool IsVariable(std::string str){
	for(size_t i = 0; i < str.length(); i++){
		if(!isalnum(str[i]) || str[i] != '_'){
			return false;
		}
	}
	return true;
}

std::string DeclaredVariableName(std::string line){
	if(line.find("DECLARE") != std::string::npos){
		line.erase(
			line.find(":") - 1
		);
		line.replace(
			line.find("DECLARE "),
			strlen("DECLARE "),
			""
		);
		return line;
	}
	else { return NO_VAR; }
}


VariableId::pseudocodeDataTypes DeclaredVarDataType(const std::string line){
	VariableId retId;
	bool debug = false;

	if((line.find("INTEGER") != std::string::npos) && (line.find("ARRAY") != std::string::npos)){
		retId.variableDataType = VariableId::pseudocodeDataTypes::Int_A;
	}
	else if((line.find("REAL") != std::string::npos) && (line.find("ARRAY") != std::string::npos)){
		retId.variableDataType = VariableId::pseudocodeDataTypes::Real_A;
	}
	else if((line.find("STRING") != std::string::npos)&&(line.find("ARRAY") != std::string::npos)){
		retId.variableDataType = VariableId::pseudocodeDataTypes::String_A;
	}
	else if((line.find("CHAR") != std::string::npos) && (line.find("ARRAY") != std::string::npos)){
		retId.variableDataType = VariableId::pseudocodeDataTypes::Char_A;
	}
	else if((line.find("BOOL") != std::string::npos) && (line.find("ARRAY") != std::string::npos)){
		retId.variableDataType = VariableId::pseudocodeDataTypes::Bool_A;
	}

	else if(line.find("INTEGER") != std::string::npos){
		retId.variableDataType = VariableId::pseudocodeDataTypes::Int;
	}
	else if(line.find("REAL") != std::string::npos){
		retId.variableDataType = VariableId::pseudocodeDataTypes::Real;
	}
	else if(line.find("STRING") != std::string::npos){
		retId.variableDataType = VariableId::pseudocodeDataTypes::String;
	}
	else if(line.find("CHAR") != std::string::npos){
		retId.variableDataType = VariableId::pseudocodeDataTypes::Char;
	}
	else if(line.find("BOOL") != std::string::npos){
		retId.variableDataType = VariableId::pseudocodeDataTypes::Bool;
	}

	else{
		retId.variableDataType = VariableId::pseudocodeDataTypes::None;
	}

	if(debug){
		std::cout << "line=\"" << line << "\"\n";
		std::cout << "corresponding datatype=" << retId.variableDataType << "\n";
	}

	return retId.variableDataType;
}

void DetectAddDeclaredVariable(
	std::vector<VariableId> &varIdGrp,
	std::string line,
	int lineNo
){
	std::string varName = DeclaredVariableName(line);
	VariableId::pseudocodeDataTypes dataType = DeclaredVarDataType(line);
	VariableId temp(varName, dataType, lineNo);
	varIdGrp.insert(varIdGrp.begin(), temp);
	return;
}

/* Python variable naming rules
*
* A variable name must start with a letter or the underscore character.
* A variable name cannot start with a number.
* A variable name can only contain alpha-numeric characters and underscores
* Variable names are case-sensitive
* The reserved words(keywords) cannot be used naming the variable.
*/

bool StringContainsCharInSet(
	std::string &originalLine,
	const std::string &charSet
){
	size_t i = 0;
	size_t j = 0;
	while(i < originalLine.length()){
		while(j < charSet.length()){
			if(originalLine[i] == charSet[j]) { return true; }
			else { j++; }
		}
		j = 0;
		i++;
	}
	return false;
}

int IndexOfCharInSet(std::string &originalLine, const std::string &charSet){
	size_t i = 0;
	size_t j = 0;
	while(i < originalLine.length()){
		while(j < charSet.length()){
			if(originalLine[i] == charSet[j]) { return i; }
			else { j++; }
		}
		j = 0;
		i++;
	}
	return -1;
}

std::vector<VariableId> DetectNamesInLine(std::string line){
	std::vector<VariableId> namesUsedInLine;
	OutLineStats lineType;
	bool debug = false;

	lineType.DetectMode(line);
	if(lineType.mode == OutLineStats::lineMode::Assign){
		if(debug){
			std::cout << "Parsing assign statement\n";
			std::cout << "Line: " << line << "\n";
		}

		/*
		* Replaces the following with space (" "):
		* brackets, signs as well as the "<-" sign:: ()+-*^/<>[]"',
		*/
		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		if(debug){
			std::cout << "Line after removing unwanted chars: \"" << line << "\"\n";
		}

		//remove indent and extra spaces
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		ExcessSpaceRemover(line);
		
		if (debug){
			std::cout << "Line after removing space and indent: \"" <<line<< "\"\n";
		}

		/*
		* Read each name and store them into namesUsedInLine
		* hint:
		*	go through each character
		*	check wether alphanumeric or underscore
		*	if yes then add to a temp string
		*	else add string to variableid then add to vector
		*/
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::TxtOut){
		if(debug){
			std::cout << "Parsing output statement\n";
			std::cout << "Line: " << line << "\n";
		}

		line.replace(line.find("OUTPUT"), strlen("OUTPUT"), "");

		/*
		* Recursively remove anything between qoutes
		*/
		while(0 == RemoveTextEnclosedByChar(line, '\"')) {;}
		if(debug){
			std::cout << "Line after removing qoutes:" << line << "\n";
		}

		/*
		* Remove commas before appending to variable vector, idk why though
		*/
		RemoveUnwantedCharsFromLine(line, ",");
		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::Input){
		if(debug){
			std::cout << "Parsing input statement\n";
			std::cout << "Line: " << line << "\n";
		}

		/*
		* Removing the input as well as the commas would leave a list of vars
		*/
		line.replace(line.find("INPUT"), strlen("INPUT"), "");

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		if(debug){
			std::cout << "Line after processing: " << line << "\n";
		}
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::WhileLoopStart){
		if(debug){
			std::cout << "Parsing while loop start\n";
			std::cout << "Line: " << line << "\n";
		}

		line.replace(line.find("WHILE"), strlen("WHILE"), "");
		line.replace(line.find("DO"), strlen("DO"), "");

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::ForLoopStart){
		if(debug){
		std::cout << "Parsing for loop start\n";
		std::cout << "Line: " << line << "\n";
		}
		
		line.replace(line.find("FOR"), strlen("FOR"), "");
		line.replace(line.find("<-"), strlen("<-"), "");
		line.replace(line.find("TO"), strlen("TO"), "");

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::IfStart){
		if(debug){
			std::cout << "Parsing IF start\n";
			std::cout << "Line: " << line << "\n";
		}
		line.replace(line.find("IF"), strlen("IF"), "");

		/*
		* For that one person who insists on using "THEN"
		*/
		if(line.find("THEN") != std::string::npos){
			line.replace(line.find("THEN"), strlen("THEN"), "");
		}

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	else if(lineType.mode == OutLineStats::lineMode::ElseIfKeyword){
		if(debug){
			std::cout << "Parsing ELSE IF start\n";
			std::cout << "Line: " << line << "\n";
		}

		line.replace(line.find("ELSE"), strlen("ELSE"), "");
		line.replace(line.find("IF"), strlen("IF"), "");

		/*
		* For that one person who insists on using "THEN"
		*/
		if(line.find("THEN") != std::string::npos){
			line.replace(line.find("THEN"), strlen("THEN"), "");
		}

		RemoveUnwantedCharsFromLine(line, unwanted_chars);
		RemoveLeadingIndent(line);
		ExcessSpaceRemover(line);
		StoreNameListIntoVariableVector(line, namesUsedInLine);
	}

	return namesUsedInLine;
}

void RemoveUnwantedCharsFromLine(
	std::string &line,
	const std::string &unwantedChars
){
	size_t i = 0;
	while(StringContainsCharInSet(line, unwanted_chars)){
		i = IndexOfCharInSet(line, unwanted_chars);
		if(i < 0){
			std::cout << "Error from RemoveUnwantedCharsFromLine():\n";
			std::cout << "DetectNamesInLine(): " <<
			"IndexOfCharInSet() returned negative value\n";
		}
		else{
			line.replace(i, 1, " ");
		}
	}
}

void RemoveLeadingIndent(std::string &line){
	while(line[0] == '\t' || line[0] == ' '){
		line.erase(0,1);
	}
	return;
}

void ExcessSpaceRemover(std::string &line){
	/*
	* Checks wether there are any spaces at all to begin with
	*/
	if(line.find(" ") == std::string::npos){ return; }

	for(size_t i = 0; i < line.length(); i++){
		/*
		* Find space position
		*/
		if(line[i] != ' ') { continue; }
		else{
			/*
			* Erase the next character if its a space
			*/
			while(line[i+1] == ' '){
				line.erase(i + 1, 1);
			}
		}
	}
	return;
}

/*
* Issue: doesnt store the last variable in the string
* Solution: either add a space at the end or mod the else if
*/
void StoreNameListIntoVariableVector(
	std::string &varString,
	std::vector<VariableId> &varVector
){
	std::string tempString;
	VariableId tempVarId;
	bool debug = false;

	/*
	* Hack to make the function parse the last name in the string
	*/
	varString += ' ';

	for(size_t i = 0; i < varString.length(); i++){
		if(isalnum(varString[i]) || varString[i] == '_'){
			tempString += varString[i];
		}
		else if(varString[i] == ' '){
			tempVarId.variableName = tempString;

			if(debug) {std::cout << "Placing name: \"" << tempString << "\"\n";}
			
			varVector.emplace(varVector.begin(), tempVarId);
			tempString.erase();
		}
		else{
			std::cout << "VariableId.cpp/StoreNameListIntoVariableVector(): " <<
			"Warning: else conditon reached\n";
			std::cout << "Attempting to parse string \"" << varString << "\"\n";
			std::cout << "Error at char \'" << varString[i] << "\'\n";
			std::cout << "Character index " << i << "\n";
		}
	}
}

int RemoveTextEnclosedByChar(std::string &text, const char delimiter){
	size_t startMarker = 0;
	size_t endMarker = 0;

	/*
	* Check for first occurence of string
	*/
	for(size_t i = 0; i < text.length(); i++){
		if(text[i] == delimiter){
			startMarker = i;
			break;
		}
		else if(i >= text.length() - 1){
			return -1;
		}
	}

	/*
	* Check for next occurence in string
	*/
	for(size_t i = startMarker + 1; i < text.length(); i++){
		if(text[i] == delimiter){
			endMarker = i + 1;
			break;
		}
		else if(i >= text.length() - 1){
			return -1;
		}
	}

	/*
	* Remove text between delimiters
	*/
	text.replace(startMarker, endMarker, "\0");
	return 0;
}

/**************************************************************************************************/

void MakeDeclaredVarIds(
	std::vector<VariableId> &declaredVarIds,
	std::string &srcFileName,
	std::fstream &sourceFile
){
	size_t lineNo = 1;
	std::string tempLine;
	bool debug = false;

	OpenInputFile(srcFileName, sourceFile);
	while(std::getline(sourceFile, tempLine)){
		if(tempLine.find("DECLARE") != std::string::npos){
			DetectAddDeclaredVariable(declaredVarIds, tempLine, lineNo);
		}
		if(debug) { declaredVarIds.front().PrintDetails(); }
		lineNo++;
	}
	CloseInputFile(sourceFile);
	return;
}

void MakeDetectedIds(
	std::vector<VariableId> &detectedIds,
	std::string &srcFileName,
	std::fstream &sourceFile
){
	std::string tempLine;
	size_t lineNo = 1;
	bool debug = false;

	OpenInputFile(srcFileName, sourceFile);
	while(std::getline(sourceFile, tempLine)){
		if(debug) { std::cout << "Processing line " << lineNo << "\n"; }
		std::vector<VariableId> temp;

		//associate line number with each VariableId
		temp = DetectNamesInLine(tempLine);
		for(size_t i = 0; i < temp.size(); i++){
			temp[i].lineNumber = lineNo;
			if(debug){
				std::cout << "details of var inserted:\n";
				temp[i].PrintDetails();
			}
		}

		//append the batch to detectedIds
		detectedIds.insert(
			detectedIds.end(),
			temp.begin(),
			temp.end()
		);

		lineNo++;
	}
	CloseInputFile(sourceFile);
	return;
}

/*
* This generates the true list of variable ids
* Note: this may fail if number of detected IDs < declared IDs
* hence check who is bigger before deciding to assign
*/
void MakeUtilisedVarIds
(
	const std::vector<VariableId> &declaredVarIds,
	const std::vector<VariableId> &detectedIds,
	std::vector<VariableId> &utilisedVarIds
){
	bool debug = false;
	for(size_t j = 0; j < detectedIds.size(); j++){
		VariableId tVarId;

		/*
		* Look up each declaredVarId variable name
		* Find how many times it has been used in the script
		* For each time it has been used, put a duplicate one in utilisedVarIds
		* the exception being the line number
		*/
		for(size_t i = 0; i < declaredVarIds.size(); i++){
			if(declaredVarIds[i].variableName == detectedIds[j].variableName){
				tVarId.variableName = declaredVarIds[i].variableName;
				tVarId.variableDataType = declaredVarIds[i].variableDataType;
				tVarId.lineNumber = detectedIds[j].lineNumber;
				utilisedVarIds.insert(utilisedVarIds.begin(), tVarId);
				if(debug){
					std::cout << "inserted var in utilisedVarIds with details: ";
					tVarId.PrintDetails();
				}
			}
		}
	}
	return;
}

void ExtractLineNoFromVarIdVector(
	std::vector<int> &lineNoVector,
	const std::vector<VariableId> &varIdVector
){
	bool debug = false;
	for(size_t i = 0; i < varIdVector.size(); i++){
		lineNoVector.insert(
			lineNoVector.begin(),
			varIdVector[i].lineNumber
		);
		if(debug) { std::cout << "inserted line " << varIdVector[i].lineNumber << "\n"; }
	}
	std::sort(lineNoVector.begin(), lineNoVector.end());
	return;
}

void AppendPythonDeclaredDatatype(
	const std::vector<VariableId> &declaredVarIds,
	const std::vector<int> &decVarLineNos,
	const std::string &outFileName,
	std::fstream &outFile
){
	std::vector<std::string> tempOutputFileLines;
	bool debug = false;

	ReadTxtFileContentToVector(tempOutputFileLines, outFile, outFileName);
	int decVarLineIndex = 0;
	for(size_t lineIndex = 0; lineIndex < tempOutputFileLines.size(); lineIndex++){
		if((lineIndex + 1) == (size_t)(decVarLineNos[decVarLineIndex])){
			std::string vNameTemp = tempOutputFileLines[lineIndex];

			//strip the variable name from the statement
			RemoveLeadingIndent(vNameTemp);
			RemoveUnwantedCharsFromLine(vNameTemp, "=");
			ExcessSpaceRemover(vNameTemp);
			vNameTemp.pop_back();

			//search for the matching VariableID
			for(size_t dVarInd = 0; dVarInd < declaredVarIds.size(); dVarInd++){
				//Append the datatype
				if(declaredVarIds[dVarInd].variableName == vNameTemp){
					tempOutputFileLines[lineIndex] += EquivalentPythonVar(declaredVarIds[dVarInd]);
					break;
				}
				if(debug){ 
					std::string temp = EquivalentPythonVar(declaredVarIds[dVarInd]);
					std::cout << temp << "\n";
				}
			}
			decVarLineIndex++;
		}
		if(debug){
			std::cout << "[" << lineIndex + 1 << "]: " << tempOutputFileLines[lineIndex] << "\n";
		}
	}
	WriteStringVectorToFile(tempOutputFileLines, outFile, outFileName);
	return;
}
