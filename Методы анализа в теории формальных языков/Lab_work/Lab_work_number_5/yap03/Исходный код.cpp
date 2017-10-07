#include "TScaner.h"
#include "TDiagram.h"
using namespace std;
int main()
{	
	setlocale(2, "");
	TScaner * sc;
	// ввести файл с исходным модулем:
	sc = new TScaner("input.txt");// файл по умолчанию
	TDiagram *dg = new TDiagram(sc);
	dg->S();
	int type; LEX l;
	type = sc->scaner(l);
	if (type != Tend) sc->paintError("Лишний текст в конце программы.", "");
	else cout << "Проверка завершена." << endl;
	system("pause");
}