
#include "SocketLib\SocketLib.h"

#include <stdint.h>

using namespace SocketLib;

int main()
{
	Socket::Init();
	uint8_t send_buffer[100] = { 1,3,3 };
	uint8_t recv_buffer[100];
	
	TcpClient client;
	client.Connect("127.0.0.1", 50789);
	client.Send(send_buffer, 3);
	
	

	/*TcpServer server;
	server.Bind(48484);
	server.Listen();
	Socket s = server.Accept();
	int length = s.Recv(recv_buffer, 100);
	printf("Received:\n length: %d\n data: %s\n", length, recv_buffer);*/

	/*UdpSocket socket;
	socket.Connect("127.0.0.1", 50449);
	socket.Send(send_buffer, 3);*/

	UdpSocket socket;
	socket.Bind(48484);
	int length = socket.Recv(recv_buffer, 100);
	printf("Received:\n length: %d\n data: %s\n", length, recv_buffer);

    return 0;
}

