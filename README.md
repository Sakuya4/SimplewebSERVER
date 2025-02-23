# SimplewebSERVER
使用C語言實現TCP Server

一個使用 Winsock 函式庫實現的簡單 TCP 伺服器實例。此伺服器旨在監聽來自客戶端的連接，並能夠接收從客戶端發送的數據，處理後回傳對應的響應。

### 功能

  1. 在 80 埠上監聽進來的 TCP 連接。
  2. 接收來自客戶端的數據並回應簡單的訊息。
  3. 在循環中處理多個客戶端連接。

### 環境

  1. Windows 環境（因為它使用 Winsock 函式庫）。  
  2. 支援 Winsock 函式庫的 C 編譯器（例如 MinGW、Visual Studio）。

### Step
1. clone repository
```
git clone https://github.com/yourusername/simple-tcp-server.git
cd simple-tcp-server
```
2. 這樣運行
   `gcc -o webSERVER webSERVER.c -lws2_32`

3. `./tcp_server`
   
4. 使用 telnet 或 netcat 等工具連接到伺服器。以 telnet 為例，運行以下命令：
telnet localhost 80

輸入一條訊息並按 Enter 鍵，伺服器將回應 "Hello, world!"。
...

### Code說明

#### 1. 主函數 (main)
主函數負責初始化 Winsock 函式庫，創建伺服器端的套接字，並設定伺服器的 IP 地址和端口。接著，伺服器開始監聽來自客戶端的連接請求。
```
WSADATA wsdata;
int versioncheck = WSAStartup(MAKEWORD(2,2), &wsdata);
```
WSAStartup() 函式初始化 Winsock 函式庫，並設置使用的版本號（這裡為 2.2）。

#### 2. 創建套接字
使用 socket() 函式創建一個 TCP 套接字，用於處理進來的連接。

`SOCKET SRV = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);`

AF_INET 指定使用 IPv4 協定。
SOCK_STREAM 指定這是一個流式的套接字，用於 TCP 連接。
IPPROTO_TCP 指定使用 TCP 協定。


#### 3. 綁定套接字

綁定套接字到指定的 IP 地址和端口（此範例中為 80 端口）。
```
bind(SRV, (struct sockaddr*)&SRVaddr, sizeof(SRVaddr));
listen(SRV, CONNECTMAX);
```

bind() 函式將伺服器的套接字綁定到指定的地址和端口。
listen() 函式開啟監聽，並設置最大連接佇列數量（CONNECTMAX，這裡設為 5）。
#### 4. 接受連接

伺服器進入循環，不斷等待並接受來自客戶端的連接。

```
SOCKET client = accept(SRV, (struct sockaddr*)&claddr, &cllen);
handleClient(client);
```
accept() 函式等待並接受來自客戶端的連接。
一旦成功建立連接，伺服器會調用 handleClient() 函式來處理該連接。

#### 5. 處理客戶端

handleClient() 函式從客戶端接收數據並回應 "Hello, world!" 訊息。
```
int recvResult = recv(client, recvdata, BUFFERSIZE - 1, 0);
const char* response = "Hello, world!";
send(client, response, strlen(response), 0);
```
recv() 函式接收來自客戶端的數據。
接收到數據後，伺服器會發送一個固定的回應（"Hello, world!"）。


