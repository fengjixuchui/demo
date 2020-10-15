#ifndef SOCKS5TRAN_H_
#define SOCKS5TRAN_H_

#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define SOCKS_VERSION 0x05
#define CONNECT         0x01
#define IPV4            0x01
#define IPV6            0x04
#define DOMAIN          0x03
#define BUFF_SIZE (1024*16)
#define VERSION         0x05
#define AUTH_CODE       0x02
#define TIME_OUT        6000000

#define USER_NAME       "111111"
#define PASS_WORD       "222222"

// socks�������Դ�������Э����Ϣͷ��Ӧ�� 05 00
typedef struct _METHOD_SELECT_RESPONSE
{
	char version;           // ������֧�ֵ�socks�汾��0x04��socks4������0x05��socks5��
	char select_method;     // ������ѡ��ķ�����0x00Ϊ������0x02Ϊ������֤
} METHOD_SELECT_RESPONSE;

// socks�ͻ��˷����Ĵ�������Э����Ϣͷ 05 01 00
typedef struct _METHOD_SELECT_REQUEST
{
	char version;           // �ͻ���֧�ֵİ汾��0x04����0x05
	char number_methods;    // �ͻ���֧�ֵķ���������
	char methods[255];      // �ͻ���֧�ֵķ������ͣ����255����0x00Ϊ������0x02Ϊ������֤
} METHOD_SELECT_REQUEST;

// �û�������֤�������Ӧ 01 00
typedef struct _AUTH_RESPONSE
{
	char version;           // �汾���˴��㶨Ϊ0x01
	char result;            // �������֤�����0x00Ϊ�ɹ���������Ϊʧ��
} AUTH_RESPONSE;

//�û�������֤�ͻ�������
typedef struct _AUTH_REQUEST
{
	char version;           // �汾���˴��㶨Ϊ0x01
	char name_len;          // �������ֶ��û����ĳ��ȣ�һ���ֽڣ��Ϊ0xff
	char name[255];         // �û���
	char pwd_len;           // ���ĸ��ֶ�����ĳ��ȣ�һ���ֽڣ��Ϊ0xff
	char pwd[255];          // ����
} AUTH_REQUEST;

// ������ʵ������Socks�����������Ӧ
typedef struct _SOCKS5_RESPONSE
{
	char version;           // ������֧�ֵ�Socks�汾��0x04����0x05
	char reply;             // ���������������ʵ�����Ľ����0x00�ɹ�
	char reserved;          // ����λ���㶨Ϊ0x00
	char address_type;      // Socks����������󶨵ĵ�ַ���ͣ�IPv4Ϊ0x01��IPv6Ϊ0x04������Ϊ0x03
	char address_port[1];   // ���address_typeΪ�������˴���һ�ֽ�Ϊ�������ȣ����Ϊ����������0�ַ���β��������ΪSocks����������󶨶˿�
} SOCKS5_RESPONSE;

// �ͻ�������������ʵ����
typedef struct _SOCKS5_REQUEST
{
	char version;           // �ͻ���֧�ֵ�Socks�汾��0x04����0x05
	char cmd;               // �ͻ������CONNECTΪ0x01��BINDΪ0x02��UDPΪ0x03��һ��Ϊ0x01
	char reserved;          // ����λ���㶨Ϊ0x00
	char address_type;      // �ͻ����������ʵ�����ĵ�ַ���ͣ�IPv4Ϊ0x00��IPv6Ϊ0x04��DOMAINΪ0x03
	char address_port[1];   // ���address_typeΪ�������˴���һ�ֽ�Ϊ�������ȣ����Ϊ����������0�ַ���β��������Ϊ��ʵ�����󶨶˿�
} SOCKS5_REQUEST;

struct Relay_T
{
	int fd;
	int alreadyRead;
	char buf[BUFF_SIZE];
};

class CSocket
{
public:
	CSocket();
	~CSocket();

	void Socket(int &_outFd) const;
	void Bind(int fd, const sockaddr *addr, int addrlen) const;
	void Listen(int fd, const int backlog) const;
	void Setsockopt(int fd, int optname, const void* optval, socklen_t optlen) const;
	void Connect(int fd, const sockaddr *addr, socklen_t optlen) const;
	void SetNonblocking(int fd) const;
	

	//void OnAcceptHttpEpoll();
	//void OnAcceptSockEpoll();
	//void OnTimer();
	
	void EpollEtRelay(int fd1, int fd2);
	//read rt.fd data to rt.buf,set fd out
	void OnReadEtRelay(Relay_T &rt, int fd, int epfd);
	//write rt.buf to fd, set fd in
	void OnWriteEtRelay(Relay_T &rt, int fd, int epfd);

private:

};

class CSocks5 : public CSocket
{
public:
	CSocks5();
	~CSocks5();

	void init(int port);

	static void* Accept(void *arg);
	static void* SocksRelay(void *arg);
private:

};

class CTran : public CSocket
{
public:
	CTran();
	~CTran();

	void setTranPort(int tranPort);
	int getTranPort() const;
	void setTranIp(const char* pTranIp);
	const char* getTranIp() const;
	void setSocksPort(int socksPort);
	int getSocksPort() const;

	void init(char **argv);

	static void* slaveRelay(void *arg);
private:
	//����һ��tranͨ�ŵ�port
	int _tranPort;
	int _socksPort;
	const char* _pTranIp;
};

struct Socks5_T
{
	CSocks5 *ps5;
	int fd;
};

struct Tran_T
{
	CTran *pt;
	int fd1;
	int fd2;
};

int SelectMethod(int sockfd);
int AuthPassword(int sockfd);
int ParseCommand(int sockfd);

#endif
