#include "TCP.h"

#include "Codable.h"

#include <Events/EventsManager.h>
#include <iostream>
#include <fstream>

std::string TCP::localIP = "";

TCP::TCP() {
	WSADATA w;

	if (WSAStartup(0x0202, &w)) {
		std::cout << "Winsock initialisation failed\n";
		WSACleanup();
		return;
	}

	std::cout << "Winsock Initialised\n";
}

std::string TCP::GetLocalIP() {
	if (localIP == "") {
		std::ifstream ifs;
		std::string search = "IPv4 Address. . . . . . . . . . . :";

		system("ipconfig > ip.txt");
		ifs.open("ip.txt");

		if (ifs.is_open()) {
			while (!ifs.eof()) {
				std::string line;
				std::getline(ifs, line);

				if ((line.find(search, 0)) != std::string::npos) {
					line.erase(0, 39);
					localIP = line;
					ifs.close();
				}
			}
		}
	}

	return localIP;
}

void TCP::Initialize(const USHORT & _port) {
	ip = GetLocalIP();
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

			std::cout << "Connected\n";
			//send(client, welcome.c_str(), static_cast<int>(welcome.length()), 0);
		} else {
			std::string message;

			if (Receive(sock, message)) {
				SOCKET& ts = targetSocket;
				Broadcast(message, [&ts](SOCKET outSock) {
					return outSock != ts;
				});
			} else {
				// disconnected
				closesocket(sock);
				FD_CLR(sock, &master);
			}
		}
	}
}

void TCP::Broadcast(const std::string& message) {
	for (unsigned i = 0; i < master.fd_count; ++i) {
		SOCKET outSock = master.fd_array[i];
		Send(message, outSock);
	}
}

void TCP::Broadcast(const std::string & message, std::function<bool(SOCKET)> include) {
	for (unsigned i = 0; i < master.fd_count; ++i) {
		SOCKET outSock = master.fd_array[i];
		if (include(outSock)) {
			Send(message, outSock);
		}
	}
}

void TCP::Send(const std::string & message, const SOCKET & socket) {
	const std::string encoded = Codable::Encode(message);
	send(socket, encoded.c_str(), static_cast<unsigned>(encoded.size()), 0);
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
	const std::string encoded = Codable::Encode(message);
	send(targetSocket, encoded.c_str(), static_cast<unsigned>(encoded.size()), 0);
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

	int maxSize = 0;
	int size = 0;

	ZeroMemory(buffer, sizeof(buffer));
	length = recv(sock, buffer, sizeof(buffer), 0);
	last = static_cast<int>(buffer[length]);

	if (length <= 0)
		return false;

	message += std::string(buffer, length);
	const auto result = Codable::Decode(message, 0, maxSize);
	message = result.second;

	size += message.size();

	while (size < maxSize) {
		ZeroMemory(buffer, sizeof(buffer));
		length = recv(sock, buffer, sizeof(buffer), 0);
		last = static_cast<int>(buffer[length]);

		if (length <= 0)
			return false;

		message += std::string(buffer, length);
		size += length;
	}

	return true;
}

std::string TCP::Receive() {
	std::string message;
 	auto result = Receive(targetSocket, message);
	return message;
}
