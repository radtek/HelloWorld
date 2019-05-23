/******************************************************
���ܣ�	socket�������
���ߣ�	�￡
ʱ�䣺	2019-02-18
�޸ģ�
******************************************************/
#ifndef __SOCKET_SERVER_HPP__
#define __SOCKET_SERVER_HPP__

#include "SocketBase.hpp"
#include "event.h"
#include "LogFile/include/LogFile.hpp"

class CSocketServer : public CSocketBase
{
public:
	CSocketServer(CLogFile& log, const std::string& szIP, int iPort, int iMaxConnect = 100, int iReadTimeOut = 0);
	CSocketServer(const CSocketServer& Other) = delete;
	virtual ~CSocketServer();
	CSocketServer& operator=(const CSocketServer& Other) = delete;

	static std::string GetSocketInfo(evutil_socket_t fd, std::string *pIP = nullptr, int *pPort = nullptr);

protected:
	bool Init(std::string& szError);
	bool Start(std::string& szError);
	void Stop();
	void Free();

	//�ͻ�����Ϣ
	struct SClientInfo
	{
		std::string szClientInfo;         //IP + ":" + Port
		std::string szClientIP;           //IP
		bool        bLogined;             //�Ƿ񼺵�½

		SClientInfo() : bLogined(false) {};
		~SClientInfo() {};
	};

	//�ͻ�����������
	struct SClientData
	{
		bool               bSecurityTunnel;      //�Ƿ�ȫͨ��
		bool               bReadSignLength;      //�Ƿ��ȡǩ������
		unsigned int       uiMsgTotalLength;     //��Ϣ�ܳ���
		unsigned char      *pMsgBuffer;          //��Ϣ���棬���಻���ͷ�
		unsigned int       uiMsgReadedLength;    //��Ϣ����ȡ����
		unsigned char      sessionKeyBuffer[16]; //�Ự��Կ
		unsigned long long ullMsgID;             //��ϢID

		SClientData() : bSecurityTunnel(false), bReadSignLength(false), uiMsgTotalLength(0), pMsgBuffer(nullptr), 
			uiMsgReadedLength(0), ullMsgID(0){
			memset(sessionKeyBuffer, 0, 16);
		};
		SClientData(const SClientData& Other) = delete;
		~SClientData() { Clear(); };
		SClientData& operator=(const SClientData& Other) = delete;
		void Clear() {
			uiMsgTotalLength = 0;
			uiMsgReadedLength = 0;

			if (nullptr != pMsgBuffer) {
				delete[] pMsgBuffer;
				pMsgBuffer = nullptr;
			}
		}
	};

	//�ͻ��˶���
	struct SClientObject
	{
		CSocketServer *pServerObject;  //�������
		SClientInfo   ClientInfo;      //�ͻ�����Ϣ
		SClientData   ClientData;      //�ͻ�������

		SClientObject() : pServerObject(nullptr) {};
		SClientObject(const SClientObject& Other) = delete;
		~SClientObject() { Clear(); };
		SClientObject& operator=(const SClientObject& Other) = delete;
		void Clear() { ClientData.Clear(); };
	};

	//�ͻ������ݴ�������ʵ�֣����أ�<0��ʧ�ܣ�0���ɹ���û��ȡ�����ݣ�1���ɹ�����
	virtual short ClientDataHandle(bufferevent *bev, SClientInfo &clientInfo, SClientData &clientData);

	//���ͻ����Ƿ���������
	virtual bool CheckIsPersistentConnection(const std::string &szClientIP);

private:
	static void do_accept(evutil_socket_t listenFd, short evFlags, void* arg);
	static void read_cb(bufferevent *bev, void* arg);
	static void write_cb(bufferevent *bev, void* arg);
	static void error_cb(bufferevent *bev, short evFlags, void* arg);
	static void log_cb(int severity, const char* msg);
	static void fatal_cb(int err);

protected:
	CLogFile&       m_LogPrint;       //��־�����
	int             m_iMaxConnect;    //���������
	int             m_iCurrConnect;   //��ǰ������

private:
	evutil_socket_t m_iListenFd;      //������socket���
	event_base*     m_pEventBase;     //event_base
	struct event*   m_pEventListener; //�����¼�

};

#endif