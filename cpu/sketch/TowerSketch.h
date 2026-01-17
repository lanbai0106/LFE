struct TowerSketch {
    int m, hnum;
    int bit_list[5] = {4,8,16,32};
    int m_list[5] = {0};
    uint32_t **a;
    BOBHash32 **hash;

    TowerSketch(int _m, int _hnum) : m(_m), hnum(_hnum) {
        for (int i = 0; i < _hnum; i++) {
            m_list[i] = m/bit_list[i];
        }

        a = new uint32_t*[hnum];
        for (int i = 0; i < _hnum; i++) {

            a[i] = new uint32_t[m_list[0]];
            for (int j = 0; j < m_list[i]; j++) {
                a[i][j] = 0;
            }

        }

        hash = new BOBHash32*[hnum];
        for (int i = 0; i < _hnum; i++) hash[i] = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
    }


    vector<int> insert(uint32_t key) {
        int hashid[hnum];
        vector<int> v;
        for (int i = 0; i < hnum; i++) hashid[i] = hash[i]->run((char *)&key, 4) % m_list[i];
        uint32_t ans = 1e9,maxn = -1;
        for (int i = 0; i < hnum; i++) {
            if(a[i][hashid[i]] < (1 << bit_list[i])-1) {
                a[i][hashid[i]] += 1 ;
                v.push_back(a[i][hashid[i]]);
            }else {
                v.push_back(999999999);
            }

        }
        return v;
    }

    double query(uint32_t key) {
        int hashid[hnum];
        for (int i = 0; i < hnum; i++) hashid[i] = hash[i]->run((char *)&key, 4) % m_list[i];

        uint32_t ans = 1e9;
        for (int i = 0; i < hnum; i++) {
            if(a[i][hashid[i]] < (1 << bit_list[i])-1) ans = min(ans,a[i][hashid[i]]);
        }

        return ans;
    }


    double query_ml(uint32_t key) {
        int hashid[hnum];
        for (int i = 0; i < hnum; i++) hashid[i] = hash[i]->run((char *)&key, 4) % m;

        vector<int> results;
        for (int i = 0; i < hnum; i++) {
            if(a[i][hashid[i]] < (1 << bit_list[i])-1) results.push_back(a[i][hashid[i]]);
            else results.push_back(999999999);
        }

        sort(results.begin(), results.end());
        int ans = 1e9;
        if(results[1] - results[0] > 100 && results[0] < 2000) {
            ans = min(results[0]/32,results[1]/32);
            ans = min(ans,results[2]/32);
        }else {
            ans = results[0];
        }
        return ans;
    }
};