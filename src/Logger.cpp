#include "Logger.h"
#include "MutexGuard.h"
#include <fstream>
#include <iostream>

Crux::Logger logger;

namespace Crux
{

Logger::Logger()
: m_OnlyToCout( true ), m_pMutex( new Mutex )
{
}

Logger::Logger( const std::string& fileName )
: m_OnlyToCout( false ), m_pMutex( new Mutex )
{
	m_OutStream = new std::ofstream( fileName.c_str(), std::ios_base::app );
}

Logger::~Logger()
{
	delete m_pMutex;

	if ( !m_OnlyToCout )
	{
		delete m_OutStream;
	}	
}

void Logger::Debug( const std::string& msg )
{
	MutexGuard guard( m_pMutex );
	std::cout << "<Debug> " << msg << std::endl;
	if ( !m_OnlyToCout )
	{
		*m_OutStream << "<Debug> " << msg << std::endl;
	}	
}

void Logger::Info( const std::string& msg )
{
	MutexGuard guard( m_pMutex );
	std::cout << "<Info> " << msg << std::endl;
	if ( !m_OnlyToCout )
	{
		*m_OutStream << "<Info> " << msg << std::endl;
	}
}

void Logger::Warn( const std::string& msg )
{
	MutexGuard guard( m_pMutex );
	std::cout << "<Warn> " << msg << std::endl;
	if ( !m_OnlyToCout )
	{
		*m_OutStream << "<Warn> " << msg << std::endl;
	}
}

void Logger::Error( const std::string& msg )
{
	MutexGuard guard( m_pMutex );
	std::cout << "<Error> " << msg << std::endl;
	if ( !m_OnlyToCout )
	{
		*m_OutStream << "<Error> " << msg << std::endl;
	}
}

void Logger::Fatal( const std::string& msg )
{
	MutexGuard guard( m_pMutex );
	std::cout << "<Fatal> " << msg << std::endl;
	if ( !m_OnlyToCout )
	{
		*m_OutStream << "<Fatal> " << msg << std::endl;
	}
}

}
