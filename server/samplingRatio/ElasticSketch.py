import numpy as np
import pandas as pd
import hashlib
from collections import Counter
import matplotlib.pyplot as plt
# from ..common import pac,sample_pac_5,sample_pac_10,sample_pac_50,keys_5,keys_10,keys_50,keys,real_freq,real_freq_5,real_freq_10,real_freq_50,rows,get_best_params,powers,calculate_aae,calculate_are

import hashlib
import random

from collections import Counter
import numpy as np
pac = []
sample_pac_5 = []
sample_pac_10 = []
sample_pac_50 = []
keys_5 = set()
keys_10 = set()
keys_50 = set()
keys = set()
cnt = 0
with open("../../cpu/data/mawi.txt", "r", encoding="utf-8") as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) >= 2:
            pac.append(parts[0])
            keys.add(parts[0])
            cnt+=1
            keys.add(parts[0])
            if cnt % 5 == 0:
                sample_pac_5.append(parts[0])
                keys_5.add(parts[0])
            if cnt % 10 == 0:
                sample_pac_10.append(parts[0])
                keys_10.add(parts[0])
            if cnt % 50 == 0:
                sample_pac_50.append(parts[0])
                keys_50.add(parts[0])
        if cnt == 3e7:
            break
keys_5 = list(keys_5)
keys_10 = list(keys_10)
keys_50 = list(keys_50)
keys = list(keys)

real_freq = Counter(pac)
real_freq_5 = Counter(sample_pac_5)
real_freq_10 = Counter(sample_pac_10)
real_freq_50 = Counter(sample_pac_50)

rows = 3


powers = [2**k for k in range(0, 5)]


def get_best_params(X, y):
    best_a, best_b, best_c = None, None, None
    best_error = float('inf')
    for a in powers:
        for b in powers:
            for c in powers:
                total_error = 0.0

                sum1 = 0
                for (x1, x2, x3), yi in zip(X, y):
                    y_hat = min(x1 / a, x2 / b, x3 / c)
                    if yi > y_hat:
                        sum1 += 1
                    else:
                        total_error += (y_hat - yi) / yi
                    # total_error += abs(yi - y_hat)/yi
                if sum1 > 1000:
                    total_error = float('inf')

                if total_error < best_error:
                    best_error = total_error
                    best_a, best_b, best_c = a, b, c
    return best_a, best_b, best_c

def calculate_aae(true, estimate):
    return np.mean(np.abs(np.array(true) - np.array(estimate)))

def calculate_are(true, estimate):
    return np.mean(np.abs(np.array(true) - np.array(estimate)) / np.array(true))


threshold = 1000
threshold1 = 1000
total_memory = 200
class CountMinSketch:
    def __init__(self, num_rows, num_cols):

        self.num_rows = num_rows
        self.num_cols = num_cols

        self.table = [[0] * num_cols for _ in range(num_rows)]

        self.hash_functions = [self._create_hash_function(i) for i in range(num_rows)]

    def _create_hash_function(self, row):

        seed = random.randint(0, 2 ** 32)

        def hash_function(x):
            return (hashlib.md5((str(seed) + str(x)).encode()).hexdigest())

        return hash_function


    def update(self, item, count=1):

        for i in range(self.num_rows):
            hash_value = int(self.hash_functions[i](item), 16) % self.num_cols
            self.table[i][hash_value] += count

    def estimate(self, item):

        min_estimate = float('inf')
        for i in range(self.num_rows):
            hash_value = int(self.hash_functions[i](item), 16) % self.num_cols
            min_estimate = min(min_estimate, self.table[i][hash_value])
        return min_estimate



    def estimate_ml(self, item,a,b,c):
        param_list = [a,b,c]
        min_estimate = float('inf')
        max_estimate = float(-1)
        min_ml_est = float('inf')
        for i in range(self.num_rows):
            hash_value = int(self.hash_functions[i](item), 16) % self.num_cols
            min_estimate = min(min_estimate, int(self.table[i][hash_value]))
            max_estimate = max(max_estimate, int(self.table[i][hash_value]))
            min_ml_est = min(min_ml_est,int(self.table[i][hash_value])/param_list[i])
        if max_estimate - min_estimate > threshold and min_estimate < threshold1:
            return min_ml_est,1
        return min_estimate,0

    def get_counters(self, item):
        v_list = []
        for i in range(self.num_rows):
            hash_value = int(self.hash_functions[i](item), 16) % self.num_cols
            v_list.append(int(self.table[i][hash_value]))
        return v_list

class EBucket:
    def __init__(self):
        self.f = 0
        self.pvote = 0
        self.nvote = 0
        self.flag = False
MAX_PRIME32 = 2**32 - 1
class BOBHash32:
    def __init__(self, seed):
        self.seed = seed

    def run(self, data):

        data_bytes = data.encode('utf-8')
        hash_obj = hashlib.md5(data_bytes)
        hash_obj.update(str(self.seed).encode('utf-8'))
        return int(hash_obj.hexdigest(), 16) % MAX_PRIME32
