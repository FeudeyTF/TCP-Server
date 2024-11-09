#include "TCPServer.h"
#include "TCPClient.h"

void TCPServer::_startListening()
{
	while (active)
	{
		TCPClient client = Accept();
		_onConnect.Invoke(client);
		int status = 1;
		do
		{
			char buffer[BUFFER_LENGTH]{};
			status = client.Recieve(buffer, BUFFER_LENGTH);
			string result{};
			for (int i = 0; i < BUFFER_LENGTH; i++)
				result += buffer[i];
			if (status > 0)
				_onRecieve.Invoke(result);
			else
				_onDisconnect.Invoke(client);
		} while (status > 0);
	}
};

TCPServer::TCPServer(PCSTR port, Action<string> onRecieve = NULL, Action<TCPClient> onConnect = NULL, Action<TCPClient> onDisconnect = NULL, Action<string> onErrored = NULL)
{
	_port = port;
	active = false;
	_onRecieve = onRecieve;
	_onConnect = onConnect;
	_onDisconnect = onDisconnect;
	_onErrored = onErrored;
};

int TCPServer::Start()
{
	int status_code{};
	WSADATA data;
	struct addrinfo* result = NULL;
	struct addrinfo hints;

	status_code = WSAStartup(MAKEWORD(2, 2), &data);
	if (status_code != 0)
		_onErrored.Invoke("TCP Server startup failed (WSAStartup), Code: " + status_code);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	status_code = getaddrinfo(NULL, _port, &hints, &result);
	if (status_code != 0)
	{
		_onErrored.Invoke("TCP Server startup failed (getaddrinfo), Code: " + status_code);
		Close();
		return status_code;
	}

	_listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_listener == INVALID_SOCKET)
	{
		_onErrored.Invoke("TCP Server startup failed (socket), Code: " + WSAGetLastError());
		freeaddrinfo(result);
		Close();
		return status_code;
	}

	active = true;

	status_code = bind(_listener, result->ai_addr, (int)result->ai_addrlen);
	if (status_code == SOCKET_ERROR)
	{
		_onErrored.Invoke("TCP Server startup failed (bind), Code: " + WSAGetLastError());
		freeaddrinfo(result);
		Close();
		return status_code;
	}

	freeaddrinfo(result);
	status_code = listen(_listener, SOMAXCONN);
	if (status_code == SOCKET_ERROR)
	{
		_onErrored.Invoke("TCP Server startup failed (listen), Code: " + WSAGetLastError());
		Close();
		return status_code;
	}
	_startListening();
	return 1;
}

TCPClient TCPServer::Accept()
{
	SOCKET client_socket = accept(_listener, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		_onErrored.Invoke("accept failed with error: " + WSAGetLastError());
		Close();
	}
	TCPClient client{ client_socket };
	return client;
}

void TCPServer::Close()
{
	if (active)
		closesocket(_listener);
	WSACleanup();
}