#!/usr/bin/env python3

import glob, sys, random, os

champs = glob.glob('tests/champions/*.cor')
if len(champs) < 1:
    sys.exit()

cmd = './corewar -g -q'
for i in range(4):
    cmd += ' ' + champs[random.randint(0,len(champs)-1)]
print(cmd)    
os.system(cmd)
