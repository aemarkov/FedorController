#ifndef __WINDOWSFIX_H__
#define __WINDOWSFIX_H__

#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#include <stdint.h>
#define SOCKET_TYPE SOCKET

#elif __linux__
#include <sys/socket>

#define SOCKET_TYPE int;
#define SOCKET_ERROR            (-1)
#define INVALID_SOCKET			(SOCKET)(~0)
#define DWORD					uint32_t
#endif

#ifdef __linux__
int WSAGetLastError()
{
	return errno;
}
#endif

#endif