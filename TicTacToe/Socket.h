#pragma once

#include <iostream>

#include "Address.h"

#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC     2
#define PLATFORM_UNIX    3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
#pragma comment(lib, "wsock32.lib")
#include <WinSock2.h>
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#endif

class Socket
{
public:
	Socket();
	~Socket();
	
	const bool open(const unsigned short port);
	void close();

	const bool isOpen() const;

	const bool send(const Address& dest, const void* data, const int size) const;
	const int recieve(Address& sender, void* data, const int size) const;

	static const bool initializeSockets()
	{
#if PLATFORM == PLATFORM_WINDOWS
		WSAData WsaData;

		return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
#else
		return true;
#endif
	}

	static void shutdownSockets()
	{
#if PLATFORM == PLATFORM_WINDOWS
		WSACleanup();
#endif
	}
private:
	int m_handle;
	bool m_isOpen;
};
