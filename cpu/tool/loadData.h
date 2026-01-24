#ifndef LOADDATA1_H
#define LOADDATA1_H

#include <fstream>
#include <queue>
#include <sstream>
#include "BOBHash32.h"
#include <filesystem>
// namespace fs = std::filesystem;
const int MAX_INSERT_PACKAGE = 3e7;
int packetnum;
struct Object {
    vector<uint32_t> pac_list;
    double CM_a,CM_b,CM_c;
    double CU_a,CU_b,CU_c;
    double TS_a,TS_b,TS_c;
    double ES_a,ES_b,ES_c;
    double PRE_a,PRE_b,PRE_c;
};
uint32_t convertIPv4ToUint32(char* ipAddress) {
    uint32_t result = 0;
    int octet =0;
    char ipCopy[40];
    strncpy(ipCopy,ipAddress,sizeof(ipCopy)-1);
    ipCopy[sizeof(ipCopy)-1]='\0';
    char* token=strtok(ipCopy,".");
    while(token !=nullptr) {
        octet =std::stoi(token);
        result=(result<<8)+octet;
        token = std::strtok(nullptr,".");
    }
    return result;
}

class DataLoader {
    private:
        Object obj;
        string filePath;
        std::map<std::string,std::map<std::string,std::vector<int>>> parameters_main;
    public:
        DataLoader(string filePath,std::map<std::string,std::map<std::string,std::vector<int>>> parameters_main): filePath(filePath), parameters_main(parameters_main){
            if(filePath == "ca19") {
                filePath = "data/CAIDA19.txt";
                obj.CM_a = parameters_main["CA19"]["CM"][0];
                obj.CM_b = parameters_main["CA19"]["CM"][1];
                obj.CM_c = parameters_main["CA19"]["CM"][2];
                obj.CU_a = parameters_main["CA19"]["CU"][0];
                obj.CU_b = parameters_main["CA19"]["CU"][1];
                obj.CU_c = parameters_main["CA19"]["CU"][2];
                obj.TS_a = parameters_main["CA19"]["TS"][0];
                obj.TS_b = parameters_main["CA19"]["TS"][1];
                obj.TS_c = parameters_main["CA19"]["TS"][2];
                obj.ES_a = parameters_main["CA19"]["ES"][0];
                obj.ES_b = parameters_main["CA19"]["ES"][1];
                obj.ES_c = parameters_main["CA19"]["ES"][2];
                obj.PRE_a = parameters_main["CA19"]["PRE"][0];
                obj.PRE_b = parameters_main["CA19"]["PRE"][1];
                obj.PRE_c = parameters_main["CA19"]["PRE"][2];
            }else if(filePath == "ca16") {
                filePath = "data/CAIDA16.txt";
                obj.CM_a = parameters_main["CA16"]["CM"][0];
                obj.CM_b = parameters_main["CA16"]["CM"][1];
                obj.CM_c = parameters_main["CA16"]["CM"][2];
                obj.CU_a = parameters_main["CA16"]["CU"][0];
                obj.CU_b = parameters_main["CA16"]["CU"][1];
                obj.CU_c = parameters_main["CA16"]["CU"][2];
                obj.TS_a = parameters_main["CA16"]["TS"][0];
                obj.TS_b = parameters_main["CA16"]["TS"][1];
                obj.TS_c = parameters_main["CA16"]["TS"][2];
                obj.ES_a = parameters_main["CA16"]["ES"][0];
                obj.ES_b = parameters_main["CA16"]["ES"][1];
                obj.ES_c = parameters_main["CA16"]["ES"][2];
                obj.PRE_a = parameters_main["CA16"]["PRE"][0];
                obj.PRE_b = parameters_main["CA16"]["PRE"][1];
                obj.PRE_c = parameters_main["CA16"]["PRE"][2];

            }else if(filePath == "so") {
                filePath = "data/stackoverflow.txt";
                obj.CM_a = parameters_main["SO"]["CM"][0];
                obj.CM_b = parameters_main["SO"]["CM"][1];
                obj.CM_c = parameters_main["SO"]["CM"][2];
                obj.CU_a = parameters_main["SO"]["CU"][0];
                obj.CU_b = parameters_main["SO"]["CU"][1];
                obj.CU_c = parameters_main["SO"]["CU"][2];
                obj.TS_a = parameters_main["SO"]["TS"][0];
                obj.TS_b = parameters_main["SO"]["TS"][1];
                obj.TS_c = parameters_main["SO"]["TS"][2];
                obj.ES_a = parameters_main["SO"]["ES"][0];
                obj.ES_b = parameters_main["SO"]["ES"][1];
                obj.ES_c = parameters_main["SO"]["ES"][2];
                obj.PRE_a = parameters_main["SO"]["PRE"][0];
                obj.PRE_b = parameters_main["SO"]["PRE"][1];
                obj.PRE_c = parameters_main["SO"]["PRE"][2];
            }else if(filePath == "dc") {
                filePath = "data/datacenter.txt";
                obj.CM_a = parameters_main["DC"]["CM"][0];
                obj.CM_b = parameters_main["DC"]["CM"][1];
                obj.CM_c = parameters_main["DC"]["CM"][2];
                obj.CU_a = parameters_main["DC"]["CU"][0];
                obj.CU_b = parameters_main["DC"]["CU"][1];
                obj.CU_c = parameters_main["DC"]["CU"][2];
                obj.TS_a = parameters_main["DC"]["TS"][0];
                obj.TS_b = parameters_main["DC"]["TS"][1];
                obj.TS_c = parameters_main["DC"]["TS"][2];
                obj.ES_a = parameters_main["DC"]["ES"][0];
                obj.ES_b = parameters_main["DC"]["ES"][1];
                obj.ES_c = parameters_main["DC"]["ES"][2];
                obj.PRE_a = parameters_main["DC"]["PRE"][0];
                obj.PRE_b = parameters_main["DC"]["PRE"][1];
                obj.PRE_c = parameters_main["DC"]["PRE"][2];
            }
            clear();
            load_data(filePath);
            cout << "Loading dataset" << endl;
        }
        void clear() {
            obj.pac_list.clear();
        }

        void load_data(string filePath) {

            std::ifstream pf(filePath);
            if (!pf) {
                cout << filePath << " not found." << endl;
                exit(-1);
            }

            int ret = 0;
            string line;

            while (getline(pf, line)) {
                std::istringstream stream(line);
                std::string str1, str2;
                stream >> str1 >> str2;
                bool f1 = std::any_of(str1.begin(), str1.end(), [](char ch) {
                    return std::isalpha(static_cast<unsigned char>(ch));
                }),f2 = std::any_of(str2.begin(), str2.end(), [](char ch) {
                    return std::isalpha(static_cast<unsigned char>(ch));
                });
                if(f2) {
                    continue;
                }
                char *s2 = &str2[0];
                uint32_t x = convertIPv4ToUint32(s2);
                // uint32_t x = stoul(s1), y = stoul(s2);
                if(x != 0){
                    obj.pac_list.push_back(x);
                    ret++;
                    if (ret == MAX_INSERT_PACKAGE)
                        break;

                }
                packetnum = ret;
            }
            pf.close();
        }
    Object get_object() {return obj;}
};
#endif //LOADDATA_H
