#ifndef TCP_H
#define TCP_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <vector>
#include <string>
#include <functional>

struct TCPData {
	SOCKET socket;
	std::string message;
};

class TCP {

	const USHORT port;

	SOCKET targetSocket;

	fd_set master;
	std::string welcome;

public:

	TCP(const USHORT& port);

	// server methods

	void Initialize(const std::string& _welcome);
	void Run();
	void Broadcast(const std::string& message);
	void Broadcast(const std::string& message, std::function<bool(SOCKET)> include);

	// client methods

	void Connect(const std::string& ip, std::function<void(void)> completed);
	void Send(const std::string& message);
	std::string Receive();

};

#endif
