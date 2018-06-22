#ifndef _CONFIG_H_
#define _CONFIG_H_


#include "LIni.h"
#include "LSingleton.h"

class Config :public LSingleton<Config>
{
public:
	virtual	bool	Init();

	virtual	bool	Final();

	Lint	GetId();

	Lint	GetLogLevel();

	Lstring	GetLogicManagerIp();
	Lshort	GetLogicManagerPort();
	Lstring GetLogicKey();

	Lstring	GetOutsideIp();
	Lshort	GetOutsidePort();
	Lstring	GetOutsideIp2();//���������ip
	Lshort	GetOutsidePort2(); //����������˿�

private:
	LIniConfig	m_ini;


private:
	Lshort		m_inPort;
	Lshort		m_outPort;

	Lstring		m_inIp;
	Lstring		m_outIp;
};


#define gConfig Config::Instance()

#endif