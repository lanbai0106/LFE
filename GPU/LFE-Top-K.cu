#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <cuda_runtime.h>

using uint32 = uint32_t;

const uint32 TOP_K = 4000;
uint32 memory = 200;
const uint32 CM_D = 3;
const uint32 WIDTH = memory*1024*8/3/32;


#define CUDA_CHECK(x) do {                                      \
    cudaError_t err = x;                                        \
    if (err != cudaSuccess) {                                   \
        printf("CUDA Error: %s\n", cudaGetErrorString(err));    \
        exit(1);                                                \
    }                                                           \
} while (0)

__host__ __device__
inline void mix(uint32& a, uint32& b, uint32& c) {
    a -= b; a -= c; a ^= c >> 13;
    b -= c; b -= a; b ^= a << 8;
    c -= a; c -= b; c ^= b >> 13;
    a -= b; a -= c; a ^= c >> 12;
    b -= c; b -= a; b ^= a << 16;
    c -= a; c -= b; c ^= b >> 5;
    a -= b; a -= c; a ^= c >> 3;
    b -= c; b -= a; b ^= a << 10;
    c -= a; c -= b; c ^= b >> 15;
}

__device__
inline uint32 min2(uint32 a, uint32 b) {
    return a < b ? a : b;
}

__device__
inline uint32 max2(uint32 a, uint32 b) {
    return a > b ? a : b;
}

__host__ __device__
inline uint32 hash1(uint32 key) {
    return (key * 2654435761u) >> 15;
}

__host__ __device__
inline uint32 hash2(uint32 key) {
    uint32 a = 0x9e3779b9, b = 0x9e3779b9, c = 8311;
    uint32 len = 4;
    unsigned char* str = (unsigned char*)&key;

    c += len;

    switch (len) {
        case 4: a += ((uint32)str[3] << 24);
        case 3: a += ((uint32)str[2] << 16);
        case 2: a += ((uint32)str[1] << 8);
        case 1: a += str[0];
    }

    mix(a, b, c);
    return c;
}

__host__ __device__
inline uint32 hash3(uint32 key) {
    return hash2(key ^ 0x85ebca6bu);
}

struct HeapNode {
    uint32 id;
    uint32 cnt;
};

struct TrueNode {
    uint32 id;
    uint32 cnt;
};

__device__
inline uint32 gpuHash(uint32 x, int row, uint32 width) {
    if (row == 0) return hash1(x) % width;
    if (row == 1) return hash2(x) % width;
    return hash3(x) % width;
}

__device__
inline bool lessNode(const HeapNode& a, const HeapNode& b) {
    return a.cnt != b.cnt ? a.cnt < b.cnt : a.id < b.id;
}

__device__
inline void swapNode(HeapNode* heap, uint32* pos, uint32 i, uint32 j) {
    HeapNode tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;

    pos[heap[i].id] = i + 1;
    pos[heap[j].id] = j + 1;
}

__device__
void up(HeapNode* heap, uint32* pos, uint32 i) {
    while (i > 0) {
        uint32 p = (i - 1) / 2;

        if (!lessNode(heap[i], heap[p])) {
            break;
        }

        swapNode(heap, pos, i, p);
        i = p;
    }
}

__device__
void down(HeapNode* heap, uint32* pos, uint32 heapSize, uint32 i) {
    while (true) {
        uint32 l = i * 2 + 1;
        uint32 r = i * 2 + 2;
        uint32 s = i;

        if (l < heapSize && lessNode(heap[l], heap[s])) s = l;
        if (r < heapSize && lessNode(heap[r], heap[s])) s = r;
        if (s == i) break;

        swapNode(heap, pos, i, s);
        i = s;
    }
}

__device__
uint32 updateCM(uint32* cm, uint32 width, uint32 x) {
    uint32 p0 = 2, p1 = 2, p2 = 2;

    uint32 idx0 = gpuHash(x, 0, width);
    uint32 idx1 = gpuHash(x, 1, width);
    uint32 idx2 = gpuHash(x, 2, width);

    uint32 cm_es1 = cm[idx0]++;
    uint32 cm_es2 = cm[width + idx1]++;
    uint32 minn = min2(cm_es1, cm_es2);
    uint32 maxn = max2(cm_es1, cm_es2);
    uint32 cm_es3 = cm[width*2 + idx2]++;

    minn = min2(minn, cm_es3);
    maxn = max2(maxn, cm_es3);

    if (maxn - minn > 3000 && minn < 3000) {

        uint32 est0 = cm[idx0] >> p0;
        uint32 est1 = cm[width + idx1] >> p1;
        uint32 est2 = cm[2 * width + idx2] >> p2;

        return min2(min2(est0, est1), est2);
    } else {
        return minn;
    }
}