class ElasticSketch:
    def __init__(self, l, d, m):
        self.l = l
        self.d = d
        self.m = m
        self.lamada = 8

        self.H = [EBucket() for _ in range(self.l)]
        self.cm = CountMinSketch(d, m)


        self.hashx = BOBHash32(random.randint(0, MAX_PRIME32))
        self.hash = [BOBHash32(random.randint(0, MAX_PRIME32)) for _ in range(d)]


    def update(self, key, count=1):
        pos = self.hashx.run(key) % self.l

        if self.H[pos].f == 0:
            self.H[pos].f = key
            self.H[pos].flag = False
            self.H[pos].pvote = 1
            self.H[pos].nvote = 0
        elif self.H[pos].f == key:
            self.H[pos].pvote += 1
        else:
            self.H[pos].nvote += 1
            if self.H[pos].nvote / self.H[pos].pvote < self.lamada:
                self.cm.update(key, 1)
            else:
                self.cm.update(self.H[pos].f, self.H[pos].pvote)
                self.H[pos].f = key
                self.H[pos].flag = True
                self.H[pos].pvote = 1
                self.H[pos].nvote = 1

    def estimate(self, key):

        pos = self.hashx.run(key) % self.l
        if self.H[pos].f == key:
            if not self.H[pos].flag:
                return self.H[pos].pvote
            else:
                return self.H[pos].pvote + self.cm.estimate(self.H[pos].f)
        else:
            return self.cm.estimate(self.H[pos].f)

    def ifHeavy(self, key):
        pos = self.hashx.run(key) % self.l
        if self.H[pos].f == key:
            return 1
        else:
            return 0
    def estimate_ml(self, key,a,b,c):
        pos = self.hashx.run(key) % self.l
        if self.H[pos].f == key:
            if not self.H[pos].flag:
                return self.H[pos].pvote, 0
            else:
                res, flag = self.cm.estimate_ml(self.H[pos].f,a,b,c)
                return self.H[pos].pvote + res, flag
        else:
            return self.cm.estimate_ml(self.H[pos].f,a,b,c)


total_memory *= 1024*8
heavy_ratio = 0.3
heavy_mem = int(total_memory * heavy_ratio)
light_mem = total_memory - heavy_mem

l = heavy_mem // (32+1+32+32)
cm_cols = int(light_mem/rows/32)
ratio_list = [0.2,0.1,0.02]
cm_ml_cols_lsit = []
for ratio in ratio_list:
    cm_ml_cols_lsit.append(int(cm_cols*ratio))
cm = ElasticSketch(l,rows, cm_cols)
cm_ml_5 = ElasticSketch(l,rows, cm_ml_cols_lsit[0])
cm_ml_10 = ElasticSketch(l,rows, cm_ml_cols_lsit[1])
cm_ml_50 = ElasticSketch(l,rows, cm_ml_cols_lsit[2])

for i, p in enumerate(pac):
    if i % 5 == 0:
        cm_ml_5.update(p)
    if i % 10 == 0:
        cm_ml_10.update(p)
    if i % 50 == 0:
        cm_ml_50.update(p)
    cm.update(p)



X_5 = []
y_5 = []

X_10 = []
y_10 = []

X_50 = []
y_50 = []
for item in keys_5:
    if cm_ml_5.ifHeavy(item):
        continue
    X = cm_ml_5.cm.get_counters(item)
    minn = min(X)
    maxx = max(X)
    if maxx - minn > threshold and minn < threshold1:
        X_5.append(X)
        y_5.append(real_freq_5[item])

for item in keys_10:
    if cm_ml_10.ifHeavy(item):
        continue
    X = cm_ml_10.cm.get_counters(item)
    minn = min(X)
    maxx = max(X)
    if maxx - minn > threshold and minn < threshold1:
        X_10.append(X)
        y_10.append(real_freq_10[item])

for item in keys_50:
    if cm_ml_50.ifHeavy(item):
        continue
    X = cm_ml_50.cm.get_counters(item)
    minn = min(X)
    maxx = max(X)
    if maxx - minn > threshold and minn < threshold1:
        X_50.append(X)
        y_50.append(real_freq_50[item])

best_a_5 = None
best_b_5 = None
best_c_5 = None

best_a_10 = None
best_b_10 = None
best_c_10 = None

best_a_50 = None
best_b_50 = None
best_c_50 = None

best_a_5,best_b_5,best_c_5 = get_best_params(X_5,y_5)
best_a_10,best_b_10,best_c_10 = get_best_params(X_10,y_10)
best_a_50,best_b_50,best_c_50 = get_best_params(X_50,y_50)
print(best_a_5,best_b_5,best_c_5)
print(best_a_10,best_b_10,best_c_10)
print(best_a_50,best_b_50,best_c_50)

true_frequency = []
cm_frequency = []
cm_ml_5_frequency = []
cm_ml_10_frequency = []
cm_ml_50_frequency = []

