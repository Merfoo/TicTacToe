#pragma once

#include <string>

class Address
{
public:
	Address();
	Address(const unsigned char a, const unsigned char b, const unsigned char c, const unsigned char d, const unsigned short port);
	Address(const unsigned int address, const unsigned short port);
	~Address();

	const char getA() const;
	const char getB() const;
	const char getC() const;
	const char getD() const;

	const unsigned int getAddress() const;
	const unsigned short getPort() const;

	const std::string toString() const;
private:
	unsigned char m_a;
	unsigned char m_b;
	unsigned char m_c;
	unsigned char m_d;

	unsigned int m_address;
	unsigned short m_port;

	std::string m_addressAndPort;

	const std::string getAddressAndPort() const;
};