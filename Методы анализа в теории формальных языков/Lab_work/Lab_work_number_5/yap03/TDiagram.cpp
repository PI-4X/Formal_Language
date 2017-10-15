#include "TDiagram.h"



TDiagram::TDiagram(TScaner *_sc)
{
	sc = _sc;
}


TDiagram::~TDiagram(void)
{
}


void TDiagram::S()
{
	LEX l;
	int t;
	int t_next;
	while (true)
	{
		int uk1 = sc->GetUK();
		int line1 = sc->GetLine();
		t = sc->scaner(l);
		if (t == Tend) return;
		else if (t == Tvoid) {
			int _uk1 = sc->GetUK();
			int _line1 = sc->GetLine();
			if (sc->scaner(l) == Tmain)
			{
				sc->SetUK(_uk1); sc->SetLine(_line1); mainDeclaration();
			}
			else
			{
				sc->SetUK(uk1); sc->SetLine(line1); functionDeclaration();
			}
				
		}
		else if (t == Tint64) { sc->SetUK(uk1); sc->SetLine(line1); variableDeclaration(); }
		else if (t == Tint) { sc->SetUK(uk1); sc->SetLine(line1); variableDeclaration(); }
		else if (t == Tid || t == Tfor || t == Treturn) { sc->SetUK(uk1); sc->SetLine(line1);  oneOperator();}
		else sc->paintError("(S) Ошибка в программе\0", l);
	}
}
void TDiagram::mainDeclaration()
{
	LEX l;
	int t;
	t = sc->scaner(l);
	if (t != Tmain) sc->paintError("(mainDeclaration) Ожидался символ 'main'\0", l);
	t = sc->scaner(l);
	if (t != Topenbracket)
		sc->paintError("(functionDeclaration) Ожидался символ '('\0", l);
	if (sc->scaner(l) != Tclosebracket)
		sc->paintError("(functionDeclaration) Ожидался символ ')'\0", l);
	block();
}
void TDiagram::functionDeclaration()
{
	LEX l;
	int t;
	t = sc->scaner(l);
	if (t != Tvoid) sc->paintError("(functionDeclaration) Ожидался символ 'void'\0", l);
	t = sc->scaner(l);
	if (sc->scaner(l) != Topenbracket)
		sc->paintError("(functionDeclaration) Ожидался символ '('\0", l);
	do {
		t = sc->scaner(l);
		if (t == Tclosebracket) break;
		if (t != Tint && t != Tint64)
			sc->paintError("(variableDeclaration) Ожидался символ 'int' или 'int64'\0", l);
		if (sc->scaner(l) != Tid)
			sc->paintError("(functionDeclaration) Ожидался идентификатор\0", l);

		//Добавить идентификатор	
		t = sc->scaner(l);
		if (t != Tcomma && t != Tclosebracket)
			sc->paintError("(functionDeclaration) Ожидался символ ')'\0", l);
	} while (t != Tclosebracket);
	block();
}
void TDiagram::variableDeclaration() {
	LEX l;
	int t;
	int uk1 = sc->GetUK();
	t = sc->scaner(l);
	if (t != Tint && t != Tint64)
		sc->paintError("(variableDeclaration) Ожидался символ 'int' или 'int64'\0", l);
	do {
		if (sc->scaner(l) != Tid)
			sc->paintError("(variableDeclaration) Ожидался идентификатор\0", l);

		t = sc->scaner(l);
		if (t == Tsemicolon) break;
		if (t == Tcomma) continue;
		if (t != Tassignment && t != TopenArray)
			sc->paintError("(variableDeclaration) Ожидался символ '=' или '['\0", l);
		if (t == Tassignment)
		{
			uk1 = sc->GetUK();
			t = sc->scaner(l);
			if (t == Tid)
			{
				sc->SetUK(uk1);
				oneOperator();
				break;
			}
			else if (t == Tconst10 || t == Tconst16) {}
			else
				expression();
		}		
		else
		{
			t = sc->scaner(l);
			if (t != Tconst10)
				sc->paintError("(variableDeclaration) Ожидалась символ Tconst10\0", l);
			t = sc->scaner(l);
			if (t != TcloseArray)
				sc->paintError("(variableDeclaration) Ожидался символ ']'\0", l);

			t = sc->scaner(l);
			if (t == Tsemicolon) break;

			if (t != Tassignment)
				sc->paintError("(variableDeclaration) Ожидался символ '='\0", l);

			t = sc->scaner(l);
			if (t != Topenblock)
				sc->paintError("(variableDeclaration) Ожидался символ '{'\0", l);

			while (true)
			{
				t = sc->scaner(l);
				if (t != Tconst10)
					sc->paintError("(variableDeclaration) Ожидалась символ 'Tconst10'\0", l);
				t = sc->scaner(l);
				if (t == Tcloseblock) break;
				if (t != Tcomma)
					sc->paintError("(variableDeclaration) Ожидалась символ 'Tconst10'\0", l);
			}

		}
		t = sc->scaner(l);
		if (t != Tcomma && t != Tsemicolon)
			sc->paintError("(variableDeclaration) Ожидался ';'\0", l);
	} while (t != Tsemicolon);

}
void TDiagram::block()
{
	int uk1 = 0;
	int line1 = 0;
	LEX l;
	int t;
	t = sc->scaner(l);
	if (t != Topenblock) sc->paintError("(block) Ожидался символ '{'\0", l);
	
	uk1 = sc->GetUK();
	line1 = sc->GetLine();
	t = sc->scaner(l);
	while (t != Tcloseblock)
	{
		sc->SetUK(uk1);
		t = sc->scaner(l);		
		if (t != Tint && t != Tint64) {
			sc->SetUK(uk1);
			sc->SetLine(line1);
			oneOperator();
		}
			
		else
		{
			sc->SetUK(uk1);
			sc->SetLine(line1);
			variableDeclaration();
		}			
		t = sc->scaner(l);
	}
}

