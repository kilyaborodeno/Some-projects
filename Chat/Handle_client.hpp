#ifndef HANDLE_CLIENT
#define HANDLE_CLIENT

#include <iostream>
#include <webby.hpp>
#include <fstream>
#include <vector>

using namespace std;

class Handle_client
{
	string name;
	webby::Socket sock;
public:

	Handle_client()
	{
		name = "";
		get_name = true;
		sock.close();
	}

	void connect()
	{
		if(!sock)	
			webby::accept(sock, [this](){on_connect();}, 2000);
		else
			on_connect();
	}
private:
	void on_connect()
	{
		if(!sock) 
		{
			cout << "Couldn't accept connection" << endl;
			connect();
			return;
		}

		sock.read_until("\r\n", [this](){on_request();});
	}

	void on_request()
	{
		if(!sock)
		{
			cout << "\x1b[35m" + name + "\x1b[0m" <<  " disconnected" << endl;
			connect();
			return;
		}

		string message = "";
		sock.get_line(message, "\r\n");
		cout << message << endl;

		if(get_name)
		{
			name = message;
			get_name = false;
			sock.send(string("") + "\r\n", [this](){connect();});
		}
		else
			if(message == "")
				sock.send(string("") + "\r\n", [this](){connect();});
			else
				sock.send("\x1b[35m" + name + ":\x1b[0m " + message + "\r\n", [this](){connect();});
	}

private:
	bool get_name;
};


#endif //HANDLE_CLIENT
