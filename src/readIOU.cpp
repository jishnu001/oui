#include <map>
#include <cstdio>
#include <string>
#include <fstream>
#include <algorithm>

std::map<std::string, std::string> readOUIFile(const std::string& path)
{
    std::string line;

    std::map<std::string, std::string> ouiMap;
    
    std::ifstream ouiFile(path);

    if(1){//ouiFile.is_open()) {
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


int main()
{
    std::map<std::string, std::string> ouiMap = readOUIFile("../toui.txt");
    if(ouiMap.empty()) {
        printf("\nEmpty file");
    }
    
    /*for(std::map<std::string, std::string>::iterator iterator = ouiMap.begin(); iterator != ouiMap.end(); iterator++) {
        printf("\n%s %s\n", iterator->first.c_str()  , iterator->second.c_str() );
    // Repeat if you also want to iterate through the second map.
    }*/

    std::string mac = "10:9a:dd:66:ab:c7";
    std::transform(mac.begin(), mac.end(),mac.begin(), ::toupper);
     // replace all 'x' to 'y'
    std::replace(mac.begin(), mac.end(), '-', ':');
    printf("\n %s %s\n", mac.substr(0, 8).c_str(), ouiMap[mac.substr(0, 8)].c_str());

    return 0;
}