import numpy as np
import pandas as pd
import hashlib
from collections import Counter
import matplotlib.pyplot as plt

rows = 3
import hashlib
import random

threshold = 500
threshold1 = 500
class CountMinSketch:
    def __init__(self, num_rows, num_cols):

        self.num_rows = num_rows
        self.num_cols = num_cols

        self.table = [[0] * num_cols for _ in range(num_rows)]

        self.hash_functions = [self._create_hash_function(i) for i in range(num_rows)]

    def _create_hash_function(self, row):

        seed = random.randint(0, 2**32)
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
        if max_estimate - min_estimate > threshold and min_estimate <threshold1:
            return min_ml_est,1
        return min_estimate,0

    def get_counters(self,item):
        v_list = []
        for i in range(self.num_rows):
            hash_value = int(self.hash_functions[i](item), 16) % self.num_cols
            v_list.append(int(self.table[i][hash_value]))
        return  v_list

    def __str__(self):
        return f"Count-Min Sketch with {self.num_rows} rows and {self.num_cols} columns"
second_pac_num = 30000000//60
pac_total = []
sample_pac_5_total = []
sample_pac_10_total  = []
sample_pac_50_total  = []
keys_5_total  = []
keys_10_total  = []
keys_50_total  = []
keys_total  = []
cnt = 0
with open("../data/CAIDA19.txt", "r", encoding="utf-8") as f:
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
        cnt+=1
        if cnt == 3e7:
            break
keys_5_total  = list(keys_5_total )
keys_10_total  = list(keys_10_total )
keys_50_total  = list(keys_50_total )
keys_total  = list(keys_total )
def calculate_aae(true, estimate):
    return np.mean(np.abs(np.array(true) - np.array(estimate)))

def calculate_are(true, estimate):
    return np.mean(np.abs(np.array(true) - np.array(estimate)) / np.array(true))
plt.rcParams['pdf.fonttype'] = 42
plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.size'] = 20
def get_best_params(X,y):
    best_a,best_b,best_c = None,None,None
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
                if sum1 > 50:
                    total_error = float('inf')

                if total_error < best_error:
                    best_error = total_error
                    best_a, best_b, best_c = a, b, c
    return best_a,best_b,best_c
aae_list = []
are_list = []
ncc_list = []
nic_list = []

aae_list_ml = []
are_list_ml = []
ncc_list_ml = []
nic_list_ml = []

aae_list_ml_2 = []
are_list_ml_2 = []
ncc_list_ml_2 = []
nic_list_ml_2 = []

aae_list_ml_5 = []
are_list_ml_5 = []
ncc_list_ml_5 = []
nic_list_ml_5 = []

aae_list_ml_10 = []
are_list_ml_10 = []
ncc_list_ml_10 = []
nic_list_ml_10 = []
total_memory = 100
total_memory *= 1024*8
cm_cols = int(total_memory/rows/32)
ratio_list = [0.1,0.1,0.02]
cm_ml_cols_lsit = []
for ratio in ratio_list:
    cm_ml_cols_lsit.append(int(cm_cols*ratio))
