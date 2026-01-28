void report_thr(vector<uint32_t> pac_list,Object obj) {
    int left = 3, right = 4;
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
        int memory = M_lsit[i]; memory*=(1024*8);
        double heavy_mem = memory*heavy_ratio, light_mem = memory-heavy_mem;

        // TS
        int m = memory/k;
        auto ts = new TowerSketch(m,k,obj.TS_a,obj.TS_b,obj.TS_c);

        // CM
        m = memory/k/32;
        auto cm = new CMSketch(m,k,3000,3000,obj.CM_a,obj.CM_b,obj.CM_c);

        // CU
        m = memory/k/32;
        auto cu = new CUSketch(m,k,obj.CU_a,obj.CU_b,obj.CU_c);

        // ES
        m = light_mem/k/32;
        int l = heavy_mem/(32+1+32+32);

        auto es = new ElasticSketch(l,k,m,obj.ES_a,obj.ES_b,obj.ES_c);

        // PRE
        int prel = heavy_mem/(32+32);
        auto pre = new PRECISION(prel,4,k,m,obj.PRE_a,obj.PRE_b,obj.PRE_c);

        // TS
        if(ifTS) {
            int now_pac = 0;

            while (now_pac < packetnum) {
                ts->insert( pac_list[now_pac]);
                now_pac++;
            }
            clock_t start = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                ts->query(key);
            }
            clock_t end = clock();

            cout << "TS thr: " << double(truth_frequency.size()) / ((double)(end - start)/ CLOCKS_PER_SEC) / 1000000 << endl;

            clock_t start1 = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                ts->query_ml(key);
            }
            clock_t end1 = clock();

            cout << "TS-ml thr: " << double(truth_frequency.size()) / ((double)(end1 - start1)/ CLOCKS_PER_SEC) / 1000000 << endl;
        }
        //CM
        if(ifCM) {
            int now_pac = 0;
            while (now_pac < packetnum) {
                cm->insert( pac_list[now_pac],1);
                now_pac++;
            }
            clock_t start = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                cm->query(key);
            }
            clock_t end = clock();

            cout << "CM thr: " << double(truth_frequency.size()) / ((double)(end - start)/ CLOCKS_PER_SEC) / 1000000 << endl;

            clock_t start1 = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                cm->query_ml(key);
            }
            clock_t end1 = clock();
            cout << "CM-ml thr: " << double(truth_frequency.size()) / ((double)(end1 - start1)/ CLOCKS_PER_SEC) / 1000000 << endl;

        }
        //CU
        if(ifCU) {
            int now_pac = 0;
            while (now_pac < packetnum) {
                cu->insert( pac_list[now_pac],1);
                now_pac++;
            }
            clock_t start = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                cu->query(key);
            }
            clock_t end = clock();

            cout << "CU thr: " << double(truth_frequency.size()) / ((double)(end - start)/ CLOCKS_PER_SEC) / 1000000 << endl;

            clock_t start1 = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                cu->query_ml(key);
            }
            clock_t end1 = clock();
            cout << "CU-ml thr: " << double(truth_frequency.size()) / ((double)(end1 - start1)/ CLOCKS_PER_SEC) / 1000000 << endl;
        }
        //   ES
        if(ifES) {
            int now_pac = 0;
            while (now_pac < packetnum) {
                es->insert( pac_list[now_pac]);
                now_pac++;
            }
            clock_t start = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                es->query(key);
            }
            clock_t end = clock();

            cout << "ES thr: " << double(truth_frequency.size()) / ((double)(end - start)/ CLOCKS_PER_SEC) / 1000000 << endl;

            clock_t start1 = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                es->query_ml(key);
            }
            clock_t end1 = clock();
            cout << "ES-ml thr: " << double(truth_frequency.size()) / ((double)(end1 - start1)/ CLOCKS_PER_SEC) / 1000000 << endl;
        }
        //PRE
        if(ifPRE) {
            int now_pac = 0;
            while (now_pac < packetnum) {
                pre->insert( pac_list[now_pac]);
                now_pac++;
            }
            clock_t start = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                pre->query(key);
            }
            clock_t end = clock();
            cout << "pre thr: " << double(truth_frequency.size()) / ((double)(end - start)/ CLOCKS_PER_SEC) / 1000000 << endl;

            clock_t start1 = clock();
            for(auto kv : truth_frequency) {
                uint32_t key = kv.first;
                pre->query_ml(key);
            }
            clock_t end1 = clock();
            cout << "pre-ml thr: " << double(truth_frequency.size()) / ((double)(end1 - start1)/ CLOCKS_PER_SEC) / 1000000 << endl;

        }

    }
}

