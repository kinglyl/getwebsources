#include <stdio.h>
#include <string.h>
#include <winsock2.h>
int geturl(char *url)
{
        WSADATA wd;
        int ret = 0;
        SOCKET c;
        char myurl[1024] = "", myhost[1024] = "", myget[1024] = "", myheader[1024] = "", mysource[1024] = "";
        char *pHost;
        strcpy(myurl, url);
        int i = 0;
        for (pHost = myurl; *pHost != '/' && *pHost != '\0'; pHost++)
        {
                myhost[i] = *pHost;
                i++;
        }
        if ((int)(pHost - myurl) == strlen(myurl))
        {
                strcpy(myget, "/");
        }

        else
        {
                strcpy(myget, pHost);
        }
        //printf("%s\n%s\n", myget, myhost);
        sprintf(myheader, "GET %s HTTP/1.1\r\n"
                          "Host: %s\r\n"
                          "Connection: close\r\n"
                          //"Accept: text/plain\r\n"
                          //"Accept-Charset: utf-8\r\n"
                          "\r\n"
                ,myget, myhost);
        printf("%s", myheader);
        SOCKADDR_IN saddr;
        ret = WSAStartup(MAKEWORD(2, 2), &wd); //1.初始化操作
        if (ret != 0)
        {
                return 0;
        }
        if (HIBYTE(wd.wVersion) != 2 || LOBYTE(wd.wVersion) != 2)
        {
                printf("Init Fail");
                WSACleanup();
                return 1;
        }
        //2.创建客户端socket
        c = socket(AF_INET, SOCK_STREAM, 0);
        //3.定义要连接的服务端信息
        //saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        saddr.sin_addr.s_addr = *((unsigned long *)(gethostbyname(myhost)->h_addr));
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(80);
        //4.连接服务端
        connect(c, (SOCKADDR *)&saddr, sizeof(SOCKADDR));
        send(c, myheader, strlen(myheader) + 1, 0);
        while (recv(c, mysource, 1023, 0) > 0)
        {
                printf("%s", mysource);
                strnset(mysource, '\0',1024);
        }

        closesocket(c);
        WSACleanup();
        return 0;
}
int main(void)
{
        char url[256] = "";
        printf("http://");
        scanf("%s", url);
        geturl(url);
        return 0;
}