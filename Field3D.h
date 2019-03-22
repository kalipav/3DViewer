#ifndef _FIELD3D_H_
#define _FIELD3D_H_

#include "Point3D.h"
#include "Point3D.cpp"

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
};


















#endif // _FIELD3D_H_
