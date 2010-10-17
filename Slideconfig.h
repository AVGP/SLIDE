/**
* @file Slideconfig.h
* @brief Contains the typedef for CONFIGVALUE and the SlideConfig-Class to read&write the configuration.
*/
#ifndef SlideCONFIG_H_INCLUDED
#define SlideCONFIG_H_INCLUDED

#include "Global.h"
#include <vector>
#include <cstring>

#pragma pack(0)
typedef struct
{
    char identifier[50];
    unsigned int valueSize;
    char *value;
} CONFIGVALUE;
#pragma pack(1)

/**
* @class This class contains the configuration values and provides functionality to read and write these.
*/
class SlideConfig
{
  public:
    /**
    * Creates a new instance of SlideConfig. Internally it reads the configuration file and stores the values
    * in the configValues member for later access through the public methods.
    */
    SlideConfig();

    /**
    * Gets the value of the config key with the given identifier.
    * @param The string identifier of the desired config value
    * @param The variable at this address will receive the size of the returned value (in bytes)
    * @return The matching value is returned - if no value with the given identifier is found, NULL is returned
    *         and the returnSize will be 0
    */
    void *getConfigValue(char *identifier,unsigned int *returnSize);

    /**
    * Sets the config key with the given identifier to the value given via newValue.
    * @param The identifier-string, the value will be stored for
    * @param A pointer to the desired new value.
    * @param Size of the new value.
    */
    void setConfigValue(char *identifier,void *newValue,unsigned int valueSize);

    /**
    * Writes the current config values to the config file.
    */
    void writeConfigToFile();
  private:
    std::vector<CONFIGVALUE> configValues;
};

#endif // SlideCONFIG_H_INCLUDED
