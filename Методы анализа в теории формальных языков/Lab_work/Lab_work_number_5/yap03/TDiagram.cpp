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
	int uk1 = 0;
	int line1 = 0;
	int pos = 0;
	while (true)
	{
		uk1 = sc->GetUK();
		line1 = sc->GetLine();
		pos = sc->GetPos();
		t = sc->scaner(l);
		if (t == Tend) return;
		else if (t == Tvoid) {
			int _uk1 = sc->GetUK();
			int _line1 = sc->GetLine();
			int pos1 = sc->GetPos();
			if (sc->scaner(l) == Tmain)
			{
				sc->SetUK(_uk1);
				sc->SetLine(_line1);
				sc->SetPos(pos1);
				mainDeclaration();
			}
			else
			{
				sc->SetUK(uk1);
				sc->SetLine(line1);
				sc->SetPos(pos);
				functionDeclaration();
			}
		}
		else if (t == Tint64 || t == Tint) { sc->SetUK(uk1); sc->SetLine(line1); variableDeclaration(); }
		else sc->paintError("(S) Ошибка в программе\0", l);
	}
}
void TDiagram::mainDeclaration()
{
	LEX l;
	int t;
	t = sc->scaner(l);
	if (t != Tmain) sc->paintError("(mainDeclaration) Ожидался символ 'main'\0", l);
	if (sc->scaner(l) != Topenbracket)
		sc->paintError("(functionDeclaration) Ожидался символ '('\0", l);
	if (sc->scaner(l) != Tclosebracket)
		sc->paintError("(functionDeclaration) Ожидался символ ')'\0", l);
	block();
}
void TDiagram::block()
{
	int uk1 = 0;
	int line1 = 0;
	int pos1 = 0;
	LEX l;
	int t = sc->scaner(l);
	if (t != Topenblock) sc->paintError("(block) Ожидался символ '{'\0", l);
	uk1 = sc->GetUK();
	line1 = sc->GetLine();
	pos1 = sc->GetPos();
	t = sc->scaner(l);
	while (t != Tcloseblock)
	{
		sc->SetUK(uk1);
		sc->SetLine(line1);
		sc->SetPos(pos1);
		if (t != Tint && t != Tint64) {
			
			oneOperator();
			

		}
		else
		{
			
			variableDeclaration();
			
		}
		uk1 = sc->GetUK();
		line1 = sc->GetLine();
		pos1 = sc->GetPos();
		t = sc->scaner(l);
	}
}


