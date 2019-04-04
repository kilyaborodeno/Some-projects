/*
если на 2000 порту запущен сервер, то данная программа присоединится и отправит серверу введенную строку message.
если сервер может открыть данный файл, он пошлет строку
ОК размер\r\n 
а далее передаст все байты файла f.txt
если файл открыть не удалось, то сервер ответит строкой ERROR.

Например, пусть в файле f.txt находится строка "abcd". Тогда диалог клиента и сервера будет выглядеть так:
клиент:
f.txt\r\n
сервер:
OK 4\r\nabcd
*/

#include <webby.hpp>
#include <ifaddrs.h>
#include <iostream>
#include <fstream>
#include <glcanvas.hpp>

using namespace std;
using namespace cnv;

webby::Socket sock;
string message;
void message_sent();
void read_message();
void on_data();
string my_ip();
void new_message();

int main(int argc, char** argv)
{
	new_message();	
}

// вызывается после отправки запроса о файле
void message_sent()
{
	// проверяем, все ли успешно. если да, то просим ядро webby принимать данные, пока не встретится 
	// последовательность символов \r\n. как только встретится, webby сообщит нам об этом, вызвав 
	// функцию read_message

	if(sock)
		sock.read_until("\r\n", read_message);
	else 
		cout << "Socket error" << endl;
}

// сюда попали, если среди полученных данных нашлась последовательность \r\n или произошла ошибка
void read_message()
{
	// если ошибка, то выходим из функции
	if(!sock)
	{
		cout << "Connection closed" << endl;
		return;
	}

	// читаем всю строку до \r\n
	string text;
	sock.get_line(text, "\r\n");

	// в строке может быть записано: OK размер, или ERROR.
	// читаем первое слово (то есть до пробела)
	stringstream s(text);
	string status;
	s >> status;

	// Если OK, то читаем размер и начинаем прием файла
	if(status != "ERROR")
	{
		cout << text << endl;
		new_message();
	}
	else
	{
		// сервер мог отправить ERROR и через пробел написать сообщение, читаем его и выводим на экран
		string error_text;
		getline(s, error_text);
		cout << "Server returned error: " << error_text << endl;
		new_message();
	}
}

// вызывается при приеме данных файла
/*
void on_data()
{
	// если ошибка
	if(!sock)
	{
		cout << "Connection closed" << endl;
		return;
	}

	// записываем в файл весь массив байт из памяти сокета
	file.write(sock.get_data(), sock.data_size());
	received_bytes += sock.data_size();
	cout << sock.data_size() << " " << received_bytes << endl;

	// очищаем память сокета
	sock.clear_data();

	// если приняли весь файл, сообщаем об этом, иначе просим webby получать данные дальше
	if(received_bytes == file_size)
		cout << "File downloaded" << endl;
	else
		sock.read_some(on_data);
}
*/

void new_message()
{
	if(sock) sock.close();

	// соединяемся с хостом
	sock.connect(my_ip(), "2000");

	// вводим сообщение
	getline(cin, message);
	if(message == "exit from chat" || message == "Exit from chat")
		exit(0);

	// посылаем сообщение на сервер. после посылки запроса будет вызвана функция message_sent
	sock.send(message + "\r\n", message_sent);
	
	// запускаем ядро webby
	webby::run();
}



































// Получаем IP-адрес клиента
string my_ip()
{
	string s;
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
	{
        if (!ifa->ifa_addr) continue;
        if (ifa->ifa_addr->sa_family == AF_INET)
		{
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			s = addressBuffer;
        }
    }
	return s;
}

