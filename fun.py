"""
fun

"""
import numpy

def sinusoid(nObs, A, f, Fs, phi):
    """
    Generate samples from a sinusoid 
    
    $$ x(k) = A \, \sin (2 \, \pi \, f \, k / Fs + \phi) $$
    
    nObs: number of observations
    A: amplitutde
    f: requency
    Fs: sampling rate
    phi: phase
    
    """
    pi = numpy.pi
    x = numpy.zeros((nObs, ))
    for i in range(nObs): 
        alpha = 2 * pi * f * i / Fs + phi 
        x[i] = A * numpy.sin(alpha)
    return x

def step(nObs, A): 
    x = numpy.zeros((nObs, ))
    for i in range(nObs): 
        x[i] = A
    return x
    
def testSinusoid(): 
    (nObs, A, f, Fs, phi) = (1000, 1, 10, 1000, 0)
    print(numpy.round(sinusoid(nObs, A, f, Fs, phi) * 100) / 100.)

def testStep(): 
    (nObs, A) = (1000, 2)
    print(numpy.round(step(nObs, A) * 100) / 100.)

if __name__ == "__main__": 
    # testSinusoid()
    testStep()
