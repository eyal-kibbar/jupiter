import pandas as pd
import sys, os
import code # code.interact(local=dict(globals(), **locals()))
from functools import reduce

f = sys.argv[1]
df = pd.read_csv(f)

df['reg_name'] = ''


regs_df = df[~df.Address.isnull()]

for i in range(len(regs_df)-1):
    s = regs_df.iloc[i].name
    e = regs_df.iloc[i+1].name
    df.at[s:e-1, 'reg_name'] = regs_df.iloc[i].Mnemonic

df.at[regs_df.iloc[-1].name:, 'reg_name'] = regs_df.iloc[-1].Mnemonic


def parse(r):
    if type(r.Address) == str:
        return [{
            'name': 'NRF_{}_ADDR'.format(r.reg_name),
            'value': '0x{}'.format(r.Address)
        }]

    if r.Mnemonic == 'Reserved':
        return []

    b = [int(x) for x in r.Bit.split(':')]
    if len(b) == 1:
        shft = b[0]
        bmsk = 1 << shft
    elif len(b) == 2:
        shft = min(b[0], b[1])
        print(b)
        print(list(range(b[1], b[0]+1)))
        bmsk = reduce(lambda a,b: a|b, [1 << x for x in range(b[1], b[0]+1)])
    else:
        raise Exception('invalid Bit field')
    return [
        {
            'name': 'NRF_{}_{}_SHFT'.format(r.reg_name, r.Mnemonic),
            'value': '0x{:02x}'.format(shft)
        },
        {
            'name': 'NRF_{}_{}_BMSK'.format(r.reg_name, r.Mnemonic),
            'value': '0x{:02x}'.format(bmsk)
        }
    ]

x= df.apply(parse, axis=1).explode()
x = x.dropna().apply(pd.Series)

#x.sort_values(by='name', inplace=True)
m = x['name'].apply(len).max()
fmt = '#define {:'+str(m+4)+'s} {}'
p = x.apply(lambda r: fmt.format(r['name'], r.value), axis=1)


for idx, line in p.iteritems():
    print(line)
