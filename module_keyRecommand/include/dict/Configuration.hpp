#if !defined(__CONFIGURATION_H__)
#define __CONFIGURATION_H__

#include <map>
#include <string>


using std::map, std::string;

class Configuration
{
private:
    map<string, string> _configMap;
    static Configuration *_conf;
    string _filePath;

    Configuration(const string &filepath);
    ~Configuration();

public:

    map<string, string> &getConfigMap();
    static Configuration *getInstance();
    static void destroy();
};

Configuration::Configuration(const string &filePath)
{
}

Configuration::~Configuration()
{
}




#endif // __CONFIGURATION_H__
