#include <map>
#include <cstdio>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>

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
    if(macAddress.length() < 17) {
        return false;
    }

    if(!(std::regex_match(macAddress, std::regex("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})"))))
        return false;

    return true;
}

int main(int argc, char** argv)
{
    if(argc < 2) {
        std::string usage = "Usage: " + std::string(argv[0]) + " <macaddress>";
        printf("\n%s\n", usage.c_str());
        return 1;
    }

    std::map<std::string, std::string> ouiMap = readOUIFile("../oui.txt");

    if(ouiMap.empty()) {
        printf("\nError reading oui file, check path");
        return 1;
    }
    
    /*for(std::map<std::string, std::string>::iterator iterator = ouiMap.begin(); iterator != ouiMap.end(); iterator++) {
        printf("\n%s %s\n", iterator->first.c_str()  , iterator->second.c_str() );
    // Repeat if you also want to iterate through the second map.
    }*/
    //10:9a:dd:66:ab:c7
    std::string mac = argv[1];
    std::transform(mac.begin(), mac.end(),mac.begin(), ::toupper);
     // replace all 'x' to 'y'
    std::replace(mac.begin(), mac.end(), '-', ':');

    if(!isMacAddress(mac)) {
        printf("\nNot a MAC address\n");
        return 1;
    }

    printf("\n %s %s\n", mac.substr(0, 8).c_str(), ouiMap[mac.substr(0, 8)].c_str());

    return 0;
}