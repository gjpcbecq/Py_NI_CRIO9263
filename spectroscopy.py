"""
spectroscopy

"""
import fun
import pyToNI
import numpy
import sys


def one_chan_freq(iChan, f, nObs, A, Fs, nSampPerChan):
    """
    Sweep of frequencies in a list of frequencies
    """
    X = numpy.zeros((4, nObs))
    x = fun.sinusoid(nObs, A, f, Fs, 0)
    X[iChan, :] = x
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
    fS = 10000
    nMax = 10000000
    nArg = len(sys.argv)
    print(sys.argv)
    if (nArg < 5):
        s = help()
        print(s)
    if (nArg == 5): 
        iChan = int(sys.argv[1])
        f0 = float(sys.argv[2])
        dT = float(sys.argv[3])
        nObs = int(dT * fS)
        A = float(sys.argv[4])
        if (nObs >= nMax / 4):
            nSampPerChan = nMax / 4
        else :
            nSampPerChan = nObs  
        one_chan_freq(iChan, f0, nObs, A, fS, nSampPerChan)
    return None
        
def help(): 
    s = """
    python spectroscopy.py iChan f0 dT A
    
    iChan: channel number {0, 1, 2, 3} 
    f0: frequency of the sinusoid
    dT: duration of observation
    A: amplitude of the signal < 0.5 V  
    
    Description 
    
    The frequency of the output signal is set to 100 kHz
    
    The minimal duration of one simulation is 1 s limited by the program. 
    The maximal duration of one simulation dTMax is 10 s limited by the program.
    
    Example 
    
    python spectroscopy.py 0 100 10 0.1
    
    """
    return s

if __name__ == "__main__": 
    freq()
