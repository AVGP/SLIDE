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
/**
* Structure of a single Config-Value for SlideConfig (written to /etc/Slide.conf)
* @brief Structure to hold a key-value pair for SlideConfig
*/
typedef struct
{
    char identifier[50];        /**< The string-identifier, the value will be accessable later on*/
    unsigned int valueSize;     /**< The Size (in bytes) the value itself will need*/
    char *value;                /**< Pointer to the value.*/
} CONFIGVALUE;
#pragma pack(1)

/**
* @class SlideConfig
* @brief Class for reading&writing config values
* This class contains the configuration values and provides functionality to read and write these.
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
    * @param identifier The string identifier of the desired config value
    * @param returnSize The variable at this address will receive the size of the returned value (in bytes)
    * @return The matching value is returned - if no value with the given identifier is found, NULL is returned
    *         and the returnSize will be 0
    */
    void *getConfigValue(char *identifier,unsigned int *returnSize);

    /**
    * Sets the config key with the given identifier to the value given via newValue.
    * @param identifier The identifier-string, the value will be stored for
    * @param newValue A pointer to the desired new value.
    * @param valueSize Size of the new value.
    * @todo Stub yet.
    */
    void setConfigValue(char *identifier,void *newValue,unsigned int valueSize);

    /**
    * Writes the current config values to the config file.
    * @todo Stub yet.
    */
    void writeConfigToFile();
  private:
    std::vector<CONFIGVALUE> configValues;
};

#endif // SlideCONFIG_H_INCLUDED
