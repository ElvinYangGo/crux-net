#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>
#include <map>

namespace Crux
{

class ConfigReader
{
public:
	ConfigReader( const std::string& fileName );

public:
	void Init();

	bool ReadString( const std::string& key, std::string& value );
	bool ReadInteger( const std::string& key, int& value );

private:
	void ParseLine( const std::string& line );

private:
	typedef std::map<std::string, std::string> CONFIG_MAP;
	CONFIG_MAP m_Configs;
	std::string m_FileName;
};

}	//namespace Crux

#endif	//CONFIG_READER_H

