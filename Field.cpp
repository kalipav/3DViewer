#include "Field.h"

// конструктор
Field::Field()
{
	// очищаем вектор
	m_dots.clear();

	// задаем полю пустые ячейки
	for (int i = 0; i < FIELD_LENGTH; ++i)
	{
		for (int j = 0; j < FIELD_WIDTH; ++j)
		{
			m_space[i][j] = EMPTY_SYMBOL;
		}
	};

	// зануляем расстояние между двумя соседними символами
	m_length_behind = 0;

	// зануляем координаты центральной точки
	m_center_real_coord[0] = 0;
	m_center_real_coord[1] = 0;

	std::cout << "Empty field created.\n";
}

// деструктор
Field::~Field()
{
	// проходим по вектору и освобождаем память
	for (std::vector<Dot*>::const_iterator ITER = m_dots.begin(); ITER != m_dots.end(); ITER++)
	{
		delete *ITER;
	};

	// очищаем вектор
	m_dots.clear();
}

// добавить точку
// [in] const double& - реальная координата по Х
// [in] const double& - реальная координата по Y
void Field::Add_dot(const double& r_X, const double& r_Y)
{
	// добавляем точку в вектор
	m_dots.push_back(new Dot(r_X, r_Y));

		// вычисляем расстояние между двумя соседними символами
	// временные координаты
	double min_x, min_y, max_x, max_y;

	// проходим по вектору, i нужен для инициализации временных координат
	int i = 0;
	for (std::vector<Dot*>::const_iterator ITER = m_dots.begin(); ITER != m_dots.end(); ++ITER, ++i)
	{
		if (i == 0)
		{
			max_x = min_x = (*ITER)->Get_real_x_coord();
			max_y = min_y = (*ITER)->Get_real_y_coord();
		}
		else
		{
			if (min_x > (*ITER)->Get_real_x_coord())
			{
				min_x = (*ITER)->Get_real_x_coord();
			};
			if (min_y > (*ITER)->Get_real_y_coord())
			{
				min_y = (*ITER)->Get_real_y_coord();
			};
			if (max_x < (*ITER)->Get_real_x_coord())
			{
				max_x = (*ITER)->Get_real_x_coord();
			};
			if (max_y < (*ITER)->Get_real_y_coord())
			{
				max_y = (*ITER)->Get_real_y_coord();
			};
		};
	};
	//std::cout << "max_x " << max_x << " min_x " << min_x << " max_y " << max_y << " min_y " << min_y << "\n";

	// вычисляем максимальные расстояния между точками по осям
	double max_behind_x = max_x - min_x;
	double max_behind_y = max_y - min_y;

	// находим расстояния между двумя соседними точками по х и по у соответственно (-1 для отрезков)
	double min_behind_x = max_behind_x/(FIELD_LENGTH - 1);
	double min_behind_y = max_behind_y/(FIELD_WIDTH - 1);

	// устанавливаем наибольшее значение реального расстояния между соседними точками для сохранения масштаба по осям
	min_behind_x > min_behind_y ? m_length_behind = min_behind_x : m_length_behind = min_behind_y;
	//std::cout << "m_length_behind " << m_length_behind << '\n';

		// находим реальные координаты центральной точки
	m_center_real_coord[0] = min_x + max_behind_x/2.0;
	m_center_real_coord[1] = min_y + max_behind_y/2.0;
	//std::cout << "m_center_real_coord[0] " << m_center_real_coord[0] << " m_center_real_coord[1] " << m_center_real_coord[1] << '\n';

		// устанавливаем для !всех! точек вектора смасштабированные координаты
	// проходим по вектору
	for (std::vector<Dot*>::const_iterator ITER = m_dots.begin(); ITER != m_dots.end(); ++ITER)
	{
		// устанавливаем смасштабированные координаты для каждой точки вектора
		(*ITER)->Set_scale_coord(m_length_behind, m_center_real_coord);
	};
}

