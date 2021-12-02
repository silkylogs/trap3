#pragma once
#include "global_declarations.h"
#include <vector>

void ReadTxtFileContentToVector(
	std::vector<std::string> &vector,
	std::fstream &file,
	const std::string &fileName
);

void WriteStringVectorToFile(
	std::vector<std::string> &vector,
	std::fstream &file,
	const std::string &fileName
);

void EraseFileContents(const std::string &fileName, std::fstream &file);

void OpenInputFile(const std::string &fileName, std::fstream &file);
void CloseInputFile(std::fstream &file);

void OpenOutputFile(const std::string &fileName, std::fstream &file);
void CloseOutputFile(std::fstream &file);
