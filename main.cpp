#include "rpauto.h"
#include "configman.h"
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    RPAutoManager* manager;
    
    std::string cloneName = "";
    if (argc != 2 && argc != 3)
    {
        std::cout << "Invalid Repo" << std::endl << "USAGE:" << std::endl << "autobuild <git link>" << std::endl;
        exit(2);
    }
    
    if (argc == 3 && Cstrcmp(argv[2], "--local"))
    {
       
        try{
            cloneName = GetRepoNameFromLink(argv[1]);
        }
        catch(LinkParseException e)
        {
            std::cout << e.what() << std::endl;
            exit(e.GetCode());
        }
        manager = new RPAutoManager(cloneName);
    }
    else
    {
        //Usafe
        std::string gitRepo = std::string(argv[1]);
        if(gitRepo.find('&') != std::string::npos)
        {
            exit(0xFF0000FF); //We can't have a '&' in our input
        }
        std::string commandString = "git clone --recurse-submodules --remote-submodules " + gitRepo;
        int err = system(commandString.c_str());
        if (err != 0)
        {
            std::cout << "Could not clone " << argv[1] << std::endl;
            exit(1);
        }
        std::string cloneName = "";
        try{
            cloneName = GetRepoNameFromLink(argv[1]);
        }
        catch(LinkParseException e)
        {
            std::cout << e.what() << std::endl;
            exit(e.GetCode());
        }
        manager = new RPAutoManager(cloneName);
    }
    manager->ParseConfig();
    if(!manager->ValidateTargets())
    {
        std::cout << "Failed to validate targets" << std::endl;
        exit(1);
    }
    std::cout << TERMINAL_COLOR_GREEN << "Parse successful!" << TERMINAL_COLOR_RESET << std::endl;
    manager->Build();
    delete manager;
    return 0;
}
