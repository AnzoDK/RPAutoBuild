#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#if defined(WIN32) || defined(WIN64)

typedef unsigned char uint8_t;
#define __HOST win64

#include <RPCommon.h>
#include <windows.h>
#include <string>
#elif defined(linux) || defined(Linux)
#include <RPCommon/RPCommon.h>
#define __HOST linux
#endif

//Errors
class LinkParseException : public std::exception
{
public:
    LinkParseException(int errCode, const std::string& message) noexcept : errCode(errCode), m_message(message)
    {
        std::cout << this->m_message << std::endl;
    }
    virtual ~LinkParseException() = default;
    virtual const char* what() const noexcept override
    {
        return this->m_message.c_str();
    }
    int GetCode()
    {
        return errCode;
    }
private:
    int errCode;
    std::string m_message;
};

//Common functions
static std::string GetOSDefine(std::string os, std::string arch)
{
    //as there is only 3 Oses supported a if-else is okay
    //TODO add support for win64
    if(os == "Linux")
    {
        return "linux";
    }
    else if (os == "Windows")
    {
        if(arch == "x86")
        {
            return "WIN32";
        }
        else if(arch == "x86_64")
        {
            return "WIN64"; //this might be an issue...
        }
        else
        {
            return ""; //TODO
        }
    }
    else if(os == "OSX")
    {
        return ""; //?????
    }
    else
    {
        std::cout << "Could not understand OS: " << "\"" << os << "\"" << std::endl;
        exit(1);
    }
}

static std::string GetRepoNameFromLink(std::string link)
{
    std::string cloneName = link;
    int dot = StringFindLast(".git",cloneName);
    if(dot != std::string::npos)
    {
        cloneName = cloneName.substr(0,dot);
    }
    //Its not the dot, but we are just saving on space by reusing vars
    dot = StringFindLast('/',cloneName);
    if(dot == std::string::npos)
    {
        std::cout << "Could not parse git link" << std::endl;
        throw LinkParseException(1,"Could not parse git link");
    }
    cloneName = cloneName.substr(dot+1,cloneName.length()-dot+1);
    return cloneName;
}
