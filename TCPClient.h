#include "global.h";

#pragma once
class TCPClient
{
private:
	SOCKET _client{};
	int _bufferLength{};
	Action<string> _onClientErrored{NULL};

public:
	TCPClient(SOCKET client, int bufferLength = BUFFER_LENGTH, Action<string> onError = NULL);
	int Send(char* buffer, int bufferLength, int flags = 0);
	int Recieve(char* buffer, int bufferLength);
	void Shutdown();
	void Close();
};