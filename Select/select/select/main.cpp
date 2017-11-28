#include <winsock2.h> 
#include <stdio.h> 
#pragma comment(lib, "ws2_32.lib") 
int main()
{
	// 加载win socket 
	WSADATA ws;
	int ret;
	ret = WSAStartup(MAKEWORD(2, 2), &ws);
	if (ret != 0)
	{
		printf("WSAStartup() 失败!\n");
		return -1;
	}
	// 创建侦听SOCKET 
	int sListen;
	sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (sListen == INVALID_SOCKET)
	{
		printf("socket() 失败!\n");
		return -1;
	}
	// 填充服务器地址结构 
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(8888);

	// 绑定服务器套接字 
	ret = bind(sListen, (sockaddr*)&servAddr, sizeof(servAddr));
	if (ret == SOCKET_ERROR)
	{
		printf("bind() 失败!\n");
		return -1;
	}

	// 开始侦听 
	ret = listen(sListen, 5);
	if (ret == SOCKET_ERROR)
	{
		printf("listen() 失败!\n");
		return -1;
	}
	printf("服务器启动成功，在端口%d监听…\n", ntohs(servAddr.sin_port));
	//使用select模型 
	// 创建套接字集合 
	fd_set allSockSet; // 总的套接字集合 
	fd_set readSet; // 可读套接字集合 
	fd_set writeSet; // 可写套接字集合

	FD_ZERO(&allSockSet); // 清空套接字集合 
	FD_SET(sListen, &allSockSet); // 将sListen套接字加入套接字集合中 
	char bufRecv[100]; // 接收缓冲区
					   // 进入服务器主循环 
	while (1)
	{
		FD_ZERO(&readSet); // 清空可读套接字 
		FD_ZERO(&writeSet); // 清空可写套接字 
		readSet = allSockSet; // 赋值 
		writeSet = allSockSet; // 赋值 
							   // 调用select函数，timeout设置为NULL 
		ret = select(0, &readSet, 0, NULL, NULL);
		//
		if (ret == SOCKET_ERROR)
		{
			printf("select() 失败!\n");
			return -1;
		}
		// 存在套接字的I/O已经准备好 
		if (ret > 0)
		{
			// 遍历所有套接字 
			for (int i = 0; i < allSockSet.fd_count; ++i)
			{
				int s = allSockSet.fd_array[i];
				// 存在可读的套接字 
				if (FD_ISSET(s, &readSet))
				{
					// 可读套接字为sListen 
					if (s == sListen)
					{
						// 接收新的连接 
						sockaddr_in clientAddr;
						int len = sizeof(clientAddr);
						int sClient = accept(s, (sockaddr*)&clientAddr, &len);
						// 将新创建的套接字加入到集合中 
						FD_SET(sClient, &allSockSet);
						printf(">>>>>有新的连接到来啦…\n");
						printf("目前客户端数目为：%d\n", allSockSet.fd_count - 1);
					}
					else // 接收客户端信息 
					{
						ret = recv(s, bufRecv, 100, 0);
						// 接收错误 
						if (ret == SOCKET_ERROR)
						{
							DWORD err = WSAGetLastError();
							if (err == WSAECONNRESET)
								printf("客户端被强行关闭\n");
							else
								printf("recv() 失败!");
							// 删除套接字 
							FD_CLR(s, &allSockSet);
							printf("目前客户端数目为：%d\n", allSockSet.fd_count - 1);
							break;
						}
						if (ret == 0)
						{
							printf("客户端已经退出!\n");
							// 删除套接字 
							FD_CLR(s, &allSockSet);
							printf("目前客户端数目为：%d\n", allSockSet.fd_count - 1);
							break;
						}
						bufRecv[ret] = '\0';
						printf("收到的消息：%s\n", bufRecv);
					} // end else

				}// end if

			}// end for 
		} // end if 
	}//end while 
	return 0;
}