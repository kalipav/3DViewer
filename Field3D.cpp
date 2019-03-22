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

// отобразить все точки поля с координатами
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
	std::cout << "Projection points starts.\n";

		// сбрасываем координаты проекции и смасштабированные координаты, т.к. изменяется плоскость проекции
	// проход по вектору
	for (std::vector<Point3D>::iterator iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		// сброс
		iter->Reset_coords();
	};

	//##########################################################################

		// начальные координаты точек направляющего вектора (центральная не смещается)

	// координаты центральной точки (первая точка для определения направляющего вектора плоскости)
	double central_point_coord[3] = {0, 0, 0};

	// начальные координаты второй точки направляющего вектора плоскости
	double second_point_coord[3] = {0, 0, 1};

	// азимутальный и зенитный углы в радианах
	double azimut_rad = r_AZIMUT * PI / 180;
	double zenit_rad = r_ZENIT * PI / 180;

		// !!! в формуле отсутствует радиус, потому что он равен 1 (умножение на 1)
		// установка координат второй точки направляющего вектора плоскости в зависимости от азимутального и зенитного углов

	// новая координата х
	second_point_coord[0] = std::cos(zenit_rad) * std::sin(azimut_rad);

	// новая координата y (не зависит от азимутального угла)
	second_point_coord[1] = std::sin(zenit_rad);

	// новая координата z
	second_point_coord[2] = std::cos(zenit_rad) * std::cos(azimut_rad);

	// углы в градусах
	std::cout << "Azimut ungle (grad) = " << r_AZIMUT << "\n";
	std::cout << "Zenit ungle (grad) = " << r_ZENIT << "\n";

	// координаты второй точки направляющего вектора
	//std::cout << "second_point_coord: {" << second_point_coord[0] << ", " << second_point_coord[1] << ", " << second_point_coord[2] << "}\n\n";

	//################################################################################################

		// нахождение уравнения плоскости

	// направляющий вектор плоскости
	double direction_vector[3];

	// координаты направляющего вектора (координата конечная "минус" начальная)
	direction_vector[0] = second_point_coord[0] - central_point_coord[0];
	direction_vector[1] = second_point_coord[1] - central_point_coord[1];
	direction_vector[2] = second_point_coord[2] - central_point_coord[2];

	// уравнение плоскости, проходящей через центральную точку
	// формат: a * (x - x_centr) + b * (y - y_centr) + c * (z - z_centr) = 0
	// a * x + b * y + c * z + d = 0, где:
	// a - координата х направляющего вектора
	// b - координата y направляющего вектора
	// c - координата z направляющего вектора
	// d = a * (-x_centr) + b * (-y_centr) + c * (-z_centr)

	// коэффициент а плоскости
	double a_coef_plane = direction_vector[0];

	// коэффициент b плоскости
	double b_coef_plane = direction_vector[1];

	// коэффициент c плоскости
	double c_coef_plane = direction_vector[2];

	// коэффициент d плоскости
	double d_coef_plane = a_coef_plane * (-1) * central_point_coord[0] + b_coef_plane * (-1) * central_point_coord[1] + c_coef_plane * (-1) * central_point_coord[2];

	// коэффициенты плоскости
	//std::cout << "Coefficients of plane:\n";
	//std::cout << "a_coef_plane = " << a_coef_plane << "\n";
	//std::cout << "b_coef_plane = " << b_coef_plane << "\n";
	//std::cout << "c_coef_plane = " << c_coef_plane << "\n";
	//std::cout << "d_coef_plane = " << d_coef_plane << "\n\n";

	//########################################################################

		// нахождение проекций точек на плоскость

	// проход по вектору
	for (std::vector<Point3D>::iterator iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		// координаты точки
		double dot_real_coord[3];

		// инициализация координат точки
		iter->Get_real_coords(dot_real_coord);

		// каноническое уравнение прямой, кот. проходит через точку (из m_points) и имеет направляющий вектор (вектор плоскости)
		// (x - x_coord_dot)/x_vect = (y - y_coord_dot)/y_vect = (z - z_coord_dot)/z_vect

		// (x - x_coord_dot) * y_vect = (y - y_coord_dot) * x_vect
		// (x - x_coord_dot) * z_vect = (z - z_coord_dot) * x_vect
		// (y - y_coord_dot) * z_vect = (z - z_coord_dot) * y_vect

		// y_vect * x - x_vect * y +      0 * z = x_coord_dot * y_vect - y_coord_dot * x_vect
		// z_vect * x +      0 * y - x_vect * z = x_coord_dot * z_vect - z_coord_dot * x_vect
		//      0 * x + z_vect * y - y_vect * z = y_coord_dot * z_vect - z_coord_dot * y_vect

		// ищем пересечение прямой и плоскости
		// используем 2 уравнения прямой (из 3-х на выбор 2, т.к. не всегда можно найти решение) и уравнение плоскости

		//       y_vect * x -       x_vect * y +            0 * z = x_coord_dot * y_vect - y_coord_dot * x_vect
		//       z_vect * x +            0 * y -       x_vect * z = x_coord_dot * z_vect - z_coord_dot * x_vect
		//            0 * x +       z_vect * y -       y_vect * z = y_coord_dot * z_vect - z_coord_dot * y_vect
		// a_coef_plane * x + b_coef_plane * y + c_coef_plane * z = (-1) * d_coef_plane

		// создаем матрицу 3х4
		double matrix[3][4];

			// заполняем матрицу построчно
		// если в уравнении все коэффициенты 0 - его не используем, т.к. не будет решений и все определители будут равны 0

		// если коэффициенты первого уравнения прямой равны 0, использовать 2-е и 3-е уравнение прямой
		if (direction_vector[1] == 0 && direction_vector[0] == 0)
		{
			// если все коэффициенты 0, то не найти решения
			if (direction_vector[2] == 0)
			{
				std::cout << "Ошибка! Невозможно найти координаты точки.\n";
				break;
			}
			else
			{
					// используется 2-е и 3-е уравнение прямой в 1-ой и 2-ой строке матрицы соответственно
				// столбец х
				matrix[0][0] = direction_vector[2];
				matrix[1][0] = 0;

				// столбец y
				matrix[0][1] = 0;
				matrix[1][1] = direction_vector[2];

				// столбец z
				matrix[0][2] = direction_vector[0] * (-1);
				matrix[1][2] = direction_vector[1] * (-1);

				// столбец коэффициентов
				matrix[0][3] = dot_real_coord[0] * direction_vector[2] - dot_real_coord[2] * direction_vector[0];
				matrix[1][3] = dot_real_coord[1] * direction_vector[2] - dot_real_coord[2] * direction_vector[1];
			}
		}
		// иначе использовать первое уравнение прямой и второе или третье на выбор
		else
		{
				// коэффициенты 1-го уравнения прямой в 1-ой строке матрицы
			// столбец х
			matrix[0][0] = direction_vector[1];

			// столбец y
			matrix[0][1] = direction_vector[0] * (-1);

			// столбец z
			matrix[0][2] = 0;

			// столбец коэффициентов
			matrix[0][3] = dot_real_coord[0] * direction_vector[1] - dot_real_coord[1] * direction_vector[0];

			// если коэффициенты второго уравнения равны 0, использовать 3-е, иначе - использовать 2-е уравнение
			if (direction_vector[2] == 0 && direction_vector[0] == 0)
			{
					// коэффициенты 3-го уравнения прямой во 2-ой строке матрицы
				// столбец х
				matrix[1][0] = 0;

				// столбец y
				matrix[1][1] = direction_vector[2];

				// столбец z
				matrix[1][2] = direction_vector[1] * (-1);

				// столбец коэффициентов
				matrix[1][3] = dot_real_coord[1] * direction_vector[2] - dot_real_coord[2] * direction_vector[1];
			}
			else
			{
					// коэффициенты 2-го уравнения прямой во 2-ой строке матрицы
				// столбец х
				matrix[1][0] = direction_vector[2];

				// столбец y
				matrix[1][1] = 0;

				// столбец z
				matrix[1][2] = direction_vector[0] * (-1);

				// столбец коэффициентов
				matrix[1][3] = dot_real_coord[0] * direction_vector[2] - dot_real_coord[2] * direction_vector[0];
			};
		};

			// заполняем 3-ю строку матрицы коэффициентами уравнения плоскости
		// столбец х
		matrix[2][0] = a_coef_plane;

		// столбец y
		matrix[2][1] = b_coef_plane;

		// столбец z
		matrix[2][2] = c_coef_plane;

		// столбец коэффициентов
		matrix[2][3] = (-1) * d_coef_plane;

		/*
		// отобразить матрицу
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				std::cout << matrix[i][j] << " ";
			};
			std::cout << "\n";
		};
		std::cout << "\n";
		*/

			// теорема Крамера
		// вычисляем определитель
		double determinant = matrix[0][0] * matrix[1][1] * matrix[2][2] - matrix[0][0] * matrix[1][2] * matrix[2][1]
		                   - matrix[0][1] * matrix[1][0] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0]
		                   + matrix[0][2] * matrix[1][0] * matrix[2][1] - matrix[0][2] * matrix[1][1] * matrix[2][0];

		// вывод определителя
		//std::cout << "determinant = " << determinant << "\n\n";

		// столбец коэффициентов по х буферизируем
		double buffer[3] = {matrix[0][0], matrix[1][0], matrix[2][0]};

		// заменяем первый столбец коэффициентами из 4-го столбца
		matrix[0][0] = matrix[0][3];
		matrix[1][0] = matrix[1][3];
		matrix[2][0] = matrix[2][3];

		// вычисляем определитель по х
		double determinant_x = matrix[0][0] * matrix[1][1] * matrix[2][2] - matrix[0][0] * matrix[1][2] * matrix[2][1]
				             - matrix[0][1] * matrix[1][0] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0]
				             + matrix[0][2] * matrix[1][0] * matrix[2][1] - matrix[0][2] * matrix[1][1] * matrix[2][0];

		// вывод определителя по х
		//std::cout << "determinant_x = " << determinant_x << "\n\n";

		// 1ый столбец возвращаем из буфера
		matrix[0][0] = buffer[0];
		matrix[1][0] = buffer[1];
		matrix[2][0] = buffer[2];

		// 2-ой столбец буферизируем
		buffer[0] = matrix[0][1];
		buffer[1] = matrix[1][1];
		buffer[2] = matrix[2][1];

		// заменяем второй столбец коэффициентами из 4-го столбца
		matrix[0][1] = matrix[0][3];
		matrix[1][1] = matrix[1][3];
		matrix[2][1] = matrix[2][3];

		// вычисляем определитель по y
		double determinant_y = matrix[0][0] * matrix[1][1] * matrix[2][2] - matrix[0][0] * matrix[1][2] * matrix[2][1]
				             - matrix[0][1] * matrix[1][0] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0]
				             + matrix[0][2] * matrix[1][0] * matrix[2][1] - matrix[0][2] * matrix[1][1] * matrix[2][0];

		// вывод определителя по y
		//std::cout << "determinant_y = " << determinant_y << "\n\n";

		// 2-ой столбец возвращаем из буфера
		matrix[0][1] = buffer[0];
		matrix[1][1] = buffer[1];
		matrix[2][1] = buffer[2];

		// заменяем 3-ий столбец коэффициентами из 4-го столбца
		matrix[0][2] = matrix[0][3];
		matrix[1][2] = matrix[1][3];
		matrix[2][2] = matrix[2][3];

		// вычисляем определитель по z
		double determinant_z = matrix[0][0] * matrix[1][1] * matrix[2][2] - matrix[0][0] * matrix[1][2] * matrix[2][1]
				             - matrix[0][1] * matrix[1][0] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0]
				             + matrix[0][2] * matrix[1][0] * matrix[2][1] - matrix[0][2] * matrix[1][1] * matrix[2][0];

		// вывод определителя по y
		//std::cout << "determinant_z = " << determinant_z << "\n\n";

		// спроецированные координаты
		double proj_coords[3];

			// инициализация спроецированных координат по теореме Крамера
		// координата х проекции точки на плоскость
		proj_coords[0] = determinant_x / (determinant + INF_MIN_DELTA);

		// координата y проекции точки на плоскость
		proj_coords[1] = determinant_y / (determinant + INF_MIN_DELTA);

		// координата z проекции точки на плоскость
		proj_coords[2] = determinant_z / (determinant + INF_MIN_DELTA);

		// передача спроецированных координат объекту "Point3D"
		iter->Set_projection_coords(proj_coords);
	};

	std::cout << "Projection points done.\n";
}

