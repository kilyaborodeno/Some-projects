#include <iostream>
#include <string>
#include <webby.hpp>
#include <iterator>
#include <set>

using namespace std;

// Создаем массив клиентов
set<webby::Socket*> clients;
// Создаем сокет для приема нового подключения
webby::Socket* accept_socket;

void accept();
void on_read(webby::Socket* s, set<webby::Socket*>::iterator sender);

void on_send(webby::Socket* s, set<webby::Socket*>::iterator sender)
{
	s->read_until("\r\n", [s, sender](){on_read(s, sender);});
}

// Если клиент отправил сообщение
void on_read(webby::Socket* s, set<webby::Socket*>::iterator sender)
{
	// Считываем сообщение
	string str;
	s->get_line(str, "\r\n"); 
	if(str != "") cout << str << endl;
	// Посылаем всем клиентам, кроме отправителя
	for(auto i : clients)
		if(i != *sender) i->send(str + "\r\n", [s, sender](){on_send(s, sender);});
}

void on_accepted()
{
	// Добавляем в массив нового клиента
	clients.insert(accept_socket);
	cout << "New client connected" << endl;

	for(auto i = clients.begin(); i	!= clients.end(); i++)
	{
		// Удаляем отключившегося клиента из массива
		if(!*i) 
		{
			clients.erase(i);
			continue;
		}
		// Пробуем получить сообщения
		try{
		(*(i))->read_until("\r\n", [i](){on_read(*i, i);});
		} catch(std::exception e) {};
	}
	// Запускаем функцию для подключения нового клиента
	accept();
}

void accept()
{
	accept_socket = new webby::Socket;
	webby::accept(*accept_socket, on_accepted, 2000);
}

int main()
{
	accept();
	webby::run();
}
