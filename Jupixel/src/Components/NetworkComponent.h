#pragma once

#include <WinSock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

struct NetworkComponent
{
	SOCKET Socket_listen;
	SOCKET Socket_peer;
	SOCKET Socket_client;
};
