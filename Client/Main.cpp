
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
#include <string>


bool initializeSockets();
void closeSocket(int socket);
void shutdownSockets();









int main()
{
	initializeSockets();



	int handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (handle <= 0)
	{
		printf("Failed to create socket\n");
		return 0;
	}


	int port = 2469;

	int a = 127;
	int b = 0;
	int c = 0;
	int d = 1;

	unsigned int addr = (a << 24) |
						(b << 16) |
						(c << 8 ) |
						d;

	sockaddr_in address;
	address.sin_family = AF_INET;
	//address.sin_addr.s_addr = INADDR_ANY;
	address.sin_addr.s_addr = htonl(addr);
	address.sin_port = htons((unsigned short)port);

	if (bind(handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
	{
		printf("Failed to bind socket\n");
		return 0;
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

#endif




	for (int i = 0; i < 100000; ++i)
	{
		const std::string packetData = "Hello world!";

		int sent_bytes = sendto(handle,
			packetData.c_str(),
			packetData.size(),
			0,
			(sockaddr*)&address,
			sizeof(sockaddr_in));

		if (sent_bytes != packetData.size())
		{
			printf("Failed to send packet\n");
			return 0;
		}
		else
		{
			printf("Sent packet:%s\n", packetData.c_str());
		}
	}




	std::cin.get();

	closeSocket(handle);

	shutdownSockets();

	return 0;
}













bool initializeSockets()
{
#if PLATFORM == PLATFORM_WINDOWS
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
#else
	return true;
#endif
}

void closeSocket(int socket)
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	close(socket);
#elif PLATFORM == PLATFORM_WINDOWS
	closesocket(socket);
#endif
}

void shutdownSockets()
{
#if PLATFORM == PLATFORM_WINDOWS
	WSACleanup();
#endif
}
