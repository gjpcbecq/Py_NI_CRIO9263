"""
spectroscopy

"""
import fun
import pyToNI
import numpy
import sys


def two_chan_freq_0_1(f, nObs, A, Fs, nSampPerChan):
    """
    Sweep of frequencies in a list of frequencies
    """
    X = numpy.zeros((4, nObs))
    x = fun.sinusoid(nObs, A, f, Fs, 0)
    X[0, :] = x
    X[1, :] = x
    X[2, :] = x
    X[3, :] = x
    t = (f, x[0], x[1], x[2], x[3], x[-1])
    sty = "{:<7.2f}"
    s = "f - " + sty + ": " + (sty + ", ") * 3 + "..., " + sty
    print(s.format(*t))
    try : 
        pyToNI.parseX(X, nObs, Fs, nSampPerChan)
    except Exception as e: 
        print(e)
    # ni.stimChan(iChan, x, Fs)

def freq():
    fS = 100000
    nMax = 10000000
    nArg = len(sys.argv)
    print(sys.argv)
    if (nArg < 4):
        s = help()
        print(s)
    if (nArg == 4): 
        f0 = float(sys.argv[1])
        dT = float(sys.argv[2])
        nObs = int(dT * fS)
        A = float(sys.argv[3])
        if (nObs >= nMax / 4):
            nSampPerChan = nMax / 4
        else :
            nSampPerChan = nObs  
        two_chan_freq_0_1(f0, nObs, A, fS, nSampPerChan)
    return None
        
def help(): 
    s = """
    python spectro2chan.py f0 dT A
    
    f0: frequency of the sinusoid
    dT: duration of observation
    A: amplitude of the signal < 0.5 V  
    
    Description 
    
    The frequency of the output signal is set to 100 kHz
    
    The minimal duration of one simulation is 1 s limited by the program. 
    The maximal duration of one simulation dTMax is 10 s limited by the program.
    
    Example 
    
    python spectro2chan.py 100 10 0.1
    
    """
    return s

if __name__ == "__main__": 
    freq()
