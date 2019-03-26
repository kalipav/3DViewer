#ifndef _POINT3D_H_
#define _POINT3D_H_

class Point3D
{
private:
	// реальные координаты, [0] - по х, [1] - по у, [2] - по z
	double m_real_coord[3];

	// спроецированные на заданную плоскость координаты, [0] - по х, [1] - по у, [2] - по z
	double m_projection_coord[3];

	// спроецированные на плоскость координаты c новой двухмерной системой координат, [0] - по х, [1] - по у
	double m_real_coord_2D[2];

	// относительные координаты для вывода в консоль, [0] - по х, [1] - по у
	int m_scale_coord_2D[2];

public:
	// конструктор
	Point3D(const double&, const double&, const double&);

	// деструктор
	~Point3D();

	// вывод координат точки
	void Show_coords() const;

	// поместить реальные координаты точки в принятый массив
	void Get_real_coords(double*) const;

	// выполнить сброс координат (применяется когда изменяется плоскость проекции)
	void Reset_coords();

	// установить спроецированные координаты из переданного массива
	void Set_projection_coords(const double*);

	// поместить спроецированные координаты точки в принятый массив
	void Get_proj_coords(double*) const;

	// установить реальные координаты в 2-хмерной системе координат
	void Set_2D_coords(const double*);
};

#endif

