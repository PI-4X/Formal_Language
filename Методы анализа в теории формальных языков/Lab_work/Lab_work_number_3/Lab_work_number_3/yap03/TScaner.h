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
private:
	Module  text;
	int uk;
	int pos;
	int line;
};

