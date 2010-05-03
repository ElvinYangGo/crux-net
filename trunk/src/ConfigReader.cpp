#include "ConfigReader.h"
#include <fstream>
#include <vector>
#include <sstream>
#include "Logger.h"

namespace Crux
{

ConfigReader::ConfigReader(const std::string& fileName )
: m_FileName( fileName )
{
}

void ConfigReader::Init()
{
	std::ifstream inFile( m_FileName.c_str() );
	if ( !inFile )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to open file: " << m_FileName );
		return;
	}

	const int LINE_SIZE = 100;
	char inBuf[LINE_SIZE];
	std::vector<std::string> lineVec;

	while ( inFile.getline( inBuf, LINE_SIZE ) )
	{
		lineVec.push_back( inBuf );
	}

	typedef std::vector<std::string>::iterator Iter;
	Iter it = lineVec.begin();
	Iter end = lineVec.end();
	for ( ; it != end; ++it )
	{
		ParseLine( *it );
	}

	inFile.close();
}

void ConfigReader::ParseLine(const std::string& line )
{
	//ignore comments
	if ( std::string::npos != line.find( '#' ) )
	{
		return;
	}

	size_t pos = line.find( '=' );
	if ( std::string::npos == pos )
	{
		return;
	}

	std::string filters( " \t" );

	//find the key
	size_t key_begin = line.find_first_not_of( filters );
	size_t key_end = line.find_last_not_of( filters.c_str(), pos - 1 );
	std::string key( line, key_begin, key_end - key_begin + 1 );

	//find the value
	size_t value_begin = ++pos;
	size_t value_end = line.find_last_not_of( filters );
	std::string value;

	size_t quotation_begin = line.find( '\"', value_begin );
	if ( std::string::npos != quotation_begin )
	{
		//if it is a string
		size_t quotation_end = value_end;
		if ( quotation_end <= quotation_begin )
		{
			LOG_ERROR( logger, __FUNCTION__ << " only one \" in line: " << line << ",file: " << m_FileName );
			return;
		}
		value.assign( line, quotation_begin + 1, quotation_end - quotation_begin - 1 );
	}
	else
	{
		//if it is an integer
		value_begin = line.find_first_not_of( filters, pos + 1 );
		value.assign( line, value_begin, value_end - value_begin + 1 );
	}

	m_Configs.insert( std::make_pair( key, value ) );
}

bool ConfigReader::ReadString(const std::string &key, std::string &value)
{
	CONFIG_MAP::iterator it = m_Configs.find( key );
	if ( m_Configs.end() == it )
	{
		return false;
	}

	value = it->second;

	return true;
}

bool ConfigReader::ReadInteger(const std::string& key, int& value )
{
	CONFIG_MAP::iterator it = m_Configs.find( key );
	if ( m_Configs.end() == it )
	{
		return false;
	}

	std::stringstream ss_value( it->second );

	if ( !(ss_value >> value) )
	{
		return false;
	}

	return true;
}

}	//namespace Crux

