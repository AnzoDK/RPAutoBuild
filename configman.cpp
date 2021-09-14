#include "configman.h"

/*
 This file need some serious optimization XDXDXDXDXDXDXDXD..... T_T
 */

namespace fs = std::filesystem;

void RPAutoManager::m_SetVar(std::string name, std::string val, std::string os)
{
    vars.push_back(OSSetting(name,val,os));
}

std::string RPAutoManager::m_GetVar(std::string name, std::string os)
{
    for(size_t i = 0; i < vars.size(); i++)
    {
        if(vars.at(i).name == name)
        {
            if(vars.at(i).os == "_null_")
            {
                return vars.at(i).value;
            }
            else if(os == vars.at(i).os)
            {
                return vars.at(i).value;
            }
            else
            {
                return "";
            }
        }
    }
    return "_Error_";
}



bool RPAutoManager::GetSetting(std::string line, std::vector<std::string>& pushBackVector)
{
            std::string tmp = line;
            size_t start = tmp.find("{");
            size_t end = tmp.find("}");
            if(start == std::string::npos || end == std::string::npos)
            {
                return false;
            }
            size_t e_start = tmp.find("\"",start);
            size_t e_end = 0;
            while(e_start != std::string::npos && e_end != std::string::npos)
            {
                e_end = tmp.find("\"",e_start+1);
                //Getting rid of the '"' character
                e_start++;
                pushBackVector.push_back(tmp.substr(e_start,e_end-e_start));
                e_start = tmp.find("\"",e_end+1);
            }
            return true;
}

/*bool RPAutoManager::GetSetting(std::string line, std::vector<Key<std::string>>& pushBackVector)
{
            int varC = 0;
            std::string vars[2]; 
            std::string tmp = line;
            size_t start = tmp.find("{");
            size_t end = tmp.find("}");
            if(start == std::string::npos || end == std::string::npos)
            {
                return false;
            }
            size_t e_start = tmp.find("\"",start);
            size_t e_end = 0;
            while(e_start != std::string::npos && e_end != std::string::npos)
            {
                e_end = tmp.find("\"",e_start+1);
                //Getting rid of the '"' character
                e_start++;
                vars[varC] = tmp.substr(e_start,e_end-e_start);
                e_start = tmp.find("\"",e_end+1);
                varC++;
                if(varC >= 2)
                {
                    pushBackVector.push_back(Key<std::string>(vars[0],vars[1]));
                    return true;
                }
            }
            return true;
}*/

bool RPAutoManager::GetSetting(std::string line, std::string& setting)
{
            std::string tmp = line;
            size_t start = tmp.find("{");
            size_t end = tmp.find("}");
            if(start == std::string::npos || end == std::string::npos)
            {
                return false;
            }
            size_t e_start = tmp.find("\"",start);
            size_t e_end = 0;
            size_t cc = 0;
            while(e_start != std::string::npos && e_end != std::string::npos)
            {
                if(cc > 0)
                {
                    std::cout << "WARNING: More than 1 setting is specified, but only 1 (the last) is going to be returned (using a vector instead)" << std::endl;
                }
                e_end = tmp.find("\"",e_start+1);
                //Getting rid of the '"' character
                e_start++;
                setting = tmp.substr(e_start,e_end-e_start);
                e_start = tmp.find("\"",e_end+1);
                cc++;
            }
            return true;
}

bool RPAutoManager::GetSetting(std::string line, std::vector<Key<std::string>>& pushBackVector)
{
            std::string tmp = line;
            std::string keyName = "";
            size_t start = tmp.find("{");
            size_t end = tmp.find("}");
            if(start == std::string::npos || end == std::string::npos)
            {
                return false;
            }
            size_t e_start = tmp.find("\"",start);
            size_t e_end = 0;
            std::string* keyVals = new std::string[2];
            int c = 0;
            while(e_start != std::string::npos && e_end != std::string::npos && c < 2)
            {
                e_end = tmp.find("\"",e_start+1);
                //Getting rid of the '"' character
                e_start++;
                keyVals[c] = (tmp.substr(e_start,e_end-e_start));
                e_start = tmp.find("\"",e_end+1);
                c++;
            }
            pushBackVector.push_back(Key<std::string>(keyVals[0],keyVals[1]));
            delete[] keyVals;
            return true;
}

