#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define MAXLEX 50
#define MAXTEXT 1000
#define MAXKEY 7
typedef char LEX[MAXLEX];
typedef char Module[MAXTEXT];



//Идентификатор
#define Tid				1

//Константы
#define Tconst10		21
#define Tconst16		22

//Ключевые слова
#define	Tvoid			31
#define Tint64       	32
#define Tint			33
#define Tfor			34
#define Treturn			35
#define Tmain			36

//Операции
#define Tassignment		41
#define Tor				42
#define Tand			43
#define Tequal			44
#define Tinequal		45
#define Tmore			46
#define Tless			47
#define Tequalmore		48
#define Tequalless		49
#define Tplus			50
#define Tminus			51
#define Tmul			52
#define Tdiv			53
#define Tpercent		54
#define Tplusplus		55
#define Tminusminus		56
#define Tnot			57

//Разделители
#define Tcomma		    61
#define Tsemicolon	    62

//Скобки
#define Topenblock	    71
#define Tcloseblock	    72
#define Topenbracket	73
#define Tclosebracket	74
#define TopenArray		75
#define TcloseArray     76

//Конец исходного модуля
#define Tend			101

//Ошибочная лексема
#define Terror			102

//Сообщения об ошибках
#define InvalidCharacter "Неизвестный символ"
#define LenthOfConstant  "Длина константы превышает допустимую"
#define FileIsNotOpen    "Не удалось открыть файл"


	
	
	
	
	
	
	
