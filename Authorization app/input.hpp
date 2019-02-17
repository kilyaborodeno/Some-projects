#ifndef INPUT_HPP
#define INPUT_HPP

#include <GL/glut.h>
#include "mycanvas.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

class Input{
private:
    int x, y;
    int width, height;
    unsigned int background;
    unsigned int textColor;
    bool textType; // hidden or not
    std::string label;
public:
    std::string text;
    // конструктор
    Input(int nx, int ny, bool ntType, std::string nlabel){
        x = nx;
        y = ny;
        width = 240;
        height = 20;
        background = hexcolor(255,255,255,255);
        textColor = hexcolor(0,0,0,255);
        textType = ntType;
        label = nlabel;
    }

    // отрисовка поля ввода и названия поля
    void drawBackground()
    {
        rectFill(x, y, x+width, y+height, background);
        color(hexcolor(0,0,0));
        glRasterPos2f(x - 10 - (9 * label.size()), y+height-5);
        textOut << label;
    }

    // вывод текста на экран
    void showText()
    {
        if(textType)
            showHiddenText();
        else
            showUnhiddenText();
    }

    // проверка на пустое поле и вывод ошибки
    void empty()
    {
        if(text.size() == 0)
        {
            std::string s = label;
            for(int i = 0; i < s.size(); ++i)
                if(isupper(s[i]))
                    s[i] = char(s[i] - 'A' + 'a');
            color(hexcolor(255,0,0));
            int y;
            if(label == "LOGIN")
                glRasterPos2f(120, 310);
            if(label == "PASSWORD")
                glRasterPos2f(120, 340);
            textOut << "The field of " << s << " is empty";
        }
    }

    // курсор
    void addStick(unsigned int c)
    {
        int lx;
        if(!textType)
        {
            if(text.size() < 25) lx = x+6+text.size()*9;
            else lx = x+6+25*9;
            line(lx, 138, lx, 152, c);
        }
        else
        {
            if(text.size() <= 21) lx = x+5+11*text.size();
            else lx = lx = x+5+11*21;
            line(lx, y+3, lx, y+height-3, c);
        }
    }

    // проверка на совпадение без учета регистра
    bool checkCaseInsensitive(std::string v)
    {
        bool property = false;
        std::string s = v;
        std::string lowerText = text;
        for(size_t i = 0; i < s.size(); i++)
        {
            if(isupper(s[i]))
                s[i] = char(s[i] - 'A' + 'a');
        }
        for(size_t i = 0; i < lowerText.size(); i++)
        {
            if(isupper(lowerText[i]))
                lowerText[i] = char(lowerText[i] - 'A' + 'a');
        }
        if(lowerText == s)
            property = true;
        return property;
    }

    // проверка на совпадение без учета регистра
    bool checkCaseSensitive(std::string s)
    {
        bool property = false;
        if(text == s)
            property = true;
        return property;
    }

    // проверка правильности логина для регистрации
    bool properUser()
    {
        bool empty = true;
        int atPos = -1;
        bool at = false;
        bool dot = false;
        if(text.size() >= 5)
        {
            for(size_t i = 0; i < text.size(); ++i)
            {
                if(text[i] == '@')
                {
                    atPos = i;
                    at = true;
                }

                if((text[i] == '.') && (i > atPos) && (atPos != -1))
                {
                    dot = true;
                    if((atPos >= 1) && (i-atPos > 1) && (i != text.size()-1))
                    empty = false;
                }
            }
        }
        if(!empty && at && dot)
            return true;
        return false;
    }

    // проверка правильности пароля для регистрации
    bool properPassword()
    {
        bool check = false;
        int caps = 0;
        int digits = 0;
        if(text.size() >= 6)
        {
            for(size_t i = 0; i < text.size(); ++i)
            {
                if(isupper(text[i]))
                    caps++;
                if(isdigit(text[i]))
                    digits++;
            }

            if(caps >= 1 && digits >= 2)
                return true;
        }
            return false;
    }

    // проверка попадания куросра в поле ввода
    bool under(int mx, int my)
    {
        if((mx >= x && mx <= x+width) && (my >= y && my <= y+height))
            return true;
        return false;
    }

    // осуществление ввода с клавиатура
    void increase(unsigned char letter)
    {
        text += letter;
    }
    // удаление последней буквы кнопкой Backspace
    void decrease()
    {
        text = text.substr(0, text.size()-1);
    }

private:
    // Непосредственная отрисовка читаемого текста
    void showUnhiddenText()
    {
        glRasterPos2f(x+5, y+height-5);
        if(text.size() > 25){
        textOut << text.substr(text.size()-25, text.size());
        line(x+5, y+3, x+5, y+height-3, textColor);
        }
        else textOut << text;
    }

    // Непосредственная отрисовка нечитаемого текста
    void showHiddenText()
    {
        int r = 4;
        int counter = text.size();
        if(counter > 21) counter = 21;
        if(text.size() > 21) line(x+5, y+3, x+5, y+height-3, hexcolor(0,0,0));
        int i = 0;
        while(counter){
            circleFill(x+10 + i*11, y+10, r, textColor);
            i++;
            counter--;
        }
    }
};
#endif // INPUT_HPP
