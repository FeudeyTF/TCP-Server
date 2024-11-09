#include <iostream>
#include "TCPServer.h"

void OnRecieve(string message)
{
	cout << "Recieved msg: " << message << endl;
}

void OnErrored(string error)
{
	cerr << "TCP SERVER ERROR: " << error << endl;
}

void OnClientDisconnected(TCPClient client)
{
	cout << "CLIENT DISCONNECTED" << endl;
}

void OnClientConnected(TCPClient client)
{
	cout << "CLIENT CONNECTED" << endl;
	client.Send((char*)"\0", 1);
}

int main()
{
	PCSTR port = "7791";
	TCPServer server{ port, OnRecieve, OnClientConnected , OnClientDisconnected, OnErrored };
	cout << "TCP Server successfully started at port " << port << endl;
	int status = server.Start();
	return 0;
}