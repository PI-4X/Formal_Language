#include "TScaner.h"
#include "TDiagram.h"
using namespace std;
int main()
{	
	setlocale(2, "");
	TScaner * sc;
	// ������ ���� � �������� �������:
	sc = new TScaner("input.txt");// ���� �� ���������
	TDiagram *dg = new TDiagram(sc);
	dg->S();
	int type; LEX l;
	type = sc->scaner(l);
	if (type != Tend) sc->paintError("������ ����� � ����� ���������.", "");
	else cout << "�������� ���������." << endl;
	system("pause");
}