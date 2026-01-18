import numpy as np
import pandas as pd
import hashlib
from collections import Counter
import matplotlib.pyplot as plt

rows = 3

import hashlib
import random


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

    def estimate_ml(self, item, a, b, c):
        param_list = [a, b, c]
        min_estimate = float('inf')
        max_estimate = float(-1)
        min_ml_est = float('inf')
        for i in range(self.num_rows):
            hash_value = int(self.hash_functions[i](item), 16) % self.num_cols
            min_estimate = min(min_estimate, int(self.table[i][hash_value]))
            max_estimate = max(max_estimate, int(self.table[i][hash_value]))
            min_ml_est = min(min_ml_est, int(self.table[i][hash_value]) / param_list[i])
        if max_estimate - min_estimate > 500 and min_estimate < 200:
            return min_ml_est
        return min_estimate

    def get_counters(self, item):
        v_list = []
        for i in range(self.num_rows):
            hash_value = int(self.hash_functions[i](item), 16) % self.num_cols
            v_list.append(int(self.table[i][hash_value]))
        return v_list

    def __str__(self):
        return f"Count-Min Sketch with {self.num_rows} rows and {self.num_cols} columns"


second_pac_num = 30000000 // 60
pac_total = []
sample_pac_5_total = []
sample_pac_10_total = []
sample_pac_50_total = []
keys_5_total = []
keys_10_total = []
keys_50_total = []
keys_total = []
cnt = 0
with open("../cpu/data/stackoverflow.txt", "r", encoding="utf-8") as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) >= 2:
            if cnt % second_pac_num == 0:
                pac_total.append([])
                sample_pac_5_total.append([])
                sample_pac_10_total.append([])
                sample_pac_50_total.append([])
                keys_5_total.append(set())
                keys_10_total.append(set())
                keys_50_total.append(set())
                keys_total.append(set())
            pac_total[cnt // second_pac_num].append(parts[0])
            keys_total[cnt // second_pac_num].add(parts[0])

            if cnt % 5 == 0:
                sample_pac_5_total[cnt // second_pac_num].append(parts[0])
                keys_5_total[cnt // second_pac_num].add(parts[0])
            if cnt % 10 == 0:
                sample_pac_10_total[cnt // second_pac_num].append(parts[0])
                keys_10_total[cnt // second_pac_num].add(parts[0])
            if cnt % 50 == 0:
                sample_pac_50_total[cnt // second_pac_num].append(parts[0])
                keys_50_total[cnt // second_pac_num].add(parts[0])
        cnt += 1
        if cnt == 3e7:
            break
keys_5_total = list(keys_5_total)
keys_10_total = list(keys_10_total)
keys_50_total = list(keys_50_total)
keys_total = list(keys_total)

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
                    # if yi > y_hat:
                    #     total_error = float('inf')
                    # else:
                    #     total_error += (y_hat - yi)/yi
                    total_error += (y_hat - yi) / yi

                if total_error < best_error:
                    best_error = total_error
                    best_a, best_b, best_c = a, b, c
    return best_a, best_b, best_c

def calculate_aae(true, estimate):
    return np.mean(np.abs(np.array(true) - np.array(estimate)))

def calculate_are(true, estimate):
    return np.mean(np.abs(np.array(true) - np.array(estimate)) / np.array(true))

plt.rcParams['pdf.fonttype'] = 42
plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.size'] = 20

aae_list = []
are_list = []
aae_list_ml = []
are_list_ml = []
aae_list_ml_once = []
are_list_ml_once = []

total_memory = 10
total_memory *= 1024 * 8
cm_cols = int(total_memory / rows / 16)
ratio_list = [0.2, 0.1, 0.02]
cm_ml_cols_lsit = []
for ratio in ratio_list:
    cm_ml_cols_lsit.append(int(cm_cols * ratio))

for i in range(60):

    pac = pac_total[i]
    sample_pac_5 = sample_pac_5_total[i]
    keys_5 = keys_5_total[i]
    keys = keys_total[i]
    real_freq = Counter(pac)
    real_freq_5 = Counter(sample_pac_5)

    print(len(pac), len(sample_pac_5), len(keys), len(keys_5))
    cm = CountMinSketch(rows, cm_cols)
    cm_ml_5 = CountMinSketch(num_rows=rows, num_cols=cm_ml_cols_lsit[0])

    for i, p in enumerate(pac):
        if i % 5 == 0:
            cm_ml_5.update(p)
        cm.update(p)

    threshold = 500

    X_5 = []
    y_5 = []

    for item in keys_5:
        X = cm_ml_5.get_counters(item)
        minn = min(X)
        maxx = max(X)
        if maxx - minn > threshold and minn < 200:
            X_5.append(X)
            y_5.append(real_freq_5[item])

    powers = [2 ** k for k in range(0, 6)]

    best_a_5 = None
    best_b_5 = None
    best_c_5 = None

    best_a_5, best_b_5, best_c_5 = get_best_params(X_5, y_5)
    print(best_a_5, best_b_5, best_c_5)

    true_frequency = []
    cm_frequency = []
    cm_ml_5_frequency = []
    cm_ml_once_frequency = []
    for item in keys:
        true_frequency.append(real_freq[item])
        cm_frequency.append(cm.estimate(item))
        cm_ml_5_frequency.append(cm.estimate_ml(item, best_a_5, best_b_5, best_c_5))
        cm_ml_once_frequency.append(cm.estimate_ml(item, 32, 32, 32))

    aae_cm = calculate_aae(true_frequency, cm_frequency)
    aae_cm_ml_5 = calculate_aae(true_frequency, cm_ml_5_frequency)
    aae_cm_ml_once = calculate_aae(true_frequency, cm_ml_once_frequency)
    are_cm = calculate_are(true_frequency, cm_frequency)
    are_cm_ml_5 = calculate_are(true_frequency, cm_ml_5_frequency)
    are_cm_ml_once = calculate_are(true_frequency, cm_ml_once_frequency)

    aae_list.append(aae_cm)
    are_list.append(are_cm)
    aae_list_ml.append(aae_cm_ml_5)
    are_list_ml.append(are_cm_ml_5)
    aae_list_ml_once.append(aae_cm_ml_once)
    are_list_ml_once.append(are_cm_ml_once)

    # combined = list(zip(true_frequency, cm_ml_5_frequency,cm_frequency))
    # random.shuffle(combined)
    # true_frequency, cm_ml_5_frequency,cm_frequency = zip(*combined)
    # actual = true_frequency[:10000]
    # estimated = cm_ml_5_frequency[:10000]
    #
    # plt.figure(figsize=(4, 3))
    # plt.plot(actual, estimated, '+', color='k', markersize=4)
    #
    # x = np.linspace(0, int(np.max(actual)) )
    # plt.plot(x, x, 'b-', linewidth=2)
    #
    # plt.xlabel('CM Actual Frequency', fontsize=16)
    # plt.ylabel('CM Estimated Frequency (ML)', fontsize=16)
    # plt.tick_params(labelsize=20)
    #
    # plt.tight_layout()
    # # plt.savefig('images/'+path+'.png', dpi=300, bbox_inches='tight')
    # plt.show()
    #
    #
    # # combined = list(zip(true_frequency, cm_frequency))
    # # random.shuffle(combined)
    # # true_frequency, cm_frequency = zip(*combined)
    # actual = true_frequency[:10000]
    # estimated = cm_frequency[:10000]
    #
    # plt.figure(figsize=(4, 3))
    # plt.plot(actual, estimated, '+', color='k', markersize=4)
    #
    # x = np.linspace(0, int(np.max(actual)) )
    # plt.plot(x, x, 'b-', linewidth=2)
    #
    # plt.xlabel('CM Actual Frequency', fontsize=16)
    # plt.ylabel('CM Estimated Frequency', fontsize=16)
    # plt.tick_params(labelsize=20)
    #
    # plt.tight_layout()
    # # plt.savefig('images/'+path+'.png', dpi=300, bbox_inches='tight')
    # plt.show()
print("AAE for cm_frequency:", aae_list)
print("AAE for cm_ml_5_frequency:", aae_list_ml)
print("AAE for cm_ml_once_frequency:", aae_list_ml_once)
print("ARE for cm_frequency:", are_list)
print("ARE for cm_ml_5_frequency:", are_list_ml)
print("ARE for cm_ml_once_frequency:", are_list_ml_once)
