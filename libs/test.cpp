#include <string>
#include <cassert>
#include <lua.hpp>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

//socket �м�������ʱ, �Ƿ���������sockfd��������������
int test5()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in svr;
	socklen_t svrlen;
	svr.sin_family = AF_INET;
	svr.sin_port = htons(10099);
	svr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	svrlen = sizeof(svr);
	bind(sockfd, (struct sockaddr*)&svr, svrlen);
	if(0 == fork())
	{
		for(;;){
		listen(sockfd, 5);
		cout << "server is listend!" << endl;
		accept(sockfd, NULL, NULL);
		cout << "a connection come!" << endl;
		break;
		}
		wait();
	}else
	{
		//���û�����, �����connect��ʧ��. ����Ҫ���µ�sockfd����, �����о�������!!!
		int sock = socket(AF_INET, SOCK_STREAM, 0);
		sleep(3);
		struct sockaddr_in connsvr;
		connsvr.sin_family = AF_INET;
		connsvr.sin_port = htons(10099);
		inet_aton("127.0.0.1", &connsvr.sin_addr);
		socklen_t len = sizeof(sockaddr);
		int ret = connect(sock, (struct sockaddr*)&connsvr, len);
		cout << "connect succed, ret is:" << ret <<endl; 

		int sock2 = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in connsvr2;
		connsvr2.sin_family = AF_INET;
		connsvr2.sin_port = htons(10098);
		inet_aton("127.0.0.1", &connsvr2.sin_addr);
		len = sizeof(sockaddr);
		ret = connect(sock2, (struct sockaddr*)&connsvr2, len);
		cout << "connect2 succed, ret is:" << ret <<endl; 

	}
	
	


}

//memset�ĵڶ�������дintʱ��ȡΪ1
int test4()
{
    int arr[1024];
    int rem = 1;
    memset(arr, 1, 1024 * sizeof(int));
    memset(arr, rem, 1024 * sizeof(int));
    cout << arr[0] <<endl;
    


}
//���ַ�ָ����λʱ, sizeof�ķ���ֵ����ʲô
int test3()
{
    char *cur;
    char buff[1024] = "123456789";
    cur = buff;
    cout << "sizeof cur is>>" << sizeof(cur) <<endl;
    cout << "strlen cur is>>" << strlen(cur) << endl;
    
    cur = cur + 3;
    cout << "after moved 3 sizeof cur is>>" << sizeof(*cur) << endl;
    cout << "after moved 3 strlen cur is>>" << strlen(cur) << endl;

}

//��ȡ�����ֽ��������
int test2()
{
    char buff[1024] = {0};
    int net = htonl(33);
    memcpy(buff, (char*)(&net), 4);
    cout << "net is:" << net <<endl;
    cout << "buff is:" <<buff <<endl;
    cout << "int buff:" << *((int*)(char*)buff) << endl;
    cout << "changed buff:" << ntohl(*((int*)(char*)buff)) << endl;

}
int test()
{
    for(;;){
            if(1 > 2)
            {return -1;}
        break;
        cout << "after break" << endl;
    }

}
int main() {
    test5();
    lua_State *L = luaL_newstate();
    //int r = luaL_dofile(l, "bootstrap.lua");
    //   int r = luaL_loadfile(l, "bootstrap.lua");

    //  printf("result is %d\n", r);
    // perror(strerror(errno));
    // r = lua_pcall(l, 0, LUA_MULTRET, 0);
    // printf("result is %d\n", r);
    // perror(strerror(errno));

    
    luaL_openlibs(L);

    int errorCode = 0;
    //if(errorCode != luaL_dofile(L,"bootstrap.lua"))
   // {
     //   puts(lua_tostring(L, -1));
      //  abort();
    //}


    // Clean up the lua context
    lua_close(L);
}
