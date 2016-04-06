#!/bin/python 
import matplotlib.pyplot as plt
import matplotlib.gridspec as gs
import dataParser as dp
import operator as op
import numpy as np
import sys 

from matplotlib import rc
rc('xtick', labelsize=16) 
rc('ytick', labelsize=16) 
rc('font',**{'family':'sans-serif','sans-serif':['Helvetica'],'size':16})
plt.rcParams.update({'legend.fontsize':16,'legend.linewidth':2})
rc('text', usetex=True)

def mul(vec, _): return [_*v for v in vec]
def lev(vec): _ = np.mean(vec[0:100]); return [v-_ for v in vec]


if __name__ == "__main__": 

    try: 
        DATA = dp.getpulse(sys.argv[1])
    except IndexError: 
        sys.exit()

    if (DATA != None):

        fig = plt.figure(1, figsize=(10,7))
        gs  = gs.GridSpec(2, 2)
        ax = [fig.add_subplot(gs[i]) for i in range(4)]
    
        ylabels = ["Gate Voltage $(V)$", 
                   "Drain Voltage $(mA)$",
                   "Drain Current $(mA)$",
                   "Gate Current $(mA)$"]
        fx = [1,1000,1,1000]
        cx = [1,4,2,3]
        cl = ['k', 'r', 'k', 'r']
        for i in range(4): 
            for D in DATA:
                tmp = lev( D.WAV["CH%s"%(cx[i])])  
                ax[i].plot(mul(tmp, fx[i]), cl[i])
            ax[i].set_xlabel("Sample $(\#)$")
            ax[i].set_ylabel(ylabels[cx[i]-1])
            ax[i].set_xlim(400,700)
            
        gs.tight_layout(fig)
        img_path = sys.argv[1]+".png"
        fig.savefig(img_path, format='png', dpi=fig.dpi)
        plt.show()
   
