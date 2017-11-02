#pragma once

#include "defs.h"

class TScaner
{
public:

	TScaner(string);
	~TScaner(void);
	void loadFromFile(string);
	int scaner(LEX);
	void paintError(char*,char*);
	int GetUK();
	void SetUK(int _uk);
	int GetLine();
	void SetLine(int _line);
	void SetPos(int _pos);
	int GetPos();
private:
	Module  text;
	int uk;
	int pos;
	int line;
};

