import os
import re
import sys
import numpy as np

# NORMAL CSV DATA
#####################################
def drange(start, stop, step):
    r = start
    while r < stop:
     	yield r
     	r += step

def chunks(l, n):
    for i in xrange(0, len(l), n):
        yield l[i:i+n]

def nearest(array,value):
    idx = (np.abs(np.asarray(array)-value)).argmin()
    return array[idx],idx

def stream_lines(file_name):
    _file = open(file_name)
    while True:
      line = _file.readline()
      if not line:
        _file.close()
        break
      yield line


def gen_paths(_dir):
   for dirpath,_,filenames in os.walk(_dir):
       for f in filenames: yield os.path.abspath(os.path.join(dirpath, f))

###################
# NORMAL CSV DATA #
###################
def getdata(file_name, h = None):
    gen  = stream_lines(file_name)
    if h is not None:
        while True:
            line = gen.next().split()
            if line[0] == h: continue
            else: 
                keys = line
                break
    else:
        keys = gen.next().split()

    data = dict((k, []) for k in keys)
    while True:
        try:
            line =  gen.next().split()
            if len(line) == 1: continue
            if len(line) != len(keys): continue
            if line[0] == h: continue 
            for i,k in enumerate(keys):
                try:
                    data[k].append(float(line[i]))
                except ValueError:
                    data[k].append(str(line[i]))

        except IndexError: pass
        except StopIteration: return data


################################## 
# PULSED IV DATA (FOR WAVEFORMS) #
##################################
class point:
    def __init__(self):
        self.vg  = 0.
        self.vd  = 0.
        self.WAV = {}

def getpulse(file_name, h=None): 
    
    ## Check to see if we have a PulsedIV file
    gen = stream_lines(file_name)
    line = gen.next().split()
    if (line[0] != "##"): return None
    line = gen.next().split()
    if (line[0] != "##"): return None

    DAT = []
    gen = stream_lines(file_name)
    while True: 
        try:
            line = gen.next().split()
            if (line[0] == "##"):
                p = point()
                p.vg  = float(line[1])
                p.vd  = float(line[2])
            
                keys  = gen.next().split()[1:]
                p.WAV = dict((k, []) for k in keys) 

                while True:
                    line = gen.next().split()
                    if line[0] != "##": 
                        [p.WAV[k].append(float(_)) for k,_ in zip(keys,line)]  
                    else: 
                        DAT.append(p); 
                        break
       
        except StopIteration: return DAT

