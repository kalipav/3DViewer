#ifndef _FIELD3D_H_
#define _FIELD3D_H_

#include "Point3D.h"
#include "Point3D.cpp"

// параметры метода Field3D::Kramer
struct Kramer_params
{
	// спроецированные координаты (x, y, z)
	double* proj_coords;

	// координаты направляющего вектора (x, y, z)
	const double* direction_vector;

	// координаты точки (x, y, z), через которую прохидит прямая
	const double* dot_coord;

	// коэффициент а плоскости
	const double a_coef_plane;

	// коэффициент b плоскости
	const double b_coef_plane;

	// коэффициент c плоскости
	const double c_coef_plane;

	// коэффициент d плоскости
	const double d_coef_plane;
};

class Field3D
{
private:
	// вектор точек
	std::vector <Point3D> m_points;

public:
	// конструктор
	Field3D();

	// деструктор
	~Field3D();

	// добавить точку в поле
	void Add_point(const double&, const double&, const double&);

	// отобразить все точки поля c координатами
	void Show_all_points() const;

	// спроецировать все точки на плоскость
	void Project_all_points(const double&, const double&);

	// перевести все точки в 2-хмерное пространство
	void Convert_2D(const double&, const double&);

	// решение теоремы Крамера
	bool Kramer(const Kramer_params&);

	// установка смасштабированных координат
	void Set_scale_coords_all(const double&, const double*);

	// отрисовка
	void Draw() const;
};

#endif // _FIELD3D_H_
