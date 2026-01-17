struct EBucket {
    uint32_t f;
    int pvote;
    int nvote;
    bool flag;
};


struct ElasticSketch {
    BOBHash32 **hash,*hashx;

    /*
     * heavy part:
     */
    int l;
    int lamada;
    EBucket *H;

    /*
     * light part: CM
     */
    int d,m;
    CMSketch *cm;

    ElasticSketch(int _l, int _d, int _m) : l(_l), d(_d), m(_m) {
        lamada = 8;
        H = new EBucket[l];
        for (int i = 0; i < l; i++) {
            H[i].flag = false;
            H[i].pvote = 0;
            H[i].nvote = 0;
            H[i].f = 0;
        }
        cm = new CMSketch(m,d);

        hashx = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
        hash = new BOBHash32*[d];
        for (int i = 0; i < d; i++) hash[i] = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
    }


    vector<int> insert(uint32_t key) {
        vector<int> v;
        int pos = hashx->run((char *)&key, 4) % l;
        if(H[pos].f == 0) {
            H[pos].f = key;
            H[pos].flag = false;
            H[pos].pvote=1;
            H[pos].nvote=0;
            v.push_back(-1);
            v.push_back(-1);
            v.push_back(-1);
            v.push_back(H[pos].pvote);
        }else if(H[pos].f == key) {
            H[pos].pvote++;
            v.push_back(-1);
            v.push_back(-1);
            v.push_back(-1);
            if(H[pos].flag == 1) v.push_back(H[pos].pvote+cm->query(H[pos].f));
            else v.push_back(H[pos].pvote);
        }else {
            H[pos].nvote++;
            if(H[pos].nvote/H[pos].pvote < lamada) {
                v = cm->insert(key,1);
                v.push_back(-1);
            }else {
                v = cm->insert(H[pos].f,H[pos].pvote);
                H[pos].f = key;
                H[pos].flag = true;
                H[pos].pvote=1;
                H[pos].nvote=1;
                v.push_back(H[pos].pvote);
            }
        }

        return v;
    }

    double query(uint32_t key) {
        int ans;
        int pos = hashx->run((char *)&key, 4) % l;
        if(H[pos].f == key) {
            if(H[pos].flag == 0) ans = H[pos].pvote;
            else ans = H[pos].pvote+cm->query(H[pos].f);
        }else {
            ans = cm->query(H[pos].f);
        }

        return ans;
    }

    double query_ml(uint32_t key) {
        int ans;
        int pos = hashx->run((char *)&key, 4) % l;
        if(H[pos].f == key) {
            if(H[pos].flag == 0) ans = H[pos].pvote;
            else ans = H[pos].pvote+cm->query_ml(H[pos].f);
        }else {
            ans = cm->query_ml(H[pos].f);
        }

        return ans;
    }
};