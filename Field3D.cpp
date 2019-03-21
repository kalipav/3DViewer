#include "Field3D.h"

// конструктор
Field3D::Field3D()
{
	// очищаем вектор
	m_points.clear();

	std::cout << "Field created.\n";
}

// деструктор
Field3D::~Field3D()
{}

// добавить точку в поле
//[in] const double& - координата точки по х
//[in] const double& - координата точки по y
//[in] const double& - координата точки по z
void Field3D::Add_point(const double& r_X, const double& r_Y, const double& r_Z)
{
	// создаем временную точку
	Point3D temp_point(r_X, r_Y, r_Z);

	// копируем точку в вектор
	m_points.push_back(temp_point);
}

// отобразить все точки вектора
void Field3D::Show_all_points() const
{
	// порядковый номер элемента в векторе, начиная с 1
	int i = 1;

	// проход по вектору, инкремент счетчика (начиная с единицы)
	for (std::vector<Point3D>::const_iterator ITER = m_points.begin(); ITER != m_points.end(); ++ITER, ++i)
	{
		// вывод счетчика
		std::cout << i << ". ";

		// вывод координат точки
		ITER->Show_coords();

		// перевод на новую строчку
		std::cout << std::endl;
	}
}

// спроецировать все точки на плоскость
// [in] const double& - азимутальный угол
// [in] const double& - зенитный угол
void Field3D::Project_all_points(const double& r_AZIMUT, const double& r_ZENIT)
{
		// нахождение уравнения плоскости

	// максимальные и минимальные значения по осям
	double max_x, max_y, max_z, min_x, min_y, min_z;

	// поиск максимальных значений







	//########################################################################







}