bool RPAutoManager::m_keyExists(std::string name)
{
    for(size_t i = 0; i < dependTree.size(); i++)
    {
        if(dependTree.at(i).keyName == name)
        {
            return true;
        }
    }
    return false;
}

Key<std::vector<Key<std::string>>>* RPAutoManager::m_GetTargetKey(std::string name)
{
   for(size_t i = 0; i < targetSettings.size(); i++)
    {
        if(targetSettings.at(i)->keyName == name)
        {
            return targetSettings.at(i);
        }
    }
    Key<std::vector<Key<std::string>>>* k = new Key<std::vector<Key<std::string>>>();
    k->keyName = "_Error_";
    return k;
}

Key<std::vector<std::string>> RPAutoManager::m_GetDependecyKey(std::string name)
{
   for(size_t i = 0; i < dependTree.size(); i++)
    {
        if(dependTree.at(i).keyName == name)
        {
            return dependTree.at(i);
        }
    }
    Key<std::vector<std::string>> k = Key<std::vector<std::string>>();
    k.keyName = "_Error_";
    return k;
}

Key<std::string> RPAutoManager::m_GetTargetKeySetting(std::string name, std::string setting)
{
    for(size_t i = 0; i < targetSettings.size(); i++)
    {
        if(targetSettings.at(i)->keyName == name)
        {
            for(size_t u = 0; u < targetSettings.at(i)->keyValue.size(); u++)
            {
                if(targetSettings.at(i)->keyValue.at(u).keyName == setting)
                {
                    return targetSettings.at(i)->keyValue.at(u); //returns a copy
                }
            }
        }
    }
    return Key<std::string>("","");
}

void RPAutoManager::m_UpdateTargetKeySetting(std::string name, std::string setting, std::string value)
{
    for(size_t i = 0; i < targetSettings.size(); i++)
    {
        if(targetSettings.at(i)->keyName == name)
        {
            for(size_t u = 0; u < targetSettings.at(i)->keyValue.size(); u++)
            {
                if(targetSettings.at(i)->keyValue.at(u).keyName == setting)
                {
                    targetSettings.at(i)->keyValue.at(u) = Key<std::string>(targetSettings.at(i)->keyValue.at(u).keyName, value);
                }
            }
        }
    }
}

size_t RPAutoManager::m_GetkeyIndex(std::string name)
{
    for(size_t i = 0; i < dependTree.size(); i++)
    {
        if(dependTree.at(i).keyName == name)
        {
            return i;
        }
    }
    return -1;
}

bool RPAutoManager::m_TargetKeyExists(std::string name)
{
    for(size_t i = 0; i < targetSettings.size(); i++)
    {
        if(targetSettings.at(i)->keyName == name)
        {
            return true;
        }
    }
    return false;
}





bool RPAutoManager::m_validateSpecifcTarget(std::string target)
{
        size_t c = 0;
        while(linesBackup.at(c).find("__"+target) == std::string::npos || linesBackup.at(c).find("__"+target) != 0 )
        {
            c++;
            if ( c >= linesBackup.size() )
            {
                std::cout << "Target: " << target << " Not Found!" << std::endl;
                exit(1);
            }
        }
        if(c == linesBackup.size()-1)
        {
            std::cout << "Could not find target: " << target << std::endl;
            return false;
        }
        std::cout << "Found target: " << target << " at line: " << std::to_string(c) << std::endl;
        std::string tmp = linesBackup.at(c);
        
        size_t looker_start = tmp.find('(');
        size_t looker_end = tmp.find(')');
        if(looker_start != std::string::npos && looker_end != std::string::npos)
        {
            std::string targetType = tmp.substr(looker_start+1,looker_end-looker_start-1);
            if(targetType == "_final_")
            {
                std::cout << "Target type is Final Executeable" << std::endl;
            }
            else if(targetType == "_obj_")
            {
                std::cout << "Target type is Intermediate Object file" << std::endl;
            }
            else
            {
                std::cout << "Could not determine target type. Got type: " << targetType << std::endl;
                return false;
            }
            
        }
        looker_start = tmp.find('|');
        while(looker_start != std::string::npos)
        {
            size_t internal_looker_start = tmp.find("__",looker_start);
            size_t internal_looker_end = std::string::npos;
            if(internal_looker_start == std::string::npos)
            {
                std::cout << "Found Dependency Operator, but no dependency? - " << target << " near '|'" << std::endl;
                return false;
            }
            internal_looker_end = tmp.find("__",internal_looker_start+1);
            if(internal_looker_end == std::string::npos)
            {
                std::cout << "Found Dependency Operator, but no dependency? - " << target << " near '__'" << std::endl;
                return false;
            }
            std::string dep = tmp.substr(internal_looker_start+2,internal_looker_end-internal_looker_start-2);
            //Let's not add dependencies to the tree yet, this function is just meant to check if all targets are available for build
            /*if(!m_keyExists(target))
            {
                dependTree.push_back(Key<std::vector<std::string>>(target,std::vector<std::string>()));
                dependTree.at(dependTree.size()-1).keyValue.push_back(dep);
            }
            */
            std::cout << "Found dependency: " << dep << std::endl;
            looker_start = tmp.find('|',internal_looker_end+1);
            if(!m_validateSpecifcTarget(dep))
            {
                std::cout << "Failed to validate Dependency " << dep << " of target: " << target << std::endl;
                return false;
            }
        }
        return true;
}


