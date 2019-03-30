#include "Field3D.h"

// конструктор
Field3D::Field3D()
{
	// очищаем вектор
	m_points.clear();

	//std::cout << "Field created.\n";
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
	//std::cout << "Field3D::Project_all_points starts\n";

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
	//std::cout << "Azimut ungle (grad) = " << r_AZIMUT << "\n";
	//std::cout << "Zenit ungle (grad) = " << r_ZENIT << "\n";

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

		// спроецированные координаты (заполняется методом Field3D::Kramer)
		double proj_coords[3];

		// инициализация структуры для передачи методу Field3D::Kramer
		Kramer_params params = {
			proj_coords,
			direction_vector,
			dot_real_coord,
			a_coef_plane,
			b_coef_plane,
			c_coef_plane,
			d_coef_plane
		};

		// нахождение спроецированных координат по теореме Крамера
		if (!Kramer(params))
		{
			// если система не имеет решений - выйти из цикла
			break;
		};

		// передача спроецированных координат объекту "Point3D"
		iter->Set_projection_coords(proj_coords);
	};

	//std::cout << "Field3D::Project_all_points done\n";
}

// перевести все точки в 2-хмерное пространство
// [in] const double& - азимутальный угол
// [in] const double& - зенитный угол
void Field3D::Convert_2D(const double& r_AZIMUT, const double& r_ZENIT)
{
	//std::cout << "Convertation points in 2D starts\n";

		// прямые О1Х1 и О1Y1 (оси 2-хмерной системы координат)

	// прямая О1Х1 проходит через центральную точку и точку (1, 0, 0) (до поворота плоскости по азимутальному и зенитному углам)
	// прямая О1Y1 проходит через центральную точку и точку (0, 1, 0) (до поворота плоскости по азимутальному и зенитному углам)
	// необходимо сместить вторую точку согласно азимутальному и зенитному углам

		// начальные координаты точек (до смещения)
	// координаты центральной точки (первая точка)
	double central_point_coord[3] = {0, 0, 0};

	// начальные координаты второй точки O1X1 до смещения
	double O1X1_second_point_coord[3] = {1, 0, 0};

	// начальные координаты второй точки O1Y1 до смещения
	double O1Y1_second_point_coord[3] = {0, 1, 0};


	// перевод зенитного и азимутального углов в int (для кратности 360)
	int int_AZIMUT = (int)r_AZIMUT;
	int int_ZENIT = (int)r_ZENIT;

	// перевод углов в диапазон [0;360] градусов для удобства вычислений
	if (int_AZIMUT <= -360 || int_AZIMUT >= 360)
	{
		int_AZIMUT %= 360;
		//std::cout << "int_AZIMUT = " << int_AZIMUT << "\n";
	};
	if (int_ZENIT <= -360 || int_ZENIT >= 360)
	{
		int_ZENIT %= 360;
		//std::cout << "int_ZENIT = " << int_ZENIT << "\n";
	};

	// избавляемся от отрицательных углов
	if (int_AZIMUT < 0)
	{
		int_AZIMUT += 360;
		//std::cout << "int_AZIMUT = " << int_AZIMUT << "\n";
	};
	if (int_ZENIT < 0)
	{
		int_ZENIT += 360;
		//std::cout << "int_ZENIT = " << int_ZENIT << "\n";
	};

	// азимутальный и зенитный углы в радианах
	double azimut_rad = int_AZIMUT * PI / 180;
	double zenit_rad = int_ZENIT * PI / 180;

	//double azimut_rad = r_AZIMUT * PI / 180;
	//double zenit_rad = r_ZENIT * PI / 180;

		// перемещение второй точки О1Х1
	// новая координата х О1Х1
	O1X1_second_point_coord[0] = std::cos(zenit_rad) * std::cos(azimut_rad);

	// новая координата y О1Х1 (не зависит от азимутального угла)
	O1X1_second_point_coord[1] = std::sin(zenit_rad);

	// новая координата z О1Х1
	O1X1_second_point_coord[2] = (-1) * std::cos(zenit_rad) * std::sin(azimut_rad);

	// координаты второй точки  О1Х1
	//std::cout << "O1X1_second_point_coord: {" << O1X1_second_point_coord[0] << ", " << O1X1_second_point_coord[1] << ", " << O1X1_second_point_coord[2] << "}\n";

		// перемещение второй точки О1Y1
	// новая координата х О1Y1
	O1Y1_second_point_coord[0] = (-1) * std::sin(zenit_rad) * std::sin(azimut_rad);

	// новая координата y О1Y1 (не зависит от азимутального угла)
	O1Y1_second_point_coord[1] = std::cos(zenit_rad);

	// новая координата z О1Y1
	O1Y1_second_point_coord[2] = (-1) * std::sin(zenit_rad) * std::cos(azimut_rad);

	// координаты второй точки  О1Х1
	//std::cout << "O1Y1_second_point_coord: {" << O1Y1_second_point_coord[0] << ", " << O1Y1_second_point_coord[1] << ", " << O1Y1_second_point_coord[2] << "}\n";

		// направляющие вектора прямых О1Х1 и О1Y1
	// направляющий вектор прямой О1Х1
	double O1X1_direction_vector[3] = {
			O1X1_second_point_coord[0] - central_point_coord[0],
			O1X1_second_point_coord[1] - central_point_coord[1],
			O1X1_second_point_coord[2] - central_point_coord[2]
	};

	// направляющий вектор прямой О1Y1
	double O1Y1_direction_vector[3] = {
			O1Y1_second_point_coord[0] - central_point_coord[0],
			O1Y1_second_point_coord[1] - central_point_coord[1],
			O1Y1_second_point_coord[2] - central_point_coord[2]
	};

		// перевод точек в 2D

	// проход по вектору
	for (std::vector<Point3D>::iterator iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		// спроецированные координаты точки
		double point_proj_coord[3];

		// инициализация спроецированных координат точки
		iter->Get_proj_coords(point_proj_coord);

		// координаты точки в 2-хмерном пространстве
		double point_2D_coords[2];

		//std::cout << "point_proj_coord[0] " << point_proj_coord[0] << "\n";
		//std::cout << "point_proj_coord[1] " << point_proj_coord[1] << "\n";
		//std::cout << "point_proj_coord[2] " << point_proj_coord[2] << "\n";

			// нахождение координат 2D по х
		// расстояние от прямой O1Y1 до точки будет координатой х в новой сист. координат

			// уравнение плоскости, проходящей через точку (из вектора) перпендикулярно прямой O1Y1
			// т.к. перпендикулярны, направляющий вектор плоскости равен направляющему вектору прямой
		// vec_x * (x - point_x) + vec_y * (y - point_y) + vec_z * (z - point_z) = 0

		// коэффициент а плоскости
		double O1Y1_a_plane = O1Y1_direction_vector[0];

		// коэффициент b плоскости
		double O1Y1_b_plane = O1Y1_direction_vector[1];

		// коэффициент c плоскости
		double O1Y1_c_plane = O1Y1_direction_vector[2];

		// коэффициент d плоскости
		double O1Y1_d_plane = O1Y1_a_plane * (-1) * point_proj_coord[0] + O1Y1_b_plane * (-1) * point_proj_coord[1] + O1Y1_c_plane * (-1) * point_proj_coord[2];

		//std::cout << "O1Y1_a_plane " << O1Y1_a_plane << "\n";
		//std::cout << "O1Y1_b_plane " << O1Y1_b_plane << "\n";
		//std::cout << "O1Y1_c_plane " << O1Y1_c_plane << "\n";
		//std::cout << "O1Y1_d_plane " << O1Y1_d_plane << "\n";

		// координаты проекции спроецированной точки на прямую O1Y1
		double O1Y1_proj_point[3];

			// поиск точки пересечения прямой O1Y1 и плоскости
		// инициализация структуры для передачи методу Field3D::Kramer
		Kramer_params params_x = {
			O1Y1_proj_point,
			O1Y1_direction_vector,
			// точка, через которую проходит прямая:
			central_point_coord,
			O1Y1_a_plane,
			O1Y1_b_plane,
			O1Y1_c_plane,
			O1Y1_d_plane
		};

		// нахождение спроецированных координат точки на прямую O1Y1 по теореме Крамера
		if (!Kramer(params_x))
		{
			// если система не имеет решений - выйти из цикла
			return;
		};

		//std::cout << "O1Y1_proj_point[0] " << O1Y1_proj_point[0] << "\n";
		//std::cout << "O1Y1_proj_point[1] " << O1Y1_proj_point[1] << "\n";
		//std::cout << "O1Y1_proj_point[2] " << O1Y1_proj_point[2] << "\n";

		// координата х в 2-хмерной системе координат
		// std::pow(x, y) - возведение числа х в степень y
		point_2D_coords[0] = std::sqrt(std::pow(point_proj_coord[0] - O1Y1_proj_point[0], 2) +
							 std::pow(point_proj_coord[1] - O1Y1_proj_point[1], 2) +
							 std::pow(point_proj_coord[2] - O1Y1_proj_point[2], 2));

		//std::cout << "Coord_x = " << point_2D_coords[0] << "\n";

			// нахождение координат 2D по y
		// расстояние от прямой O1X1 до точки будет координатой y в новой сист. координат

			// уравнение плоскости, проходящей через точку (из вектора) перпендикулярно прямой O1X1
			// т.к. перпендикулярны, направляющий вектор плоскости равен направляющему вектору прямой
		// vec_x * (x - point_x) + vec_y * (y - point_y) + vec_z * (z - point_z) = 0

		// коэффициент а плоскости
		double O1X1_a_plane = O1X1_direction_vector[0];

		// коэффициент b плоскости
		double O1X1_b_plane = O1X1_direction_vector[1];

		// коэффициент c плоскости
		double O1X1_c_plane = O1X1_direction_vector[2];

		// коэффициент d плоскости
		double O1X1_d_plane = O1X1_a_plane * (-1) * point_proj_coord[0] + O1X1_b_plane * (-1) * point_proj_coord[1] + O1X1_c_plane * (-1) * point_proj_coord[2];

		//std::cout << "O1X1_a_plane " << O1X1_a_plane << "\n";
		//std::cout << "O1X1_b_plane " << O1X1_b_plane << "\n";
		//std::cout << "O1X1_c_plane " << O1X1_c_plane << "\n";
		//std::cout << "O1X1_d_plane " << O1X1_d_plane << "\n";

		// координаты проекции спроецированной точки на прямую O1X1
		double O1X1_proj_point[3];

			// поиск точки пересечения прямой O1X1 и плоскости
		// повторная инициализация структуры для передачи методу Field3D::Kramer
		Kramer_params params_y = {
			O1X1_proj_point,
			O1X1_direction_vector,
			// точка, через которую проходит прямая:
			central_point_coord,
			O1X1_a_plane,
			O1X1_b_plane,
			O1X1_c_plane,
			O1X1_d_plane
		};

		// нахождение спроецированных координат точки на прямую O1X1 по теореме Крамера
		if (!Kramer(params_y))
		{
			// если система не имеет решений - выйти из цикла
			break;
		};

		//std::cout << "O1X1_proj_point[0] " << O1X1_proj_point[0] << "\n";
		//std::cout << "O1X1_proj_point[1] " << O1X1_proj_point[1] << "\n";
		//std::cout << "O1X1_proj_point[2] " << O1X1_proj_point[2] << "\n";

		// координата y в 2-хмерной системе координат
		// std::pow(x, y) - возведение числа х в степень y
		point_2D_coords[1] = std::sqrt(std::pow(point_proj_coord[0] - O1X1_proj_point[0], 2) +
							 std::pow(point_proj_coord[1] - O1X1_proj_point[1], 2) +
							 std::pow(point_proj_coord[2] - O1X1_proj_point[2], 2));

		//std::cout << "Coord_y = " << point_2D_coords[1] << "\n";

		// корректировка знака координаты в зависимости от положения проецирующей плоскости
		if (int_AZIMUT >= 0 && int_AZIMUT < 90 && int_ZENIT >= 0 && int_ZENIT < 90)
		{

		};

		if (int_AZIMUT >= 0 && int_AZIMUT < 90 && int_ZENIT >= 90 && int_ZENIT < 180)
		{
			point_2D_coords[0] *= (-1);
			point_2D_coords[1] *= (-1);
		};

		if (int_AZIMUT >= 0 && int_AZIMUT < 90 && int_ZENIT >= 180 && int_ZENIT < 270)
		{
			point_2D_coords[0] *= (-1);
			point_2D_coords[1] *= (-1);
		};

		if (int_AZIMUT >= 0 && int_AZIMUT < 90 && int_ZENIT >= 270 && int_ZENIT < 360)
		{

		};
		//##############################################################################
		if (int_AZIMUT >= 90 && int_AZIMUT < 180 && int_ZENIT >= 0 && int_ZENIT < 90)
		{
			point_2D_coords[0] *= (-1);
		};

		if (int_AZIMUT >= 90 && int_AZIMUT < 180 && int_ZENIT >= 90 && int_ZENIT < 180)
		{
			point_2D_coords[1] *= (-1);
		};

		if (int_AZIMUT >= 90 && int_AZIMUT < 180 && int_ZENIT >= 180 && int_ZENIT < 270)
		{
			point_2D_coords[1] *= (-1);
		};

		if (int_AZIMUT >= 90 && int_AZIMUT < 180 && int_ZENIT >= 270 && int_ZENIT < 360)
		{
			point_2D_coords[0] *= (-1);
		};
		//##############################################################################
		if (int_AZIMUT >= 180 && int_AZIMUT < 270 && int_ZENIT >= 0 && int_ZENIT < 90)
		{
			point_2D_coords[0] *= (-1);
		};

		if (int_AZIMUT >= 180 && int_AZIMUT < 270 && int_ZENIT >= 90 && int_ZENIT < 180)
		{
			point_2D_coords[1] *= (-1);
		};

		if (int_AZIMUT >= 180 && int_AZIMUT < 270 && int_ZENIT >= 180 && int_ZENIT < 270)
		{
			point_2D_coords[1] *= (-1);
		};

		if (int_AZIMUT >= 180 && int_AZIMUT < 270 && int_ZENIT >= 270 && int_ZENIT < 360)
		{
			point_2D_coords[0] *= (-1);
		};
		//##############################################################################
		if (int_AZIMUT >= 270 && int_AZIMUT < 360 && int_ZENIT >= 0 && int_ZENIT < 90)
		{

		};

		if (int_AZIMUT >= 270 && int_AZIMUT < 360 && int_ZENIT >= 90 && int_ZENIT < 180)
		{
			point_2D_coords[0] *= (-1);
			point_2D_coords[1] *= (-1);
		};

		if (int_AZIMUT >= 270 && int_AZIMUT < 360 && int_ZENIT >= 180 && int_ZENIT < 270)
		{
			point_2D_coords[0] *= (-1);
			point_2D_coords[1] *= (-1);
		};

		if (int_AZIMUT >= 270 && int_AZIMUT < 360 && int_ZENIT >= 270 && int_ZENIT < 360)
		{

		};
		//##############################################################################



		//std::cout << "Coords in 2D: {" << point_2D_coords[0] << ", " << point_2D_coords[1] << "}\n";

		// установить реальные координаты в 2-хмерной системе координат
		iter->Set_2D_coords(point_2D_coords);
	};

	//std::cout << "Convertation points in 2D done\n";
}

