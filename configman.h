#pragma once
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include "rpauto.h"


class TargetBuildException : public std::exception
{
public:
    TargetBuildException(int errCode, const std::string& message) noexcept : errCode(errCode), m_errMsg(message)
    {
        
    }
    TargetBuildException(const std::string& message) noexcept : errCode(-1), m_errMsg(message)
    {
        
    }
    virtual ~TargetBuildException() = default;
    virtual const char* what() const noexcept override
    {
        return this->m_errMsg.c_str();
    }
    int GetCode()
    {
        return errCode;
    }
    int errCode;
private:
    std::string m_errMsg;
};


class RPAutoManager
{
public:
    RPAutoManager(std::string workFolder) {m_workFolder = workFolder;};
    ~RPAutoManager() {};
    bool GetSetting(std::string line, std::vector<std::string>& pushBackVector);
    bool GetSetting(std::string line, std::vector<Key<std::string>>& pushBackVector);
    bool GetSetting(std::string line, std::string& settingString);
    bool ValidateTargets();
    void ParseConfig();
    void Build();
private:
    std::string m_workFolder;
    std::vector<std::string> linesBackup = std::vector<std::string>();
    std::vector<std::string> oses = std::vector<std::string>();
    std::vector<std::string> targetConfig = std::vector<std::string>();
    std::vector<std::string> targets = std::vector<std::string>();
    std::vector<Key<std::string>> autobuildSettings = std::vector<Key<std::string>>();
    std::vector<Key<std::vector<std::string>>> dependTree = std::vector<Key<std::vector<std::string>>>();
    std::vector<std::string> compiler = std::vector<std::string>();
    std::vector<Key<std::vector<Key<std::string>>>*> targetSettings = std::vector<Key<std::vector<Key<std::string>>>*>();
    std::string arch = "";
    
    std::string m_GetDefaultCompiler(size_t osIndex);
    bool m_validateSpecifcTarget(std::string target);
    bool m_keyExists(std::string name);
    bool m_TargetKeyExists(std::string name);
    size_t m_GetkeyIndex(std::string name);
    Key<std::vector<Key<std::string>>>* m_GetTargetKey(std::string name);
    Key<std::string> m_GetTargetKeySetting(std::string name, std::string setting);
    Key<std::vector<std::string>> m_GetDependecyKey(std::string name);
    bool m_TargetSettingExists(std::string name, std::string setting);
    void m_ParseTarget(std::string target, size_t targetIndex);
    void m_BuildAndResolveDeps(size_t index);
    void m_buildTarget(size_t index,std::string target);
    
    bool m_TargetDepExists(size_t index, std::string dep);
    
    std::string m_GetOsEx(std::string os, bool isLib)
    {
        if(os == "Windows" || os == "windows")
        {
            if(isLib)
            {
                return ".dll";
            }
            return ".exe";
        }
        else if(os == "Linux" || os == "linux")
        {
            if(isLib)
            {
                return ".so";
            }
            return ".out";
        }
        else
        {
            std::cout << "Could not parse OS: '" << os << "'" << std::endl;;
            exit(1);
        }
    }
    
};
