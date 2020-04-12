#include "NetworkSystem.h"

#define _WIN32_WINNT 0x0600

#include <WinSock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#include <stdio.h>
#include <stdlib.h>

bool net_init()
{
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d))
	{
		fprintf(stderr, "net_init failed.\n");
		return false;
	}

	DWORD asize = 20000;
	PIP_ADAPTER_ADDRESSES adapters;

	do 
	{
		adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);

		if (!adapters)
		{
			printf("Couldn't allocate %ld bytes for adapters.\n", asize);
			WSACleanup();
			return false;
		}

		int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);
		if (r == ERROR_BUFFER_OVERFLOW)
		{
			printf("GetAdaptersAddresses wants %ld bytes.\n", asize);
			free(adapters);
		} else if (r == ERROR_SUCCESS)
		{
			break;
		}
		else
		{
			printf("Error from GetAdaptersAddresses: %d\n", r);
			WSACleanup();
			return false;
		}
	} while (!adapters);
	
	PIP_ADAPTER_ADDRESSES adapter = adapters;
	while (adapter)
	{
		printf("\nAdapter name: %S\n", adapter->FriendlyName);

		PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;

		while (address)
		{
			printf("\t%s", address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");

			char ap[100];

			getnameinfo(address->Address.lpSockaddr, address->Address.iSockaddrLength, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);

			printf("\t%s\n", ap);

			address = address->Next;
		}

		adapter = adapter->Next;
	}

	free(adapters);
	printf("net_init successful.\n"); 
	return true;
}

void net_quit()
{
	WSACleanup();
}

bool net_init_server_component(NetworkComponent& component)
{
	net_init_component(component);

	if (listen(component.Socket_listen, 2) < 0)
	{
		fprintf(stderr, "listen() failed. (%d)\n", WSAGetLastError());
		return false;
	}

	struct sockaddr_storage client_address;
	socklen_t client_len = sizeof(client_address);

	component.Socket_client = accept(component.Socket_listen, (struct sockaddr*) &client_address, &client_len);

	if (component.Socket_client == INVALID_SOCKET)
	{
		fprintf(stderr, "accept() failed. (%d)\n", WSAGetLastError());
		return false;
	}

	printf("Server setup successful\n");
	return true;
}

bool net_init_client_component(NetworkComponent& component)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	struct addrinfo* peer_address;
	if (getaddrinfo("98.128.192.136", "6969", &hints, &peer_address))
	{
		fprintf(stderr, "getaddrinfo() failed. (%d)", WSAGetLastError());
		return false;
	}

	printf("Remote address is: ");
	char address_buffer[100];
	char service_buffer[100];
	getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer, sizeof(address_buffer),
		service_buffer, sizeof(service_buffer), NI_NUMERICHOST);
	printf("%s %s\n", address_buffer, service_buffer);

	component.Socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
	if (component.Socket_peer == INVALID_SOCKET)
	{
		fprintf(stderr, "socket() failed. (%d)\n", WSAGetLastError());
		return false;
	}

	printf("Connecting...\n");
	if (connect(component.Socket_peer, peer_address->ai_addr, peer_address->ai_addrlen))
	{
		fprintf(stderr, "connect() failed. (%d)\n", WSAGetLastError());
		return false;
	}
	freeaddrinfo(peer_address);

	printf("Connecting successful\n");
	return true;
}

bool net_init_component(NetworkComponent& component)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* bind_address;
	getaddrinfo(0, "6969", &hints, &bind_address);

	component.Socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
	if (component.Socket_listen == INVALID_SOCKET)
	{
		fprintf(stderr, "socket() failed. (%d)\n", WSAGetLastError());
		return false;
	}

	if (bind(component.Socket_listen, bind_address->ai_addr, bind_address->ai_addrlen))
	{
		fprintf(stderr, "bind() failed. (%d)\n", WSAGetLastError());
		return false;
	}
	freeaddrinfo(bind_address);

	return true;
}
