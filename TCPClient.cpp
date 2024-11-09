#include "TCPClient.h"

TCPClient::TCPClient(SOCKET client, int bufferLength, Action<string> onError)
{
	_client = client;
	_bufferLength = bufferLength;
	_onClientErrored = onError;
}

int TCPClient::Send(char* buffer, int bufferLength, int flags)
{
	int status = send(_client, buffer, bufferLength, flags);
	if (status == SOCKET_ERROR)
	{
		_onClientErrored.Invoke("TCP Client send failed (send), Code:" + WSAGetLastError());
		Close();
	}
	return status;
}

int TCPClient::Recieve(char* buffer, int bufferLength)
{
	int status = recv(_client, buffer, bufferLength, 0);
	if (status < 0)
	{
		_onClientErrored.Invoke("TCP Client recieve failed (recv), Code:" + WSAGetLastError());
		Close();
	}
	return status;
}

void TCPClient::Shutdown()
{
	int status_code = shutdown(_client, SD_SEND);
	if (status_code == SOCKET_ERROR)
	{
		_onClientErrored.Invoke("TCP Client shutdown failed (shutdown), Code" + WSAGetLastError());
	}
	Close();
}

void TCPClient::Close()
{
	closesocket(_client);
	WSACleanup();
}
