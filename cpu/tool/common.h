#ifndef COMMON_H
#define COMMON_H
map<uint32_t, int> truth_frequency;
double Tsaae_list[10], cmaae_list[10], precaae_list[10], cuaae_list[10], esaae_list[10],preaae_list[10];
double Tsare_list[10], cmare_list[10], precare_list[10], cuare_list[10], esare_list[10],preare_list[10];

double tsaaeH_list[10], cmaaeH_list[10], precaaeH_list[10], cuaaeH_list[10], esaaeH_list[10],preaaeH_list[10];
double tsareH_list[10], cmareH_list[10], precareH_list[10], cuareH_list[10], esareH_list[10],preareH_list[10];

double Tsaae_list_ml[10], cmaae_list_ml[10], precaae_list_ml[10], cuaae_list_ml[10], esaae_list_ml[10],preaae_list_ml[10];
double Tsare_list_ml[10], cmare_list_ml[10], precare_list_ml[10], cuare_list_ml[10], esare_list_ml[10],preare_list_ml[10];

double tsaaeH_list_ml[10], cmaaeH_list_ml[10], precaaeH_list_ml[10], cuaaeH_list_ml[10], esaaeH_list_ml[10],preaaeH_list_ml[10];
double tsareH_list_ml[10], cmareH_list_ml[10], precareH_list_ml[10], cuareH_list_ml[10], esareH_list_ml[10],preareH_list_ml[10];

int interval_num = 9;
int Hist[10];
int Hist_ts[10];
int Hist_cm[10];
int Hist_cu[10];
int Hist_es[10];
int Hist_pre[10];
int Hist_cm_ml[10];
int Hist_cu_ml[10];
int Hist_ts_ml[10];
int Hist_es_ml[10];
int Hist_pre_ml[10];

void clear() {
    // truth_frequency.clear();
    for (int i = 0; i < 10; i++) {
        // Hist[i] = 0;
        Hist_ts[i] = 0;
        Hist_cm[i] = 0;
        Hist_cu[i] = 0;
        Hist_es[i] = 0;
        Hist_pre[i] = 0;
        Hist_cm_ml[i] = 0;
        Hist_cu_ml[i] = 0;
        Hist_ts_ml[i] = 0;
        Hist_es_ml[i] = 0;
        Hist_pre_ml[i] = 0;
    }
}
void update_H(int v,int* a) {
    if(v == 1) a[0]++;
    if(v == 11) a[1]++,a[0]--;
    if(v == 101) a[2]++,a[1]--;
    if(v == 501) a[3]++,a[2]--;
    if(v == 1001) a[4]++,a[3]--;
    if(v == 1501) a[5]++,a[4]--;
    if(v == 2001) a[6]++,a[5]--;
    if(v == 3001) a[7]++,a[6]--;
    if(v == 4001) a[8]++,a[7]--;
}

void print_H(int tmp[]) {
    for (int i = 0; i < interval_num; i++) {
        cout << tmp[i] << " ";
    }
    cout << endl;
}
void print_H_all() {
    cout << "Ture: "; print_H(Hist);
    cout << "TS: "; print_H(Hist_ts);
    cout << "TS-ml: "; print_H(Hist_ts_ml);
    cout << endl;
    cout << "Ture: "; print_H(Hist);
    cout << "CM: "; print_H(Hist_cm);
    cout << "CM-ml: "; print_H(Hist_cm_ml);
    cout << endl;
    cout << "Ture: "; print_H(Hist);
    cout << "CU: "; print_H(Hist_cu);
    cout << "CU-ml: "; print_H(Hist_cu_ml);
    cout << endl;
    cout << "Ture: "; print_H(Hist);
    cout << "ES: "; print_H(Hist_es);
    cout << "ES-ml: "; print_H(Hist_es_ml);
    cout << endl;
    cout << "Ture: "; print_H(Hist);
    cout << "PRE: "; print_H(Hist_pre);
    cout << "PRE-ml: "; print_H(Hist_pre_ml);
}

void print_aae_are(double tmp[], double are[]) {
    cout << "aae: ";
    for (int i = 0; i < 5; i++) {
        cout << tmp[i] << " ";
    }
    cout << endl;
    cout << "are: ";
    for (int i = 0; i < 5; i++) {
        cout << are[i] << " ";
    }
    cout << endl;
}

void print_aae_are_all() {
    cout << "Frequency: \n";
    cout << "TS: "; print_aae_are(Tsaae_list,Tsare_list);
    cout << "TS-ml: "; print_aae_are(Tsaae_list_ml,Tsare_list_ml);
    cout << endl;
    cout << "CM: "; print_aae_are(cmaae_list,cmare_list);
    cout << "CM-ml: "; print_aae_are(cmaae_list_ml,cmare_list_ml);
    cout << endl;
    cout << "CU: "; print_aae_are(cuaae_list,cuare_list);
    cout << "CU-ml: "; print_aae_are(cuaae_list_ml,cuare_list_ml);
    cout << endl;
    cout << "ES: "; print_aae_are(esaae_list,esare_list);
    cout << "ES-ml: "; print_aae_are(esaae_list_ml,esare_list_ml);
    cout << endl;
    cout << "PRE: "; print_aae_are(preaae_list,preare_list);
    cout << "PRE-ml: "; print_aae_are(preaae_list_ml,preare_list_ml);
}

void print_H_aae_are_all() {
    cout << "Distribution: \n";
    cout << "TS: "; print_aae_are(tsaaeH_list,tsareH_list);
    cout << "TS-ml: "; print_aae_are(tsaaeH_list_ml,tsareH_list_ml);
    cout << endl;
    cout << "CM: "; print_aae_are(cmaaeH_list,cmareH_list);
    cout << "CM-ml: "; print_aae_are(cmaaeH_list_ml,cmareH_list_ml);
    cout << endl;
    cout << "CU: "; print_aae_are(cuaaeH_list,cuareH_list);
    cout << "CU-ml: "; print_aae_are(cuaaeH_list_ml,cuareH_list_ml);
    cout << endl;
    cout << "ES: "; print_aae_are(esaaeH_list,esareH_list);
    cout << "ES-ml: "; print_aae_are(esaaeH_list_ml,esareH_list_ml);
    cout << endl;
    cout << "PRE: "; print_aae_are(preaaeH_list,preareH_list);
    cout << "PRE-ml: "; print_aae_are(preaaeH_list_ml,preareH_list_ml);
}

long long rd() {
    return 1ll*rand()*rand()+rand();
}
#endif //COMMON_H
