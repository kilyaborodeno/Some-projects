#ifndef DATA_HPP
#define DATA_HPP

#include <vector>
#include <fstream>
#include <iostream>

class Data{
private:
    std::vector<std::string> v;
public:
    // Функция read считывает информацию из файла и возвращает массив со строками логинов и паролей
    void read()
    {
        std::ifstream file("Table.txt");
        std::string s;
        if(!file)
        {
            file.close();
            return;
        }
        while(!file.eof()){
            getline(file,s);
            v.push_back(s);
        }
        file.close();
    }

    // функция возвращает размер таблицы
    unsigned int Size()
    {
        return v.size();
    }

    // функция сверяет логин и пароль с данными в таблице
    bool consult(Input& login, Input& password)
    {
        for(size_t i = 0; i < v.size()/2; ++i)
            if(login.checkCaseInsensitive(v[i*2]) && password.checkCaseSensitive(v[i*2+1]))
                return true;
        return false;
    }

    // проверка существования введенного логина
    bool same(Input& login)
    {
        for(size_t i = 0; i < v.size()/2; ++i)
            if(login.checkCaseInsensitive(v[i*2]))
                return true;
        return false;
    }

    // проверка существования введенного пароля
    bool sameCaseSensitice(Input& password)
    {
        for(size_t i = 0; i < v.size()/2; ++i)
            if(password.checkCaseSensitive(v[i*2+1]))
                return true;
        return false;
    }

    // непосредственная регистрация пользователя
    void registerUser(Input& login, Input& password)
    {
        std::ofstream in("Table.txt", std::ios_base::app);
        in << login.text << '\n';
        in << password.text << '\n';
        read();
    }
};

#endif // DATA_HPP
