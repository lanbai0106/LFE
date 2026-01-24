struct PBucket {
    uint32_t f;
    int cnt;
};


struct PRECISION {
    BOBHash32 **hash,*hashx;

    int min_stage, new_val;

    /*
     * heavy part:
     */
    int l,k;
    PBucket **H;

    /*
     * light part: CM
     */
    int d,m;
    CMSketch *cm;
    double p_a,p_b,p_c;
    PRECISION(int _l, int _k, int _d, int _m,double p_a,double p_b,double p_c) : l(_l), k(_k), d(_d) ,m(_m),p_a(p_a), p_b(p_b), p_c(p_c)  {
        min_stage = -1;
        H = new PBucket*[l];
        for (int i = 0; i < l; i++) {
            H[i] = new PBucket[k];
            for (int j = 0; j < k; j++) {
                H[i][j].f = 0;
                H[i][j].cnt = 0;
            }
        }
        cm = new CMSketch(m,d,1000,2000,p_a,p_b,p_c);

        hashx = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
        hash = new BOBHash32*[d];
        for (int i = 0; i < d; i++) hash[i] = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
    }


    vector<int> insert(uint32_t key) {
        vector<int> v;
        int pos = hashx->run((char *)&key, 4) % l;
        if(min_stage!=-1) {
            v = cm->insert(H[pos][min_stage].f,H[pos][min_stage].cnt);
            H[pos][min_stage].f = key;
            H[pos][min_stage].cnt = new_val;
            v.push_back(H[pos][min_stage].cnt);
            min_stage = -1;
        }
        bool flag = false;
        int minn = 1e9;
        for (int i = 0; i < k; i++) {
            if(H[pos][i].f == 0) {
                flag = true;
                H[pos][i].f = key;
                H[pos][i].cnt = 1;
                v.push_back(-1);
                v.push_back(-1);
                v.push_back(-1);
                v.push_back(H[pos][i].cnt);
            }else if(H[pos][i].f == key) {
                H[pos][i].cnt += 1;
                flag = true;
                v.push_back(-1);
                v.push_back(-1);
                v.push_back(-1);
                v.push_back(H[pos][i].cnt);
            }else {
                if(H[pos][i].cnt < minn) {
                    minn = H[pos][i].cnt;
                    min_stage = i;
                }
            }
        }
        if(!flag) {
            double log2_value = std::log2(minn);  // 计算 log2(v)
            int rounded_up = std::ceil(log2_value);
            new_val = 1 << rounded_up;
            if(rd() % new_val == 0) {
                return insert(key);
            }else {
                v = cm->insert(key,1);
                min_stage = -1;
                v.push_back(-1);
            }
        }else {
            min_stage = -1;
        }

        return v;
    }

    double query(uint32_t key) {
        int ans;
        int pos = hashx->run((char *)&key, 4) % l;
        bool flag = false;
        for (int i = 0; i < k; i++) {
            if(H[pos][i].f == key) {
                flag = true;
                ans = H[pos][i].cnt;
            }
        }
        if(!flag) ans = cm->query(key);
        return ans;
    }

    double query_ml(uint32_t key) {
        int ans;
        int pos = hashx->run((char *)&key, 4) % l;
        bool flag = false;
        for (int i = 0; i < k; i++) {
            if(H[pos][i].f == key) {
                flag = true;
                ans = H[pos][i].cnt;
            }
        }
        if(!flag) ans = cm->query_ml(key);
        return ans;

    }
};