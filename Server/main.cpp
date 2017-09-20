
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
	else
	{
		printf("Created socket\n");
	}


	int port = 2469;

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons((unsigned short) port);

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



	std::cin.get();



	while (true)
	{
		unsigned char packetData[256];

		unsigned int max_packet_size = sizeof(packetData);

#if PLATFORM == PLATFORM_WINDOWS
		typedef int socklen_t;
#endif

		sockaddr_in from;
		socklen_t fromLength = sizeof(from);

		int bytes = recvfrom(handle,
			(char*)packetData,
			max_packet_size,
			0,
			(sockaddr*)&from,
			&fromLength);

		/*if (bytes <= 0)
		{
			printf("Received nothing, breaking\n");
			break;
		}*/

		if (bytes > 0)
		{
			printf("%s\n", packetData);
		}
		else if(bytes == SOCKET_ERROR)
		{
			printf("Socket error occurred:\n");
			int error = WSAGetLastError();
			switch (error)
			{
			case WSANOTINITIALISED:
				printf("Wsa was not successfully initialized.\n");
				break;
			default:
				break;
			}
		}

		unsigned int from_address = ntohl(from.sin_addr.s_addr);

		unsigned int from_port = ntohs(from.sin_port);
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
