struct TowerSketch {
    int m, hnum;
    int bit_list[4] = {8,16,31};
    int m_list[5] = {0};
    uint32_t **a;
    BOBHash32 **hash;
    int p_a,p_b,p_c;
    int results[4];
    TowerSketch(int _m, int _hnum,int p_a,int p_b,int p_c) : m(_m), hnum(_hnum),p_a(p_a), p_b(p_b), p_c(p_c)  {
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
                v.push_back(99999999);
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
        for (int i = 0; i < hnum; i++) hashid[i] = hash[i]->run((char *)&key, 4) % m_list[i];

        int minn = 1e9,maxn = -1;
        for (int i = 0; i < hnum; i++) {
            if(a[i][hashid[i]] < (1 << bit_list[i])-1) {
                results[i] = a[i][hashid[i]];
                if(a[i][hashid[i]] < minn) {
                    minn = a[i][hashid[i]];
                }
                if(a[i][hashid[i]] > maxn) {
                    maxn = a[i][hashid[i]];
                }
            }
            else results[i] = 9999999;
        }

        int ans = 1e9;
        if(maxn - minn > 3000 && minn < 2000) {
            ans = min(results[0]>>p_a,results[1]>>p_b);
            ans = min(ans,int(results[2]>>p_c));
        }else {
            ans = results[0];
        }
        return ans;
    }
};