void TDiagram::oneOperator()
{
	LEX l;
	int t;
	int uk1 = sc->GetUK();
	int line1 = sc->GetLine();
	t = sc->scaner(l);	
	switch (t)
	{
		case Tfor:
		{
			t = sc->scaner(l);
			if(t!=Topenbracket) 
				sc->paintError("(oneOperator) Ожидался символ '('\0", l);
			t = sc->scaner(l);
			if (t != Tint)
				sc->paintError("(oneOperator) Ожидался идентификатор\0", l);
			t = sc->scaner(l);
			if (t != Tid) 
				sc->paintError("(oneOperator) Ожидался идентификатор\0", l);
			t = sc->scaner(l);
			if (t != Tassignment) 
				sc->paintError("(oneOperator) Ожидался символ '='\0", l);
			expression();
			t = sc->scaner(l);
			if (t != Tsemicolon) 
				sc->paintError("(oneOperator) Ожидался символ ';'\0", l);

			expression();

			t = sc->scaner(l);
			if (t != Tsemicolon) sc->paintError("(oneOperator) Ожидался символ ';'\0", l);
			
			uk1 = sc->GetUK();
			t = sc->scaner(l);
			if (t == Tminusminus || t == Tplusplus)
			{
				t = sc->scaner(l);
				if(t != Tid)
					sc->paintError("(oneOperator) Ожидался идентификатор\0", l);
			}
			else
			{
				sc->SetUK(uk1);
				expression();
			}					
			t = sc->scaner(l);			
			if (t != Tclosebracket) sc->paintError("(oneOperator) Ожидался символ ')'\0", l);

			oneOperator();
			break;
		}
		case Tid:
		{
			t = sc->scaner(l);
			if (t == Topenbracket) {	
				sc->SetUK(uk1);
				sc->SetLine(line1);
				callFunction();
				break;
			}
			if (t == Tassignment) {				
				expression();
				if (sc->scaner(l) != Tsemicolon)
					sc->paintError("(oneOperator) Ожидался символ ';'\0", l);
				break;
			}
			sc->SetUK(uk1);
			sc->SetLine(line1);
			expression();
			if (sc->scaner(l) != Tsemicolon)
				sc->paintError("(oneOperator) Ожидался символ ';'\0", l);
			break;
		}
		case Treturn:
		{
			if (sc->scaner(l) != Tsemicolon)
				sc->paintError("(oneOperator) Ожидался символ ';'\0", l);
			break;
		}		
		case Topenblock:
		{
			sc->SetUK(uk1);
			sc->SetLine(line1);
			block();
			break;
		}
		case Tsemicolon:
			break;
		case Topenbracket:
		case Tplus:
		case Tplusplus:
		case Tminus:
		case Tminusminus:
		case Tnot:
			sc->SetUK(uk1);
			sc->SetLine(line1);
			expression();
			if (sc->scaner(l) != Tsemicolon)
				sc->paintError("(oneOperator) Ожидался символ ';'\0", l);
			break;
		default:
			 sc->paintError("(oneOperator) Ожидался оператор\0", l);		
	}
}