// отрисовка
void Field::Draw()
{
	// создаем поле
	char field[FIELD_LENGTH][FIELD_WIDTH];

	// все ячейки поля делаем пустыми
	for (int i = 0; i < FIELD_LENGTH; i++)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			field[i][j] = EMPTY_SYMBOL;
		}
	};

		// заполняем ячейки поля точками
	int temp_coord[2];

	// проходим по вектору точек
	for (std::vector<Dot*>::const_iterator ITER = m_dots.begin(); ITER != m_dots.end(); ++ITER)
	{
		// получаем смасштабированные координаты точки, помещаем в переданный функции массив
		(*ITER)->Get_scale_coord(temp_coord);

		int first = temp_coord[0];
		int second = temp_coord[1];

		field[first][second] = FULL_SYMBOL;
	};

	// отрисовка
	for (int i = 0; i < FIELD_LENGTH; ++i)
	{
		std::cout << "-";
	};
	std::cout << '\n';
			;
    for (int i = FIELD_WIDTH - 1; i >= 0; i--) // сначала по ширине с конца массива, причем -1, чтобы не выйти за границы
    {
    	for (int j = 0; j < FIELD_LENGTH; j++) // потом по длине с начала массива
    	{
    		std::cout << field[j][i];
    	};
    	std::cout << "\n";
    };

	for (int i = 0; i < FIELD_LENGTH; ++i)
	{
		std::cout << "-";
	};
	std::cout << '\n';
}

// добавить отрезок
// [in] const double& - реальная координата по Х начала отрезка
// [in] const double& - реальная координата по Y начала отрезка
// [in] const double& - реальная координата по Х конца отрезка
// [in] const double& - реальная координата по Y конца отрезка
void Field::Add_line_segment(const double& r_DOT1_X, const double& r_DOT1_Y, const double& r_DOT2_X, const double& r_DOT2_Y)
{
	// переписать количество точек, используемых для построения отрезка            <-------------------------------

	// вычисляем количество точек, необходимых для отображения отрезка
	int count_of_dots = std::sqrt(FIELD_LENGTH*FIELD_LENGTH + FIELD_WIDTH*FIELD_WIDTH);
	std::cout << "count_of_dots = " << count_of_dots << '\n';

	// уравнение для отрезка на плоскости: (х-х1)/(х2-х1) = (у-у1)/(у2-у1),
	// где х1 и у1 координаты первой точки, х2 и у2 координаты второй точки

	// длина сегмента проекции отрезка на ось х
	double length_segm_x_proection = std::abs(r_DOT2_X - r_DOT1_X)/count_of_dots;
	// длина сегмента проекции отрезка на ось y
	double length_segm_y_proection = std::abs(r_DOT2_Y - r_DOT1_Y)/count_of_dots;

	// выбираем длину сегмента (бОльшую)
	double segment_length;
	length_segm_x_proection > length_segm_y_proection ? segment_length = length_segm_x_proection : segment_length = length_segm_y_proection;
	std::cout << "segment_length = " << segment_length << '\n';

	// добавляем точки отрезка в вектор
	for (int i = 0; i <= count_of_dots; i++)
	{
		// временная координата х
		double x_dot;

		// условие для установки направления построения отрезка
		if (r_DOT1_X < r_DOT2_X)
		{
			x_dot = r_DOT1_X + i * segment_length;
		}
		else
		{
			x_dot = r_DOT1_X - i * segment_length;
		};

		//std::cout << "r_DOT2_X - r_DOT1_X = " << r_DOT2_X - r_DOT1_X << '\n';

		// из уравнения прямой находим реальную координату у
		// чтобы исключить деление на ноль добавляется бесконечно малое
		double y_dot = (x_dot - r_DOT1_X) / (r_DOT2_X - r_DOT1_X + INF_MIN_DELTA) * (r_DOT2_Y - r_DOT1_Y) + r_DOT1_Y;
		Add_dot(x_dot, y_dot);
	};
}








