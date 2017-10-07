#pragma once
#include "TScaner.h"
class TDiagram
{
private:
	TScaner *sc;

public:
	TDiagram(TScaner *_sc);
	~TDiagram(void);
	void S();

	void functionDeclaration();
	void variableDeclaration();
	void mainDeclaration();

	void block();
	void oneOperator();
	void callFunction();
	
	void expression();
	void expression2();
	void expression3();
	void expression4();
	void expression5();
	void expression6();
	void expression7();
	void expression8();

};