void TDiagram::callFunction()
{
	LEX l;
	int t;
	t = sc->scaner(l);
	if (t != Tid)
		sc->paintError("(callFunction) Ожидался идентификатор\0", l);
	t = sc->scaner(l);
	if (t != Topenbracket)
		sc->paintError("(callFunction) Ожидался символ'('\0", l);
	
	int uk1 = sc->GetUK();
	int line1 = sc->GetLine();

	t = sc->scaner(l);
	if (t != Tclosebracket) {
		sc->SetUK(uk1);
		sc->SetLine(line1);
		do {
			expression();		
			t = sc->scaner(l);
			if (t != Tclosebracket && t != Tcomma)
				sc->paintError("(callFunction) Ожидался символ')'\0", l);
		} while (t != Tclosebracket);
		
	}
	if (sc->scaner(l) != Tsemicolon)
		sc->paintError("(callFunction) Ожидался ';'\0", l);

}

void TDiagram::expression()
{
	expression2();
	int uk1 = sc->GetUK();
	LEX l;
	int t;	
	t = sc->scaner(l);
	while (t == Tor) {
		expression2();
		uk1 = sc->GetUK();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
}

void TDiagram::expression2()
{
	expression3();
	int uk1 = sc->GetUK();
	LEX l;
	int t;
	t = sc->scaner(l);
	while (t == Tand) {	
		expression3();
		uk1 = sc->GetUK();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
}
void TDiagram::expression3()
{
	expression4();
	int uk1 = sc->GetUK();
	LEX l;
	int t;

	
	t = sc->scaner(l);
	while (t == Tequal || t == Tinequal) {
		expression4();
		uk1 = sc->GetUK();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
}
void TDiagram::expression4()
{
	expression5();
	int uk1 = sc->GetUK();
	LEX l;
	int t;
	
	t = sc->scaner(l);
	while (t == Tmore || t == Tless || t == Tequalmore || t == Tequalless) {
		expression5();
		uk1 = sc->GetUK();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
}
void TDiagram::expression5()
{
	expression6();
	int uk1 = sc->GetUK();
	LEX l;
	int t;
	
	t = sc->scaner(l);
	while (t == Tplus || t == Tminus) {
		expression6();
		uk1 = sc->GetUK();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
}
void TDiagram::expression6()
{
	expression7();
	int uk1 = sc->GetUK();
	LEX l;
	int t;
	
	t = sc->scaner(l);
	while (t == Tmul || t == Tdiv || t == Tpercent) {
		expression7();
		uk1 = sc->GetUK();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
}
void TDiagram::expression7()
{
	int uk1 = sc->GetUK();
	LEX l;
	int t;	
	t = sc->scaner(l);
	if (!(t == Tnot || t == Tminus || t == Tplus))
		sc->SetUK(uk1);
	expression8();
}
void TDiagram::expression8()
{
	int uk1 = sc->GetUK();
	LEX l;
	int t;
	
	t = sc->scaner(l);
	switch (t)
	{
	case Tconst10:
	case Tconst16:
		break;
	case Tminusminus:
	case Tplusplus:
		if ((t = sc->scaner(l)) != Tid)
			sc->paintError("(expression) Ожидался идентификатор\0", l);
	case Tid:		
		uk1 = sc->GetUK();
		t = sc->scaner(l);
		if (!(t == Tplusplus || t == Tminusminus)) {
			sc->SetUK(uk1);		
		}
		break;
	case Topenbracket:
		if ((t = sc->scaner(l)) != Tclosebracket)
			sc->paintError("(expression) Ожидался символ ')'\0", l);
		break;
	default:
		sc->paintError("(expression) Ожидалось выражение\0", l);
	}

}