#include <iostream>
#include <webby.hpp>
#include <fstream>

using namespace std;

webby::Socket client;

//vector<string> messages;

void on_connect();
void send_out();

void reset();
//void send_data();

// получаем сообщение
int main()
{
	// ожидаем подключение н апорт 2000, которое будет связано с sock
	webby::accept(client, on_connect, 2000);

	// запускаем ядро webby
	webby::run();
}

void reset()
{
	client.close();
	webby::accept(client, on_connect, 2000);
}

// пустая функция
void empty(){}

// рассылаем сообщение всем клиентам
void send_out()
{
	if(!client)
	{
		cout << "Client disconnected" << endl;
		reset();
		return;
	}
	cout << "SENDING OUT:  ";

	string message;
	client.get_line(message, "\r\n");
	cout << message << endl;

	if(message != "")
	{
		cout << "FINISHED" << endl;
		client.send(message+"\r\n", reset);
	}
	else
	{
		cout << "An error has occurred " << endl;
		client.send("ERROR error\r\n", reset);
	}

}

// при успешном подключении читаем сообщение и вызываем функцию send_out()
void on_connect()
{
	if(!client) 
	{
		cout << "Couldn't accept connection" << endl;
		return;
	}

	client.read_until("\r\n", send_out);
}

/*
 * будет нужна для обмена файлами между пользователями
void send_data()
{
	const int mtu = 512;
	char buf[mtu];

	const int n = min(mtu, file_size - bytes_sent);

	if(n) 
	{
		file.read(buf, n);
		client.send(buf, n, send_data);
		bytes_sent += n;
	}
	else
	{
		cout << "Data transfer finished" << endl;
		reset();
	}
}
*/
