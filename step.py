"""
step.py

"""
import fun
import pyToNI
import numpy
import sys

def one_chan_step(iChan, nObs, A, Fs, nSampPerChan): 
    """
    Generate samples from a step function with amplitude A
    """
    X = numpy.zeros((4, nObs))
    x = fun.step(nObs, A)
    X[iChan, :] = x
    t = (x[0], x[1], x[2], x[3], x[-1])
    sty = "{:<7.2f}"
    s = "" + (sty + ", ") * 3 + "..., " + sty
    print(s.format(*t))
    try : 
        pyToNI.parseX(X, nObs, Fs, nSampPerChan)
    except Exception as e: 
        print(e)
    return None

def step(): 
    fS = 10000
    nMax = 10000000
    nArg = len(sys.argv)
    if (nArg < 4):
        s = help()
        print(s)
    if (nArg == 4): 
        iChan = int(sys.argv[1])
        dT = float(sys.argv[2])
        nObs = int(dT * fS)
        A = float(sys.argv[3])
        if (nObs >= nMax / 4):
            nSampPerChan = nMax / 4
        else :
            nSampPerChan = nObs  
        one_chan_step(iChan, nObs, A, fS, nSampPerChan)

def help(): 
    s = """
    python step.py iChan dT A
    
    iChan: channel number {0, 1, 2, 3} 
    dT: duration of observation
    A: amplitude of the signal < 0.5 V  
    
    Description 
    
    The frequency of the output signal is set to 100 kHz
    
    The minimal duration of one simulation is 1 s limited by the program. 
    The maximal duration of one simulation dTMax is 10 s limited by the program.
    
    Example 
    
    python step.py 0 10 0.1
    
    """
    return s

if __name__ == "__main__": 
    step()
