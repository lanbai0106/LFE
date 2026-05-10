struct CMSketch {
    int m, hnum;
    uint32_t **a;
    BOBHash32 **hash;
    double thre_1,thre_2;
    int p_a,p_b,p_c;
    int results[4];
    CMSketch(int _m, int _hnum, int _thre_1, int _thre_2,int p_a,int p_b,int p_c) : m(_m), hnum(_hnum), thre_1(_thre_1), thre_2(_thre_2),p_a(p_a), p_b(p_b), p_c(p_c) {

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
            if(a[i][hashid[i]] < (1 << 16)-1) {
                a[i][hashid[i]] += cnt ;
            }
            v.push_back(a[i][hashid[i]]);
        }
        return v;
    }

    double query(uint32_t key) {
        int hashid[hnum];
        for (int i = 0; i < hnum; i++) hashid[i] = hash[i]->run((char *)&key, 4) % m;

        uint32_t ans = 1e9,maxn = -1;
        for (int i = 0; i < hnum; i++) {
            ans = min(ans,a[i][hashid[i]]);
        }
        return ans;
    }

    double query_ml(uint32_t key) {
        int hashid[hnum];
        for (int i = 0; i < hnum; i++) hashid[i] = hash[i]->run((char *)&key, 4) % m;


        int minn = 1e9,maxn = -1;
        for (int i = 0; i < hnum; i++) {
            results[i] = a[i][hashid[i]];
            if(a[i][hashid[i]] < minn) {
                minn = a[i][hashid[i]];
            }
            if(a[i][hashid[i]] > maxn) {
                maxn = a[i][hashid[i]];
            }
        }

        int ans = 1e9;
        if(maxn - minn > thre_1 && minn < thre_2) {
            ans = min(results[0]>>p_a,results[1]>>p_b);
            ans = min(ans,int(results[2]>>p_c));
        }else {
            ans = minn;
        }
        return ans;
    }
};
