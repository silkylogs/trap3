#include "PseudocodeSynIdConv.h"

// Note to self: compile individual implementation files with command
// gcc -c filename.cpp
// to get the filename.o file

void OutLineStats::DetectMode(std::string &line){
	// Note: The checking is unordered in respect to the enum element
	// sequence because std::line.find() would wrongly detect keywords
	// otherwise

	// Blank space
	if(line.empty()){
		mode = BlankLine;
		return;
	}
	// Array declare
	else if(
		line.find("DECLARE") != std::string::npos &&
		line.find(":") != std::string::npos &&
		line.find("ARRAY") != std::string::npos &&
		line.find("[") != std::string::npos &&
		line.find("]") != std::string::npos &&
		line.find("OF") != std::string::npos
	){
		mode = ArrayDeclare;
		return;
	}
	// Normal declare
	else if(
		line.find("DECLARE") != std::string::npos &&
		line.find(":") != std::string::npos
	){
		mode = NormalDeclare;
		return;
	}
	// For loop start
	else if(
		line.find("FOR") != std::string::npos &&
		line.find("TO") != std::string::npos
	){
		mode = ForLoopStart;
		return;
	}
	// Assignment statement
	else if(line.find("<-") != std::string::npos){
		mode = Assign;
		return;
	}
	// Output
	else if(line.find("OUTPUT") != std::string::npos){
		mode = TxtOut;
		return;
	}
	// Input
	else if(line.find("INPUT") != std::string::npos){
		mode = Input;
		return;
	}
	// While loop start
	else if(
		line.find("WHILE") != std::string::npos &&
		line.find("DO") != std::string::npos
	){
		mode = WhileLoopStart;
		return;
	}
	// While loop end
	else if(line.find("ENDWHILE") != std::string::npos){
		mode = WhileLoopEnd;
		return;
	}
	// For loop end
	else if(line.find("NEXT") != std::string::npos){
		mode = ForLoopEnd;
		return;
	}
	// Comment only
	else if(line.find("//") != std::string::npos){
		mode = CommentOnly;
		return;
	}
	// Else if keyword
	else if(line.find("ELSE IF") != std::string::npos){
		mode = ElseIfKeyword;
		return;
	}
	// Endif keyword
	else if(line.find("ENDIF") != std::string::npos){
		mode = IfEnd;
		return;
	}
	// If start
	else if(line.find("IF") != std::string::npos){
		mode = IfStart;
		return;
	}
	// Then keyword
	else if(line.find("THEN") != std::string::npos){
		mode = ThenKeyword;
		return;
	}
	// Else keyword
	else if(line.find("ELSE") != std::string::npos){
		mode = ElseKeyword;
		return;
	}
	// Undefined line
	else{
		mode = UndefinedLine;
		std::cout << "PseudocodeSynIdConv.cpp/OutLineStats.DetectMode()\n";
		std::cout << "In line: " << line << "\n";
		std::cout << "Warning: Undefined line detected\n";
	}
}

void OutLineStats::DetectIndentLevel(std::string &line){
	int i = 0;
	while(line[i] == '\t' || line[i] == ' '){
		i++;
		indentLevel++;
	}
}

void OutLineStats::AttemptLineClassify(std::string &line){
	DetectIndentLevel(line);
	DetectMode(line);
}

