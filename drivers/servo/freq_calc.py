import os, sys
import itertools
import pandas as pd
import code # code.interact(local=dict(globals(), **locals()))

cpu_freq = [16 * 1000 * 1000]
scaler = [1, 8, 64, 256, 1024]
comp = range(1, 255, 1)
resolution = range(1, 1024*8, 1)

df = pd.DataFrame(
    list(itertools.product(cpu_freq, scaler, comp, resolution)),
    columns=['cpu_freq', 'scaler', 'comp', 'resolution'])

df['T'] = (1 / df.cpu_freq) * df.scaler * df.comp * df.resolution
df['F'] = 1 / df['T']
df['tick'] = df['T'] / df.resolution

code.interact(local=dict(globals(), **locals()))
