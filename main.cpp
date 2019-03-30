#define EMPTY_SYMBOL ' ' // символ незанятой ячейки поля
#define FULL_SYMBOL 'X'  // символ занятой ячейки поля

// при шрифте FreeMono8: макс. высота - 63, длина - 201
#define FIELD_LENGTH 69 // длина поля, должно быть НЕЧЕТНОЕ!!! MAX = 69
#define FIELD_WIDTH 33 // ширина поля, должно быть НЕЧЕТНОЕ!!! MAX = 33

#define INF_MAX_DELTA 1E300  // бесконечно большое значение для delta
#define INF_MIN_DELTA 1E-300  // бесконечно малое значение для delta

#include <iostream>
#include <vector>
#include <cmath>

#include "Field.h"
#include "Field.cpp"

//########################################################################

#include <unistd.h> // для usleep(1000000); + sleep(1); [если задержка 1 сек]
#include <cstdlib>  // для очистки консоли - std::system("clear"); + rand();

#define PI 3.1415926535897932384626433832795    // число пи

#define AZIMUT_ANGLE (double) 180 // в градусах !!!
#define ZENIT_ANGLE  (double) 0 // в градусах !!!

#define LENGTH_BEHIND_PIXELS 1   // расстояние между двумя пикселями в консоли
#define CENTR_2D_COORD_X     0   // реальная координата Х центра поля в консоли
#define CENTR_2D_COORD_Y     0   // реальная координата Y центра поля в консоли

#include "Field3D.h"
#include "Field3D.cpp"

int main()
{
	Field3D my_field;

	my_field.Add_point(0,0,0);
	my_field.Add_point(-10,10,-10);
	my_field.Add_point(10,10,-10);
	my_field.Add_point(-10,10,10);
	my_field.Add_point(10,10,10);



	//my_field.Add_point(0,0,0);
	//my_field.Add_point(1,1,0);
	//my_field.Add_point(2,2,0);
	//my_field.Add_point(3,3,0);
	//my_field.Add_point(4,4,0);
	//my_field.Add_point(5,5,0);
	//my_field.Add_point(6,6,0);
	//my_field.Add_point(7,7,0);
	//my_field.Add_point(8,8,0);
	//my_field.Add_point(9,9,0);

	//my_field.Add_point(0,0,1);
	//my_field.Add_point(4,2,0);

	//my_field.Add_point(1,2,0);
	//my_field.Add_point(3,4,0);

	//my_field.Add_point(10,10,10);
	//my_field.Add_point(2,2,0);

	double azimut = 0, zenit = 0;

	for (int i = 0; i < 1000; ++i, azimut +=1, zenit += 0)
	{
		// очистка экрана
		std::system("clear");

		my_field.Project_all_points(azimut, zenit);
		my_field.Convert_2D(AZIMUT_ANGLE, ZENIT_ANGLE);
		double centr[2] = {0, 0};
		my_field.Set_scale_coords_all(LENGTH_BEHIND_PIXELS, centr);
		my_field.Draw();

		// пауза между отрисовками
		usleep(10000);
	};

	/*
	my_field.Project_all_points(AZIMUT_ANGLE, ZENIT_ANGLE);

	my_field.Show_all_points();

	my_field.Convert_2D(AZIMUT_ANGLE, ZENIT_ANGLE);

	my_field.Show_all_points();

	double centr[2] = {CENTR_2D_COORD_X, CENTR_2D_COORD_Y};
	my_field.Set_scale_coords_all(LENGTH_BEHIND_PIXELS, centr);

	my_field.Show_all_points();

	my_field.Draw();
	*/

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
