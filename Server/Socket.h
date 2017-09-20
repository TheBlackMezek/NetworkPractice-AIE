#pragma once

#include "Address.h"

class Socket
{
public:
	Socket();
	~Socket();

	bool open(unsigned short port);
	
	void close();

	bool isOpen();

	bool Send(const Address& destionation,
		const void* data,
		int size);

	int receive(Address& sender,
		void* data,
		int size);

private:

	int handle;
};

