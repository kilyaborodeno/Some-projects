#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <GL/glut.h>
#include <iostream>
#include "mycanvas.hpp"

class Button{
public:
    int x;
    int y;
    std::string s; // Says
    bool state;
    bool action; // действие кнопки
public:

    // конструктор
    Button(int nx, int ny, std::string ns)
    {
        this->x = nx;
        this->y = ny;
        this->s = ns;
        this->state = false;
        this->action = false;
    }

    //  проверка попадания курсора в зону кнопки
    bool under(int mx, int my)
    {
        if((mx >= x && mx <= x+100) && (my >= y && my <= y+30))
            return true;
        return false;
    }

    // переход кнопки в ненажатое состояние
    void state0()
    {
        state = false;
    }

    // отрисовка кнопки
    void draw()
    {
        switch(state)
        {
            case false:
                notPressed();
            break;
            case true:
                pressed();
            break;
        }
    }

private:
    //  непосредственная отрисовка ненажатой кнопки
    void notPressed(){
        rectFill(x-5, y+5, x+95, y+35, hexcolor(200,200,200));

        color(hexcolor(200, 200, 200));
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(x-5, y+5);
        glVertex2d(x, y);
        glVertex2d(x, y+5);
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(x+95, y+35);
        glVertex2d(x+100, y+30);
        glVertex2d(x+95, y+30);
        glEnd();

        rectFill(x, y, x+100, y+30, hexcolor(255,255,255));

        color(hexcolor(0, 0, 0));
        int x1, y1;
        x1 = x + (100 - s.size()*9)/2;
        y1 = y + 20;
        glRasterPos2f(x1, y1);
        textOut << (std::string)s;
    }
    //  непосредственная отрисовка нажатой кнопки
    void pressed(){
        color(hexcolor(178,178,178));
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(x-5, y+5);
        glVertex2d(x, y);
        glVertex2d(x, y+5);
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(x+95, y+35);
        glVertex2d(x+100, y+30);
        glVertex2d(x+95, y+30);
        glEnd();

        rectFill(x-5, y+5, x+95, y+35, hexcolor(255,255,255));

        color(hexcolor(0, 0, 0));
        int x1, y1;
        x1 = x - 5 + (100 - s.size()*9)/2;
        y1 = y + 25;
        glRasterPos2f(x1, y1);
        textOut << (std::string)s;
    }
public: ~Button(){}
};

#endif // BUTTON_HPP
