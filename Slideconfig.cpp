#include "Slideconfig.h"
#include <string>

SlideConfig::SlideConfig()
{
    Logger::getInstance()->log((std::string)"Starting config.");
    configValues.clear();
    std::ifstream conf((char *)"/etc/Slide.conf");
    if(conf.is_open())
    {
        Logger::getInstance()->log((std::string)"Reading...");
        CONFIGVALUE val;
        std::string line;
        while(!conf.eof())
        {
            getline(conf,line);
            if(line.size() < 1 || line[0] == '#') continue;
            val.identifier = line;
            getline(conf,val.value);
            configValues.push_back(val);
        }
    }
    else exit(EXIT_FAILURE);
}

void *SlideConfig::getConfigValue(char *identifier,unsigned int *returnSize)
{
    for(unsigned int i=0;i<configValues.size();i++)
    {
        if(strncmp(configValues[i].identifier.c_str(),identifier,strlen(identifier)) == 0)
        {
            *returnSize = configValues[i].value.size();
            return (void *)configValues[i].value.c_str();
        }
    }   
    return NULL;
}

void SlideConfig::setConfigValue(char *indentifier,void *newValue,unsigned int valueSize)
{}

void SlideConfig::writeConfigToFile()
{}
