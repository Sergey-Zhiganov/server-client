#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
using namespace std;

int main()
{
	WSADATA wsaData;
	ADDRINFO* addresult;
	ADDRINFO hints;
	SOCKET ConnectSocket = INVALID_SOCKET;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	setlocale(LC_ALL, "ru");
	const char* sendBuffer = "Hello from Server";
	char recvBuffer[512];

	int result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0) {
		cout << "Ошибка WSAStartup" << endl;
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	result = getaddrinfo(NULL, "666", &hints, &addresult);
	if (result != 0) {
		cout << "Ошибка getaddrinfo" << endl;
		WSACleanup();
		return 1;
	}
	ListenSocket = socket(addresult->ai_family, addresult->ai_socktype, addresult->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		cout << "Ошибка socket" << endl;
		WSACleanup();
		return 1;
	}
	result = bind(ListenSocket, addresult->ai_addr, (int) addresult->ai_addrlen);
	if (result == SOCKET_ERROR) {
		cout << "Ошибка bind" << endl;
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		freeaddrinfo(addresult);
		WSACleanup();
		return 1;
	}
	result = listen(ListenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR) {
		cout << "Ошибка listen" << endl;
		closesocket(ListenSocket);
		freeaddrinfo(addresult);
		WSACleanup();
		return 1;
	}
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		cout << "Ошибка accept" << endl;
		closesocket(ListenSocket);
		freeaddrinfo(addresult);
		WSACleanup();
		return 1;
	}
	do {
		ZeroMemory(recvBuffer, 512);
		result = recv(ClientSocket, recvBuffer, 512, 0);
		if (result > 0) {
			cout << "Получено " << result << " байт" << endl;
			cout << "Получено: " << recvBuffer << endl;
			result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), 0);
			if (result == SOCKET_ERROR) {
				cout << "Ошибка send" << endl;
				closesocket(ListenSocket);
				freeaddrinfo(addresult);
				WSACleanup();
				return 1;
			}
			else if (result == 0) {
				cout << "Закрываю соединение..." << endl;
			}
			/*else {
				cout << "Ошибка при передачи данных" << endl;
				closesocket(ListenSocket);
				freeaddrinfo(addresult);
				WSACleanup();
				return 1;
			}*/
		}
	} while (result > 0);
	result = shutdown(ClientSocket, SD_SEND);
	if (result == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		freeaddrinfo(addresult);
		WSACleanup();
		return 1;
	}
	closesocket(ConnectSocket);
	freeaddrinfo(addresult);
	WSACleanup();
	return 0;
}