__device__
void insertOne(
    uint32 x,
    uint32* cm,
    HeapNode* heap,
    uint32* pos,
    uint32* heapSize,
    uint32 width,
    uint32 k
) {
    uint32 est = updateCM(cm, width, x);

    uint32 p = pos[x];

    if (p != 0) {
        uint32 idx = p - 1;
        heap[idx].cnt++;
        down(heap, pos, *heapSize, idx);
        return;
    }

    if (*heapSize < k) {
        uint32 idx = *heapSize;
        heap[idx].id = x;
        heap[idx].cnt = est;
        pos[x] = idx + 1;
        (*heapSize)++;
        up(heap, pos, idx);
        return;
    }

    if (est > heap[0].cnt) {
        pos[heap[0].id] = 0;

        heap[0].id = x;
        heap[0].cnt = est;
        pos[x] = 1;

        down(heap, pos, *heapSize, 0);
    }
}

__global__
void insertKernel(
    uint32* keys,
    uint32 len,
    uint32* cm,
    HeapNode* heap,
    uint32* pos,
    uint32* heapSize,
    uint32 width,
    uint32 k
) {
    if (blockIdx.x != 0 || threadIdx.x != 0) {
        return;
    }

    for (uint32 i = 0; i < len; i++) {
        insertOne(keys[i], cm, heap, pos, heapSize, width, k);
    }
}

std::vector<uint32> readFile(
    const char* filename,
    std::unordered_map<uint32, uint32>& trueCnt
) {
    std::ifstream fin(filename);

    if (!fin.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }

    std::vector<uint32> keys;
    std::unordered_map<std::string, uint32> labelToId;

    std::string flowLabel, otherField;
    uint32 nextId = 1;

    while (fin >> flowLabel >> otherField) {
        auto ret = labelToId.emplace(flowLabel, nextId);

        if (ret.second) {
            nextId++;
        }

        uint32 id = ret.first->second;

        keys.push_back(id);
        trueCnt[id]++;
    }

    return keys;
}

std::vector<TrueNode> getTrueTopK(
    const std::unordered_map<uint32, uint32>& trueCnt
) {
    std::vector<TrueNode> res;

    for (auto& p : trueCnt) {
        res.push_back({p.first, p.second});
    }

    std::sort(res.begin(), res.end(), [](const TrueNode& a, const TrueNode& b) {
        return a.cnt != b.cnt ? a.cnt > b.cnt : a.id < b.id;
    });

    if (res.size() > TOP_K) {
        res.resize(TOP_K);
    }

    return res;
}

void evaluate(
    const std::vector<TrueNode>& trueTopK,
    const std::vector<HeapNode>& heap,
    const std::vector<uint32>& pos
) {
    double aae = 0.0;
    double are = 0.0;

    for (auto& item : trueTopK) {
        uint32 real = item.cnt;
        uint32 est = 0;

        uint32 p = pos[item.id];

        if (p != 0) {
            est = heap[p - 1].cnt;
        }

        double err = std::fabs((double)est - (double)real);

        aae += err;
        are += err / (double)real;
    }

    aae /= trueTopK.size();
    are /= trueTopK.size();

    printf("AAE: %.6lf\n", aae);
    printf("ARE: %.6lf\n", are);
}

int main() {
    const char* filename = "../data/CAIDA19.txt";

    std::unordered_map<uint32, uint32> trueCnt;
    std::vector<uint32> keys = readFile(filename, trueCnt);
    std::vector<TrueNode> trueTopK = getTrueTopK(trueCnt);

    uint32 len = (uint32)keys.size();
    uint32 flowNum = (uint32)trueCnt.size();

    uint32* d_keys;
    uint32* d_cm;
    HeapNode* d_heap;
    uint32* d_pos;
    uint32* d_heapSize;

    CUDA_CHECK(cudaMalloc(&d_keys, len * sizeof(uint32)));
    CUDA_CHECK(cudaMalloc(&d_cm, CM_D * WIDTH * sizeof(uint32)));
    CUDA_CHECK(cudaMalloc(&d_heap, TOP_K * sizeof(HeapNode)));
    CUDA_CHECK(cudaMalloc(&d_pos, (flowNum + 1) * sizeof(uint32)));
    CUDA_CHECK(cudaMalloc(&d_heapSize, sizeof(uint32)));

    CUDA_CHECK(cudaMemcpy(d_keys, keys.data(), len * sizeof(uint32), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemset(d_cm, 0, CM_D * WIDTH * sizeof(uint32)));
    CUDA_CHECK(cudaMemset(d_pos, 0, (flowNum + 1) * sizeof(uint32)));
    CUDA_CHECK(cudaMemset(d_heapSize, 0, sizeof(uint32)));

    insertKernel<<<1, 1>>>(
        d_keys,
        len,
        d_cm,
        d_heap,
        d_pos,
        d_heapSize,
        WIDTH,
        TOP_K
    );

    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    std::vector<HeapNode> heap(TOP_K);
    std::vector<uint32> pos(flowNum + 1);

    CUDA_CHECK(cudaMemcpy(heap.data(), d_heap, TOP_K * sizeof(HeapNode), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaMemcpy(pos.data(), d_pos, (flowNum + 1) * sizeof(uint32), cudaMemcpyDeviceToHost));

    evaluate(trueTopK, heap, pos);

    cudaFree(d_keys);
    cudaFree(d_cm);
    cudaFree(d_heap);
    cudaFree(d_pos);
    cudaFree(d_heapSize);

    return 0;
}
