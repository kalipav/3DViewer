#ifndef _DOT_H_
#define _DOT_H_

class Dot
{
private:
	// координаты по x и y реальные
	double m_real_coord[2];

	// координаты по x и y смасштабированные
	int m_scale_coord[2];

public:
	// конструктор
	Dot(const double&, const double&);

	// установить значения реальных координат
	void Set_real_coord(const double&, const double&);

	// вернуть реальную координату точки по х
	double Get_real_x_coord() const;

	// вернуть реальную координату точки по y
	double Get_real_y_coord() const;

	// установка смасштабированных координат точки
	void Set_scale_coord(const double&, const double*);

	// помещает в принятый массив смасштабированные координаты точки
	void Get_scale_coord(int*) const;
};

#endif
