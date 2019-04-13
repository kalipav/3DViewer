#include <iostream>
#include <vector>
#include <cmath>
#include <windows.h>
#include <cstdlib>  // для очистки консоли - std::system("cls"); + rand();

#include "Field.h"
#include "Field3D.h"

int main()
{
	Field3D my_field;

	//my_field.Add_point(0,0,0);
	//my_field.Add_point(2,2,0);
	//my_field.Add_point(-2,2,0);
	//my_field.Add_point(2,-2,0);
	//my_field.Add_point(-2,-2,0);

	//my_field.Add_point(0,2,0);

	//my_field.Add_point(3,-3,3);
	//my_field.Add_point(3,-3,-3);
	//my_field.Add_point(-3,-3,3);
	//my_field.Add_point(-3,-3,-3);

	//my_field.Add_point(-3,0,-3);
	//my_field.Add_point(-2,0,-2);
	//my_field.Add_point(-1,0,-1);
	my_field.Add_point(0,0,0);
	//my_field.Add_point(1,0,1);
	//my_field.Add_point(2,0,2);
	//my_field.Add_point(3,0,3);

	my_field.Add_point(-2,-3,0);
	my_field.Add_point(2,3,0);

	double azimut = 0, zenit = 0;

	for (int i = 0; i < 1000; ++i, azimut += 10, zenit += 0)
	{
		// очистка экрана
		std::system("cls");

		my_field.Project_all_points(azimut, zenit);
		my_field.Convert_2D(azimut, zenit);
		double centr[2] = {CENTR_2D_COORD_X, CENTR_2D_COORD_Y};
		my_field.Set_scale_coords_all(LENGTH_BEHIND_PIXELS, centr);
		my_field.Show_all_points();
		my_field.Draw();
		std::cout << i << "\n";

		// пауза между отрисовками
		Sleep(100);
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
