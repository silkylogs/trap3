#pragma once
#include "global_declarations.h"
#include <vector>

void ReadTxtFileContentToVector
(
	std::vector<std::string> &vector,
	std::fstream &file,
	const char *fileName
);
void WriteStringVectorToFile
(
	std::vector<std::string> &vector,
	std::fstream &file,
	const char *fileName
);

void EraseFileContents(const char *fileName, std::fstream &file);

void OpenInputFile(const char *fileName, std::fstream &file);
void CloseInputFile(std::fstream &file);

void OpenOutputFile(const char *fileName, std::fstream &file);
void CloseOutputFile(std::fstream &file);