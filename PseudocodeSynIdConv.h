#pragma once
#include "global_declarations.h"

class OutLineStats
{
	public:
	int indentLevel;
	enum lineMode
	{
		ArrayDeclare	= 0,
		NormalDeclare	= 1,
		Assign			= 2,
		TxtOut			= 3,
		Input			= 4,
		WhileLoopStart	= 5,
		WhileLoopEnd	= 6,
		ForLoopStart	= 7,
		ForLoopEnd		= 8,
		CommentOnly		= 9,

		IfStart			= 10,
		ThenKeyword		= 11,
		ElseIfKeyword	= 12,
		ElseKeyword		= 13,
		IfEnd			= 14,

		BlankLine		= 15,
		UndefinedLine	= 16
	} mode;

	OutLineStats()
	{
		indentLevel = 0;
		mode = UndefinedLine;
	};

	void DetectMode(std::string &line);
	void DetectIndentLevel(std::string &line);
	void AttemptLineClassify(std::string &line);
};

void PseudocodeSynConv(OutLineStats &olc, std::string &line);