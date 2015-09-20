#include "Address.h"

#include <iostream>

Address::Address()
{
}

Address::Address(const unsigned char a, const unsigned char b, const unsigned char c, const unsigned char d, const unsigned short port) :
	m_a(a),
	m_b(b),
	m_c(c),
	m_d(d),
	m_address((m_a << 24) | (m_b << 16) | (m_c << 8) | m_d),
	m_port(port),
	m_addressAndPort(getAddressAndPort())
{
}

Address::Address(const unsigned int address, const unsigned short port) :
	m_a(address >> 24),
	m_b((address << 8) >> 24),
	m_c((address << 16) >> 24),
	m_d((address << 24) >> 24),
	m_address(address),
	m_port(port),
	m_addressAndPort(getAddressAndPort())
{
}

Address::~Address()
{
}

const char Address::getA() const
{
	return m_a;
}

const char Address::getB() const
{
	return m_b;
}

const char Address::getC() const
{
	return m_c;
}

const char Address::getD() const
{
	return m_d;
}

const unsigned int Address::getAddress() const
{
	return m_address;
}

const unsigned short Address::getPort() const
{
	return m_port;
}

const std::string Address::toString() const
{
	return m_addressAndPort;
}

const std::string Address::getAddressAndPort() const
{
	// Not happy with this...
	const std::string p(1, '.');
	const std::string a = std::to_string(m_a);
	const std::string b = std::to_string(m_b);
	const std::string c = std::to_string(m_c);
	const std::string d = std::to_string(m_d);

	return a + p + b + p + c + p + d + ":" + std::to_string(getPort());
}

const bool Address::operator == (const Address& r) const
{
	if (m_address != r.getAddress() || m_port != r.getPort())
		return false;

	return true;
}

const bool Address::operator != (const Address& r) const
{
	if (m_address != r.getAddress() || m_port != r.getPort())
		return true;

	return false;
}