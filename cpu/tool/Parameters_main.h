std::vector<int> CM_CA16 = {32,32,32};
std::vector<int> CM_CA19 = {4,4,4};
std::vector<int> CM_DC = {32,32,32};
std::vector<int> CM_SO = {32,32,32};

std::vector<int> CU_CA16 = {32,32,32};
std::vector<int> CU_CA19 = {4,4,4};
std::vector<int> CU_DC = {32,32,32};
std::vector<int> CU_SO = {32,32,32};

std::vector<int> ES_CA16 = {32,32,32};
std::vector<int> ES_CA19 = {4,2,4};
std::vector<int> ES_DC = {32,32,32};
std::vector<int> ES_SO = {32,32,32};

std::vector<int> TS_CA16 = {32,32,32};
std::vector<int> TS_CA19 = {2,1,1};
std::vector<int> TS_DC = {32,32,32};
std::vector<int> TS_SO = {32,32,32};

std::vector<int> PRE_CA16 = {32,32,32};
std::vector<int> PRE_CA19 = {16,16,16};
std::vector<int> PRE_DC = {32,32,32};
std::vector<int> PRE_SO = {32,32,32};



std::map<std::string,std::map<std::string,std::vector<int>>> read_parameters_main() {
    std::map<std::string,std::map<std::string,std::vector<int>>> parameters_main;
    parameters_main["CA16"]["CM"] = CM_CA16;
    parameters_main["CA19"]["CM"] = CM_CA19;
    parameters_main["DC"]["CM"] = CM_DC;
    parameters_main["SO"]["CM"] = CM_SO;

    parameters_main["CA16"]["CU"] = CU_CA16;
    parameters_main["CA19"]["CU"] = CU_CA19;
    parameters_main["DC"]["CU"] = CU_DC;
    parameters_main["SO"]["CU"] = CU_SO;

    parameters_main["CA16"]["ES"] = ES_CA16;
    parameters_main["CA19"]["ES"] = ES_CA19;
    parameters_main["DC"]["ES"] = ES_DC;
    parameters_main["SO"]["ES"] = ES_SO;

    parameters_main["CA16"]["PRE"] = PRE_CA16;
    parameters_main["CA19"]["PRE"] = PRE_CA19;
    parameters_main["DC"]["PRE"] = PRE_DC;
    parameters_main["SO"]["PRE"] = PRE_SO;

    parameters_main["CA16"]["TS"] = TS_CA16;
    parameters_main["CA19"]["TS"] = TS_CA19;
    parameters_main["DC"]["TS"] = TS_DC;
    parameters_main["SO"]["TS"] = TS_SO;
    return parameters_main;
}
