#define EMPTY_SYMBOL ' ' // символ незанятой ячейки поля
#define FULL_SYMBOL 'X'  // символ занятой ячейки поля

// при шрифте FreeMono8: макс. длина - 63, высота - 201
#define FIELD_LENGTH 201 // длина поля, должно быть НЕЧЕТНОЕ!!! MAX = 69
#define FIELD_WIDTH 63 // ширина поля, должно быть НЕЧЕТНОЕ!!! MAX = 33

#define INF_MAX_DELTA 1E300  // бесконечно большое значение для delta
#define INF_MIN_DELTA 1E-300  // бесконечно малое значение для delta

#include <iostream>
#include <vector>
#include <cmath>

#include "Field.h"
#include "Field.cpp"

//########################################################################

#define PI 3.1415926535    // число пи

#define AZIMUT_ANGLE (double) 39   // в градусах !!!
#define ZENIT_ANGLE  (double) 471    // в градусах !!!

#include "Field3D.h"
#include "Field3D.cpp"


int main()
{
	Field3D my_field;

	my_field.Add_point(0,0,0);
	my_field.Add_point(0,3,3);
	my_field.Add_point(4,2,0);

	my_field.Show_all_points();

	my_field.Project_all_points(AZIMUT_ANGLE, ZENIT_ANGLE);

	my_field.Show_all_points();

	my_field.Convert_2D(AZIMUT_ANGLE, ZENIT_ANGLE);

	my_field.Show_all_points();

	return 0;
}

/*
 * main2D
 *
int main()
{
	Field my_field;

	my_field.Add_line_segment(456,-43,140,55);

	my_field.Draw();

	return 0;
}

 */
