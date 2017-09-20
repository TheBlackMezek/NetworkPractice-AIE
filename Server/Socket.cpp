#include "Socket.h"


#define PLATFORM_WINDOWS	1
#define PLATFORM_MAC		2
#define PLATFORM_UNIX		3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif


#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h>
#pragma comment (lib, "wsock32.lib") //not sure if I need these both
#elif	PLATFORM == PLATFORM_MAC ||
PLATFORM == PLATFORM_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#endif


#include <iostream>



Socket::Socket()
{
}


Socket::~Socket()
{
}



bool Socket::open(unsigned short port)
{
	int handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (handle <= 0)
	{
		printf("Failed to create socket\n");
		return 0;
	}
	else
	{
		printf("Created socket\n");
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons((unsigned short)port);

	if (bind(handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
	{
		printf("Failed to bind socket\n");
		return 0;
	}
	else
	{
		printf("Successfully bound to socket.\n");
	}


#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

	int nonBlocking = 1;
	if (fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
	{
		printf("Failedee to set non-blocking\n");
		return 0;
	}

#elif PLATFORM == PLATFORM_WINDOWS

	DWORD nonBlocking = 1;
	if (ioctlsocket(handle, FIONBIO, &nonBlocking) != 0)
	{
		printf("Failed to set non-blocking\n");
		return 0;
	}
	else
	{
		printf("Successfully set non-blocking\n");
	}

#endif

	return 1;
}

void Socket::close()
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	close(handle);
#elif PLATFORM == PLATFORM_WINDOWS
	closesocket(handle);
#endif
}

bool Socket::isOpen()
{

}

bool Socket::Send(const Address& destionation,
	const void* data,
	int size)
{
	int address = destionation.GetAddress();

	int sent_bytes =
		sendto(handle,
		(const char*)data,
			size,
			0,
			(sockaddr*)&destionation.address,
			sizeof(sockaddr_in));

	if (sent_bytes != size)
	{
		printf("failed to send packet\n");
		return false;
	}

	return 1;
}

int Socket::receive(Address& sender,
	void* data,
	int size)
{
#if PLATFORM == PLATFORM_WINDOWS
	typedef int socklen_t;
#endif

	int bytes = recvfrom(handle,
		(char*)data,
		size,
		0,
		(sockaddr*)&sender.address,
		(int*)sizeof(int));

	if (bytes <= 0)
	{

	}

	return 1;

}
