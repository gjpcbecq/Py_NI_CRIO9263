import numpy
import sys
import pyToNI

def one_chan_WN(iChan, S, nObs, A, Fs, nSampPerChan): 
    """
    Generate samples from a White noise function with amplitude A
    """
    print(S)
    numpy.random.seed(S)
    X = numpy.zeros((4, nObs))
    x = A * numpy.random.randn(nObs)
    X[iChan, :] = x
    try : 
        pyToNI.parseX(X, nObs, Fs, nSampPerChan)
    except Exception as e: 
        print(e)
    return None
    
def WN():
    fS = 10000
    nMax = 10000000
    nArg = len(sys.argv)
    if (nArg != 5):
        s = help()
        print(s)
    if (nArg == 5): 
        iChan = int(sys.argv[1])
        dT = float(sys.argv[2])
        nObs = int(dT * fS)
        S = int(sys.argv[3])
        A = float(sys.argv[4])
        if (nObs >= nMax / 4):
            nSampPerChan = nMax / 4
        else :
            nSampPerChan = nObs  
        one_chan_WN(iChan, S, nObs, A, fS, nSampPerChan)

def help(): 
    """
    """
    s = """
    python whiteNoise.py iChan dT S A
    
    S: seed 
    A: amplitude
    iChan: number of channel
    """
    print(s)

if __name__ == "__main__":
    WN()
           
