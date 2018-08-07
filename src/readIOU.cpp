#include <map>
#include <cstdio>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>
#include <future>

std::map<std::string, std::string> readOUIFile(const std::string& path)
{
    std::string line;

    std::map<std::string, std::string> ouiMap;
    
    std::ifstream ouiFile(path);

    if(ouiFile.is_open()) {
        while(getline(ouiFile,line))  {
            if(line.find("(hex)") != std::string::npos) {
                std::string ss(line);
                std::string mac, companyName = " ";
               
                size_t pos = ss.find(" ");
                mac = ss.substr(0 , pos);
                ss = ss.substr(pos);
                pos = ss.find(")");
                companyName = ss.substr(pos+1);
                std::replace(mac.begin(), mac.end(), '-', ':');
                ouiMap.insert(std::make_pair(mac, companyName));
               
            }
        }
        ouiFile.close();
    }
    return ouiMap;
}

bool isMacAddress(const std::string& macAddress)
{
    if(macAddress.length() < 8) {
        return false;
    }

    if((std::regex_match(macAddress.substr(0, 8), std::regex("^([0-9A-Fa-f]{2}[:-]){2}([0-9A-Fa-f]{2})"))))
        return true;

    return false;
}

int main(int argc, char** argv)
{
    if(argc < 2) {
        std::string usage = "Usage: " + std::string(argv[0]) + " <macaddress>";
        printf("\n%s\n", usage.c_str());
        return 1;
    }
    
    std::map<std::string, std::string> ouiMap;
    //std::thread th(readOUIFile, "../oui.txt", std::ref(ouiMap));
    auto handle = std::async(std::launch::async,
                             readOUIFile, "../oui.txt");
    
    
   /**/
    
    /*for(std::map<std::string, std::string>::iterator iterator = ouiMap.begin(); iterator != ouiMap.end(); iterator++) {
        printf("\n%s %s\n", iterator->first.c_str()  , iterator->second.c_str() );
    // Repeat if you also want to iterate through the second map.
    }*/
    //10:9a:dd:66:ab:c7
    std::string mac = argv[1];
    
    if(!isMacAddress(mac)) {
        printf("\nNot a MAC address\n");
        return 1;
    }

    std::transform(mac.begin(), mac.end(),mac.begin(), ::toupper);
     // replace all 'x' to 'y'
    std::replace(mac.begin(), mac.end(), '-', ':');

    std::string oui = mac.substr(0, 8);
    //th.join();
    ouiMap = handle.get();
    if(ouiMap.empty()) {
        printf("\nError reading oui file, check path");
        return 1;
    }

    auto it = ouiMap.find(oui);
    if (it != ouiMap.end()) {
        printf("\n %s %s\n", oui.c_str(), ouiMap[oui].c_str());
    }
    else {
        printf("\nManufacturer not found for MAC address: %s", mac.c_str());
    }
    return 0;
}