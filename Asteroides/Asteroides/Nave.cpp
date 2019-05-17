#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void GotoXY(int x,int y) {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);
}

void OcultarCursor() {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1; //valores de 1 a 100 (estilo cursor)
	cci.bVisible = FALSE; //valores true o false(true muestra en pantalla,false no)
	SetConsoleCursorInfo(hCon,&cci); //cci por referencia para que puedan ser modificados sus datos
}

void PintarLimites() {
	for (int i = 2; i < 78;i++) {
		GotoXY(i, 3);
		printf("%c", 205);
		GotoXY(i, 33);
		printf("%c", 205);
	}
	for (int i = 4; i < 33; i++) {
		GotoXY(2, i);
		printf("%c", 186);
		GotoXY(77, i);
		printf("%c", 186);
	}

	GotoXY(2,3);
	printf("%c", 201);
	GotoXY(2,33);
	printf("%c", 200);
	GotoXY(77,3);
	printf("%c", 187);
	GotoXY(77,33);
	printf("%c", 188);

}

class NAVE {
	int x, y,corazones,vidas;
public:
	NAVE(int x, int y,int corazones,int vidas) :x(x), y(y), corazones(corazones), vidas(vidas) {}; //Constructor
	void Pintar();
	void Borrar();
	void Mover();
	void PintarCorazones();
	void RestarCorazones() { corazones--; };
	void Morir();
	int GetX() { return x; }
	int GetY() { return y; }
};


void NAVE::Pintar() {
	GotoXY(x, y);
	printf("  %c", 30);
	GotoXY(x, y+1);
	printf(" %c%c%c", 40,207,41);
	GotoXY(x, y+2);
	printf("%c%c %c%c", 219, 190, 190,219);
}

void NAVE::Borrar() {
	GotoXY(x, y);
	printf("        ");
	GotoXY(x, y+1);
	printf("        ");
	GotoXY(x, y+2);
	printf("        ");
}

void NAVE::Mover() {
	if (_kbhit()) { //comprobar si hay hit en el keyboard
		char tecla = _getch(); //comprobar que tecla se ha pulsado
		Borrar();
		if (tecla == IZQUIERDA && x > 3)  x--;
		if (tecla == DERECHA && x+6 < 77) x++;
		if (tecla == ARRIBA && y > 4) y--;
		if (tecla == ABAJO && y+3 < 33) y++;
		if (tecla == 'e') corazones--;
		Pintar();
		PintarCorazones();
		
	}
}

void NAVE::PintarCorazones() {
	GotoXY(50, 2);
	printf("VIDAS %d", vidas);
	GotoXY(64,2);
	printf("VIDAS");
	GotoXY(70, 2);
	printf("     ");
	for (int i = 0; i < corazones; i++) {
		GotoXY(70+i, 2);
		printf("%c", 207);
	}
}

void NAVE::Morir() {
	if (corazones == 0) {
		Borrar();
		GotoXY(x, y);
		printf("   **   ");
		GotoXY(x, y+1);
		printf("  ****  ");
		GotoXY(x, y+2);
		printf("   **   ");
		Sleep(200);

		Borrar();
		GotoXY(x, y);
		printf(" * ** *");
		GotoXY(x, y + 1);
		printf("  ****  ");
		GotoXY(x, y + 2);
		printf(" * ** *");
		Sleep(200);
		Borrar();

		vidas--;
		corazones = 5;
		PintarCorazones();
		Pintar();
	}
}

class ASTEROIDE {
	int x, y;
public:
	ASTEROIDE(int x, int y) :x(x), y(y) {};
	void Pintar();
	void Mover();
	void Choque(class NAVE &n); //pasar por referencia para modificar valores
};

void ASTEROIDE::Pintar() {
	GotoXY(x, y);
	printf("%c", 184);
}

void ASTEROIDE::Mover() {
	GotoXY(x,y);
	printf(" ");
	y++;
	if (y > 32) {
		x = rand() % 71 + 4; 
		y = 4;
	}
	Pintar(); //polimorfismo del metodo pintar
}

void ASTEROIDE::Choque(class NAVE &n) {
	if (x >= n.GetX() && x < n.GetX() + 6 && y >= n.GetY() && y<= n.GetY()+2) {
		n.RestarCorazones();
		n.Borrar();
		n.Pintar();
		n.PintarCorazones();
		x = rand() % 71 + 4;
		y = 4;
	}
}

class DISPARO{
	int x, y;
public:
	DISPARO(int x, int y) :x(x), y(y) {};
	void Mover();
};

void DISPARO::Mover() {
	GotoXY(x, y);
	printf(" ");
	if (y > 4)
		y--;
	GotoXY(x, y);
	printf("*");
}


int main() {
	bool game_over = false;
	NAVE n(40, 28, 5, 3);
	ASTEROIDE a(10, 4), a2(4, 8), a3(15, 10), a4(5, 8);
	list < DISPARO*> D;
	list < DISPARO*>::iterator iter;

	OcultarCursor();
	PintarLimites();

	n.PintarCorazones();
	n.Pintar();
	while (!game_over) {
		
		if (_kbhit) {
			char tecla = _getch();
			if (tecla == 'a') {
				D.push_back(new DISPARO(n.GetX() + 2, n.GetY() - 1));
			}
		}

		for (iter = D.begin(); iter != D.end();iter++) {
			(*iter)->Mover();
		}

		a.Mover();
		a.Choque(n);
		a2.Mover();
		a2.Choque(n);
		a3.Mover();
		a3.Choque(n);
		a4.Mover();
		a4.Choque(n);

		n.Morir();
		n.Mover();
		//Sleep(30); //Dar descanso al procesador por 30 ms
	}
}