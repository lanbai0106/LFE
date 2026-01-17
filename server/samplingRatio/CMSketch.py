import numpy as np
import pandas as pd
import hashlib
from collections import Counter
import matplotlib.pyplot as plt
# from ..common import pac,sample_pac_5,sample_pac_10,sample_pac_50,keys_5,keys_10,keys_50,keys,real_freq,real_freq_5,real_freq_10,real_freq_50,rows,get_best_params,powers,calculate_aae,calculate_are
# # import ..common.py
# from ..sketch.CMSketch import CountMinSketch
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
with open("../../cpu/data/stackoverflow.txt", "r", encoding="utf-8") as f:
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


powers = [2**k for k in range(0, 6)]


def get_best_params(X, y):
    best_a, best_b, best_c = None, None, None
    best_error = float('inf')
    for a in powers:
        for b in powers:
            for c in powers:
                total_error = 0.0

                for (x1, x2, x3), yi in zip(X, y):
                    y_hat = min(x1 / a, x2 / b, x3 / c)
                    total_error += abs(yi - y_hat)

                if total_error < best_error:
                    best_error = total_error
                    best_a, best_b, best_c = a, b, c
    return best_a, best_b, best_c

def calculate_aae(true, estimate):
    return np.mean(np.abs(np.array(true) - np.array(estimate)))

def calculate_are(true, estimate):
    return np.mean(np.abs(np.array(true) - np.array(estimate)) / np.array(true))


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
        if max_estimate - min_estimate > 100 and min_estimate < 2000:
            return min_ml_est
        return min_estimate

    def get_counters(self, item):
        v_list = []
        for i in range(self.num_rows):
            hash_value = int(self.hash_functions[i](item), 16) % self.num_cols
            v_list.append(int(self.table[i][hash_value]))
        return v_list




total_memory = 300
total_memory *= 1024 * 8
cm_cols = int(total_memory / rows / 16)
ratio_list = [0.2, 0.1, 0.02]
cm_ml_cols_lsit = []
for ratio in ratio_list:
    cm_ml_cols_lsit.append(int(cm_cols * ratio))
cm = CountMinSketch(rows, cm_cols)
cm_ml_5 = CountMinSketch(num_rows=rows, num_cols=cm_ml_cols_lsit[0])
cm_ml_10 = CountMinSketch(num_rows=rows, num_cols=cm_ml_cols_lsit[1])
cm_ml_50 = CountMinSketch(num_rows=rows, num_cols=cm_ml_cols_lsit[2])

for i, p in enumerate(pac):
    if i % 5 == 0:
        cm_ml_5.update(p)
    if i % 10 == 0:
        cm_ml_10.update(p)
    if i % 50 == 0:
        cm_ml_50.update(p)
    cm.update(p)

threshold = 100

X_5 = []
y_5 = []

X_10 = []
y_10 = []

X_50 = []
y_50 = []
for item in keys_5:
    X = cm_ml_5.get_counters(item)
    X_5.append(X)
    y_5.append(real_freq_5[item])

for item in keys_10:
    X = cm_ml_10.get_counters(item)
    X_10.append(X)
    y_10.append(real_freq_10[item])

for item in keys_50:
    X = cm_ml_50.get_counters(item)
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

for item in keys:
    true_frequency.append(real_freq[item])
    cm_frequency.append(cm.estimate(item))
    cm_ml_5_frequency.append(cm.estimate_ml(item,best_a_5,best_b_5,best_c_5))
    cm_ml_10_frequency.append(cm.estimate_ml(item,best_a_10,best_b_10,best_c_10))
    cm_ml_50_frequency.append(cm.estimate_ml(item,best_a_50,best_b_50,best_c_50))


aae_cm = calculate_aae(true_frequency, cm_frequency)
aae_cm_ml_5 = calculate_aae(true_frequency, cm_ml_5_frequency)
aae_cm_ml_10 = calculate_aae(true_frequency, cm_ml_10_frequency)
aae_cm_ml_50 = calculate_aae(true_frequency, cm_ml_50_frequency)
are_cm = calculate_are(true_frequency, cm_frequency)
are_cm_ml_5 = calculate_are(true_frequency, cm_ml_5_frequency)
are_cm_ml_10 = calculate_are(true_frequency, cm_ml_10_frequency)
are_cm_ml_50 = calculate_are(true_frequency, cm_ml_50_frequency)

print("AAE for cm_frequency:", aae_cm)
print("AAE for cm_ml_5_frequency:", aae_cm_ml_5)
print("AAE for cm_ml_10_frequency:", aae_cm_ml_10)
print("AAE for cm_ml_50_frequency:", aae_cm_ml_50)

print("ARE for cm_frequency:", are_cm)
print("ARE for cm_ml_5_frequency:", are_cm_ml_5)
print("ARE for cm_ml_10_frequency:", are_cm_ml_10)
print("ARE for cm_ml_50_frequency:", are_cm_ml_50)

plt.plot(true_frequency[:1000], linewidth=6, label="True Frequency")
# plt.plot(cm_frequency[:1000], linewidth= 2, label="CM Frequency")
plt.plot(cm_ml_5_frequency[:1000], linewidth=2, label="CM ML 5 Frequency")
plt.legend()
plt.show()