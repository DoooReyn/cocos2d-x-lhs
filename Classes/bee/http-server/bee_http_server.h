#ifndef __go_http_server_h__
#define __go_http_server_h__

#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include "bee/thirds/mongoose.h"

#ifdef _WINDOWS_
#include <Windows.h>
#else
#include <pthread.h>
#endif

using namespace std;
using namespace cocos2d;

class HttpServer : public Ref {
public:
	enum State
	{
		None = 0,
		Running,
		Paused,
		Dead
	};
	static HttpServer* create(std::string dir, std::string ip, std::string port);
	void start();
	void pause();
	void resume();
	void shutdown();
	bool paused() { return m_state == State::Paused;  }
	bool running() { return m_state == State::Running; }
	std::string dir() { return m_dir; }
	std::string ip() { return m_ip; }
	std::string port() { return m_port; }

private:
	virtual ~HttpServer();
	virtual bool init(std::string dir, std::string ip, std::string port);
	HttpServer(std::string dir, std::string ip, std::string port)
		:m_dir(".")
		,m_ip("0.0.0.0")
		,m_port("16888")
		,m_state(State::None)
	{
		if (dir.size() > 0) {
			m_dir = dir;
		}
		if (ip.size() > 0) {
			m_ip = ip;
		}
		if (port.size() > 0) {
			m_port = port;
		}
	}

	void _onRefresh();
	static void _runWithThread(void *userdata);
private:
	std::string m_ip;
	std::string m_port;
	std::string m_dir;
	std::thread* m_thread;
	struct mg_mgr m_mgr;
	State m_state;
};

#endif //__go_http_server_h__