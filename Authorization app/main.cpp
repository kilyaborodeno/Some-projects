#include <GL/glut.h>
#include <stdlib.h>
#include "Button.hpp"
#include "mycanvas.hpp"
#include "input.hpp"
#include "Data.hpp"
#include <iostream>
#include <fstream>
#include <vector>

int W = 640, H = 480;
int frame = 1;
Input login(210, 135, 0, "LOGIN"), password(210, 165, 1, "PASSWORD"); // ���� �����
int input = 3;
Button Enter(260, 200, "ENTER"), Register(260, 250, "REGISTER"); // ������
Button Back(50, 30, "BACK"), Push(280, 225, "PUSH");
Data table; // ������� ������� � �������
bool registration = false; //���� �����������

// ������� ��������������� ����������������� ������������
void greetUser(){
    rectFill(200,20,600,220,hexcolor(255,255,255));
    rectFill(205,25,595,215,hexcolor(160,160,160));

    color(hexcolor(255,255,0));
    glRasterPos2f(220,60);
    textOut << "Hello,";

    color(hexcolor(255,255,0));
    glRasterPos2f(220,80);
    textOut << login.text << "!";
}

// ��������� ������� �� ����������
void key(unsigned char key, int mx, int my)
{
    if(key == 27)
        exit(EXIT_SUCCESS);
    if(frame == 1)
    {
        // ���� ������
        if(input == 1)
        {
            Enter.action = false;
            Register.action = false;
            if(key == 8)
                login.decrease();
            else
                login.increase(key);
        }
        // ���� ������
        if(input == 2)
        {
            Enter.action = false;
            Register.action = false;
            if(key == 8)
                password.decrease();
            else
                password.increase(key);
        }
    }
}

// ��������� ���� ��������
void staticObjects(int fr);
void display()
{
    unsigned int c = changingColor();

    glClearColor(0.7,0.7,0.7,1);
    glClear(GL_COLOR_BUFFER_BIT);

    //������ �����
    if(frame == 1)
    {
        staticObjects(1);
        if(input == 1)
            login.addStick(c);
        if(input == 2)
            password.addStick(c);
        // ��������� ������ ��� ������� �� ������ "ENTER"
        if(Enter.action)
        {
            login.empty();
            password.empty();
        }
        if(Enter.action && login.text.size() > 0)
        {
            if(!table.same(login))
            {
                color(hexcolor(255,0,0));
                glRasterPos2f(120, 310);
                textOut << "Login does not exist";
            }
        }
        if(Enter.action && password.text.size() > 0)
        {
            if(!table.sameCaseSensitice(password))
            {
                color(hexcolor(255,0,0));
                glRasterPos2f(120, 340);
                textOut << "Wrong password";
            }
        }
        // ��������� ������ ��� ������� �� ������ "REGISTER"
        if(Register.action)
        {
            login.empty();
            password.empty();
            if(registration)
            {
                color(hexcolor(0,0,255));
                glRasterPos2f(120, 310);
                textOut << "You have successfully registered";
            }
            else if(table.same(login) && login.text.size())
            {
                color(hexcolor(255,0,0));
                glRasterPos2f(120, 310);
                textOut << "Username already exists";
            }
            else if(!login.properUser() && login.text.size())
            {
                color(hexcolor(255,0,0));
                glRasterPos2f(120, 310);
                textOut << "Inappropriate username";
            }
            if(!password.properPassword() && password.text.size())
            {
                color(hexcolor(255,0,0));
                glRasterPos2f(120, 340);
                textOut << "Inappropriate password";
            }
        }
    }
    // ������ �����
    if(frame == 2)
    {
        staticObjects(2);
    }
    glutSwapBuffers();
}

// ��������� ��������� �������� (���� ������, ������ ...)
void staticObjects(int fr)
{
    switch(fr){
    case 1:
        login.drawBackground();
        login.showText();
        password.drawBackground();
        password.showText();
        Enter.draw();
        Register.draw();
    break;
    case 2:
        Back.draw();
        Push.draw();
        if(Push.action)
            greetUser();
    break;
    }
}

// ��������� �����
void bstate0();
void click(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            if(frame == 1)
            {
                //  ����������� ������� �����
                if(login.under(mx, my))
                    input = 1;
                if(password.under(mx, my))
                    input = 2;
                // ������ ������ "ENTER" (�������� ������������ ������ � ������)
                if(Enter.under(mx, my))
                {
                    Register.action = false;
                    Enter.state = true;
                    Enter.action = true;
                    if(table.consult(login, password))
                        frame = 2;
                }
                // ������ ������ "REGISTER" (�������� ������������ ������ � ������, ����������� ������������)
                if(Register.under(mx, my))
                {
                    registration = false;
                    Enter.action = false;
                    Register.state = true;
                    Register.action = true;
                    if(login.properUser() && password.properPassword() && !table.same(login))
                    {
                        table.registerUser(login, password);
                        registration = true;
                    }

                    table.read();
                }
            }
            if(frame == 2)
            {
                // ������� �� ���������� �����
                if(Back.under(mx, my))
                {
                    frame = 1;
                    Push.action = false;
                }
                // ������ ����������
                if(Push.under(mx, my))
                {
                    Push.state = true;
                    Push.action = true;
                }
            }
        }
        else
            bstate0();
    }
}

// ������� ���� ������ � ��������� ���������
void bstate0()
{
    Enter.state0();
    Register.state0();
    Back.state0();
    Push.state0();
}

// ��������������� ���� ����������������
static void idle(void)
{
    glutPostRedisplay();
}

// ������� �� ���� �������� ������� ����
static void resize(int width, int height){
    glutReshapeWindow(W,H);
    glutPositionWindow(100,100);
}

// ��� ��������� �������� �������� ������ ��� ��������� �� ���� �����
void mouseMove(int mx, int my){
    if(frame == 1 && (login.under(mx, my) || password.under(mx, my)))
        glutSetCursor(GLUT_CURSOR_TEXT);
    else
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

// Entry Point
int main(int argc, char *argv[]){
    table.read();

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    window(W, H, "AUTHORIZATION APP");

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(click);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouseMove);

    glClearColor(0.7,0.7,0.7,1);

    glutMainLoop();

    return EXIT_SUCCESS;
}
