#! /usr/bin/env python


import os, sys
import fire, re
import pandas as pd
import code # code.interact(local=dict(globals(), **locals()))
import itertools


class ElfParser:
	def _parse_func_name(self, l):
		m = re.fullmatch('(?P<func_address>[0-9a-fA-F]+) <(?P<func_name>[_a-zA-Z0-9]+)>:$', l.strip())
		if m is None:
			return None
		return m.groupdict()

	def _parse_func_call(self, l):
		m = re.search('\s+call\s+0x(?P<callee_address>[0-9a-fA-F]+)', l)
		if m is None:
			return None
		return m.groupdict()

	def _find_paths(self, df, func_address, path):
		x = df[df.func_address == func_address]
		func_name = x.func_name.unique()
		if len(func_name) == 0:
			return [path.copy()]

		assert(len(func_name) == 1)
		func_name = func_name[0]
		path = path + [func_name]

		callees = x.callee_address.dropna().drop_duplicates()
		if len(callees) == 0:
			return [path.copy()]
		else:
			paths = callees.apply(lambda c: self._find_paths(df, c, path))
			return list(itertools.chain.from_iterable(paths.tolist()))


	def get_func_address(self, df, func_name):
		n = df[df.func_name == func_name].func_address.unique()
		assert len(n) == 1, print(n)
		return n[0]


	def _add_func_size(self, path, su_df):
		res = []
		for p in path:
			candidates = su_df[su_df.func_name == p]
			if len(candidates) > 1:
				print('found {len(candidates)} for {p}')
			res.append(candidates.iloc[0]['size'] if len(candidates) == 1 else 0)
		return [f'{n}:{s}' for n,s in zip(path, res)]

	def parse_elf(self, func_name, elf_path, stack_usage_csv):
		su_df = pd.read_csv(stack_usage_csv)
		su_df['func_name'] = su_df['func'].apply(lambda x: x.split(':')[3])

		os.system(f'avr-objdump -D {elf_path} > {elf_path}.txt')
		with open(f'{elf_path}.txt', 'r') as f:
			lines = f.readlines()
		df = pd.DataFrame({'rawline': lines})

		funcs = df.rawline.apply(self._parse_func_name)
		funcs = funcs.dropna().apply(pd.Series)
		df = df.join(funcs)

		calls = df.rawline.apply(self._parse_func_call)
		calls = calls.dropna().apply(pd.Series)
		df = df.join(calls)

		df = df.assign(func_address=df.func_address.fillna(method='ffill'))
		df = df.assign(func_name=df.func_name.fillna(method='ffill'))

		df = df.dropna(subset=['func_address', 'func_name'])
		df = df.assign(func_address=df.func_address.apply(lambda x: x.lstrip('0')))

		addr = self.get_func_address(df, func_name)
		paths = self._find_paths(df, addr, [])
		paths_df = pd.DataFrame({'paths': paths})
		paths_df['paths'] = paths_df.paths.apply(self._add_func_size, su_df=su_df)

		paths_df['cost'] = paths_df.paths.apply(lambda x: sum([int(s.split(':')[1]) for s in x]))
		paths_df.to_csv(f'{func_name}.csv', index=False)
		#paths_df = pd.DataFrame({paths: paths})
		code.interact(local=dict(globals(), **locals()))

# echo func,size,type,comments > stack_usage.csv
# find out/ -name '*.su' | xargs -l1 cat | awk '{print$1,$2,$3}' | sed 's/ /,/g' >> stack_usage.csv
# ./analize_paths.py parse_elf quad_dbg_loop out/atmega328p/jupiter-quad-dbg.elf stack_usage.csv
if __name__ == '__main__':
	fire.Fire(ElfParser)
