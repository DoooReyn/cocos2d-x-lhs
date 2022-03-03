#include "bee_http_server.h"

#define CS_TAG "===============> HttpServer : "
#define HS_LOG(format, ...) cocos2d::log(CS_TAG format, ##__VA_ARGS__)

static void __serve__(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	if (ev == MG_EV_HTTP_MSG)
	{
		struct mg_mgr *pMgr = (struct mg_mgr *)fn_data;
		HttpServer *server = (HttpServer *)pMgr->userdata;
		if (server != NULL)
		{
			struct mg_http_serve_opts opts;
			std::string dir = server->dir();
			opts.root_dir = dir.c_str();
			mg_http_serve_dir(c, (mg_http_message *)ev_data, &opts);
		}
	}
}

HttpServer *HttpServer::create(std::string dir, std::string ip, std::string port)
{
	HttpServer *ret = new (std::nothrow) HttpServer(dir, ip, port);
	if (ret && ret->init(dir, ip, port))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

HttpServer::~HttpServer()
{
	// Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
	CC_SAFE_DELETE(m_thread);
}

bool HttpServer::init(std::string dir, std::string ip, std::string port)
{
	retain();

	m_thread = new std::thread(&HttpServer::_runWithThread, this);
	m_thread->detach();

	// Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);

	return true;
}

void HttpServer::start()
{
	if (m_state == State::None)
	{
		m_state = State::Running;
	}
}

void HttpServer::pause()
{
	if (m_state == State::Running)
	{
		m_state = State::Paused;
	}
}

void HttpServer::resume()
{
	if (m_state == State::Paused)
	{
		m_state = State::Running;
	}
}

void HttpServer::shutdown()
{
	if (m_state != State::Dead)
	{
		m_state = State::Dead;
	}
}

void HttpServer::_onRefresh()
{
	struct mg_connection *c;

	mg_mgr_init(&m_mgr);
	m_mgr.userdata = (void *)this;

	std::string _url = m_ip + ":" + m_port;
	const char *url = _url.c_str();
	if ((c = mg_http_listen(&m_mgr, url, __serve__, &m_mgr)) == NULL)
	{
		HS_LOG("Cannot listen on [%s %s].", m_dir.c_str(), url);
		return;
	}

	HS_LOG("Starting HttpServer on [%s %s]", m_dir.c_str(), url);
	m_state = State::Running;

	while (m_state != State::Dead)
	{
		if (m_state == State::Running)
			mg_mgr_poll(&m_mgr, 1000);
	}

	if (m_state == State::Dead)
	{
		HS_LOG("Exiting HttpServer on [%s %s]", m_dir.c_str(), url);
		mg_mgr_free(&m_mgr);
		release();
	}
}

void HttpServer::_runWithThread(void *userdata)
{
	if (userdata)
	{
		static_cast<HttpServer *>(userdata)->_onRefresh();
	}
}