cnt5_1,cnt5_2,cnt10_1,cnt10_2,cnt50_1,cnt50_2 = 0,0,0,0,0,0
for item in keys:
    true_frequency.append(real_freq[item])
    res_cm = cm.estimate(item)
    cm_frequency.append(res_cm)
    res,flag = cm.estimate_ml(item,best_a_5,best_b_5,best_c_5)
    cm_ml_5_frequency.append(res)
    if flag == 1 and res_cm >= real_freq[item] and res != cm_frequency[-1]:
        if res >= real_freq[item]:
            cnt5_1 += 1
        else:
            cnt5_2 += 1
    res,flag = cm.estimate_ml(item,best_a_10,best_b_10,best_c_10)
    cm_ml_10_frequency.append(res)
    if flag == 1 and res_cm >= real_freq[item] and res != cm_frequency[-1]:
        if res >= real_freq[item]:
            cnt10_1 += 1
        else:
            cnt10_2 += 1
    res,flag = cm.estimate_ml(item,best_a_50,best_b_50,best_c_50)
    cm_ml_50_frequency.append(res)
    if flag == 1 and res_cm >= real_freq[item] and res != cm_frequency[-1]:
        if res >= real_freq[item]:
            cnt50_1 += 1
        else:
            cnt50_2 += 1



aae_cm = calculate_aae(true_frequency, cm_frequency)
aae_cm_ml_5 = calculate_aae(true_frequency, cm_ml_5_frequency)
aae_cm_ml_10 = calculate_aae(true_frequency, cm_ml_10_frequency)
aae_cm_ml_50 = calculate_aae(true_frequency, cm_ml_50_frequency)
are_cm = calculate_are(true_frequency, cm_frequency)
are_cm_ml_5 = calculate_are(true_frequency, cm_ml_5_frequency)
are_cm_ml_10 = calculate_are(true_frequency, cm_ml_10_frequency)
are_cm_ml_50 = calculate_are(true_frequency, cm_ml_50_frequency)

print("AAE for cm_frequency:", aae_cm)
print("AAE for cm_ml_5_frequency:", aae_cm_ml_5, (aae_cm - aae_cm_ml_5)/aae_cm)
print("AAE for cm_ml_10_frequency:", aae_cm_ml_10,(aae_cm - aae_cm_ml_10)/aae_cm)
print("AAE for cm_ml_50_frequency:", aae_cm_ml_50,(aae_cm - aae_cm_ml_50)/aae_cm)

print("ARE for cm_frequency:", are_cm)
print("ARE for cm_ml_5_frequency:", are_cm_ml_5,(are_cm - are_cm_ml_5)/are_cm)
print("ARE for cm_ml_10_frequency:", are_cm_ml_10,(are_cm - are_cm_ml_10)/are_cm)
print("ARE for cm_ml_50_frequency:", are_cm_ml_50,(are_cm - are_cm_ml_50)/are_cm)

print("cm_ml_5_true:", cnt5_1)
print("for cm_ml_10_true:", cnt10_1)
print("for cm_ml_50_true:", cnt50_1)

print("cm_ml_5_false:", cnt5_2)
print("for cm_ml_10_false:", cnt10_2)
print("for cm_ml_50_false:", cnt50_2)
# plt.plot(true_frequency[:1000], linewidth=6, label="True Frequency")
# # plt.plot(cm_frequency[:1000], linewidth= 2, label="CM Frequency")
# plt.plot(cm_ml_5_frequency[:1000], linewidth=2, label="CM ML 5 Frequency")
# plt.legend()
# plt.show()

plt.rcParams['pdf.fonttype'] = 42
plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.size'] = 20

combined = list(zip(true_frequency, cm_ml_5_frequency,cm_frequency))
random.shuffle(combined)
true_frequency, cm_ml_5_frequency,cm_frequency = zip(*combined)
actual = true_frequency[:10000]
estimated = cm_ml_5_frequency[:10000]

plt.figure(figsize=(4, 3))
plt.plot(actual, estimated, '+', color='k', markersize=4)

x = np.linspace(0, int(np.max(actual)) )
plt.plot(x, x, 'b-', linewidth=2)

plt.xlabel('ES Actual Frequency', fontsize=16)
plt.ylabel('ES Estimated Frequency (ML)', fontsize=16)
plt.tick_params(labelsize=20)

plt.tight_layout()
# plt.savefig('images/'+path+'.png', dpi=300, bbox_inches='tight')
plt.show()


# combined = list(zip(true_frequency, cm_frequency))
# random.shuffle(combined)
# true_frequency, cm_frequency = zip(*combined)
actual = true_frequency[:10000]
estimated = cm_frequency[:10000]

plt.figure(figsize=(4, 3))
plt.plot(actual, estimated, '+', color='k', markersize=4)

x = np.linspace(0, int(np.max(actual)) )
plt.plot(x, x, 'b-', linewidth=2)

plt.xlabel('ES Actual Frequency', fontsize=16)
plt.ylabel('ES Estimated Frequency', fontsize=16)
plt.tick_params(labelsize=20)

plt.tight_layout()
# plt.savefig('images/'+path+'.png', dpi=300, bbox_inches='tight')
plt.show()