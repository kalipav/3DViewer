#include "Dot.h"

// конструктор
// [in] const double& - реальная координата по X
// [in] const double& - реальная координата по Y
Dot::Dot(const double& r_X, const double& r_Y)
{
	// установка реальных координат
	m_real_coord[0] = r_X;
	m_real_coord[1] = r_Y;

	// обнуление смасштабированных координат
	m_scale_coord[0] = 0;
	m_scale_coord[1] = 0;

	std::cout << "Constructor. Set real coordinates of dot.\n";
}

// установить значения координат
// [in] const double& - координата по X
// [in] const double& - координата по Y
void Dot::Set_real_coord(const double& r_X, const double& r_Y)
{
	m_real_coord[0] = r_X;
	m_real_coord[1] = r_Y;

	std::cout << "Set user coordinates of dot.\n";
}

// вернуть реальную координату точки по х
// [out] double - реальная координата точки по X
double Dot::Get_real_x_coord() const
{
	return m_real_coord[0];
}

// вернуть реальную координату точки по y
// [out] double - реальная координата точки по Y
double Dot::Get_real_y_coord() const
{
	return m_real_coord[1];
}

// установка смасштабированных координат точки
// [in] const double& - реальное расстояние между двумя соседними символами
// [in] const double* - реальные координаты центральной точки, массив с длиной [0] и шириной [1]
// [in] const double* - смасштабированные координаты центральной точки, массив с длиной [0] и шириной [1]
void Dot::Set_scale_coord(const double& r_LENGTH_BEHIND, const double* p_REAL_CENTER_COORD)
{
	// массив временных реальных координат для прохода по всему полю от левого верхнего угла вправо (по длине) и вниз (по ширине)
	double temp_real_coord[2];

	// массив временных смасштабированных координат для прохода по всему полю от левого верхнего угла вправо (по длине) и вниз (по ширине)
	int temp_scale_coord[2];

	// установка реальных координат верхней левой точки
	temp_real_coord[0] = p_REAL_CENTER_COORD[0] - FIELD_LENGTH / 2 * r_LENGTH_BEHIND;
	temp_real_coord[1] = p_REAL_CENTER_COORD[1] - FIELD_WIDTH / 2 * r_LENGTH_BEHIND;
	//std::cout << "temp_real_coord[0] " << temp_real_coord[0] << " temp_real_coord[1] " << temp_real_coord[1] << '\n';

	// установка смасштабированных координат верхней левой точки (начало отсчета)
	temp_scale_coord[0] = 0;
	temp_scale_coord[1] = 0;

		// поиск смасштабированной координаты по Х для точки класса Dot
	double delta = INF_DELTA; // величина, с помощью которой определяется наиболее близкая смасштабированная координата
	//std::cout << "delta " << delta << '\n';

	// проходим по длине поля
	while (temp_scale_coord[0] < FIELD_LENGTH)
	{
		if (delta > std::abs(m_real_coord[0] - temp_real_coord[0]))
		{
			//std::cout << "x=" << temp_scale_coord[0] << " d=" << std::abs(m_real_coord[0] - temp_real_coord[0]) << '\n';
			// обновляем delta
			delta = std::abs(m_real_coord[0] - temp_real_coord[0]);

			// присваивает максимально близкую смасштабированную координату при данной delta
			m_scale_coord[0] = temp_scale_coord[0];
		};

		// устанавливаем следующие значения для реал. и смасшт. координат
		temp_real_coord[0] += r_LENGTH_BEHIND;
		temp_scale_coord[0]++;
	};


		// поиск смасштабированной координаты по Y для точки класса Dot
	// возвращаем большое значение для дельты
	delta = INF_DELTA;

	// проходим по ширине поля
	while (temp_scale_coord[1] < FIELD_WIDTH)
	{
		if (delta > std::abs(m_real_coord[1] - temp_real_coord[1]))
		{
			//std::cout << "y=" << temp_scale_coord[1] << " d=" << std::abs(m_real_coord[1] - temp_real_coord[1]) << '\n';
			// обновляем delta
			delta = std::abs(m_real_coord[1] - temp_real_coord[1]);

			// присваивает максимально близкую смасштабированную координату при данной delta
			m_scale_coord[1] = temp_scale_coord[1];
		};

		// устанавливаем следующие значения для реал. и смасшт. координат
		temp_real_coord[1] += r_LENGTH_BEHIND;
		temp_scale_coord[1]++;
	};
	//std::cout << "m_scale_coord[0] " << m_scale_coord[0] << " m_scale_coord[1] " << m_scale_coord[1] << '\n';
}

// помещает в принятый массив смасштабированные координаты точки
// [in/out] int* - массив для размещения смасштабированных координат точки
void Dot::Get_scale_coord(int* p_coord) const
{
	p_coord[0] = m_scale_coord[0];
	p_coord[1] = m_scale_coord[1];
}

