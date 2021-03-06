#pragma once
#include "global_declarations.h"
#include "FileUtils.h"
#include <algorithm>

const std::string unwanted_chars = "=(:)+-^*/<>[]\"\',";

/******************************************************************************/

class VariableId
{
	public:
	std::string variableName;
	int lineNumber;
	enum pseudocodeDataTypes
	{
		Int 	= 0,
		Int_A	= 1,
		Real	= 2,
		Real_A	= 3,
		String	= 4,
		String_A= 5,
		Char	= 6,
		Char_A	= 7,
		Bool	= 8,
		Bool_A	= 9,
		None	= 100
	} variableDataType;

	VariableId(){
		variableName = NO_VAR;
		variableDataType = pseudocodeDataTypes::None;
		lineNumber = -1;
	}
	VariableId(std::string varName, int varDataType, int lineNo){
		variableName = varName;
		variableDataType = (pseudocodeDataTypes)varDataType;
		lineNumber = lineNo;
	}
	void PrintDetails(void);
};

void PrintVariableIdVector(std::vector<VariableId> &v);

std::string EquivalentPythonVar(VariableId vId);
bool IsVariable(std::string str);
std::string DeclaredVariableName(std::string line);
VariableId::pseudocodeDataTypes DeclaredVarDataType(const std::string line);

void DetectAddDeclaredVariable(
	std::vector<VariableId> &varIdGrp,
	std::string line,
	int lineNo
);

bool StringContainsCharInSet(
	std::string &originalLine,
	const std::string &charSet
);

//returns negative number if not found
int IndexOfCharInSet(std::string &originalLine, const std::string &charSet);
std::vector<VariableId> DetectNamesInLine(std::string line);

void RemoveUnwantedCharsFromLine(
	std::string &line,
	const std::string &unwantedChars
);

void RemoveLeadingIndent(std::string &line);

//removes excessive spacing (which is more than 1)
void ExcessSpaceRemover(std::string &line);

void StoreNameListIntoVariableVector(
	std::string &varString,
	std::vector<VariableId> &varVector
);

// Returns zero on success
int RemoveTextEnclosedByChar(std::string &text, const char delimiter);

/******************************************************************************/

void MakeDeclaredVarIds(
	std::vector<VariableId> &declaredVarIds,
	std::string &srcFileName,
	std::fstream &sourceFile
);

void MakeDetectedIds(
	std::vector<VariableId> &detectedIds,
	std::string &srcFileName,
	std::fstream &sourceFile
);

void MakeUtilisedVarIds(
	const std::vector<VariableId> &declaredVarIds,
	const std::vector<VariableId> &detectedIds,
	std::vector<VariableId> &utilisedVarIds
);

void ExtractLineNoFromVarIdVector(
	std::vector<int> &lineNoVector,
	const std::vector<VariableId> &varIdVector
);

void AppendPythonDeclaredDatatype(
	const std::vector<VariableId> &declaredVarIds,
	const std::vector<int> &decVarLineNos,
	const std::string &outFileName,
	std::fstream &outFile
);
