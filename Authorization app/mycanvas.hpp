#ifndef MYCANVAS_HPP
#define MYCANVAS_HPP


#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>

#define M_PI 3.14159265358979323846

// Создание окна
inline void window(int w = 640, int h = 480, char* n = "LOGIN"){
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(w, h);
	glutCreateWindow(n);
	gluOrtho2D(0, w, h, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

// функция дает возможность хранить 4 параметра цвета в 1 числе
inline int hexcolor(int r, int g, int b, int alpha = 255){
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) +
		(alpha & 0xff);
}

// задает цвет
inline void color4(int r, int g, int b, int alpha = 255){
	glColor4f(float(r)/255, float(g)/255, float(b)/255, float(alpha)/255);
}

// преобразует число в 4 параметра цвета
inline void color(int c){

	color4(
		(c >> 24) & 0xff,
		(c >> 16) & 0xff,
		(c >> 8) & 0xff,
		c & 0xff
	);
}

// окружность
inline void circle(float x, float y, float radius, unsigned int c = 0x000000ff){
	glBegin(GL_LINES);
	color(c);
	float vx = x + radius;
	float vy = y;

	const int n = 1.5*radius;

	for(int i = 0; i <= n; ++i){
		glVertex2f(vx,vy);
		vx = x + radius*cos(float(i) * 2*M_PI/n);
		vy = y + radius*sin(float(i) * 2*M_PI/n);
		glVertex2f(vx,vy);
	}
	glEnd();
}

// круг
inline void circleFill(float x, float y, float radius, unsigned int c = 0x000000ff){
	glBegin(GL_TRIANGLE_FAN);
	color(c);
	float vx = x + radius;
	float vy = y;

	const int n = 1.5*radius;

	for(float i = 0; i <= n; i+=0.1){
		glVertex2f(vx,vy);
		vx = x + radius*cos(float(i) * 2*M_PI/n);
		vy = y + radius*sin(float(i) * 2*M_PI/n);
		glVertex2f(vx,vy);
	}
	glEnd();
}

// заполненный прямоугольник
inline void rectFill(float x1, float y1, float x2, float y2, unsigned int c = 0x000000ff){
	glPushMatrix();
	glBegin(GL_POLYGON);
	color(c);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
	glPopMatrix();
}

// линия
inline void line(float x1, float y1, float x2, float y2, int c = 0x000000ff)
{
	glBegin(GL_LINES);
		color(c);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}
// изменение цвета в зависимости от времени
unsigned int changingColor()
{
    long long int t = glutGet(GLUT_ELAPSED_TIME)/300;
    if(t&2)
        return hexcolor(0,0,0);
    return hexcolor(255,255,255);
}

// отрисовка текста
class TextOut {};

static TextOut textOut;

inline void print(const std::string& s){
	for(size_t i = 0; i < s.size(); ++i)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s[i]);
}

// перегрузка оператора
template<typename T>
TextOut& operator<<(TextOut& o, const T& v){
	std::stringstream str;
	str << v;
	const  std::string& s = str.str();
	print(s);

	return o;
}

#endif // MYCANVAS_HPP


