#include "global.h"
#include "TCPClient.h"

#pragma once
class TCPServer
{
private:
	SOCKET _listener{};
	PCSTR _port;
	bool active;
	Action<string> _onRecieve{ NULL };
	Action<TCPClient> _onConnect{ NULL };
	Action<TCPClient> _onDisconnect{ NULL };
	Action<string> _onErrored{NULL};
	void _startListening();

public:
	TCPServer(PCSTR port, Action<string> onRecieve, Action<TCPClient> onConnect, Action<TCPClient> onDisconnect, Action<string> onErrored);
	int Start();
	TCPClient Accept();
	void Close();
};

