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
with open("../cpu/data/stackoverflow.txt", "r", encoding="utf-8") as f:
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
