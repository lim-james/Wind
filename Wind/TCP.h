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

	SOCKET targetSocket;
	fd_set master;

	std::string ip;
	USHORT port;

	std::function<void(std::string, TCP*)> clientHandler;

public:

	TCP();

	// server methods

	void Initialize(const USHORT & _port);
	void ServerStep();
	void Broadcast(const std::string& message);
	void Broadcast(const std::string& message, std::function<bool(SOCKET)> include);

	// client methods

	bool Connect(const std::string& _ip, const USHORT & _port, std::function<void(TCP*)> completed);
	void ClientStep();
	void Send(const std::string& message);

	template<typename Context>
	void BindReceiveHandler(void(Context::*callback)(std::string, TCP*), Context* context);

	// common

	const std::string& GetIP() const;
	const USHORT& GetPort() const;
	
private:

	bool Receive(SOCKET& sock, std::string& message);

	std::string Receive();

};

template<typename Context>
void TCP::BindReceiveHandler(void(Context::*callback)(std::string, TCP*), Context* context) {
	clientHandler = std::bind(callback, context, std::placeholders::_1, std::placeholders::_2);
}

#endif
