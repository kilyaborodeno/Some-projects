#include <iostream>
#include <webby.hpp>
#include <fstream>

using namespace std;

webby::Socket new_sock;;
webby::Socket on_service;
//vector<string> messages;

void on_connect();
void send_out();
//void service_client(webby::Socket client);
bool fr = true;

void reset();
//void send_data();

// получаем сообщение
int main()
{
	// ожидаем подключение на порт 2000, которое будет связано с sock
	webby::accept(on_service, on_connect, 2000);

	// запускаем ядро webby
	webby::run();
}

void reset()
{
	try{
		// ожидаем подключение на порт 2000, которое будет связано с sock
		webby::accept(new_sock, on_connect, 2000);

		// запускаем ядро webby
		webby::run();

	}catch(std::exception& e){};

	on_connect();
	//on_service.close();
//	webby::accept(on_service, on_connect, 2000);
	/*
	for(auto i : clients)
	{
		cout << i << ' ' << endl;
		webby::accept(on_service, on_connect, 2000);
	}
	*/
}

// пустая функция
void empty(){}

// рассылаем сообщение всем клиентам
void send_out()
{
	if(!on_service)
	{
		cout << "Client disconnected" << endl;
		reset();
		return;
	}
	cout << "SENDING OUT:  ";

	string message = "";
	try{
		new_sock.get_line(message, "\r\n");
	}catch(std::exception& e){};

	if(message == "") on_service.get_line(message, "\r\n");
	cout << message << endl;

	if(message != "")
	{
		cout << "FINISHED" << endl;
		if(new_sock) new_sock.send(message+"\r\n", reset);
		if(on_service) on_service.send(message+"\r\n", reset);
	}
	else
	{
		cout << "An error has occurred " << endl;
		on_service.send("ERROR error\r\n", reset);
	}

}

// при успешном подключении читаем сообщение и вызываем функцию send_out()
void on_connect()
{
	//if(fr) on_service;// = clients[0];
	if(!on_service) 
	{
		cout << "Couldn't accept connection" << endl;
		reset();
		return;
	}

	try{
		new_sock.read_until("\r\n", send_out);
	}catch(std::exception& e){};

	on_service.read_until("\r\n", send_out);
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
		on_service.send(buf, n, send_data);
		bytes_sent += n;
	}
	else
	{
		cout << "Data transfer finished" << endl;
		reset();
	}
}
*/