// перевести все точки в 2-хмерное пространство
void Field3D::Convert_2D(const double& r_AZIMUT, const double& r_ZENIT)
{
	std::cout << "Convertation points in 2D starts.\n";

		// начальные координаты точек направляющего вектора (центральная не смещается)

	// координаты центральной точки (первая точка для определения направляющего вектора плоскости)
	double central_point_coord[3] = {0, 0, 0};

	// начальные координаты второй точки направляющего вектора плоскости
	double second_point_coord[3] = {0, 0, 1};

	// азимутальный и зенитный углы в радианах
	double azimut_rad = r_AZIMUT * PI / 180;
	double zenit_rad = r_ZENIT * PI / 180;

		// !!! в формуле отсутствует радиус, потому что он равен 1 (умножение на 1)
		// установка координат второй точки направляющего вектора плоскости в зависимости от азимутального и зенитного углов

	// новая координата х
	second_point_coord[0] = std::cos(zenit_rad) * std::sin(azimut_rad);

	// новая координата y (не зависит от азимутального угла)
	second_point_coord[1] = std::sin(zenit_rad);

	// новая координата z
	second_point_coord[2] = std::cos(zenit_rad) * std::cos(azimut_rad);

	// координаты второй точки направляющего вектора
	//std::cout << "second_point_coord: {" << second_point_coord[0] << ", " << second_point_coord[1] << ", " << second_point_coord[2] << "}\n\n";

	//########################################################################

		// прямые О1Х1 и О1Y1 (оси 2-хмерной системы координат)

	// прямая О1Х1 проходит через центральную точку и точку (1, 0, 0) (до поворота плоскости по азимутальному и зенитному углам)
	// прямая О1Y1 проходит через центральную точку и точку (0, 1, 0) (до поворота плоскости по азимутальному и зенитному углам)
	// необходимо сместить вторую точку согласно азимутальному и зенитному углам

	// начальные координаты второй точки O1X1 до смещения
	double O1X1_second_point_coord[3] = {1, 0, 0};

	// начальные координаты второй точки O1Y1 до смещения
	double O1Y1_second_point_coord[3] = {0, 1, 0};





	//################################################################################################

		// нахождение уравнения плоскости

	// направляющий вектор плоскости
	double direction_vector[3];

	// координаты направляющего вектора (координата конечная "минус" начальная)
	direction_vector[0] = second_point_coord[0] - central_point_coord[0];
	direction_vector[1] = second_point_coord[1] - central_point_coord[1];
	direction_vector[2] = second_point_coord[2] - central_point_coord[2];

	// уравнение плоскости, проходящей через центральную точку
	// формат: a * (x - x_centr) + b * (y - y_centr) + c * (z - z_centr) = 0
	// a * x + b * y + c * z + d = 0, где:
	// a - координата х направляющего вектора
	// b - координата y направляющего вектора
	// c - координата z направляющего вектора
	// d = a * (-x_centr) + b * (-y_centr) + c * (-z_centr)

	// коэффициент а плоскости
	double a_coef_plane = direction_vector[0];

	// коэффициент b плоскости
	double b_coef_plane = direction_vector[1];

	// коэффициент c плоскости
	double c_coef_plane = direction_vector[2];

	// коэффициент d плоскости
	double d_coef_plane = a_coef_plane * (-1) * central_point_coord[0] + b_coef_plane * (-1) * central_point_coord[1] + c_coef_plane * (-1) * central_point_coord[2];

	// коэффициенты плоскости
	//std::cout << "Coefficients of plane:\n";
	//std::cout << "a_coef_plane = " << a_coef_plane << "\n";
	//std::cout << "b_coef_plane = " << b_coef_plane << "\n";
	//std::cout << "c_coef_plane = " << c_coef_plane << "\n";
	//std::cout << "d_coef_plane = " << d_coef_plane << "\n\n";

	//########################################################################

		// перевод точек в 2D

	// проход по вектору
	for (std::vector<Point3D>::iterator iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		// координаты точки
		double dot_real_coord[3];

		// инициализация координат точки
		iter->Get_real_coords(dot_real_coord);
	};






}


