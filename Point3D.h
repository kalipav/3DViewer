#ifndef _POINT3D_H_
#define _POINT3D_H_

class Point3D
{
private:
	// реальные координаты, [0] - по х, [1] - по у, [2] - по z
	double m_real_coord[3];

	// спроецированные на плоскость координаты c новой двухмерной системой координат, [0] - по х, [1] - по у
	double m_projection_coord[2];

	// относительные координаты для вывода в консоль, [0] - по х, [1] - по у
	int m_scale_coord[2];

public:
	// конструктор
	Point3D(const double&, const double&, const double&);

	// деструктор
	~Point3D();

	// вывод координат точки
	void Show_coords() const;
};

#endif

