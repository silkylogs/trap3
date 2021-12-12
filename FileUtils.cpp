#include "FileUtils.h"

void ReadTxtFileContentToVector(
	std::vector<std::string> &vector,
	std::fstream &file,
	const std::string &fileName
)
{
	std::string line;
	file.open(fileName);

	if(!file.good()){
		std::cerr << "File cannot be opened\n";
		return;
	}
	std::cout << "Opened file for reading\n";

	while(getline(file, line)){
		vector.insert(vector.end(), line);
	}

	file.close();
	std::cout << "File closed\n";
	return;
}

void WriteStringVectorToFile(
	std::vector<std::string> &vector,
	std::fstream &file,
	const std::string &fileName
)
{
	std::string line;

	file.open(fileName, std::ios::out);

	if(file.fail()){
		std::cerr << "File cannot be opened\n";
		return;
	}
	std::cout << "Opened file for writing\n";

	for(size_t i = 0; i < vector.size(); i++){
		file << vector[i] << "\n";
	}

	file.close();
	std::cout << "File closed\n";
	return;
}

void EraseFileContents(const std::string &fileName, std::fstream &file){
	file.open(fileName, std::ofstream::out | std::ofstream::trunc);
	file.close();
}

void OpenInputFile(const std::string &fileName, std::fstream &file){
	//printf("Opening %s...\n", fileName);
	std::cout << "Opening " << fileName << "...\n";
	file.open(fileName);
	if(!file.is_open()){
		std::cerr << "Failed to open file for input\n";
		std::cerr << "Press enter to exit the program\n";
		std::cin.ignore();
		exit(1);
	}
	else{
		std::cout << "Success opening file for input\n";
	}
	return;
}

void CloseInputFile(std::fstream &file){
	file.close();
	std::cout << "Closed input file\n";
	return;
}

void OpenOutputFile(const std::string &fileName, std::fstream &file){
	//printf("Opening %s...\n", fileName);
	std::cout << "Opening " << fileName << "...\n";
	file.open(fileName);
	if(!file.is_open()){
		std::cerr << "Failed to open output file\n";
		std::cerr << "Press enter to exit the program\n";
		std::cin.ignore();
		exit(1);
	}
	else{
		std::cout << "Success opening output file\n";
	}
	return;
}

void CloseOutputFile(std::fstream &file){
	file.close();
	std::cout << "Closed output file\n";
	return;
}