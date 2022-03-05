#! /usr/bin/env python


prescaler = [1, 4, 16, 64]
cpu_freq = 16 * 1000 * 1000
target_freq = 400 * 1000

for p in prescaler:
     res = (cpu_freq - 16*target_freq) / (2 * p * target_freq)
     print(res)