bool RPAutoManager::ValidateTargets()
{
    if(oses.size() != targets.size())
    {
        std::cout << "Not enough or too many targets specified for the specified Operating Systems!" << std::endl;
        return false;
    }
    for(size_t u = 0; u < oses.size(); u++)
    {
        
        if(!m_validateSpecifcTarget(targets.at(u)))
        {
            return false;
        }
        
    }
    return true;
}

bool RPAutoManager::m_TargetDepExists(size_t index, std::string dep)
{
    for(size_t i = 0; i < dependTree.at(index).keyValue.size(); i++)
    {
        if(dependTree.at(index).keyValue.at(i) == dep)
        {
            return true;
        }
    }
    return false;
}

void RPAutoManager::ParseConfig()
{
    

    if(!fs::exists(m_workFolder+"/"+m_autoFile))
    {
        std::cout << "No " + m_autoFile + " file found" << std::endl;
        //DEBUG
        std::cout << "Search path: " << m_workFolder+"/"+m_autoFile << std::endl;
        exit(1);
    }
    std::string currLine = "";
    std::vector<std::string> lines = std::vector<std::string>();
    std::ifstream file(m_workFolder+"/"+m_autoFile);
    while (std::getline(file, currLine))
    {
        lines.push_back(currLine);
        linesBackup.push_back(currLine);
    }
    file.close();
    
    size_t count = 0;
    
    while(lines.size() > 0)
    {
        if(lines.at(count).find("#") != std::string::npos)
        {
            lines.erase(lines.begin()+count);
            count = 0;
            continue;
        }
        if(lines.at(count).find("OS=") != std::string::npos)
        {
            /*
            std::string tmp = lines.at(count);
            size_t start = tmp.find("{");
            size_t end = tmp.find("}");
            if(start == std::string::npos || end == std::string::npos)
            {
                std::cout << "Could not parse setting \"OS\"" << std::endl;
                exit(1);
            }
            size_t e_start = tmp.find("\"",start);
            size_t e_end = 0;
            while(e_start != std::string::npos && e_end != std::string::npos)
            {
                e_end = tmp.find("\"",e_start+1);
                //Getting rid of the '"' character
                e_start++;
                oses.push_back(tmp.substr(e_start,e_end-e_start));
                e_start = tmp.find("\"",e_end+1);
            }
            lines.erase(lines.begin()+count);
            count = 0;
            continue;
            */
            if(!GetSetting(lines.at(count),oses))
            {
                std::cout << "Could not parse setting \"OS\"" << std::endl;
                exit(1);
            }
            else
            {
                lines.erase(lines.begin()+count);
                count = 0;
                continue;
            }
        }
        if(lines.at(count).find("OS_TARGET=") != std::string::npos)
        {
            
            if(!GetSetting(lines.at(count),targets))
            {
                std::cout << "Could not parse setting \"OS_TARGET\"" << std::endl;
                exit(1);
            }
            else
            {
                lines.erase(lines.begin()+count);
                count = 0;
                continue;
            }
            
        }
        if(lines.at(count).find("SET_BUILD_VAR=") != std::string::npos)
        {
            
            if(!GetSetting(lines.at(count),autobuildSettings))
            {
                std::cout << "Could not parse setting \"SET_BUILD_VAR\"" << std::endl;
                exit(1);
            }
            else
            {
                lines.erase(lines.begin()+count);
                count = 0;
                continue;
            }
            
        }
        if(lines.at(count).find("OS_COMPILER=") != std::string::npos)
        {
            
            if(!GetSetting(lines.at(count),compiler))
            {
                std::cout << "Could not parse setting \"OS_COMPILER\"" << std::endl;
                exit(1);
            }
            else
            {
                lines.erase(lines.begin()+count);
                count = 0;
                continue;
            }
            
        }
        if(lines.at(count).find("ARCH=") != std::string::npos)
        {
            
            if(!GetSetting(lines.at(count),arch))
            {
                std::cout << "Could not parse setting \"OS_COMPILER\"" << std::endl;
                exit(1);
            }
            else
            {
                lines.erase(lines.begin()+count);
                count = 0;
                continue;
            }
            
        }
        if(lines.at(count).find("SET_VAR=") != std::string::npos)
        {
            std::vector<std::string> sett = std::vector<std::string>();
            if(!GetSetting(lines.at(count),sett))
            {
                std::cout << "Could not parse setting \"OS_COMPILER\"" << std::endl;
                exit(1);
            }
            else
            {
                if(sett.size() != 3 && sett.size() != 2)
                {
                    std::cout << "SET_VAR has too few or too many arguments" << std::endl;
                    exit(1);
                }
                
                if(sett.size() == 3)
                {
                    m_SetVar(sett.at(0),sett.at(1),sett.at(2));
                }
                else
                {
                    m_SetVar(sett.at(0),sett.at(1));
                }
                
                lines.erase(lines.begin()+count);
                count = 0;
                continue;
            }
        }
        //If a line is not cought by one of the statements it ends here and gets deleted, and we assume that it was not meant to be parsed
        if(count <= lines.size())
        {
            lines.erase(lines.begin()+count);
            count = 0;
            continue;
        }
        count++;
    }
    
}
#if (defined(linux) || defined(Linux))
std::string RPAutoManager::m_GetDefaultCompiler(size_t osIndex)
{
    std::string os = oses.at(osIndex);
    if(os == "Windows" || os == "windows")
    {
        if(arch == "x86_64")
        {
            return "x86_64-w64-mingw32-g++";
        }
        else if(arch == "x86")
        {
            return "i686-w64-mingw32-g++";
        }
        else
        {
            return "x86_64-w64-mingw32-g++";
        }
        
    }
    else if (os == "Linux" || os == "linux")
    {
        if(arch == "x86_64")
        {
            return "g++";
        }
        else if(arch == "x86")
        {
            return "g++";
            
        }
        else
        {
            return "g++";
        }
    }
    else
    {
        std::cout << "Couldn't parse OS: " << os << std::endl;
        exit(1);
    }
}
#elif (defined(win32) || defined(win64))
std::string RPAutoManager::m_GetDefaultCompiler(size_t osIndex)
{
    //Determine if a supported VS Installation is present
    std::string vsPath = "";
    std::string vsBase = "C:\\Program Files (x86)\\Microsoft Visual Studio\\";
    std::string vsPost = "\\Community\\VC\\Tools\\MSVC\\14.29.30133\\bin\\Hostx64\\x64\\cl.exe";
    for(int i = 2015; i < 2022; i++)
    {
        DWORD ftyp = GetFileAttributesA(std::string(vsBase + std::to_string(i)).c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES)
        {
            std::cout << "Version: " << std::to_string(i) << " Not found" << std::endl;
            continue;
        }
        

        if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        {
            std::cout << "Found version: " << std::to_string(i) << std::endl;
            vsPath = std::to_string(i);
            break;
        }
    }
    bool MSCVwork = true;
    DWORD ftyp = GetFileAttributesA(std::string(vsBase + std::to_string(i)).c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
    {
        MSCVwork = false;
    }    
    
    int gpp = 0;
    gpp = system("g++ --version");
    
    std::string os = oses.at(osIndex);
    if(os == "Windows" || os == "windows")
    {
        if(arch == "x86_64" || arch == "x86")
        {
            if(vsPath != "" && MSCVwork)
            {
                return vsBase + vsPath + vsPost;
            }
            else
            {
                if(gpp != 0)
                {
                    std::cout << "No windows compiler found - Please install either g++ or Visual Studio 2015/2016/2017/2018/2019/2020 (2019 is tested)" << std::endl;
                    exit(1);
                }
                else
                {
                    return "g++";
                }
            }
            
        }
        else
        {
            std::cout << "Invalid Arch" << std::endl;
            exit(1);
        }
    }
    else if (os == "Linux" || os == "linux")
    {
        if(arch == "x86_64" || arch == "x86")
        {
            if(gpp != 0)
            {
                return "g++";
            }
            else
            {
                std::cout << "No Linux compiler found - Please install g++" << std::endl;
                exit(1);
            }
            
        }
        else
        {
            std::cout << "Invalid Arch" << std::endl;
            exit(1);
        }
    }
    else
    {
        std::cout << "Couldn't parse OS: " << os << std::endl;
        exit(1);
    }
}
#endif

bool RPAutoManager::m_TargetSettingExists(std::string name, std::string setting)
{
    for(size_t i = 0; i < targetSettings.size(); i++)
    {
        if(targetSettings.at(i)->keyName == name)
        {
            for(size_t u = 0; u < targetSettings.at(i)->keyValue.size(); u++)
            {
                if(targetSettings.at(i)->keyValue.at(u).keyName == setting)
                {
                    return true;
                }
            }
        }
    }
    return false;
}


void RPAutoManager::m_ParseTarget(std::string target, size_t targetIndex)
{
    size_t c = 0;
        while(linesBackup.at(c).find("__"+target) == std::string::npos || linesBackup.at(c).find("__"+target) != 0 )
        {
            c++;
            if ( c >= linesBackup.size() )
            {
                std::cout << "Target: " << target << " Not Found!" << std::endl;
                exit(1);
            }
        }
        if(c == linesBackup.size()-1)
        {
            std::cout << "Could not find target: " << target << std::endl;
            exit(1);
        }
        std::cout << "Found target: " << target << " at line: " << std::to_string(c) << std::endl;
        std::string tmp = linesBackup.at(c);
        
        size_t looker_start = tmp.find('(');
        size_t looker_end = tmp.find(')');
        //Get Obj type
        if(looker_start != std::string::npos && looker_end != std::string::npos)
        {
            std::string targetType = tmp.substr(looker_start+1,looker_end-looker_start-1);
            if(targetType == "_final_")
            {
                std::cout << "Target type is Final Executeable" << std::endl;
                if(!m_TargetKeyExists(target))
                {
                    targetSettings.push_back(new Key<std::vector<Key<std::string>>>(target, std::vector<Key<std::string>>())); //init the *cursed* object xD
                    m_GetTargetKey(target)->keyValue = std::vector<Key<std::string>>();
                    m_GetTargetKey(target)->keyValue.push_back(Key<std::string>("target_type","_final_"));
                }
            }
            else if(targetType == "_obj_")
            {
                std::cout << "Target type is Intermediate Object file" << std::endl;
                if(!m_TargetKeyExists(target))
                {
                    targetSettings.push_back(new Key<std::vector<Key<std::string>>>(target,std::vector<Key<std::string>>()));
                    if(!m_TargetSettingExists(target, "flags"))
                    {
                        m_GetTargetKey(target)->keyValue = std::vector<Key<std::string>>();
                        m_GetTargetKey(target)->keyValue.push_back(Key<std::string>("flags","c"));
                        
                    }
                    if(!m_TargetSettingExists(target, "target_type"))
                    {
                        m_GetTargetKey(target)->keyValue.push_back(Key<std::string>("target_type","_obj_"));
                    }
                }
                
            }
            else
            {
                std::cout << "Could not determine target type. Got type: " << targetType << std::endl;
                exit(1);
            }
            size_t c_i = c+1;
            while(linesBackup.at(c_i).find("- ") != std::string::npos && linesBackup.at(c_i).find("- ") == 0 )
            {
                std::string setting = linesBackup.at(c_i).substr(2,linesBackup.at(c_i).find(":")-2);
                std::string val = linesBackup.at(c_i).substr(linesBackup.at(c_i).find(":")+1);
                if(!m_TargetSettingExists(target, setting))
                {
                    m_GetTargetKey(target)->keyValue.push_back(Key<std::string>(setting,val));
                }
                else
                {
                    //Checking if the setting is already applied and adding it if not
                    if(m_GetTargetKeySetting(target, setting).keyValue.find(val) == std::string::npos)
                    {
                        
                        m_UpdateTargetKeySetting(target, setting,m_GetTargetKeySetting(target, setting).keyValue+=std::string(";"+val));
                    }
                }
                c_i++;
                if(c_i >= linesBackup.size())
                {
                    break;
                }
            }
            
        }
        looker_start = tmp.find('|');
        while(looker_start != std::string::npos)
        {
            size_t internal_looker_start = tmp.find("__",looker_start);
            size_t internal_looker_end = std::string::npos;
            if(internal_looker_start == std::string::npos)
            {
                std::cout << "Found Dependency Operator, but no dependency? - " << target << " near '|'" << std::endl;
                exit(1);
            }
            internal_looker_end = tmp.find("__",internal_looker_start+1);
            if(internal_looker_end == std::string::npos)
            {
                std::cout << "Found Dependency Operator, but no dependency? - " << target << " near '__'" << std::endl;
                exit(1);
            }
            std::string dep = tmp.substr(internal_looker_start+2,internal_looker_end-internal_looker_start-2);
            if(!m_keyExists(target))
            {
                dependTree.push_back(Key<std::vector<std::string>>(target,std::vector<std::string>()));
                dependTree.at(m_GetkeyIndex(target)).keyValue.push_back(dep);
            }
            else
            {
                if(!m_TargetDepExists(m_GetkeyIndex(target),dep))
                {
                    dependTree.at(m_GetkeyIndex(target)).keyValue.push_back(dep);
                }

            }
            std::cout << "Found dependency: " << dep << std::endl;
            looker_start = tmp.find('|',internal_looker_end+1);
            m_ParseTarget(dep,targetIndex);
        }
        
}


void RPAutoManager::m_buildTarget(size_t i,std::string target)
{
    Key<std::vector<Key<std::string>>>* targetKey = m_GetTargetKey(target);
    
    //Check for missing deps
    if(m_GetDependecyKey(target).keyName != "_Error_")
    {
        //we missed a dep.... Let's address that
        for(size_t u = 0; u < m_GetDependecyKey(target).keyValue.size(); u++)
        {
            m_buildTarget(i,m_GetDependecyKey(target).keyValue.at(u));
        }
    }
    
    if(targetKey->keyName == "_Error_")
    {
        std::cout << "Cannot find target: '" << target << "'" << std::endl;
        exit(1);
    }
    std::string buildCommand = "";
    //compiler
    if(compiler.at(i) == "_null_")
    {
        buildCommand += m_GetDefaultCompiler(i);
    }
    else
    {
        buildCommand += compiler.at(i);
    }
    buildCommand += " ";
    
    //flags
    if(m_TargetSettingExists(target,"flags"))
    {
        Key<std::string> flags = m_GetTargetKeySetting(target, "flags");
        if(flags.keyValue.at(flags.keyValue.length()-1) != ';')
        {
            flags.keyValue+=";";
        }
        flags.keyValue += "D"+GetOSDefine(oses.at(i),arch)+";";
        if(flags.keyValue.find("_var(") != std::string::npos)
        {
            size_t varStart = flags.keyValue.find("_var(");
            while(varStart != std::string::npos)
            {
                size_t nextSetting = flags.keyValue.find(";",varStart);
                size_t varEnd = flags.keyValue.find(")_",varStart);
            
                if((varStart != std::string::npos && varEnd != std::string::npos) && (nextSetting > varEnd || nextSetting == std::string::npos))
                {
                    //varStart+=5;
                    std::string var = m_GetVar(flags.keyValue.substr(varStart+5,varEnd-(varStart+5)),oses.at(i));
                    if(var == "_Error_")
                    {
                        std::cout << "Variable: '" << flags.keyValue.substr(varStart+5,varEnd-(varStart+5)) << "' is not set" << std::endl;
                        exit(1);
                    }
                    std::string post = flags.keyValue.substr(varEnd+2);
                    if(var == "")
                    {
                        post = post.substr(1);
                    }
                    std::string pre = flags.keyValue.substr(0,varStart);
                    flags.keyValue = pre+var+post;
                    std::cout << "Loaded var: '" << var << "' FullString: '" << flags.keyValue << "'" << std::endl;
                    varStart = flags.keyValue.find("_var(",varEnd);
                }
                else
                {
                    break;
                }
            }
            
        }
        std::cout << flags.keyValue << std::endl;
        size_t front = 0;
        bool die = false;
        while(front != std::string::npos && !die)
        {
            size_t next = flags.keyValue.find(";",front+1);
            if(next == std::string::npos)
            {
                next = flags.keyValue.length();
                die = 1;
                if(front == flags.keyValue.length()-1)
                {
                    break;
                }
            }
            if(front != 0)
            {
                front++;
            }
            buildCommand += "-"+flags.keyValue.substr(front,next-front)+" ";
            front = next;
        }
    }
    if(!m_TargetSettingExists(target,"in"))
    {
        std::cout << "Target: '" << target << "' does not build anything?" << std::endl;
        exit(1);
    }
    Key<std::string> in = m_GetTargetKeySetting(target,"in");
    if(in.keyValue == "_depends_")
    {
        for(size_t u = 0; u < m_GetDependecyKey(target).keyValue.size(); u++)
        {
            if(m_TargetSettingExists(m_GetDependecyKey(target).keyValue.at(u),"out"))
            {
                buildCommand += " "+m_GetTargetKeySetting(m_GetDependecyKey(target).keyValue.at(u),"out").keyValue;
            }
            else
            {
                buildCommand += " "+m_GetDependecyKey(target).keyValue.at(u)+".o";
            }
        }
    }
    else
    {
        if(in.keyValue.find("_var(") != std::string::npos)
        {
            size_t varStart = in.keyValue.find("_var(");
            while(varStart != std::string::npos)
            {
                size_t nextSetting = in.keyValue.find(";",varStart);
                size_t varEnd = in.keyValue.find(")_",varStart);
            
                if((varStart != std::string::npos && varEnd != std::string::npos) && (nextSetting > varEnd || nextSetting == std::string::npos))
                {
                    //varStart+=5;
                    std::string var = m_GetVar(in.keyValue.substr(varStart+5,varEnd-(varStart+5)),oses.at(i));
                    if(var == "_Error_")
                    {
                        std::cout << "Variable: '" << in.keyValue.substr(varStart+5,varEnd-(varStart+5)) << "' is not set" << std::endl;
                        exit(1);
                    }
                    std::string post = in.keyValue.substr(varEnd+2);
                    if(var == "")
                    {
                        post = post.substr(1);
                    }
                    std::string pre = in.keyValue.substr(0,varStart);
                    in.keyValue = pre+var+post;
                    std::cout << "Loaded var: '" << var << "' FullString: '" << in.keyValue << "'" << std::endl;
                    varStart = in.keyValue.find("_var(",varEnd);
                }
                else
                {
                    break;
                }
            }
            
        }
        std::cout << in.keyValue << std::endl;
        size_t front = 0;
        bool die = false;
        while(front != std::string::npos && !die)
        {
            size_t next = in.keyValue.find(";",front+1);
            if(next == std::string::npos)
            {
                next = in.keyValue.length();
                die = 1;
                if(front == in.keyValue.length()-1)
                {
                    break;
                }
            }
            if(front != 0)
            {
                front++;
            }
            buildCommand +=  " ./"+m_workFolder+"/"+in.keyValue.substr(front,next-front)+" ";
            front = next;
        }
        
        //buildCommand += " ./"+m_workFolder+"/"+in.keyValue; 
    }
    
    //Output
    std::string out = "";
    bool libState = 1;
    if(!m_TargetSettingExists(target,"lib"))
    {
        libState = 0;
    }
    else if(m_GetTargetKeySetting(target,"lib").keyValue == "_nolib_")
    {
        libState = 0;
    }
    if(!m_TargetSettingExists(target,"out"))
    {
        if(m_GetTargetKeySetting(target,"target_type").keyValue == "_obj_")
        {
            out = target+".o";
        }
        else
        {
            out = target+m_GetOsEx(oses.at(i),libState);
        }
    }
    else
    {
        if(m_GetTargetKeySetting(target,"target_type").keyValue == "_obj_")
        {
            out =  m_GetTargetKeySetting(target,"out").keyValue+m_GetOsEx(oses.at(i),libState)+".o";
        }
        else
        {
             out = m_GetTargetKeySetting(target,"out").keyValue+m_GetOsEx(oses.at(i),libState);
        }
       
    }
    buildCommand += " -o " + out+" ";
    
    //Linker
    if(m_TargetSettingExists(target,"link"))
    {
        Key<std::string> links = m_GetTargetKeySetting(target, "link");
        if(links.keyValue.find("_var(") != std::string::npos)
        {
            size_t varStart = links.keyValue.find("_var(");
            while(varStart != std::string::npos)
            {
                size_t nextSetting = links.keyValue.find(";",varStart);
                size_t varEnd = links.keyValue.find(")_",varStart);
            
                if((varStart != std::string::npos && varEnd != std::string::npos) && (nextSetting > varEnd || nextSetting == std::string::npos))
                {
                    //varStart+=5;
                    std::string var = m_GetVar(links.keyValue.substr(varStart+5,varEnd-(varStart+5)),oses.at(i));
                    if(var == "_Error_")
                    {
                        std::cout << "Variable: '" << links.keyValue.substr(varStart+5,varEnd-(varStart+5)) << "' is not set" << std::endl;
                        exit(1);
                    }
                    std::string post = links.keyValue.substr(varEnd+2);
                    std::string pre = links.keyValue.substr(0,varStart);
                    if(var == "")
                    {
                        if(post.size() > 0)
                        {
                            post = post.substr(1);
                        }
                        
                    }
                    
                    links.keyValue = pre+var+post;
                    std::cout << "Loaded var: '" << var << "' FullString: '" << links.keyValue << "'" << std::endl;
                    varStart = links.keyValue.find("_var(",varEnd);
                }
                else
                {
                    break;
                }
            }
            
        }
        std::cout << links.keyValue << std::endl;
        size_t front = 0;
        bool die = false;
        while(front != std::string::npos && !die && links.keyValue != "")
        {
            size_t next = links.keyValue.find(";",front+1);
            if(next == std::string::npos)
            {
                next = links.keyValue.length();
                die = 1;
                if(front == links.keyValue.length()-1)
                {
                    break;
                }
            }
            if(front != 0)
            {
                front++;
            }
            buildCommand += "-l"+links.keyValue.substr(front,next-front)+" ";
            front = next;
        }
    }
    //Need to escape 'n stuff at some point
    std::cout << "Running: \"" << buildCommand << "\"" << std::endl;  
    int err = system(buildCommand.c_str());
    if(err != 0)
    {
        throw TargetBuildException("Target: '" + target + "' failed to build for the platform: '" + oses.at(i) + "'");
    }
    
}


void RPAutoManager::m_BuildAndResolveDeps(size_t index)
{
    std::string targetString = targets.at(index);
    /*Key<std::vector<Key<std::string>>>* targetKey = m_GetTargetKey(targetString);
    
    Key<std::vector<std::string>> depkey = m_GetDependecyKey(targetString);
    if(depkey.keyName != "_Error_")
    {
        for(size_t i = 0; i < depkey.keyValue.size(); i++)
        {
            m_buildTarget(index,depkey.keyValue.at(i));
        }
    }*/
    m_buildTarget(index,targetString);
    
}


void RPAutoManager::Build()
{
    for(size_t i = 0; i < oses.size(); i++)
    {
        //TODO check for the "clean","true" key here
        std::string target = targets.at(i);
        m_ParseTarget(target, i);
        
        try {m_BuildAndResolveDeps(i);}
        catch(TargetBuildException e)
        {
            std::cout << TERMINAL_COLOR_RED << "Build for platform: '" + oses.at(i) + "' failed with the following message:" << std::endl;
            std::cout << e.what() << std::endl;
            std::cout << TERMINAL_COLOR_YELLOW << "Skipping platform" << TERMINAL_COLOR_RESET << std::endl;
            continue;
        }
        std::cout << TERMINAL_COLOR_GREEN << "Build for '" << oses.at(i) << "' Succeeded!" << TERMINAL_COLOR_RESET << std::endl;
        
         //This also run "delete" on all the keys :)
        
        
    }
    targetSettings.clear();
}