void TDiagram::functionDeclaration()
{
	int uk1 = 0;
	int line1 = 0;
	int pos1 = 0;
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
		uk1 = sc->GetUK();
		line1 = sc->GetLine();
		pos1 = sc->GetPos();
		if (t != Tint && t != Tint64)
			sc->paintError("(variableDeclaration) Ожидался символ 'int' или 'int64'\0", l);
		if (sc->scaner(l) != Tid)
			sc->paintError("(functionDeclaration) Ожидался идентификатор\0", l);


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
	int pos1 = sc->GetPos();
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

void TDiagram::oneOperator()
{
	LEX l;
	int t;
	int uk1 = sc->GetUK();
	int line1 = sc->GetLine();
	int pos1 = sc->GetPos();
	int ukArray = 0;
	int lineArray = 0;
	t = sc->scaner(l);
	switch (t)
	{
	case Tfor:
	{
		t = sc->scaner(l);
		if (t != Topenbracket)
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
		line1 = sc->GetLine();
		pos1 = sc->GetPos();
		t = sc->scaner(l);
		if (t == Tminusminus || t == Tplusplus)
		{
			t = sc->scaner(l);
			if (t != Tid)
				sc->paintError("(oneOperator) Ожидался идентификатор\0", l);
		}
		else
		{
			sc->SetUK(uk1);
			sc->SetLine(line1);
			sc->SetPos(pos1);
			expression();
		}
		t = sc->scaner(l);
		if (t != Tclosebracket) sc->paintError("(oneOperator) Ожидался символ ')'\0", l);

		oneOperator();
		break;
	}
	case Tid:
	{
		ukArray = sc->GetUK();
		lineArray = sc->GetLine();
		t = sc->scaner(l);
		if (t == Topenbracket) {
			sc->SetUK(uk1);
			sc->SetLine(line1);
			sc->SetPos(pos1);
			callFunction();
			break;
		}
		if (t == Tassignment) {
			expression();
			if (sc->scaner(l) != Tsemicolon)
				sc->paintError("(oneOperator) Ожидался символ ';'\0", l);
			break;
		}
		if (t == TopenArray)
		{
			/*uk1 = sc->GetUK();
			line1 = sc->GetLine();*/
			
			sc->SetUK(ukArray);
			sc->SetLine(lineArray);
			sc->SetPos(pos1);
			expression();
			if (sc->scaner(l) != Tsemicolon)
				sc->paintError("(oneOperator) Ожидался символ ';'\0", l);
			break;

		}
		sc->SetUK(uk1);
		sc->SetLine(line1);
		sc->SetPos(pos1);
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
		sc->SetPos(pos1);

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
		sc->SetPos(pos1);
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
	int uk = sc->GetUK();
	int line = sc->GetLine();
	int pos1 = sc->GetPos();
	t = sc->scaner(l);
	if (t != Tid)
		sc->paintError("(callFunction) Ожидался идентификатор\0", l);
	t = sc->scaner(l);
	if (t != Topenbracket)
		sc->paintError("(callFunction) Ожидался символ'('\0", l);

	uk = sc->GetUK();
	line = sc->GetLine();
	sc->SetPos(pos1);
	t = sc->scaner(l);
	if (t != Tclosebracket) {
		sc->SetUK(uk);
		sc->SetLine(line);
		sc->SetPos(pos1);
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
	int pos1 = sc->GetPos();
	int line = sc->GetLine();
	LEX l;
	int t;
	t = sc->scaner(l);
	while (t == Tor) {
		expression2();
		uk1 = sc->GetUK();
		pos1 = sc->GetPos();
		line = sc->GetLine();
		t = sc->scaner(l);
	}
	sc->SetLine(line);
	sc->SetUK(uk1);
	sc->SetPos(pos1);
}

void TDiagram::expression2()
{
	expression3();
	int uk1 = sc->GetUK();
	int pos1 = sc->GetPos();
	int line = sc->GetLine();
	LEX l;
	int t;
	t = sc->scaner(l);
	while (t == Tand) {
		expression3();
		uk1 = sc->GetUK();
		pos1 = sc->GetPos(); 
		line = sc->GetLine();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
	sc->SetPos(pos1);
	sc->SetLine(line);
}
void TDiagram::expression3()
{
	expression4();
	int uk1 = sc->GetUK();
	int pos1 = sc->GetPos();
	int line = sc->GetLine();
	LEX l;
	int t;


	t = sc->scaner(l);
	while (t == Tequal || t == Tinequal) {
		expression4();
		uk1 = sc->GetUK();
		pos1 = sc->GetPos();
		line = sc->GetLine();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
	sc->SetPos(pos1);
	sc->SetLine(line);
}
void TDiagram::expression4()
{
	expression5();
	int uk1 = sc->GetUK();
	int pos1 = sc->GetPos();
	int line = sc->GetLine();
	LEX l;
	int t;

	t = sc->scaner(l);
	while (t == Tmore || t == Tless || t == Tequalmore || t == Tequalless) {
		expression5();
		uk1 = sc->GetUK();
		pos1 = sc->GetPos();
		line = sc->GetLine();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
	sc->SetPos(pos1);
	sc->SetLine(line);
}
void TDiagram::expression5()
{
	expression6();
	int uk1 = sc->GetUK();
	int pos1 = sc->GetPos();
	int line = sc->GetLine();
	LEX l;
	int t;

	t = sc->scaner(l);
	while (t == Tplus || t == Tminus) {
		expression6();
		uk1 = sc->GetUK();
		pos1 = sc->GetPos();
		line = sc->GetLine();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
	sc->SetPos(pos1);
	sc->SetLine(line);
}
void TDiagram::expression6()
{
	expression7();
	int uk1 = sc->GetUK();
	int pos1 = sc->GetPos();
	int line = sc->GetLine();
	LEX l;
	int t;

	t = sc->scaner(l);
	while (t == Tmul || t == Tdiv || t == Tpercent) {
		expression7();
		uk1 = sc->GetUK();
		pos1 = sc->GetPos();
		line = sc->GetLine();
		t = sc->scaner(l);
	}
	sc->SetUK(uk1);
	sc->SetPos(pos1);
	sc->SetLine(line);
}
void TDiagram::expression7()
{
	int uk1 = sc->GetUK();
	int pos1 = sc->GetPos();
	int line = sc->GetLine();
	LEX l;
	int t;
	t = sc->scaner(l);
	if (!(t == Tnot || t == Tminus || t == Tplus))
	{
		sc->SetUK(uk1);
		sc->SetPos(pos1);
		sc->SetLine(line);
	}
		
	expression8();
}
void TDiagram::expression8()
{
	int uk1 = sc->GetUK();
	int pos1 = sc->GetPos();
	int line = sc->GetLine();
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
		pos1 = sc->GetPos();
		line = sc->GetLine();
		t = sc->scaner(l);
		if (!(t == Tplusplus || t == Tminusminus)) {
			sc->SetUK(uk1);
			sc->SetPos(pos1);
			sc->SetLine(line);
		}
		break;
	case TopenArray:
	{
		expression();
		t = sc->scaner(l);
		if (t == TcloseArray)
		{
			t = sc->scaner(l);
			expression();
			break;
		}
		sc->paintError("(expression) Ожидалось выражение\0", l);
	}
	case Topenbracket:
	{
		expression();
		if ((t = sc->scaner(l)) != Tclosebracket)
			sc->paintError("(expression) Ожидался символ ')'\0", l);
		break;
	}
		
	default:
		sc->paintError("(expression) Ожидалось выражение\0", l);
	}

}