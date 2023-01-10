# Py_NI_CRIO9263
C and Python codes to communicate with a National Instrument CRIO-9263

This is a set of C and Python codes to communicate with a National Instrument CRIO-9263, voltage analog output module with 4 outputs, 16 bits, 100 KS/s per channel, mounted on a cDAQ-9181 ethernet chassis. 
Tests are done on a Windows OS. 

Python modules depends on numpy package. 
It has been developped for Python 2.7 but should work with Python 3.5 

Copyright G. Becq, Gipsa-lab, UMR 5216, CNRS, INPG, UGA. 

For the compilation, linking is done using the following paths for NI libraries:   
./include/NIDAQmx.h
./lib/NIDAQmx.lib

*spectro_all_seq.bat* 
 generates different sequences of sine waves;
 it contains different calls to *spectroscopy.py* and *spectro4chan.py* 
 with different parameters. 
*step.bat* 
 generates step function sequences using *step.py*
*wn.bat* 
 generates white noise sequences using *whiteNoise.py*
*spectroscopy.py*, *step.py*, *whiteNoise.py*
 uses modules *pyToNI.py* and *fun.py*
*pyToNI.py* 
 contains classes to use functions in the library *ni9263.lib* using 
 ctypes and numpy. 
*fun.py* 
 contains definitions to generate signals in ndarray objects from numpy.  
*ni9263.lib* 
 compiled version of generateSignal.c   
*makefile* 
 instruction to obtain the library from *generateSignal.c* and *testSignal.c*
*generateSignal.c* *generateSignal.h*
 contains functions to communicate with NIDAQmx.lib and the instrument. 
*testSignal.c*
 test a signal for compilation. 



