#include "TScaner.h"


TScaner::TScaner(string path)
{
	loadFromFile(path);
	uk = 0;
	line = 1;
	pos = 1;
}

LEX keywords[MAXKEY] = { "void", "int", "int64",
	"for","return", "main" };
int indexkeywords[MAXKEY] = { Tvoid, Tint,
	Tint64, Tfor,Treturn ,Tmain };


TScaner::~TScaner(void)
{
}

int TScaner::GetUK()
{
	return uk;
}

void TScaner::SetUK(int _uk)
{
	uk = _uk;
}
int TScaner::GetLine()
{
	return line;
}

void TScaner::SetLine(int _line)
{
	line = _line;
}

void TScaner::loadFromFile(string path)
{
	ifstream inp(path);
	if (!inp.is_open()) paintError(FileIsNotOpen, "");
	int i;
	for (i = 0; i < MAXTEXT && !inp.eof(); i++)
	{
		text[i] = inp.get();
	}
	text[i - 1] = '\0';
	inp.close();
}


void TScaner::paintError(char* error, char* l)
{
	if (l[0] == '\0')
		cout << "Ошибка: " << error << endl;
	else
		cout << "Ошибка: " << error << ". Неверный символ: " << l << endl;
}


int TScaner::scaner(LEX lex)
{
	int len;
	for (len = 0; len < MAXLEX; len++) lex[len] = 0;
	len = 0;
start:
	//Пропуск незначимых символов
	while (text[uk] == ' ' || text[uk] == '\n' || text[uk] == '\t')
	{
		if (text[uk] == '\n')
		{
			line++;
			pos = 1;
		}
		else
			pos++;
		uk++;
	};
	//Пропуск однострочного комментария
	if (text[uk] == '/' && text[uk + 1] == '/')
	{
		uk += 2;
		pos += 2;
		while (text[uk] != '\n'&&text[uk] != '\0')
		{
			pos++;
			uk++;
		}
		if (text[uk] == '\n')
		{
			pos = 1;
			line++;
			uk++;
		}
		goto start;
	}
	//Пропуск многострочного комментария
	if (text[uk] == '/' && text[uk + 1] == '*') {
		uk += 2;
		while (text[uk] != '\0') {
			if (text[uk] == '*' && text[uk + 1] == '/') {
				uk += 2;
				goto start;
			}
			uk++;
		}
		goto start;
	}
	//Конец модуля
	if (text[uk] == 0)
		return Tend;
	//Идентификатор
	if (text[uk] >= 'a' && text[uk] <= 'z' ||
		text[uk] >= 'A' &&  text[uk] <= 'Z')
	{
		while (text[uk] >= 'a' && text[uk] <= 'z' ||
			text[uk] >= 'A' &&  text[uk] <= 'Z' ||
			text[uk] >= '0' && text[uk] <= '9' ||
			text[uk] == '_')
			if (len < MAXLEX - 1)
			{
				lex[len++] = text[uk++];
				pos++;
			}
			else
			{
				uk++;
				pos++;
			};
		for (int i = 0; i < MAXKEY; i++)
		{
			if (strcmp(keywords[i], lex) == 0)
				return indexkeywords[i];
		}
		return Tid;
	}
	//Константы
	if (text[uk] >= '1' && text[uk] <= '9')
	{
	number: while (text[uk] >= '0' && text[uk] <= '9')
	{
		if (len < MAXLEX - 1)
		{
			lex[len++] = text[uk++];
			pos++;
		}
		else
		{
			paintError(LenthOfConstant, lex);
			while (text[uk] >= '1' && text[uk] <= '9') { uk++; pos++; }
			return Terror;
		}

	}
			return Tconst10;
	}

	if (text[uk] == '0')
	{
		lex[len++] = text[uk++];
		pos++;
		if (text[uk] >= '0' && text[uk] <= '9') goto number;
		if (text[uk] == 'x' || text[uk] == 'X') {
			lex[len++] = text[uk++];
			pos++;
			while (text[uk] >= '0' && text[uk] <= '9' ||
				text[uk] >= 'A' && text[uk] <= 'F')
			{
				if (len < MAXLEX - 1)
				{
					lex[len++] = text[uk++];
					pos++;
				}
				else
				{
					uk++;
					pos++;
				}
			}
			if (len >= 9) return Terror;
			return Tconst16;
		}
	}


	


	if (text[uk] == '=')
	{
		lex[len++] = text[uk++];
		pos++;
		if (text[uk] == '=')
		{
			lex[len++] = text[uk++];
			pos++;
			return Tassignment;
		}
		return Tassignment;
	}
	if (text[uk] == '!'&&text[uk + 1] == '=')
	{
		lex[len++] = text[uk++];
		lex[len++] = text[uk++];
		pos += 2;
		return Tinequal;
	}
	if (text[uk] == '>')
	{
		lex[len++] = text[uk++];
		pos++;
		if (text[uk] == '=')
		{
			lex[len++] = text[uk++];
			pos++;
			return Tmore;
		}
		return Tmore;
	}
	if (text[uk] == '<')
	{
		lex[len++] = text[uk++];
		pos++;
		if (text[uk] == '=')
		{
			lex[len++] = text[uk++];
			pos++;
			return Tequalless;
		}
		return Tless;
	}
	if (text[uk] == '+')
	{
		lex[len++] = text[uk++];
		pos++;
		if (text[uk] == '+')
		{
			lex[len++] = text[uk++];
			pos++;
			return Tplusplus;
		}
		return Tplus;
	}
	if (text[uk] == '-')
	{
		lex[len++] = text[uk++];
		pos++;
		if (text[uk] == '-')
		{
			lex[len++] = text[uk++];
			pos++;
			return Tminusminus;
		}
		return Tminus;
	}
	if (text[uk] == '/')
	{
		lex[len++] = text[uk++];
		pos++;
		return Tdiv;
	}
	if (text[uk] == '*')
	{
		lex[len++] = text[uk++];
		pos++;
		return Tmul;
	}
	if (text[uk] == '%')
	{
		lex[len++] = text[uk++];
		pos++;
		return Tpercent;
	}

	if (text[uk] == '|')
	{
		lex[len++] = text[uk++];
		pos++;
		if (text[uk] == '|')
		{
			lex[len++] = text[uk++];
			pos++;
			return Tor;
		}
		//return Terror;
	}
	if (text[uk] == '&')
	{
		lex[len++] = text[uk++];
		pos++;
		if (text[uk] == '&')
		{
			lex[len++] = text[uk++];
			pos++;
			return Tand;
		}
		//return Terror;
	}
	if (text[uk] == ',')
	{
		lex[len++] = text[uk++];
		pos++;
		return Tcomma;
	}
	if (text[uk] == ';')
	{
		lex[len++] = text[uk++];
		pos++;
		return Tsemicolon;
	}
	if (text[uk] == '{')
	{
		lex[len++] = text[uk++];
		pos++;
		return Topenblock;
	}
	if (text[uk] == '}')
	{
		lex[len++] = text[uk++];
		pos++;
		return Tcloseblock;
	}
	if (text[uk] == '(')
	{
		lex[len++] = text[uk++];
		pos++;
		return Topenbracket;
	}
	if (text[uk] == ')')
	{
		lex[len++] = text[uk++];
		pos++;
		return Tclosebracket;
	}
	if (text[uk] == '[')
	{
		lex[len++] = text[uk++];
		pos++;
		return TopenArray;
	}
	if (text[uk] == ']')
	{
		lex[len++] = text[uk++];
		pos++;
		return TcloseArray;
	}
	lex[len++] = text[uk++];
	paintError(InvalidCharacter, lex);
	pos++;
	return Terror;
}