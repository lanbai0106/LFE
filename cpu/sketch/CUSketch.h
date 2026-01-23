struct CUSketch {
    int m, hnum;
    uint32_t **a;
    BOBHash32 **hash;

    CUSketch(int _m, int _hnum) : m(_m), hnum(_hnum) {

        a = new uint32_t*[hnum];
        for (int i = 0; i < _hnum; i++) {

            a[i] = new uint32_t[m];
            for (int j = 0; j < m; j++) {
                a[i][j] = 0;
            }

        }

        hash = new BOBHash32*[hnum];
        for (int i = 0; i < _hnum; i++) hash[i] = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
    }


    vector<int> insert(uint32_t key,int cnt) {
        int hashid[hnum];
        for (int i = 0; i < hnum; i++) hashid[i] = hash[i]->run((char *)&key, 4) % m;
        uint32_t ans = 1e9,maxn = -1;
        vector<int> v;
        for (int i = 0; i < hnum; i++) {
            ans = min(ans, a[i][hashid[i]]);
        }

        for (int i = 0; i < hnum; i++) {
            if(a[i][hashid[i]] == ans  && (a[i][hashid[i]] < (1 << 16)-1)) {
                a[i][hashid[i]] += cnt ;
            }
            v.push_back(a[i][hashid[i]]);
        }

        return v;
    }

    double query(uint32_t key) {
        int hashid[hnum];
        for (int i = 0; i < hnum; i++) hashid[i] = hash[i]->run((char *)&key, 4) % m;

        uint32_t ans = 1e9;
        for (int i = 0; i < hnum; i++) {
            ans = min(ans,a[i][hashid[i]]);
        }

        return ans;
    }

    double query_ml(uint32_t key) {
        int hashid[hnum];
        for (int i = 0; i < hnum; i++) hashid[i] = hash[i]->run((char *)&key, 4) % m;

        vector<int> results;
        for (int i = 0; i < hnum; i++) {
            results.push_back(a[i][hashid[i]]);
        }

        sort(results.begin(), results.end());
        int ans = 1e9;
        if(results[1] - results[0] > 3000 && results[0] < 3000) {
            ans = min(results[0]/32,results[1]/32);
            ans = min(ans,results[2]/32);
        }else {
            ans = results[0];
        }
        return ans;
    }
};