last_2_a_5,last_2_b_5,last_2_c_5 = None,None,None
last_5_a_5,last_5_b_5,last_5_c_5 = None,None,None
last_10_a_5,last_10_b_5,last_10_c_5 = None,None,None
for secon in range(60):
    print("second: ",secon)
    pac = pac_total[secon]
    sample_pac_5 = sample_pac_5_total[secon]
    keys_5 = keys_5_total[secon]
    keys = keys_total[secon]
    real_freq = Counter(pac)
    real_freq_5 = Counter(sample_pac_5)

    cm = CountMinSketch(rows, cm_cols)
    cm_ml_5 = CountMinSketch(num_rows=rows, num_cols=cm_ml_cols_lsit[0])

    for i, p in enumerate(pac):
        if i % 5 == 0:
            cm_ml_5.update(p)
        cm.update(p)

    X_5 = []
    y_5 = []

    for item in keys_5:
        X = cm_ml_5.get_counters(item)
        minn = min(X)
        maxx = max(X)
        if maxx - minn > threshold and minn < threshold1:
            X_5.append(X)
            y_5.append(real_freq_5[item])

    powers = [2**k for k in range(0, 6)]  # 2^0 到 2^15

    best_a_5 = None
    best_b_5 = None
    best_c_5 = None

    best_a_5,best_b_5,best_c_5 = get_best_params(X_5,y_5)
    if (secon % 2) == 0:
        last_2_a_5,last_2_b_5,last_2_c_5 = best_a_5,best_b_5,best_c_5
    if (secon % 5) == 0:
        last_5_a_5,last_5_b_5,last_5_c_5 = best_a_5,best_b_5,best_c_5
    if (secon % 10) == 0:
        last_10_a_5,last_10_b_5,last_10_c_5 = best_a_5,best_b_5,best_c_5

    true_frequency = []
    cm_frequency = []
    cm_ml_5_frequency = []
    cm_ml_2second_frequency = []
    cm_ml_5second_frequency = []
    cm_ml_10second_frequency = []
    cnt1_1=0
    cnt2_1=0
    cnt5_1=0
    cnt10_1=0
    cnt1_2=0
    cnt2_2=0
    cnt5_2=0
    cnt10_2=0
    for item in keys:
        ans, flag = cm.estimate_ml(item,best_a_5,best_b_5,best_c_5)

        true_frequency.append(real_freq[item])
        cm_frequency.append(cm.estimate(item))
        if flag and cm_frequency[-1]!= ans:
            if ans >= real_freq[item]:
                cnt1_1 += 1
            else:
                cnt1_2 += 1
        cm_ml_5_frequency.append(ans)
        ans1,flag1 = cm.estimate_ml(item,last_2_a_5,last_2_b_5,last_2_c_5)
        cm_ml_2second_frequency.append(ans1)
        if flag1 and cm_frequency[-1]!= ans1:
            if ans1 >= real_freq[item]:
                cnt2_1 += 1
            else:
                cnt2_2 += 1
        ans1,flag1 = cm.estimate_ml(item,last_5_a_5,last_5_b_5,last_5_c_5)
        cm_ml_5second_frequency.append(ans1)
        if flag1 and cm_frequency[-1]!= ans1:
            if ans1 >= real_freq[item]:
                cnt5_1 += 1
            else:
                cnt5_2 += 1
        ans1,flag1 = cm.estimate_ml(item,last_10_a_5,last_10_b_5,last_10_c_5)
        cm_ml_10second_frequency.append(ans1)
        if flag1 and cm_frequency[-1]!= ans1:
            if ans1 >= real_freq[item]:
                cnt10_1 += 1
            else:
                cnt10_2 += 1

    aae_cm = calculate_aae(true_frequency, cm_frequency)
    aae_cm_ml_5 = calculate_aae(true_frequency, cm_ml_5_frequency)
    aae_cm_ml_2second = calculate_aae(true_frequency, cm_ml_2second_frequency)
    aae_cm_ml_5second = calculate_aae(true_frequency, cm_ml_5second_frequency)
    aae_cm_ml_10second = calculate_aae(true_frequency, cm_ml_10second_frequency)
    are_cm = calculate_are(true_frequency, cm_frequency)
    are_cm_ml_5 = calculate_are(true_frequency, cm_ml_5_frequency)
    are_cm_ml_2second = calculate_are(true_frequency, cm_ml_2second_frequency)
    are_cm_ml_5second = calculate_are(true_frequency, cm_ml_5second_frequency)
    are_cm_ml_10second = calculate_are(true_frequency, cm_ml_10second_frequency)


    aae_list.append(aae_cm)
    are_list.append(are_cm)

    aae_list_ml.append(aae_cm_ml_5)
    are_list_ml.append(are_cm_ml_5)
    ncc_list_ml.append(cnt1_1)
    nic_list_ml.append(cnt1_2)
    aae_list_ml_2.append(aae_cm_ml_2second)
    are_list_ml_2.append(are_cm_ml_2second)
    ncc_list_ml_2.append(cnt2_1)
    nic_list_ml_2.append(cnt2_2)
    aae_list_ml_5.append(aae_cm_ml_5second)
    are_list_ml_5.append(are_cm_ml_5second)
    ncc_list_ml_5.append(cnt5_1)
    nic_list_ml_5.append(cnt5_2)
    aae_list_ml_10.append(aae_cm_ml_10second)
    are_list_ml_10.append(are_cm_ml_10second)
    ncc_list_ml_10.append(cnt10_1)
    nic_list_ml_10.append(cnt10_2)
