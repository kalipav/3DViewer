#include "Point3D.h"
#include <math.h>
#include <iostream>

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
	m_projection_coord[2] = 0.0;

	// обнуляем спроецированные координаты (в 2-хмере)
	m_real_coord_2D[0] = 0.0;
	m_real_coord_2D[1] = 0.0;

	// обнуляем смасштабированные координаты (в 2-хмере)
	m_scale_coord_2D[0] = 0;
	m_scale_coord_2D[1] = 0;

	//std::cout << "Point created.\n";
}

// деструктор
Point3D::~Point3D()
{}

// вывод координат точки
void Point3D::Show_coords() const
{
	std::cout << "real: " << m_real_coord[0] << ", " << m_real_coord[1] << ", " << m_real_coord[2] << "; ";
	std::cout << "projection: " << m_projection_coord[0] << ", " << m_projection_coord[1] << ", " << m_projection_coord[2] << "; ";
	std::cout << "projection 2D: " << m_real_coord_2D[0] << ", " << m_real_coord_2D[1] << "; ";
	std::cout << "scale: " << m_scale_coord_2D[0] << ", " << m_scale_coord_2D[1];
}

// поместить реальные координаты точки в принятый массив
// [in/out] double* - указатель на принятый массив
void Point3D::Get_real_coords(double* p_coords) const
{
	p_coords[0] = m_real_coord[0];
	p_coords[1] = m_real_coord[1];
	p_coords[2] = m_real_coord[2];
}

// выполнить сброс координат (применяется когда изменяется плоскость проекции)
void Point3D::Reset_coords()
{
	// обнуляем спроецированные координаты
	m_projection_coord[0] = 0.0;
	m_projection_coord[1] = 0.0;
	m_projection_coord[2] = 0.0;

	// обнуляем спроецированные координаты (в 2-хмере)
	m_real_coord_2D[0] = 0.0;
	m_real_coord_2D[1] = 0.0;

	// обнуляем смасштабированные координаты (в 2-хмере)
	m_scale_coord_2D[0] = 0;
	m_scale_coord_2D[1] = 0;

	//std::cout << "Point coords reset.\n";
}

// установить спроецированные координаты из переданного массива
// [in] const double* - массив спроецированных координат
void Point3D::Set_projection_coords(const double* p_PROJ_COORDS)
{
	std::abs(p_PROJ_COORDS[0]) < 1E-6 ? m_projection_coord[0] = 0 : m_projection_coord[0] = p_PROJ_COORDS[0];
	std::abs(p_PROJ_COORDS[1]) < 1E-6 ? m_projection_coord[1] = 0 : m_projection_coord[1] = p_PROJ_COORDS[1];
	std::abs(p_PROJ_COORDS[2]) < 1E-6 ? m_projection_coord[2] = 0 : m_projection_coord[2] = p_PROJ_COORDS[2];
}

// поместить спроецированные координаты точки в принятый массив
// [in/out] double* - указатель на принятый массив
void Point3D::Get_proj_coords(double* p_coords) const
{
	p_coords[0] = m_projection_coord[0];
	p_coords[1] = m_projection_coord[1];
	p_coords[2] = m_projection_coord[2];
}

// установить реальные координаты в 2-хмерной системе координат
// [in] const double* - указатель на принятый массив
void Point3D::Set_2D_coords(const double* p_COORDS)
{
	std::abs(p_COORDS[0]) < 1E-6 ? m_real_coord_2D[0] = 0 : m_real_coord_2D[0] = p_COORDS[0];
	std::abs(p_COORDS[1]) < 1E-6 ? m_real_coord_2D[1] = 0 : m_real_coord_2D[1] = p_COORDS[1];
}

// поместить координаты точки в 2-хмерном пространстве в принятый массив
// [in/out] double* - указатель на принятый массив
void Point3D::Get_2D_coords(double* p_coords) const
{
	p_coords[0] = m_real_coord_2D[0];
	p_coords[1] = m_real_coord_2D[1];
}

// установить смасштабированные координаты в 2-хмерной системе координат
// [in] const int* - указатель на принятый массив
void Point3D::Set_scale_coords(const int* p_COORDS)
{
	m_scale_coord_2D[0] = p_COORDS[0];
	m_scale_coord_2D[1] = p_COORDS[1];
}

// поместить смасштабированные координаты точки в принятый массив
// [in/out] double* - указатель на принятый массив
void Point3D::Get_scale_coords(int* p_coords) const
{
	p_coords[0] = m_scale_coord_2D[0];
	p_coords[1] = m_scale_coord_2D[1];
}


