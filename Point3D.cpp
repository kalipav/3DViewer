#include "Point3D.h"

// конструктор
//[in] const double& - координата точки по х
//[in] const double& - координата точки по y
//[in] const double& - координата точки по z
Point3D::Point3D(const double& r_X, const double& r_Y, const double& r_Z)
{
	// инициализируем реальные координаты точки
	m_real_coord[0] = r_X;
	m_real_coord[1] = r_Y;
	m_real_coord[2] = r_Z;

	// обнуляем спроецированные координаты
	m_projection_coord[0] = 0.0;
	m_projection_coord[1] = 0.0;

	// обнуляем смасштабированные координаты
	m_scale_coord[0] = 0;
	m_scale_coord[1] = 0;

	std::cout << "Point created.\n";
}

// деструктор
Point3D::~Point3D()
{}

// вывод координат точки
void Point3D::Show_coords() const
{
	std::cout << "real: " << m_real_coord[0] << ", " << m_real_coord[1] << ", " << m_real_coord[2] << "; ";
	std::cout << "projection: " << m_projection_coord[0] << ", " << m_projection_coord[1] << "; ";
	std::cout << "scale: " << m_scale_coord[0] << ", " << m_scale_coord[1];
}








