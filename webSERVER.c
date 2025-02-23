#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>


#pragma comment(lib,"ws2_32.lib")

#define TRUE 1
#define FALSE 0
#define CONNECTMAX 5 //自己調整 
#define BUFFERSIZE 1024


int check_error_int(int return_data, const char* show_info)
{
    if (return_data != 0)
    {
        perror(show_info);
        getchar();
        return -1;
    }
    return 0;
}

int check_error_socket(SOCKET sock, const char* show_info)
{
    if(sock == INVALID_SOCKET)
    {
        perror(show_info);
        getchar();
        return -1;
    }
    return 0;
}

// 處理客戶端連線的函式 (簡單範例)
void handleClient(SOCKET client) {
    char recvdata[BUFFERSIZE] = "";
    int recvResult = recv(client, recvdata, BUFFERSIZE - 1, 0); // 預留一個位置給 '\0'

    if (recvResult > 0) {
        recvdata[recvResult] = '\0'; // 加上字串結束符號
        printf("Received data: %s, length: %d\n", recvdata, recvResult);

        const char* response = "Hello, world!";
        int sendResult = send(client, response, strlen(response), 0);
        if (sendResult == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
        }
    } else if (recvResult == 0) {
        printf("Connection closing...\n");
    } else {
        printf("recv failed: %d\n", WSAGetLastError());
    }

    closesocket(client); // 關閉 Socket
}





int main()
{
    printf("Welcome use my server\n");
    // 啟動 Winsock
    // int versioncheck = WSAStartup(2.2, &wsdata); <-- ERROR
    WSADATA wsdata;
    int versioncheck = WSAStartup(MAKEWORD(2,2), &wsdata);
    if(check_error_int(versioncheck, "WSAStartup error, socket fail.\n") != 0) return -1;

    // Create Socket
    SOCKET SRV = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//1. IPv4決定Socket地址類型, 2.提供具有串流資料傳輸機制的循序雙向位元組串流, 3. TCP service
    if(check_error_socket(SRV, "create socket error.\n") != 0) return -1;

    // Setting Server(SRV) address
    struct sockaddr_in SRVaddr;
    SRVaddr.sin_family = AF_INET;  //IPv4
    SRVaddr.sin_port = htons(80); //TCP port, htons 可將主機位元組順序中的IP埠號碼轉換為網路位元組順序中的IP埠號碼。(note: 網路大端，PC小端)
    SRVaddr.sin_addr.s_addr = INADDR_ANY;

    // bind Socket
    versioncheck = bind(SRV, (struct sockaddr*)&SRVaddr, sizeof(SRVaddr));
    if(check_error_int(versioncheck, "bind fail.\n") != 0) return -1;
    
    // listen Socket
    versioncheck = listen(SRV, CONNECTMAX); //Max waiting connect. 自己調整
    if(check_error_int(versioncheck, "listen fail.\n") != 0) return -1;

    printf("====Server start, now waiting connections====\n");    

    // people connect SRV.
    struct sockaddr_in claddr; //client address struct
    int cllen = sizeof(claddr); //address struct size
    while(TRUE)
    {
    SOCKET client =  accept(SRV, (struct sockaddr*)&claddr, &cllen);
    if(check_error_socket(client, "accept fail.\n") != 0) return -1;
    
    printf("Client connected.\n");
    handleClient(client);
    
    /* Thread */
    // CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handleClient, (LPVOID)client, 0, NULL);
    }


    /* 廢案 */
    // char recvdata[BUFFERSIZE] = "";
    // recv(client, recvdata, BUFFERSIZE, 0);
    // printf("%s catch data:%d.\n",recvdata, strlen(recvdata)); //test
    // send(client, "Hello, world!",20 ,0);

    closesocket(SRV);
    WSACleanup(); // release source

    getchar();
    return 0;
}