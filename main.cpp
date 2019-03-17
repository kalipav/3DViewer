#define EMPTY_SYMBOL ' ' // символ незанятой ячейки поля
#define FULL_SYMBOL 'X'  // символ занятой ячейки поля

#define FIELD_LENGTH 69 // длина поля, должно быть НЕЧЕТНОЕ!!! MAX = 69
#define FIELD_WIDTH 33 // ширина поля, должно быть НЕЧЕТНОЕ!!! MAX = 33

#define INF_DELTA 1E300  // бесконечно большое значение для delta

#include <iostream>
#include <vector>
#include <cmath>

#include "Field.h"
#include "Field.cpp"


int main()
{
	Field my_field;

	my_field.Add_dot(0,0);
	my_field.Add_dot(5,10);
	my_field.Add_dot(10,0);
	my_field.Add_dot(5,-10);

	my_field.Draw();

	return 0;
}