// решение теоремы Крамера
// [in/out] Kramer_param& - параметры для решения теоремы Крамера
// [out] bool - false - нет решений, true - есть решение
bool Field3D::Kramer(const Kramer_params& r_PARAMS)
{
	/*
	std::cout << "r_PARAMS.dot_coord[0] " << r_PARAMS.dot_coord[0] << "\n";
	std::cout << "r_PARAMS.dot_coord[1] " << r_PARAMS.dot_coord[1] << "\n";
	std::cout << "r_PARAMS.dot_coord[2] " << r_PARAMS.dot_coord[2] << "\n\n";

	std::cout << "r_PARAMS.direction_vector[0] " << r_PARAMS.direction_vector[0] << "\n";
	std::cout << "r_PARAMS.direction_vector[1] " << r_PARAMS.direction_vector[1] << "\n";
	std::cout << "r_PARAMS.direction_vector[2] " << r_PARAMS.direction_vector[2] << "\n\n";

	std::cout << "r_PARAMS.a_coef_plane " << r_PARAMS.a_coef_plane << "\n";
	std::cout << "r_PARAMS.b_coef_plane " << r_PARAMS.b_coef_plane << "\n";
	std::cout << "r_PARAMS.c_coef_plane " << r_PARAMS.c_coef_plane << "\n";
	std::cout << "r_PARAMS.d_coef_plane " << r_PARAMS.d_coef_plane << "\n\n";
	*/

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
	if (std::abs(r_PARAMS.direction_vector[1]) < 1E-6 && std::abs(r_PARAMS.direction_vector[0]) < 1E-6)
	{
		// если все коэффициенты 0, то не найти решения
		if (std::abs(r_PARAMS.direction_vector[2]) < 1E-6)
		{
			std::cout << "Error! Impossible to find the projection coordinates of the point\n";
			return false;
		}
		else
		{
				// используется 2-е и 3-е уравнение прямой в 1-ой и 2-ой строке матрицы соответственно
			// столбец х
			matrix[0][0] = r_PARAMS.direction_vector[2];
			matrix[1][0] = 0;

			// столбец y
			matrix[0][1] = 0;
			matrix[1][1] = r_PARAMS.direction_vector[2];

			// столбец z
			matrix[0][2] = r_PARAMS.direction_vector[0] * (-1);
			matrix[1][2] = r_PARAMS.direction_vector[1] * (-1);

			// столбец коэффициентов
			matrix[0][3] = r_PARAMS.dot_coord[0] * r_PARAMS.direction_vector[2] - r_PARAMS.dot_coord[2] * r_PARAMS.direction_vector[0];
			matrix[1][3] = r_PARAMS.dot_coord[1] * r_PARAMS.direction_vector[2] - r_PARAMS.dot_coord[2] * r_PARAMS.direction_vector[1];
		}
	}
	// иначе использовать первое уравнение прямой и второе или третье на выбор
	else
	{
			// коэффициенты 1-го уравнения прямой в 1-ой строке матрицы
		// столбец х
		matrix[0][0] = r_PARAMS.direction_vector[1];

		// столбец y
		matrix[0][1] = r_PARAMS.direction_vector[0] * (-1);

		// столбец z
		matrix[0][2] = 0;

		// столбец коэффициентов
		matrix[0][3] = r_PARAMS.dot_coord[0] * r_PARAMS.direction_vector[1] - r_PARAMS.dot_coord[1] * r_PARAMS.direction_vector[0];

		// если коэффициенты второго уравнения равны 0, использовать 3-е, иначе - использовать 2-е уравнение
		if (std::abs(r_PARAMS.direction_vector[2]) < 1E-6 && std::abs(r_PARAMS.direction_vector[0]) < 1E-6)
		{
				// коэффициенты 3-го уравнения прямой во 2-ой строке матрицы
			// столбец х
			matrix[1][0] = 0;

			// столбец y
			matrix[1][1] = r_PARAMS.direction_vector[2];

			// столбец z
			matrix[1][2] = r_PARAMS.direction_vector[1] * (-1);

			// столбец коэффициентов
			matrix[1][3] = r_PARAMS.dot_coord[1] * r_PARAMS.direction_vector[2] - r_PARAMS.dot_coord[2] * r_PARAMS.direction_vector[1];
		}
		else
		{
				// коэффициенты 2-го уравнения прямой во 2-ой строке матрицы
			// столбец х
			matrix[1][0] = r_PARAMS.direction_vector[2];

			// столбец y
			matrix[1][1] = 0;

			// столбец z
			matrix[1][2] = r_PARAMS.direction_vector[0] * (-1);

			// столбец коэффициентов
			matrix[1][3] = r_PARAMS.dot_coord[0] * r_PARAMS.direction_vector[2] - r_PARAMS.dot_coord[2] * r_PARAMS.direction_vector[0];
		};
	};

		// заполняем 3-ю строку матрицы коэффициентами уравнения плоскости
	// столбец х
	matrix[2][0] = r_PARAMS.a_coef_plane;

	// столбец y
	matrix[2][1] = r_PARAMS.b_coef_plane;

	// столбец z
	matrix[2][2] = r_PARAMS.c_coef_plane;

	// столбец коэффициентов
	matrix[2][3] = (-1) * r_PARAMS.d_coef_plane;

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

		// инициализация спроецированных координат по теореме Крамера
	// координата х проекции точки на плоскость
	r_PARAMS.proj_coords[0] = determinant_x / (determinant + INF_MIN_DELTA);

	// координата y проекции точки на плоскость
	r_PARAMS.proj_coords[1] = determinant_y / (determinant + INF_MIN_DELTA);

	// координата z проекции точки на плоскость
	r_PARAMS.proj_coords[2] = determinant_z / (determinant + INF_MIN_DELTA);

	// система имеет решение
	//std::cout << "System has a solution\n";
	return true;
};

