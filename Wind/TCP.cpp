#include "TCP.h"

#include <Events/EventsManager.h>
#include <iostream>

TCP::TCP() {
	WSADATA w;

	if (WSAStartup(0x0202, &w)) {
		std::cout << "Winsock initialisation failed\n";
		WSACleanup();
		return;
	}

	std::cout << "Winsock Initialised\n";
}

void TCP::Initialize(const USHORT & _port) {
	ip = "127.0.0.1";
	port = _port; 

	targetSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(targetSocket, (LPSOCKADDR)& addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cout << "Bind of socket failed\n";
		WSACleanup();
		return;
	}

	std::cout << "Bound to " << port << '\n';

	if (listen(targetSocket, 1) == SOCKET_ERROR) {
		std::cout << "Listening failed\n";
		WSACleanup();
		return;
	}

	std::cout << "Listening on port " << port << '\n';

	FD_ZERO(&master);
	FD_SET(targetSocket, &master);
}

void TCP::ServerStep() {
	fd_set copy = master;

	int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

	for (int i = 0; i < socketCount; ++i) {
		SOCKET sock = copy.fd_array[i];

		// new connection
		if (sock == targetSocket) {
			SOCKET client = accept(targetSocket, nullptr, nullptr);
			FD_SET(client, &master);
		} else {
			// New message
			char buffer[4096];
			ZeroMemory(buffer, sizeof(buffer));

			const int length = recv(sock, buffer, sizeof(buffer), 0);
			if (length <= 0) {
				// disconnected
				closesocket(sock);
				FD_CLR(sock, &master);
			} else {
				// message
				const std::string message(buffer, length);
				std::cout << message << '\n';

				SOCKET& ts = targetSocket;
				Broadcast(message, [&ts, &sock](SOCKET outSock) {
					return outSock != ts && outSock != sock;
				});
			}
		}
	}
}

void TCP::Broadcast(const std::string& message) {
	for (unsigned i = 0; i < master.fd_count; ++i) {
		SOCKET outSock = master.fd_array[i];
		send(outSock, message.c_str(), static_cast<unsigned>(message.length()), 0);
	}
}

void TCP::Broadcast(const std::string & message, std::function<bool(SOCKET)> include) {
	for (unsigned i = 0; i < master.fd_count; ++i) {
		SOCKET outSock = master.fd_array[i];
		if (include(outSock)) {
			send(outSock, message.c_str(), static_cast<unsigned>(message.length()), 0);
		}
	}
}

bool TCP::Connect(const std::string & _ip, const USHORT & _port, std::function<void(TCP*)> completed) {
	ip = _ip;
	port = _port;

	targetSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	InetPton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);

	if (connect(targetSocket, (sockaddr*)& addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cout << "Failed to connect to " << ip << "::" << port << '\n';
		WSACleanup();
		return false;
	}

	std::cout << "Connected to " << ip << "::" << port << '\n';
	
	if (completed)
		completed(this);

	return true;
}

void TCP::ClientStep() {
	if (clientHandler)
		clientHandler(Receive(), this);
}

void TCP::Send(const std::string& message) {
	send(targetSocket, message.c_str(), static_cast<unsigned>(message.length()), 0);
}

const std::string & TCP::GetIP() const {
	return ip;
}

const USHORT & TCP::GetPort() const {
	return port;
}

bool TCP::Receive(SOCKET & sock, std::string & message) {
	char buffer[4096];

	int length = 0;
	int last = 0;

	do {
		ZeroMemory(buffer, sizeof(buffer));
		length = recv(sock, buffer, sizeof(buffer), 0);
		last = static_cast<int>(buffer[length]);

		if (length <= 0)
			return false;

		message += std::string(buffer, length);
	} while (last != 0);

	return true;
}

std::string TCP::Receive() {
	std::string message;
	Receive(targetSocket, message);
	return message;
}