void PseudocodeSynConv(OutLineStats &olc, std::string &line){
	//comment replacement
	if(line.find("//") != std::string::npos){
		line.replace(line.find("//"), strlen("//"), "#");
	}
	if(olc.mode == olc.CommentOnly){
		return;
	}

	if(olc.mode == olc.ArrayDeclare){
		std::string temp;
		temp.erase();

		line.replace(
			line.find("DECLARE "),
			strlen("DECLARE "),
			""
		);
		line.replace(
			line.find(":"),
			strlen(":"),
			"="
		);
		line.erase(
			line.find("ARRAY")
		);

		/* //to be moved to variable
		if(olc.lineDataType == OutLineStats::pseudocodeDataTypes::Integer)
		{
			temp = "[int(0)]";
		}
		else if(olc.lineDataType == OutLineStats::pseudocodeDataTypes::Real)
		{
			temp = "[float(0)]";
		}
		else if
		(
			olc.lineDataType == OutLineStats::pseudocodeDataTypes::String ||
			olc.lineDataType == OutLineStats::pseudocodeDataTypes::Char
		)
		{
			temp = "[str(0)]";
		}
		else if(olc.lineDataType == OutLineStats::pseudocodeDataTypes::Bool)
		{
			temp = "[bool(0)]";
		}

		line += temp;
		*/
	}

	else if(olc.mode == olc.NormalDeclare){
		std::string temp;
		temp.erase();

		line.replace(
			line.find("DECLARE "),
			strlen("DECLARE "),
			""
		);
		line.replace(
			line.find(":"),
			strlen(":"),
			"= :" //the extra ':' acts as a marker
		);
		line.erase(
			line.find(":")
		);

		/* //to be moved to variable
		if(olc.lineDataType == OutLineStats::pseudocodeDataTypes::Integer)
		{
			temp = "int(0)";
		}

		else if(olc.lineDataType == OutLineStats::pseudocodeDataTypes::Bool)
		{
			temp = "bool(0)";
		}

		else if(olc.lineDataType == OutLineStats::pseudocodeDataTypes::Real)
		{
			temp = "float(0)";
		}
		else if
		(
			olc.lineDataType == OutLineStats::pseudocodeDataTypes::String ||
			olc.lineDataType == OutLineStats::pseudocodeDataTypes::Char
		)
		{
			temp = "str(0)";
		}
		else if(olc.lineDataType == OutLineStats::pseudocodeDataTypes::None)
		{
			;
		}
		else
		{
			;
		}

		line += temp;
		*/
	}

	else if(olc.mode == olc.Assign){
		line.replace(
			line.find("<-"),
			strlen("<-"),
			"="
		);
	}

	else if(olc.mode == olc.TxtOut){
		line.replace(
			line.find("OUTPUT "),
			strlen("OUTPUT "),
			"print("
		);
		line += ")";
	}

	else if(olc.mode == olc.Input){
		line.replace(
			line.find("INPUT "),
			strlen("INPUT "),
			""
		);
		line += " = input()";
	}

	else if(olc.mode == olc.WhileLoopStart){
		line.replace(
			line.find("WHILE "),
			strlen("WHILE "),
			"while "
		);
		line.replace(
			line.find(" DO"),
			strlen(" DO"),
			":"
		);
	}

	else if(olc.mode == olc.ForLoopStart){
		line.replace(
			line.find("FOR "),
			strlen("FOR "),
			"for "
		);
		line.replace(
			line.find("<-"),
			strlen("<-"),
			"in range("
		);
		line.replace(
			line.find("TO"),
			strlen("TO"),
			","
		);
		line.append("):");
	}

	else if(olc.mode == olc.ForLoopEnd){
		line.replace(
			line.find("NEXT"),
			strlen("NEXT"),
			""
		);
	}

	else if(olc.mode == olc.IfStart){
		line.replace(
			line.find("IF "),
			strlen("IF "),
			"if "
		);
		line.append(":");

		//in case someone decides to write it in the form "IF <condition> THEN"
		if(line.find("THEN") != std::string::npos){
			line.replace(
				line.find("THEN"),
				strlen("THEN"),
				""
			);
		}
	}

	else if(olc.mode == olc.ThenKeyword){
		line.replace(
			line.find("THEN"),
			strlen("THEN"),
			""
		);
	}

	else if(olc.mode == olc.ElseIfKeyword){
		line.replace(
			line.find("ELSE IF"),
			strlen("ELSE IF"),
			"elif"
		);
		line.append(":");
	}

	else if(olc.mode == olc.ElseKeyword){
		line.replace(
			line.find("ELSE"),
			strlen("ELSE"),
			"else:"
		);
	}

	else if(olc.mode == olc.IfEnd){
		line.replace(
			line.find("ENDIF"),
			strlen("ENDIF"),
			""
		);
	}

	else if(olc.mode == olc.BlankLine) { ; }

	else if(olc.mode == olc.UndefinedLine){
		/*line.insert(0, "#");
		line.append(" <--- Undefined line detected here");*/
		std::cout << "PseudocodeSynIdConv.cpp/ConvertLine()\n" 
		<< "In line: " << line << "\n"
		<< "Warning: Converting line known to be undefined according to pseudocode convention\n";
	}

	else{
		std::cout << "PseudocodeSynIdConv.cpp/ConvertLine():\n";
		std::cout << "This condition is never to be. \nWhy are you hacking?";
		exit(1);
	}
}
