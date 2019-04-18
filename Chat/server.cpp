#include <iostream>
#include <webby.hpp>
#include <fstream>
#include <Handle_client.hpp>

using namespace std;

vector<Handle_client> clients(1);

int main()
{
	clients[0].connect();
	webby::run();
}

