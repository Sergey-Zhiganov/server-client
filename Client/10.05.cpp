#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>
using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");
	WSADATA wsaData;
	ADDRINFO* addresult;
	ADDRINFO hints;
	SOCKET ConnectSocket = INVALID_SOCKET;
	const char* sendBuffer = "Hello from Client";
	const char* sendBuffer2 = "Bye from Client";

	int result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0) {
		cout << "Ошибка WSAStartup" << endl;
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo("192.168.1.147", "666", &hints, &addresult);
	if (result != 0) {
		cout << "Ошибка getaddrinfo" << endl;
		WSACleanup();
		return 1;
	}
	ConnectSocket = socket(addresult->ai_family, addresult->ai_socktype, addresult->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		cout << "Ошибка socket" << endl;
		WSACleanup();
		return 1;
	}
	result = connect(ConnectSocket, addresult->ai_addr, (int) addresult->ai_addrlen);
	if (result == SOCKET_ERROR) {
		cout << "Ошибка connect" << endl;
		closesocket(ConnectSocket);
		freeaddrinfo(addresult);
		WSACleanup();
		return 1;
	}
	result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), MSG_DONTROUTE);
	if (result == SOCKET_ERROR) {
		cout << "Ошибка send (1/2)" << endl;
		closesocket(ConnectSocket);
		freeaddrinfo(addresult);
		WSACleanup();
		return 1;
	}
	chrono::milliseconds timespan(1000);
	this_thread::sleep_for(timespan);
	result = send(ConnectSocket, sendBuffer2, (int)strlen(sendBuffer), MSG_DONTROUTE);
	if (result == SOCKET_ERROR) {
		cout << "Ошибка send (2/2)" << endl;
		closesocket(ConnectSocket);
		freeaddrinfo(addresult);
		WSACleanup();
		return 1;
	}
	this_thread::sleep_for(timespan);
	cout << "Всё окей" << endl;
	closesocket(ConnectSocket);
	freeaddrinfo(addresult);
	WSACleanup();
}
