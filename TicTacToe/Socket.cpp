#include "Socket.h"

Socket::Socket() :
	m_handle(-1),
	m_isOpen(false)
{
}

Socket::~Socket()
{
}

const bool Socket::open(const unsigned short port)
{
	// Create socket
	m_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (m_handle <= 0)
	{
		std::cout << "Failed to create socket on port: " << port << std::endl;
		return false;
	}

	// Bind socket to the specified port
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons((unsigned short)port);
	
	if (bind(m_handle, (const sockaddr*)&address, sizeof(address)) < 0)
	{
		std::cout << "Failed to bind socket on port: " << port << std::endl;
		return false;
	}

	// Set socket to non-blocking mode
#if PLATFORM == PLATFORM_WINDOWS
	DWORD nonBlocking = -1;

	if (ioctlsocket(m_handle, FIONBIO, &nonBlocking) != 0)
	{
		std::cout << "Failed to set socket to non-blocking on socket: " << m_handle << std::endl;
		return false;
	}
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	int nonBlocking = 1;

	if (fcntl(m_handle, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
	{
		std::cout << "Failed to set socket to non-blocking on socket: " << m_handle << std::endl;
		return false;
 	}
#endif

	m_isOpen = true;

	return true;
}

void Socket::close()
{
#if PLATFORM == PLATFORM_WINDOWS
	closesocket(m_handle);
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	::close(m_handle);
#endif

	m_isOpen = false;
}

const bool Socket::isOpen() const
{
	return m_isOpen;
}

const bool Socket::send(const Address& dest, const void* data, const int size) const
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(dest.getAddress());
	addr.sin_port = htons(dest.getPort());
	
	return sendto(m_handle, (const char*)data, size, 0, (const sockaddr*)&addr, sizeof(addr)) == size;
}

const int Socket::recieve(Address& sender, void* data, const int size) const
{
#if PLATFORM == PLATFORM_WINDOWS
	typedef int socklen_t;
#endif

	sockaddr_in from;
	socklen_t fromLength = sizeof(from);
	int bytes = recvfrom(m_handle, (char*)data, size, 0, (sockaddr*)&from, &fromLength);
	sender = Address(ntohl(from.sin_addr.s_addr), ntohs(from.sin_port));

	return bytes;
}
