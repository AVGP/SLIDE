#include "Slideconfig.h"

SlideConfig::SlideConfig()
{
    Logger::getInstance()->log((std::string)"Starting config.");
    configValues.clear();
    std::ifstream conf((char *)"/etc/Slide.conf");
    if(conf.is_open())
    {
        Logger::getInstance()->log((std::string)"Reading...");
        CONFIGVALUE val;
        int line = 1;
        while(!conf.eof())
        {
            conf.getline(val.identifier,50);
            
            if(strncmp(val.identifier,"#",1) == 0) continue;
            char t[5];
            conf.getline(t,5);
            val.valueSize = atoi(t);
            val.value = (char *)malloc(val.valueSize+1);
            conf.getline(val.value,val.valueSize+1);
            configValues.push_back(val);
            line++;
        }
    }
    else exit(EXIT_FAILURE);
}

void *SlideConfig::getConfigValue(char *identifier,unsigned int *returnSize)
{
    for(unsigned int i=0;i<configValues.size();i++)
    {
        if(strncmp(configValues[i].identifier,identifier,strlen(identifier)) == 0)
        {
            *returnSize = configValues[i].valueSize;
            return (void *)configValues[i].value;
        }
    }
    return NULL;
}

void SlideConfig::setConfigValue(char *indentifier,void *newValue,unsigned int valueSize)
{}

void SlideConfig::writeConfigToFile()
{}
