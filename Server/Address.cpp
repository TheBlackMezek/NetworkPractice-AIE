#include "Address.h"



Address::Address()
{
}


Address::~Address()
{
}


Address::Address(unsigned char a,
	unsigned char b,
	unsigned char c,
	unsigned char d,
	unsigned short port)
{
	address = (a << 24) |
		(b << 16) |
		(c << 8) |
		d;

	this->port = port;
}

Address::Address(unsigned int address,
	unsigned short port)
{
	this->address = address;
	this->port = port;
}

unsigned int Address::GetAddress() const
{
	return address;
}

unsigned char Address::GetA() const
{
	return 0;
}
unsigned char Address::GetB() const
{
	return 0;
}
unsigned char Address::GetC() const
{
	return 0;
}
unsigned char Address::GetD() const
{
	return 0;
}

unsigned short Address::GetPort()
{
	return port;
}