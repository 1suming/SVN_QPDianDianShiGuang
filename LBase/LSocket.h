#ifndef _L_SOCKET_H_
#define _L_SOCKET_H_

#include "LBuff.h"

class LNet;

enum SOCKET_CONNECT_STATUS
{
	SOCKET_CONNECT_STATUS_DISCONNECTING = 0,	//���ڶϿ�
	SOCKET_CONNECT_STATUS_DISCONNECTED,		//�Ѿ��Ͽ�
	SOCKET_CONNECT_STATUS_CONNECTING,		//��������
	SOCKET_CONNECT_STATUS_CONNECTED			//���ӳɹ�
};

typedef struct
{
	unsigned short		bodySize;
	unsigned short		checksum;
	int					timestamp;
	int					msgId;
}MsgHeader;

/*
*	LSocket������ָ��,������LSocket��صĶ�����Ҫ���������ָ��
*/
class LSocket;
typedef boost::shared_ptr<LSocket> LSocketPtr;

typedef boost::function<void(LBuffPtr recv, LSocketPtr s)>				MsgPackReceiver;
typedef boost::function<bool(LSocketPtr s, const MsgHeader& header)>	MsgHeaderChecker;

/*
*	brief ��ԭʼ�׽��ֵķ�װ��ʵ�ֻ�����
*	���ӣ��շ���Ϣ�Ĺ���
*/
class LSocket:public boost::enable_shared_from_this<LSocket>
{
public:
	/*
	*	brief ���캯��
	*	param ls LNet����
	*/
	LSocket(LNet& ls, MsgPackReceiver receiver, MsgHeaderChecker checker);

	/*
	*	brief ���캯��
	*	param ls LNet����
	*/
	LSocket(LNet& ls);

	/*
	*	brief ��������
	*/
	virtual~LSocket();

	/*
	*	brief ���ӵ�ָ��ip�Ͷ˿ڵķ�������������ʽ
	*	param endpoint ָ����ip�˿�
	*/
	virtual void Connect(const boost::asio::ip::tcp::endpoint& endpoint);

	/*
	*	brief ���ӵ�ָ��ip�Ͷ˿ڵķ�������������ʽ
	*	param ip ָ����ip
	*	param port ָ����port
	*/
	virtual void Connect(const Lstring& ip,Lshort port);

	virtual void Connect(const Lstring& ip,Lshort port, bool& success);

	/* 
	*	brief ���ӵ�ָ��ip�Ͷ˿ڵķ�������������
	*	param endpoint ָ����ip�˿�
	*/
	virtual void AsyncConnect(const boost::asio::ip::tcp::endpoint& endpoint);
	virtual void AsyncConnect(const Lstring& ip,Lshort port);

	/* 
	*	brief �첽���ӻص�����
	*	param e ���Ӵ�����
	*/
	virtual bool HandleAsyncConnect(const boost::system::error_code& e);

	/*	
	*	brief ���ӳɹ�����ʼ�շ���Ϣ
	*/
	virtual void Start();

	/*	
	*	brief ����ֹͣ������
	*/
	virtual void Stop();

	/* 
	*  brief �յ�һ����������Ϣ��
	*/
	virtual void OnRecvOneMsgPack(LBuffPtr buff);

	/* 
	*	brief ������Ϣ���첽���õȴ�
	*/
	void Send(LBuffPtr pBuff); 

	/* 
	*	brief ��ȡԭʼsocket
	*/
	boost::asio::ip::tcp::socket& Socket();

	/* 
	*	brief ��ȡԶ��������ip��ַ
	*/
	Lstring GetRemoteIp();
	
	/* 
	*	brief ��ȡԶ�������Ķ˿�
	*/
	int GetRemotePort();

	/* 
	*	brief ��ȡ���Socket�Ƿ񼤻�
	*/
	bool GetActive();

	/* 
	*	brief �ߵ����socket
	*/
	void Kick();

	bool IsStart(){return m_start;}

	SOCKET_CONNECT_STATUS GetSocketConnectStatus(){return m_connectStatus;}

	int getSendPoolSize();
private:
	/*	
	*	brief �첽���ص�
	*	param e ������
	*	param bytes_transferred ��ȡ����Ϣ����
	*/
	void HandleRead(const boost::system::error_code& e,std::size_t readSize);

	/* 
	*	brief �첽д�ص�
	*	param e ������
	*/
	void HandleWrite(const boost::system::error_code& e, std::size_t writeSize);

	bool Check();
	bool OnRecv();

	bool onRecvHttp();
	bool onRecvBinary();
protected:
	void _completeReadData(Lint iCompleteLen);
	void _closeSocket();
private:
	LNet& m_ns;
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::strand m_strand;
	
	SOCKET_CONNECT_STATUS m_connectStatus;

	bool m_start;
	bool m_sending;
	int m_remoteIp;
	int m_remotePort;
	bool m_active;//�������������һֱ������Ϣ�������ӣ����������ӳ��ֵĸ��ʺܵͣ��Ȳ�����
	Lstring m_ip;
private:
	//for socket resource
	boost::mutex m_mutexSocketForRead;
	boost::mutex m_mutexSocketForWrite;

	//for write
	std::list<LBuffPtr> m_sendPool;
	LBuffPtr m_curretSendBuffer;
	int m_iCurrentSendSize;

	//for read
	char* m_pRecvBuff;
	LBuffPtr m_currentReadBuffer;

	MsgPackReceiver		m_receiver;
	MsgHeaderChecker	m_headerChecker;
};

/*
*	LSocket������ָ��,������LSocket��صĶ�����Ҫ���������ָ��
*/
typedef boost::shared_ptr<LSocket> LSocketPtr;

#endif