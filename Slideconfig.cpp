#include "Slideconfig.h"

SlideConfig::SlideConfig()
{
    configValues.clear();
    std::ifstream conf("Slide.conf");
    if(conf)
    {
        CONFIGVALUE val;
        while(!conf.eof())
        {
            conf >> val.identifier;
            conf >> val.valueSize;
            val.value = malloc(val.valueSize);
            conf >> val.value;
            configValues.push_back(val);
        }
    }
    else exit(EXIT_FAILURE);
}

void *SlideConfig::getConfigValue(char *identifier,unsigned int *returnSize)
{
    for(int i=0;i<configValues.size();i++)
    {
        if(strncmp(configValues[i].identifier,identifier,strlen(identifier)) == 0)
        {
            *returnSize = configValues[i].valueSize;
            return configValues[i].value;
        }
    }
}

void SlideConfig::setConfigValue(char *indentifier,void *newValue,unsigned int valueSize)
{}

void SlideConfig::writeConfigToFile()
{}
