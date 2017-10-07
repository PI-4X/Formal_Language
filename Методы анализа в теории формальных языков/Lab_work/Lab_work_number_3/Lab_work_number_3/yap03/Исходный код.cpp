#include "TScaner.h"

int main()
{
	setlocale(2,"");
	TScaner *sc;
	int type; 
	LEX l;
	sc = new TScaner("input.txt");
	do 
	{
		type=sc->scaner(l);
		printf("%s - тип %d \n",l, type);
	}
	while(type!=Tend);
	system("pause");
}