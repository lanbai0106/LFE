void report_M(vector<uint32_t> pac_list,std::map<std::string,std::map<std::string,std::vector<int>>> parameters_main) {
    int left = 1, right = 2;
    int M_lsit[6] = {100,200,300,400,500};
    double heavy_ratio = 0.3;

    int k = 3;
    bool ifTS = 1, ifCM = 1, ifES = 1, ifCU = 1, ifPRE = 1;
    int now_pac = 0;
    while (now_pac < packetnum) {
        truth_frequency[pac_list[now_pac]]++;
        update_H(truth_frequency[pac_list[now_pac]],Hist);
        now_pac++;
    }
    for(int i = left;i < right;i++) {

        cout <<  "i = " << i<< endl;
        clear();
        int memory = M_lsit [i]; memory*=(1024*8);
        double heavy_mem = memory*heavy_ratio, light_mem = memory-heavy_ratio;

        double tsaae = 0, cmaae = 0, precaae = 0, cuaae = 0, esaae = 0, preaae = 0;
        double tsare = 0, cmare = 0, precare = 0, cuare = 0, esare = 0, preare = 0;

        double tsaae_ml = 0, cmaae_ml = 0, precaae_ml = 0, cuaae_ml = 0, esaae_ml = 0, preaae_ml = 0;
        double tsare_ml = 0, cmare_ml = 0, precare_ml = 0, cuare_ml = 0, esare_ml = 0, preare_ml = 0;

        double tsaaeH = 0, cmaaeH = 0, precaaeH = 0, cuaaeH = 0, esaaeH = 0, preaaeH = 0;
        double tsareH = 0, cmareH = 0, precareH = 0, cuareH = 0, esareH = 0, preareH = 0;

        double tsaaeH_ml = 0, cmaaeH_ml = 0, precaaeH_ml = 0, cuaaeH_ml = 0, esaaeH_ml = 0, preaaeH_ml = 0;
        double tsareH_ml = 0, cmareH_ml = 0, precareH_ml = 0, cuareH_ml = 0, esareH_ml = 0, preareH_ml = 0;

        // TS
        int m = memory/k;
        auto ts = new TowerSketch(m,k);

        // CM
        m = memory/k/16;
        auto cm = new CMSketch(m,k,3000,3000);

        // CU
        m = memory/k/16;
        auto cu = new CUSketch(m,k);

        // ES
        m = light_mem/k/16;
        int l = heavy_mem/(32+1+16+16);
        auto es = new ElasticSketch(l,k,m);

        // PRE
        int prel = heavy_mem/(32+16);
        auto pre = new PRECISION(prel,4,k,m);
        int now_pac = 0;
        while (now_pac < packetnum) {

             // TS
            if(ifTS) {
                auto results = ts->insert( pac_list[now_pac]);
                sort(results.begin(),results.end());
                update_H(results[0],Hist_ts);

                int ts_ml = 1e9;
                if(results[1] - results[0] > 100 && results[0] < 2000) {
                    ts_ml = min(results[0]/32,results[1]/32);
                    ts_ml = min(ts_ml,results[2]/32);
                    if(ts_ml == results[0]) {
                        update_H(results[0],Hist_ts_ml);
                    }
                }else {
                    update_H(results[0],Hist_ts_ml);
                }
            }
            //CM
            if(ifCM) {
                auto results = cm->insert( pac_list[now_pac],1);
                sort(results.begin(),results.end());
                update_H(results[0],Hist_cm);
                // CM_ml no
                int cm_ml = 1e9;
                if(results[1] - results[0] > 100 && results[0] < 2000) {
                    cm_ml = min(results[0]/32,results[1]/32);
                    cm_ml = min(cm_ml,results[2]/32);
                    if(cm_ml == results[0]) {
                        update_H(results[0],Hist_cm_ml);
                    }
                }else {
                    update_H(results[0],Hist_cm_ml);
                }
            }
            //CU
            if(ifCU) {
                auto results = cu->insert( pac_list[now_pac],1);
                sort(results.begin(),results.end());
                update_H(results[0],Hist_cu);

                int cu_ml = 1e9;
                if(results[1] - results[0] > 100 && results[0] < 2000) {
                    cu_ml = min(results[0]/32,results[1]/32);
                    cu_ml = min(cu_ml,results[2]/32);
                    if(cu_ml == results[0]) {
                        update_H(results[0],Hist_cu_ml);
                    }
                }else {
                    update_H(results[0],Hist_cu_ml);
                }
            }

            //   ES
            if(ifES) {
                auto results = es->insert( pac_list[now_pac] );
                if(results[3]!=-1) {
                    update_H(results[3],Hist_es);
                    update_H(results[3],Hist_es_ml);
                }
                if(results[0]!= -1) {
                    results.pop_back();
                    sort(results.begin(),results.end());

                    update_H(results[0],Hist_es);

                    int es_ml = 1e9;
                    if(results[1] - results[0] > 100 && results[0] < 2000) {
                        es_ml = min(results[0]/32,results[1]/32);
                        es_ml = min(es_ml,results[2]/32);
                        if(es_ml == results[0]) {
                            update_H(results[0],Hist_es_ml);
                        }
                    }else {
                        update_H(results[0],Hist_es_ml);
                    }
                }
            }
            //PRE
            if(ifPRE) {
                auto results = pre->insert( pac_list[now_pac] );
                if(results[3]!=-1) {
                    update_H(results[3],Hist_pre);
                    update_H(results[3],Hist_pre_ml);
                }
                if(results[0]!= -1) {
                    results.pop_back();
                    sort(results.begin(),results.end());

                    update_H(results[0],Hist_pre);

                    int pre_ml = 1e9;
                    if(results[1] - results[0] > 100 && results[0] < 2000) {
                        pre_ml = min(results[0]/32,results[1]/32);
                        pre_ml = min(pre_ml,results[2]/32);
                        if(pre_ml == results[0]) {
                            update_H(results[0],Hist_pre_ml);
                        }
                    }else {
                        update_H(results[0],Hist_pre_ml);
                    }
                }
            }
            now_pac = now_pac + 1;
        }


        for(auto kv:truth_frequency) {
            uint32_t key = kv.first;
            int value = kv.second;
            int cmv = cm->query(key);
            cmaae += abs(value-cmv);
            cmare += abs(value-cmv)/value;
            int cmmlv = cm->query_ml(key);
            cmaae_ml += abs(value-cmmlv);
            cmare_ml += abs(value-cmmlv)/value;

            int cuv = cu->query(key);
            cuaae += abs(value-cuv);
            cuare += abs(value-cuv)/value;
            int cuvml = cu->query_ml(key);
            cuaae_ml += abs(value-cuvml);
            cuare_ml += abs(value-cuvml)/value;

            int tsv = ts->query(key);
            tsaae += abs(value-tsv);
            tsare += abs(value-tsv)/value;
            int tsmlv = es->query_ml(key);
            tsaae_ml += abs(value-tsmlv);
            tsare_ml += abs(value-tsmlv)/value;

            int esv = es->query(key);
            esaae += abs(value-esv);
            esare += abs(value-esv)/value;
            int esmlv = es->query_ml(key);
            esaae_ml += abs(value-esmlv);
            esare_ml += abs(value-esmlv)/value;

            int prev = pre->query(key);
            preaae += abs(value-prev);
            preare += abs(value-prev)/value;
            int premlv = pre->query_ml(key);
            preaae_ml += abs(value-premlv);
            preare_ml += abs(value-premlv)/value;
        }

        for (int i = 0; i < interval_num; i++) {
            cmaaeH += abs(Hist_cm[i]-Hist[i]);
            cmareH += abs(Hist_cm[i]-Hist[i])/Hist[i];
            cmaaeH_ml += abs(Hist_cm_ml[i]-Hist[i]);
            cmareH_ml += abs(Hist_cm_ml[i]-Hist[i])/Hist[i];


            cuaaeH += abs(Hist_cu[i]-Hist[i]);
            cuareH += abs(Hist_cu[i]-Hist[i])/Hist[i];
            cuaaeH_ml += abs(Hist_cu_ml[i]-Hist[i]);
            cuareH_ml += abs(Hist_cu_ml[i]-Hist[i])/Hist[i];

            tsaaeH += abs(Hist_ts[i]-Hist[i]);
            tsareH += abs(Hist_ts[i]-Hist[i])/Hist[i];
            tsaaeH_ml += abs(Hist_ts_ml[i]-Hist[i]);
            tsareH_ml += abs(Hist_ts_ml[i]-Hist[i])/Hist[i];
            
            esaaeH += abs(Hist_es[i]-Hist[i]);
            esareH += abs(Hist_es[i]-Hist[i])/Hist[i];
            esaaeH_ml += abs(Hist_es_ml[i]-Hist[i]);
            esareH_ml += abs(Hist_es_ml[i]-Hist[i])/Hist[i];

            preaaeH += abs(Hist_pre[i]-Hist[i]);
            preareH += abs(Hist_pre[i]-Hist[i])/Hist[i];
            preaaeH_ml += abs(Hist_pre_ml[i]-Hist[i]);
            preareH_ml += abs(Hist_pre_ml[i]-Hist[i])/Hist[i];
        }
        cmaae_list[i] = cmaae/truth_frequency.size();
        cmare_list[i] = cmare/truth_frequency.size();
        cmaae_list_ml[i] = cmaae_ml/truth_frequency.size();
        cmare_list_ml[i] = cmare_ml/truth_frequency.size();

        cuaae_list[i] = cuaae/truth_frequency.size();
        cuare_list[i] = cuare/truth_frequency.size();
        cuaae_list_ml[i] = cuaae_ml/truth_frequency.size();
        cuare_list_ml[i] = cuare_ml/truth_frequency.size();

        Tsaae_list[i] = tsaae/truth_frequency.size();
        Tsare_list[i] = tsare/truth_frequency.size();
        Tsaae_list_ml[i] = tsaae_ml/truth_frequency.size();
        Tsare_list_ml[i] = tsare_ml/truth_frequency.size();

        esaae_list[i] = esaae/truth_frequency.size();
        esare_list[i] = esare/truth_frequency.size();
        esaae_list_ml[i] = esaae_ml/truth_frequency.size();
        esare_list_ml[i] = esare_ml/truth_frequency.size();

        preaae_list[i] = preaae/truth_frequency.size();
        preare_list[i] = preare/truth_frequency.size();
        preaae_list_ml[i] = preaae_ml/truth_frequency.size();
        preare_list_ml[i] = preare_ml/truth_frequency.size();

        /*
         * Dist
         */
        cmaaeH_list[i] = cmaaeH/interval_num;
        cmareH_list[i] = cmareH/interval_num;
        cmaaeH_list_ml[i] = cmaaeH_ml/interval_num;
        cmareH_list_ml[i] = cmareH_ml/interval_num;

        cuaaeH_list[i] = cuaaeH/interval_num;
        cuareH_list[i] = cuareH/interval_num;
        cuaaeH_list_ml[i] = cuaaeH_ml/interval_num;
        cuareH_list_ml[i] = cuareH_ml/interval_num;

        tsaaeH_list[i] = tsaaeH/interval_num;
        tsareH_list[i] = tsareH/interval_num;
        tsaaeH_list_ml[i] = tsaaeH_ml/interval_num;
        tsareH_list_ml[i] = tsareH_ml/interval_num;


        esaaeH_list[i] = esaaeH/interval_num;
        esareH_list[i] = esareH/interval_num;
        esaaeH_list_ml[i] = esaaeH_ml/interval_num;
        esareH_list_ml[i] = esareH_ml/interval_num;

        preaaeH_list[i] = preaaeH/interval_num;
        preareH_list[i] = preareH/interval_num;
        preaaeH_list_ml[i] = preaaeH_ml/interval_num;
        preareH_list_ml[i] = preareH_ml/interval_num;
        // print_H_all();
    }
    print_aae_are_all();
    print_H_aae_are_all();


}

