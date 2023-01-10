import ctypes
import numpy
import time


class NITask(): 
    def __init__(self):
        ni9263lib = ctypes.cdll.LoadLibrary('ni9263.lib')
        self.rate = 100.0
        self.data_init = numpy.array([0., 0., 0., 0.])
        self.data = self.data_init
        self.ni_init = ni9263lib.ni9263_play_4ch_init_ptp
        self.ni_play = ni9263lib.ni9263_play_4ch_ptp
        self.ni_stop = ni9263lib.ni9263_play_4ch_stop_ptp
        self.param_type_data = numpy.ctypeslib.ndpointer(dtype=numpy.double)
        self.param_type_rate = ctypes.c_double
        self.param_type_taskHandle = ctypes.c_long
        self.ni_init.argtypes = [self.param_type_rate, \
            self.param_type_data]
        self.ni_stop.argtypes = [self.param_type_taskHandle]
        self.ni_play.argtypes = [self.param_type_taskHandle, \
            self.param_type_data]
        self.ni_init.restype = ctypes.c_long
        self.ni_stop.restype = ctypes.c_long
        self.ni_play.restype = ctypes.c_long
        self.taskHandle = 0
#       TaskHandle ni9263_play_4ch_init_ptp(float64 rate, float64 *data){
#       ni9263_play_4ch_stop_ptp(TaskHandle taskHandle)
#       ni9263_play_4ch_ptp(TaskHandle taskHandle, double *data)
        
    def init(self):
        print("Initialization")
        self.taskHandle = self.ni_init(self.rate, self.data)
        print("This is task handle :" + str(self.taskHandle))

    def fire(self, iChan): 
        """
        Activate channel number iChan
        """
        self.data[iChan] = 0.1
        self.ni_play(self.taskHandle, self.data)
        return None

    def playData(self, data, nData): 
        """
        Play data 
        """
        for i in range(nData):
            self.data = data[i]
            self.ni_play(self.taskHandle, self.data)
        return None

    def __del__(self): 
        """
        Kill the task
        """
        if (self.taskHandle == 0):
            pass
        else:
            res = self.ni_stop(self.taskHandle)
            print("killing task " + str(self.taskHandle) + " with res " + str(res))
        return None
        
def parseToNI(data, nObs, rate, nSampPerChan):
    ni9263lib = ctypes.cdll.LoadLibrary('ni9263.lib')
    ni9263_genX4 = ni9263lib.genX4
    typeData = numpy.ctypeslib.ndpointer(dtype=numpy.double)
    nObs = ctypes.c_long(nObs)
    print("nObs: {0}".format(nObs))
    rate = ctypes.c_double(rate)
    print("rate: {0}".format(rate))
    nSampPerChan = ctypes.c_ulong(nSampPerChan)
    print("nSampPerChan: {0}".format(nSampPerChan))
    ni9263_genX4.argtypes = [typeData, typeData, typeData, typeData, ctypes.c_long, ctypes.c_double,
        ctypes.c_ulong]
    res = ni9263_genX4(data[0, :], data[1, :], data[2, :], data[3, :], 
        nObs, rate, nSampPerChan)
    return res

parseX = parseToNI

def fourSinus(): 
    """
    Generate four sinus 
    x1, x2, x3, x3
    (t1, t2, t3, t4) = (1, 0.5, 0.333, 0.25)
    """
    nObs = 100000
    tS = 0.0001
    x = numpy.empty((4, nObs))
    t1 = 1
    t2 = 1./2
    t3 = 1./3
    t4 = 1./4
    for i in range(nObs): 
        x[0, i] = 0.1 * numpy.array(numpy.sin(2 * numpy.pi * i * tS / t1))
        x[1, i] = 0.1 * numpy.array(numpy.sin(2 * numpy.pi * i * tS / t2))
        x[2, i] = 0.1 * numpy.array(numpy.sin(2 * numpy.pi * i * tS / t3))
        x[3, i] = 0.1 * numpy.array(numpy.sin(2 * numpy.pi * i * tS / t4))
        """
    x[0, :] = numpy.array([0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2])
    x[1, :] = numpy.array([-0.1, -0.2, -0.3, -0.4, -0.1, -0.2, -0.3, -0.4, -0.1, -0.2])
    x[2, :] = numpy.array([0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2])
    x[3, :] = numpy.array([-0.1, -0.2, -0.3, -0.4, -0.1, -0.2, -0.3, -0.4, -0.1, -0.2])
    """
    rate = 1000.
    nSampPerChan = 10000
    res = parseToNI(x, nObs, rate, nSampPerChan)
    if (res < 0): 
        print('problem {0}'.format(res))

def oneDipole(f1, f2, f3, f4):
    """
    Generate one dipole 
    
    x1 = 0.1 * sin(w * t)
    x2 = - x1
    x3 = x4 = 0
    
    """
    nObs = 100001
    tS = 0.0001
    x = numpy.empty((4, nObs))
    (t1, t2, t3, t4) = (1/f1, 1/f2, 1/f3, 1/f4)
    print((t1, t2, t3, t4))
    for i in range(nObs): 
        x[0, i] = 0.15 * numpy.array(numpy.sin(2 * numpy.pi * i * tS / t1))
        x[1, i] = -0.15 * numpy.array(numpy.sin(2 * numpy.pi * i * tS / t2))
        x[2, i] = 0.
        x[3, i] = 0.
    rate = 10000.
    nSampPerChan = 10000
    res = parseToNI(x, nObs, rate, nSampPerChan)
    if (res < 0): 
        print('problem {0}'.format(res))
    return



if __name__ == '__main__':
    f0 = 100.
    print("f0 : " + str(f0))
    oneDipole(f0, f0, f0, f0)

