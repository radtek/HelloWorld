#include "../include/SocketServer.hpp"
#include <event2/thread.h>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

static CLogFile* g_pLogPrint = nullptr; //��־���������ĺ���ʹ��
const std::string g_szLibeventLog = "LibeventError.log";

CSocketServer::CSocketServer(CLogFile& log, const std::string& szIP, int iPort, int iMaxConnect, int iReadTimeOut) : 
CSocketBase(szIP, iPort, iReadTimeOut), m_LogPrint(log), m_iMaxConnect(iMaxConnect), m_iCurrConnect(0), m_iListenFd(-1),
m_pEventBase(nullptr), m_pEventListener(nullptr)
{
}

CSocketServer::~CSocketServer()
{
}

/*********************************************************************
���ܣ�	��ʼ��socket����
������	szError ������Ϣ�����
���أ�	�ɹ�����true
�޸ģ�
*********************************************************************/
bool CSocketServer::Init(std::string& szError)
{
	//���IP��ʽ
	if (!CheckIPFormat(m_szServerIP))
	{
		szError = "Init check server IP format wrong !";
		return false;
	}

	//���˿�
	if (m_iServerPort <= 0 || m_iServerPort >= 65536)
	{
		szError = "Init check server port wrong !";
		return false;
	}

	
#ifdef _WIN32
	if (!m_bInitWSA)
	{
		WSADATA wsa_data;
		if (0 != WSAStartup(MAKEWORD(2, 2), &wsa_data))
		{
			szError = "Init WSAStartup failed !";
			return false;
		}
		else
			m_bInitWSA = true;
	}
#endif

	//����������־���
	if (nullptr == g_pLogPrint)
	{
		g_pLogPrint = &m_LogPrint;
	}

	//���ô�����־��ӡ�ص�����
	event_set_log_callback(log_cb);

	//������������ص�����
	event_set_fatal_callback(fatal_cb);

	//����ʹ���߳�
	int iRet = 0;
#ifdef _WIN32
	iRet = evthread_use_windows_threads();
#else
	iRet = evthread_use_pthreads();
#endif

	if (0 != iRet)
	{
		szError = "Use libevent thread failed !";
		return false;
	}

	//����socket���
	m_iListenFd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_iListenFd <= 0)
	{
		szError = "Create socket fd failed !";
		return false;
	}

	evutil_make_listen_socket_reuseable(m_iListenFd);
	evutil_make_socket_nonblocking(m_iListenFd);

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	if (m_szServerIP.empty())
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		evutil_inet_pton(AF_INET, m_szServerIP.c_str(), &sin.sin_addr);
	sin.sin_port = htons(m_iServerPort);

	//�󶨵�ַ
	if (bind(m_iListenFd, (const struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		szError = "Bind socket address failed !";
		return false;
	}

	//����
	if (listen(m_iListenFd, m_iMaxConnect) < 0)
	{
		szError = "Socket server listen failed !";
		return false;
	}

	//����event_base
	m_pEventBase = event_base_new();
	if (nullptr == m_pEventBase)
	{
		szError = "Create event_base fd failed !";
		return false;
	}
	
	//���������¼�
	m_pEventListener = event_new(m_pEventBase, m_iListenFd, EV_READ | EV_PERSIST, do_accept, this);
	if (nullptr == m_pEventListener)
	{
		szError = "Create listen event failed !";
		return false;
	}

	//��Ӽ����¼�
	if (0 > event_add(m_pEventListener, nullptr))
	{
		szError = "Add event to event_base failed !";
		event_free(m_pEventListener);
		return false;
	}

	return true;
}

/*********************************************************************
���ܣ�	��������
������	szError ������Ϣ
���أ�	����ʧ�ܷ���false���ɹ������ѭ�������᷵��
�޸ģ�
*********************************************************************/
bool CSocketServer::Start(std::string& szError)
{
	if (!m_bStatus)
	{
		m_bStatus = true;
		
		if (nullptr != m_pEventBase)
		{
			m_LogPrint << E_LOG_LEVEL_ALL << "Server socket start on [" << m_szServerIP << ":" 
				<< m_iServerPort << "] ..." << logendl;

			//�����¼�����ѭ��
			if (0 > event_base_dispatch(m_pEventBase))
			{
				m_bStatus = false;
				szError = "Socket server start failed !";
				return false;
			}
		}
		else
		{
			szError = "Socket server not be inited !";
			return false;
		}
	}
	else
	{
		szError = "Socket server is start now.";
		return false;
	}

	return true;
}

/*********************************************************************
���ܣ�	ֹͣ����
������	��
���أ�	��
�޸ģ�
*********************************************************************/
void CSocketServer::Stop()
{
	if (m_bStatus)
	{
		timeval tv;
		tv.tv_sec = 3;
		tv.tv_usec = 0;

		m_bStatus = false;

		//ֹͣ�¼�����ѭ��
		event_base_loopexit(m_pEventBase, &tv);
	}
}

/*********************************************************************
���ܣ�	�ͷ��������Դ
������	��
���أ�	��
�޸ģ�
*********************************************************************/
void CSocketServer::Free()
{
	//�رռ�����socket���
	if (m_iListenFd > 0)
	{
		evutil_closesocket(m_iListenFd);
		m_iListenFd = -1;
	}

	//�رռ����¼�
	if (m_pEventListener != nullptr)
	{
		event_free(m_pEventListener);
		m_pEventListener = nullptr;
	}

	//�ر�event_base
	if (m_pEventBase != nullptr)
	{
		event_base_free(m_pEventBase);
		m_pEventBase = nullptr;
	}

#ifdef _WIN32
	if (m_bInitWSA)
	{
		WSACleanup();
		m_bInitWSA = false;
	}
#endif

	//�ͷ�libevent
	libevent_global_shutdown();
}

/*********************************************************************
���ܣ�	��ȡsocket��Ϣ
������	fd socket���
*		pIP IP��ַ�����������Ҫ��NULL
*		pPort �˿ڣ����������Ҫ��NULL
���أ�	IP��ַ:�˿��ַ���
�޸ģ�
*********************************************************************/
std::string CSocketServer::GetSocketInfo(evutil_socket_t fd, std::string *pIP, int *pPort)
{
	sockaddr_in sin;
	socklen_t len = sizeof(sin);
	char buf[64]{ '\0' };
	std::string szReturn;

	getpeername(fd, (struct sockaddr *)&sin, &len);
	evutil_inet_ntop(AF_INET, &sin.sin_addr, buf, sizeof(buf));
	szReturn = buf;

	if (nullptr != pIP)
		*pIP = szReturn;

	if (nullptr != pPort)
		*pPort = sin.sin_port;

	szReturn.append(std::string(":") + std::to_string(sin.sin_port));

	return std::move(szReturn);
}

/*********************************************************************
���ܣ�	�ͻ������ݴ���
������	szClientStr �ͻ�����Ϣ������
*		bev bufferevent������
*		clientInfo �ͻ�����Ϣ������
*		clientData �ͻ������ݣ����
���أ�	<0��ʧ�ܣ�0���ɹ������ݲ�ȫ��1���ɹ�����
�޸ģ�
*********************************************************************/
short CSocketServer::ClientDataHandle(bufferevent *bev, SClientInfo &clientInfo, SClientData &clientData)
{
	size_t iMaxReadLength = bufferevent_get_max_single_read(bev);

	if (nullptr == clientData.pMsgBuffer)
	{
		clientData.pMsgBuffer = new unsigned char[iMaxReadLength + 1]{ '\0' };
	}

	//��ȡ����
	clientData.uiMsgTotalLength = (unsigned int)bufferevent_read(bev, clientData.pMsgBuffer, iMaxReadLength);

	//���
	if (0 == clientData.uiMsgTotalLength)
	{
		m_LogPrint << E_LOG_LEVEL_PROMPT << "Recv from [" << clientInfo.szClientInfo << "] data_len:0 !"
			<< logendl;
		return 0;
	}
	else
	{
		m_LogPrint << E_LOG_LEVEL_PROMPT << "Recv from [" << clientInfo.szClientInfo << "] data_len:"
			<< clientData.uiMsgTotalLength << " data:\n0x" << GetHexString(clientData.pMsgBuffer, clientData.uiMsgTotalLength)
			<< logendl;

		//������ͬ����
		if (0 == bufferevent_write(bev, clientData.pMsgBuffer, clientData.uiMsgTotalLength))
		{
			m_LogPrint << E_LOG_LEVEL_PROMPT << "Send to [" << clientInfo.szClientInfo << "] same data success !"
				<< logendl;
			return 1;
		}
		else
		{
			m_LogPrint << E_LOG_LEVEL_PROMPT << "Send to [" << clientInfo.szClientInfo << "] same data failed:"
				<< evutil_socket_error_to_string(evutil_socket_geterror(bufferevent_getfd(bev))) << logendl;
			return -1;
		}
	}
}

/*********************************************************************
���ܣ�	���ͻ����Ƿ���������
������	szClientIP �ͻ���IP
���أ�	�������ӷ���true
�޸ģ�
*********************************************************************/
bool CSocketServer::CheckIsPersistentConnection(const std::string &szClientIP)
{
	return false;
}

/*********************************************************************
���ܣ�	���տͻ������Ӵ���
������	listen_fd ������socket���
*		evFlags �¼���־λ
*		arg ����
���أ�	��
�޸ģ�
*********************************************************************/
void CSocketServer::do_accept(evutil_socket_t listen_fd, short evFlags, void* arg)
{
	CSocketServer* classBase = (CSocketServer*)arg;
	struct sockaddr_in sin;
	socklen_t sin_len = sizeof(sin);

	//��������
	evutil_socket_t client_fd = ::accept(listen_fd, (struct sockaddr*)&sin, &sin_len);
	if (client_fd <= 0)
	{
		classBase->m_LogPrint << E_LOG_LEVEL_SERIOUS << "Socket server accept connect failed, return value <= 0 !"
			<< logendl;
	}
	else
	{
		char buf[64]{ '\0' };
		SClientObject* pClient = new SClientObject();

		evutil_inet_ntop(AF_INET, &sin.sin_addr, buf, sizeof(buf));
		pClient->ClientInfo.szClientIP = buf;
		pClient->ClientInfo.szClientInfo = pClient->ClientInfo.szClientIP + std::string(":") + std::to_string(sin.sin_port);
		pClient->pServerObject = classBase;

		classBase->m_LogPrint << E_LOG_LEVEL_PROMPT << "Client [" << pClient->ClientInfo.szClientInfo << "] connected !"
			<< logendl;

		//���ò�����
		evutil_make_socket_nonblocking(client_fd);

		//�����¼�
		struct bufferevent *bev = bufferevent_socket_new(classBase->m_pEventBase, client_fd, 
			BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
		if (nullptr != bev)
		{
			//��д��ʱ
			if (classBase->m_iTimeOut > 0 && !classBase->CheckIsPersistentConnection(pClient->ClientInfo.szClientIP))
			{
				timeval tvRead, tvWrite;

				tvRead.tv_sec = classBase->m_iTimeOut;
				tvRead.tv_usec = 0;

				tvWrite.tv_sec = 3; //Ĭ��
				tvWrite.tv_usec = 0;

				bufferevent_set_timeouts(bev, &tvRead, nullptr);
			}

			//���ûص�
			bufferevent_setcb(bev, read_cb, nullptr, error_cb, (void*)pClient);
			bufferevent_enable(bev, EV_READ | EV_PERSIST);

			classBase->m_LogPrint << E_LOG_LEVEL_SERIOUS << "Socket server current connected client count:" 
				<< ++(classBase->m_iCurrConnect) << logendl;
		}
		else
		{
			classBase->m_LogPrint << E_LOG_LEVEL_SERIOUS << "Client [" << pClient->ClientInfo.szClientInfo
				<< "] create new socket event failed !" << logendl;
			
			delete pClient;
			pClient = nullptr;
		}
	}
}

/*********************************************************************
���ܣ�	���¼�����
������	bev bufferevent
*		arg ����
���أ�	��
�޸ģ�
*********************************************************************/
void CSocketServer::read_cb(struct bufferevent *bev, void* arg)
{
	SClientObject* client = (SClientObject*)arg;
	int iRet = 1;

	//ѭ����ȡ�������������ֹ�����һ��ʱ��ֻ���յ�ǰ��һ��
	do
	{
		//����ͻ�������
		iRet = client->pServerObject->ClientDataHandle(bev, client->ClientInfo, client->ClientData);

		if (0 == iRet) //û�������˳����ȴ���һ�ε����ݵ���
			break;
		else if (1 == iRet) //����ɹ�����������
			client->ClientData.Clear();
		else //���󣬶Ͽ��ͻ��˵����ӣ���ֹ����������
		{
			client->pServerObject->m_LogPrint << E_LOG_LEVEL_WARN << "Recv from [" << client->ClientInfo.szClientInfo
				<< "] msg read and handle failed, server active disconnected !" << logendl;

			client->pServerObject->m_LogPrint << E_LOG_LEVEL_SERIOUS << "Socket server current connected client count:"
				<< --(client->pServerObject->m_iCurrConnect) << logendl;

			client->Clear();
			delete client;
			client = nullptr;
			bufferevent_free(bev);
			
			break;
		}
	} while (1 == iRet);
}

/*********************************************************************
���ܣ�	д�¼�����
������	bev bufferevent
*		arg ����
���أ�	��
�޸ģ�
*********************************************************************/
void CSocketServer::write_cb(struct bufferevent *bev, void* arg)
{
	SClientObject* client = (SClientObject*)arg;

	client->pServerObject->m_LogPrint << E_LOG_LEVEL_PROMPT << "Client [" << client->ClientInfo.szClientInfo
		<< "] msg write completed." << logendl;
}

/*********************************************************************
���ܣ�	�����¼�����
������	bev bufferevent
*		evFlags �¼���־λ
*		arg ����
���أ�	��
�޸ģ�
*********************************************************************/
void CSocketServer::error_cb(struct bufferevent *bev, short evFlags, void* arg)
{
	SClientObject* client = (SClientObject*)arg;

	//������
	if (evFlags & BEV_EVENT_EOF)
	{
		client->pServerObject->m_LogPrint << E_LOG_LEVEL_WARN << "Client [" << client->ClientInfo.szClientInfo
			<< "] normal disconnected." << logendl;
	}
	else if (evFlags & BEV_EVENT_ERROR)
	{
		client->pServerObject->m_LogPrint << E_LOG_LEVEL_WARN << "Client [" << client->ClientInfo.szClientInfo
			<< "] unnormal disconnected, error:" << evutil_socket_error_to_string(evutil_socket_geterror(bufferevent_getfd(bev)))
			<< logendl;
	}
	else if (evFlags & BEV_EVENT_TIMEOUT)
	{
		client->pServerObject->m_LogPrint << E_LOG_LEVEL_WARN << "Client [" << client->ClientInfo.szClientInfo
			<< "] timeout disconnected." << logendl;
	}
	else
	{
		client->pServerObject->m_LogPrint << E_LOG_LEVEL_WARN << "Client [" << client->ClientInfo.szClientInfo
			<< "] abnormal disconnected, evFlags:" << evFlags << logendl;
	}

	//���δ�������Ϣ
	if (client->ClientData.uiMsgTotalLength > 0 && nullptr != client->ClientData.pMsgBuffer)
	{
		client->pServerObject->m_LogPrint << E_LOG_LEVEL_WARN << "Client [" << client->ClientInfo.szClientInfo
			<< "] has untreated data:\n0x" << GetHexString(client->ClientData.pMsgBuffer, client->ClientData.uiMsgTotalLength)
			<< logendl;
	}

	client->pServerObject->m_LogPrint << E_LOG_LEVEL_SERIOUS << "Socket server current connected client count:"
		<< --(client->pServerObject->m_iCurrConnect) << logendl;

	//�ͷ���Դ
	client->Clear();
	delete client;
	client = nullptr;
	bufferevent_free(bev);
}

/*********************************************************************
���ܣ�	������־����
������	severity ��־�ȼ�
*		msg ��־��Ϣ
���أ�	��
�޸ģ�
*********************************************************************/
void CSocketServer::log_cb(int severity, const char* msg)
{
	//��־�������
	if (nullptr != g_pLogPrint)
	{
		(*g_pLogPrint) << E_LOG_LEVEL_SERIOUS << "Libevent inner error, level:" << severity
			<< " msg:" << msg << logendl;
	}
	else
	{
		char buf[1001] = { '\0' };
		snprintf(buf, 1000, "echo \"%s >> [SERIOUS]: Libevent inner error, level:%d msg:%s\" >> %s", 
			DateTimeString().c_str(), severity, msg, g_szLibeventLog.c_str());
		system(buf);
	}
}

/*********************************************************************
���ܣ�	����������Ĭ����exit(1)
������	err �����
���أ�	��
�޸ģ�
*********************************************************************/
void CSocketServer::fatal_cb(int err)
{
	//����������
	if (nullptr != g_pLogPrint)
	{
		(*g_pLogPrint) << E_LOG_LEVEL_SERIOUS << "Libevent fatal error, active call exit(), errno:"
			<< err << logendl;
	}
	else
	{
		char buf[1001] = { '\0' };
		snprintf(buf, 1000, "echo \"%s >> [SERIOUS]: Libevent fatal error, active call exit(), errno:%d\" >> %s",
			DateTimeString().c_str(), err, g_szLibeventLog.c_str());
		system(buf);
	}

	exit(1);
}