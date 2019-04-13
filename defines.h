#ifndef _DEFINES_H_
#define _DEFINES_H_

#define EMPTY_SYMBOL ' ' // символ незанятой ячейки поля
#define FULL_SYMBOL 'X'  // символ занятой ячейки поля

// при шрифте FreeMono8: макс. высота - 63, длина - 201
#define FIELD_LENGTH 11 // длина поля, должно быть НЕЧЕТНОЕ!!! MAX = 69
#define FIELD_WIDTH 11 // ширина поля, должно быть НЕЧЕТНОЕ!!! MAX = 33

#define INF_MAX_DELTA 1E300  // бесконечно большое значение для delta
#define INF_MIN_DELTA 1E-300  // бесконечно малое значение для delta

#define PI 3.1415926535897932384626433832795    // число пи

//#define AZIMUT_ANGLE (double) 0 // в градусах !!!
//#define ZENIT_ANGLE  (double) 0 // в градусах !!!

#define LENGTH_BEHIND_PIXELS 1   // расстояние между двумя пикселями в консоли
#define CENTR_2D_COORD_X     0   // реальная координата Х центра поля в консоли
#define CENTR_2D_COORD_Y     0   // реальная координата Y центра поля в консоли

#endif