// установка смасштабированных координат
// [in] const double& - расстояние между двумя соседними пикселами
// [in] const double& - координаты центральной точки
void Field3D::Set_scale_coords_all(const double& r_LENGTH_BEHIND_PIXELS, const double* p_CENTER_REAL_COORD)
{
	// проход по вектору
	for (std::vector<Point3D>::iterator iter = m_points.begin(); iter != m_points.end(); ++iter)
	{
		// массив временных реальных координат для прохода по всему полю от левого верхнего угла вправо (по длине) и вниз (по ширине)
		double temp_real_coord[2];

		// установка реальных координат верхней левой точки
		temp_real_coord[0] = p_CENTER_REAL_COORD[0] - FIELD_LENGTH / 2 * r_LENGTH_BEHIND_PIXELS;
		temp_real_coord[1] = p_CENTER_REAL_COORD[1] - FIELD_WIDTH / 2 * r_LENGTH_BEHIND_PIXELS;

		// массив реальных координат верхней правой точки
		double temp_right_real_coord[2];

		// установка реальных координат нижней правой точки
		temp_right_real_coord[0] = p_CENTER_REAL_COORD[0] + FIELD_LENGTH / 2 * r_LENGTH_BEHIND_PIXELS;
		temp_right_real_coord[1] = p_CENTER_REAL_COORD[1] + FIELD_WIDTH / 2 * r_LENGTH_BEHIND_PIXELS;

		// координаты точки
		double point_2D_coord[2];

		// инициализация координат точки
		iter->Get_2D_coords(point_2D_coord);

		// когда точка находится за пределами видимой области - переход к вычислениям следующей точки
		if (temp_real_coord[0] > point_2D_coord[0] || temp_real_coord[1] > point_2D_coord[0] ||
			temp_right_real_coord[0] < point_2D_coord[0] || temp_right_real_coord[1] < point_2D_coord[0])
		{
			continue;
		};

		// массив временных смасштабированных координат для прохода по всему полю от левого верхнего угла вправо (по длине) и вниз (по ширине)
		int temp_scale_coord[2] = {0, 0};

		// смасштабированные координаты точки
		int point_scale_coords[2];

			// поиск смасштабированной координаты по Х для точки
		double delta = INF_MAX_DELTA; // величина, с помощью которой определяется наиболее близкая смасштабированная координата
		//std::cout << "delta " << delta << '\n';

		// проходим по длине поля
		while (temp_scale_coord[0] < FIELD_LENGTH)
		{
			if (delta > std::abs(point_2D_coord[0] - temp_real_coord[0]))
			{
				//std::cout << "x=" << temp_scale_coord[0] << " d=" << std::abs(m_real_coord[0] - temp_real_coord[0]) << '\n';
				// обновляем delta
				delta = std::abs(point_2D_coord[0] - temp_real_coord[0]);

				// присваивает максимально близкую смасштабированную координату при данной delta
				point_scale_coords[0] = temp_scale_coord[0];
			};

			// устанавливаем следующие значения для реал. и смасшт. координат
			temp_real_coord[0] += r_LENGTH_BEHIND_PIXELS;
			temp_scale_coord[0]++;
		};

			// поиск смасштабированной координаты по Y для точки класса Dot
		// возвращаем большое значение для дельты
		delta = INF_MAX_DELTA;

		// проходим по ширине поля
		while (temp_scale_coord[1] < FIELD_WIDTH)
		{
			if (delta > std::abs(point_2D_coord[1] - temp_real_coord[1]))
			{
				//std::cout << "y=" << temp_scale_coord[1] << " d=" << std::abs(m_real_coord[1] - temp_real_coord[1]) << '\n';
				// обновляем delta
				delta = std::abs(point_2D_coord[1] - temp_real_coord[1]);

				// присваивает максимально близкую смасштабированную координату при данной delta
				point_scale_coords[1] = temp_scale_coord[1];
			};

			// устанавливаем следующие значения для реал. и смасшт. координат
			temp_real_coord[1] += r_LENGTH_BEHIND_PIXELS;
			temp_scale_coord[1]++;
		};

		//	установить для точки смасштабированные координаты
		iter->Set_scale_coords(point_scale_coords);

		//std::cout << "point_scale_coords: " << point_scale_coords[0] << " " << point_scale_coords[1] << '\n';
	};
}

// отрисовка
void Field3D::Draw() const
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
	double coords_2D[2];

	// проходим по вектору точек
	for (std::vector<Point3D>::const_iterator ITER = m_points.begin(); ITER != m_points.end(); ++ITER)
	{
		// получаем смасштабированные координаты точки, помещаем в переданный функции массив
		ITER->Get_scale_coords(temp_coord);

		// получаем 2д реальные точки (для отсева точек выходящих за пределы поля)
		ITER->Get_2D_coords(coords_2D);

		// отсев точек выходящих за пределы поля
		if (temp_coord[0] == 0 && temp_coord[1] == 0 && coords_2D[0] != 0.0 && coords_2D[1] != 0.0)
		{
			continue;
		};

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
