#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <time.h>
#include <random>
#include "tool/Parameters_main.h"
#include "tool/loadData.h"
#include "tool/common.h"

#include "sketch/CMSketch.h"
#include "sketch/CUSketch.h"
#include "sketch/ElasticSketch.h"
#include "sketch/TowerSketch.h"
#include "sketch/PRECISION.h"


#include "report/report_Hist.h"
#include "report/report_thr.h"
#include <filesystem>
using namespace std;

int main()
{
    std::map<std::string,std::map<std::string,std::vector<int>>> parameters_main = read_parameters_main();
    DataLoader data_loader = DataLoader("so",parameters_main); Object obj = data_loader.get_object();
    vector<uint32_t> pac_list  = obj.pac_list;
    bool ifM = 0, ifThr = 1;
    if(ifM) {
        report_M(obj.pac_list,obj);
    }
    if(ifThr) {
        report_thr(obj.pac_list,obj);
    }
    return 0;
}
