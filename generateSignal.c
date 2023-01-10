#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
// #include <string.h>
#include "C:\Program Files (x86)\National Instruments\NI-DAQ\DAQmx ANSI C Dev\include\NIDAQmx.h"
#define PI 3.1415926
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
#define PRINTLINE printf("___________________________________________\n");
#define DISP(a) printf("checking >>> %s\n", a);
#define False 0
#define True 1
#define Success 0
#define Failure 1

void mySinus(double *x, int32 nObs, double fS, double f0, double a, double phi){
    int32 i; 
    for(i=0; i<nObs; i++){
        x[i] = a * sin(2 * PI * (double) i * f0 / fS + phi);
    }
}

int printSignal(double *x, int32 nObs){
    int32 i; 
    if (nObs < 10){
        printf("[");
        for(i=0; i<nObs-1; i++){
            printf("%+5.3f, ", x[i]);
        }
        printf("%+5.3f]", x[nObs-1]);
    }
    else {
        printf("[");
        for(i=0; i<5; i++){
            printf("%+5.3f, ", x[i]);
        }
        printf("..."); 
        for(i=nObs-5; i<nObs-1; i++){
            printf("%+5.3f, ", x[i]);
        }
        printf("%+5.3f]", x[nObs-1]);
    }
    printf(" (%i samples)\n", nObs);
    return 0; 
}

int sequenceOnCDAQ(double *x1, double *x2){
    // initialisation
    // 
    // close
    int status = 0; 
    return status;
}

int convertDoubleToFloat64(float64 *y, double *x, int32 n){
    int32 i; 
    for(i=0; i<n; i++){
        y[i] = (float64) x[i]; 
    }
    return 0; 
}

int prepareSignalNonInterleaved(float64 *y, double *x1, double *x2, int32 n){
    int32 i; 
    for(i=0; i<n; i++){
        y[i] = (float64) x1[i];
    }
    for(i=n; i<2*n; i++){
        y[i] = (float64) x2[i-n]; 
    }
    return 0; 
}

int prepareSignal1Ch(float64 *y, double *x, int32 n){
    int32 i; 
    for(i=0; i<n; i++){
        y[i] = x[i];
        }
    return 0; 
}


int prepareSignalInterleaved4Ch(float64 *y, double *x1, double *x2, double *x3, \
    double *x4, int32 n){
    int32 i; 
    for(i=0; i<n; i++){
        y[4*i] = x1[i];
        y[4*i+1] = x2[i];
        y[4*i+2] = x3[i];
        y[4*i+3] = x4[i];
    }
    return 0; 
}

int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
	int32   error=0;
	char    errBuff[2048]={'\0'};

	// Check to see if an error stopped the task.
	DAQmxErrChk (status);

Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		DAQmxClearTask(taskHandle);
		printf("DAQmx Error: %s\n",errBuff);
	}
	return 0;
}

int ni9263_play_4ch(double *x1, double *x2, double *x3, double *x4, int32 nObs, \
    float64 rate, uInt32 nSamplePerChannel){
    /* play finite samples of x1, x2, x3, x4 on the ni9263
    */
    // cDAQ9181 ethernet
    const char *device_name = "cDAQ9181-186CE4F";
    // Module Analogic output NI 9263
    const char *device_moduleName = "cDAQ9181-186CE4FMod1";
    /*
    const char *chan0 = strcat(device_moduleName, "/ao0"); 
    const char *chan1 = strcat(device_moduleName, "/ao1"); 
	*/
    const char *listOfChannel = "cDAQ9181-186CE4FMod1/ao0:3"; 
	int error=0;
	TaskHandle taskHandle=0;
	char errBuff[2048]={'\0'};
//	float64 data[1] = {1.0};
	int32 nSample; 
	int32 nChan = 4; 
	nSample = nChan * nObs; 
	float64 *data = malloc(nSample * sizeof(float64)); 
	int32 sampsPerChanWritten[0]; 
	uInt64 sampsPerChanToGenerate = nSamplePerChannel; 
	prepareSignalInterleaved4Ch(data, x1, x2, x3, x4, nObs);
	printSignal(data, nSample); 
	int cond, res; 
	int32 iBlockBeg, iBlockEnd; 
	
	res = DAQmxSelfTestDevice(device_name);
	while (res != 0) {
	    res = DAQmxSelfTestDevice(device_name);
	}
	printf("res self-test: %i \n", res); 
	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	DAQmxErrChk(DAQmxCreateTask("task1", &taskHandle));
	DAQmxErrChk(DAQmxCreateAOVoltageChan(taskHandle, listOfChannel, "", \
	    -1.0, 1.0, DAQmx_Val_Volts, ""));
	DISP("before clktiming")
	printf("nSample: %i\n", nSample); 
	printf("nSamplePerChannel: %i\n", nSamplePerChannel); 
	printf("rate: %5.2f\n", rate); 
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", \
	    rate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, \
	    sampsPerChanToGenerate));
	DISP("after clktiming")
	DISP("before buffering")
	DAQmxErrChk(DAQmxCfgOutputBuffer(taskHandle, nSamplePerChannel));
	DISP("after buffering")
	DAQmxErrChk(DAQmxRegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

	/*********************************************/
	// DAQmx Write Code
	/*********************************************/
	int32 k = 0;
	iBlockBeg = k * (nSamplePerChannel * nChan); 
	iBlockEnd = (k + 1) * (nSamplePerChannel * nChan); 
	cond = (iBlockEnd < nSample); 
	printf("cond: %i \n", cond); 
	DISP("Entering the loop")
	while (cond){
        DAQmxErrChk(DAQmxWriteAnalogF64(taskHandle, nSamplePerChannel, 0, \
            DAQmx_Val_WaitInfinitely, \
            DAQmx_Val_GroupByScanNumber, data+iBlockBeg, sampsPerChanWritten, NULL));
        //printf("written: %16d \n", sampsPerChanWritten[0]);
        /*********************************************/
        // DAQmx Start Code
        /*********************************************/
        DAQmxErrChk(DAQmxStartTask(taskHandle));
        DAQmxErrChk(DAQmxWaitUntilTaskDone(taskHandle, \
            DAQmx_Val_WaitInfinitely));
        DAQmxStopTask(taskHandle);
        k++; 
        iBlockBeg = k * (nSamplePerChannel * nChan); 
        iBlockEnd = (k + 1) * (nSamplePerChannel * nChan); 
        cond = (iBlockEnd < nSample); 
	}
	DAQmxResetDevice(device_name);
	free(data); 
	return 0; 
	
Error:
	if( DAQmxFailed(error) )
		DAQmxGetExtendedErrorInfo(errBuff,2048);
	if( taskHandle!=0 ) {
		/*********************************************/       
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if( DAQmxFailed(error) ) printf("DAQmx Error: %s\n",errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	free(data); 
	return 1;
}    

int genX4(double *x1, double *x2, double *x3, double *x4, int32 nObs, \
    float64 rate, uInt32 nSamplePerChannel){
    /* play x1, x2, x3, x4 on the ni9263
    */
    printf("rate: %5.2g\n", rate); 
    // cDAQ9181 ethernet
    const char *device_name = "cDAQ9181-186CE4F";
    // Module Analogic output NI 9263
    const char *device_moduleName = "cDAQ9181-186CE4FMod1";
    /*
    const char *chan0 = strcat(device_moduleName, "/ao0"); 
    const char *chan1 = strcat(device_moduleName, "/ao1"); 
	*/
    const char *listOfChannel = "cDAQ9181-186CE4FMod1/ao0:3"; 
	int error=0;
	TaskHandle taskHandle=0;
	char errBuff[2048]={'\0'};
//	float64 data[1] = {1.0};
	int32 nSample; 
	int32 nChan = 4; 
	nSample = nChan * nObs; 
	float64 *data = malloc(nSample * sizeof(float64)); 
	int32 sampsPerChanWritten[0]; 
	uInt64 sampsPerChanToGenerate = nSamplePerChannel;
	prepareSignalInterleaved4Ch(data, x1, x2, x3, x4, nObs);
	printSignal(data, nSample); 
	int cond, res; 
	int32 iBlockBeg, iBlockEnd; 
	// test selfTest
	res = DAQmxSelfTestDevice(device_name);
	while (res != 0) {
	    res = DAQmxSelfTestDevice(device_name);
	}
	printf("res self-test: %i \n", res); 
	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	DAQmxErrChk(DAQmxCreateTask("task1", &taskHandle));
	DAQmxErrChk(DAQmxCreateAOVoltageChan(taskHandle, listOfChannel, "", \
	    -1.0, 1.0, DAQmx_Val_Volts, ""));
	DISP("before clktiming")
	printf("nObs: %i\n", nObs); 
	printf("nSample: %i\n", nSample); 
	printf("nSamplePerChannel: %i\n", nSamplePerChannel); 
	printf("rate: %5.2f\n", rate); 
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", \
	    rate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, \
	    sampsPerChanToGenerate));
	DISP("after clktiming")
	DISP("before buffering")
	DAQmxErrChk(DAQmxCfgOutputBuffer(taskHandle, nSamplePerChannel));
	DISP("after buffering")
	DAQmxErrChk(DAQmxRegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

	/*********************************************/
	// DAQmx Write Code
	/*********************************************/
	int32 k = 0;
	iBlockBeg = k * (nSamplePerChannel * nChan); 
	iBlockEnd = (k + 1) * (nSamplePerChannel * nChan); 
	printf("iBlockBeg: %i \n", iBlockBeg); 
	printf("iBlockEnd: %i \n", iBlockEnd); 
	cond = (iBlockEnd <= nSample); 
	printf("cond: %i \n", cond); 
	DISP("Entering the loop")
	while (cond){
        DAQmxErrChk(DAQmxWriteAnalogF64(taskHandle, nSamplePerChannel, 0, \
            DAQmx_Val_WaitInfinitely, \
            DAQmx_Val_GroupByScanNumber, data+iBlockBeg, sampsPerChanWritten, NULL));
        printf("written: %16d \n", sampsPerChanWritten[0]);
        /*********************************************/
        // DAQmx Start Code
        /*********************************************/
        DAQmxErrChk(DAQmxStartTask(taskHandle));
        DAQmxErrChk(DAQmxWaitUntilTaskDone(taskHandle, \
            DAQmx_Val_WaitInfinitely));
        DAQmxStopTask(taskHandle);
        k++;
        printf("k++ %5i \n", k); 
        iBlockBeg = k * (nSamplePerChannel * nChan); 
        iBlockEnd = (k + 1) * (nSamplePerChannel * nChan); 
        cond = (iBlockEnd < nSample); 
	}
	DAQmxResetDevice(device_name);
	free(data); 
	return 0; 
	
Error:
	if( DAQmxFailed(error) )
		DAQmxGetExtendedErrorInfo(errBuff,2048);
	if( taskHandle!=0 ) {
		/*********************************************/       
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if( DAQmxFailed(error) ) printf("DAQmx Error: %s\n",errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	free(data); 
	return 1;
}    

TaskHandle ni9263_play_4ch_init_ptp(float64 rate, float64 *data){
    /* initialization of the point to point output on the ni9263
    */
    // cDAQ9181 ethernet
    const char *device_name = "cDAQ9181-186CE4F";
    // Module Analogic output NI 9263
    const char *device_moduleName = "cDAQ9181-186CE4FMod1";
    /*
    const char *chan0 = strcat(device_moduleName, "/ao0"); 
    const char *chan1 = strcat(device_moduleName, "/ao1"); 
	*/
    const char *listOfChannel = "cDAQ9181-186CE4FMod1/ao0:3"; 
	int error = 0;
	char errBuff[2048]={'\0'};
	int res = Failure; 
	while (res == Failure){
	    res = DAQmxSelfTestDevice(device_name);
	}
	TaskHandle taskHandle = 0; 
	DAQmxErrChk(DAQmxCreateTask("task1", &taskHandle));
	char *a; 
	sprintf(a, "%2i\n", taskHandle); 
	DISP(a)
	DAQmxErrChk(DAQmxCreateAOVoltageChan(taskHandle, listOfChannel, "", \
	    -1.0, 1.0, DAQmx_Val_Volts, ""));
	DISP("before clktiming")
	uInt64 sampsPerChanToGenerate = 1; 
	/* DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", \
	    rate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, \
	    sampsPerChanToGenerate));
    */
	DISP("after clktiming")
	DISP("before buffering")
	//uInt64 nSamplePerChannel = 4; 
	//DAQmxErrChk(DAQmxCfgOutputBuffer(taskHandle, nSamplePerChannel));
	DISP("after buffering")
	// DAQmxErrChk(DAQmxRegisterDoneEvent(taskHandle, 0, DoneCallback, NULL));
    int32 numSampsPerChan = 1;
    int32 *sampsPerChanWritten; 
    sampsPerChanWritten = NULL; 
    bool32 autoStart = 1; 
    float64 timeOut = 10.0; 
    float64 timeToWait = 10.0; 
    DAQmxErrChk(DAQmxWriteAnalogF64(taskHandle, numSampsPerChan, autoStart, \
        timeOut, DAQmx_Val_GroupByScanNumber, data, sampsPerChanWritten, NULL));
    DAQmxErrChk(DAQmxStartTask(taskHandle));
    // DAQmxErrChk(DAQmxWaitUntilTaskDone(taskHandle, timeToWait));
	sprintf(a, "%2i\n", taskHandle); 
	DISP(a)
    return taskHandle; 
	
Error:
	if (DAQmxFailed(error)) DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		/*********************************************/       
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error)) printf("DAQmx Error: %s\n", errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	return taskHandle;
}

int ni9263_play_4ch_stop_ptp(TaskHandle taskHandle){
	int error = 0;
    char errBuff[2048] = {'\0'};
    DAQmxErrChk(DAQmxStopTask(taskHandle));
    DAQmxErrChk(DAQmxClearTask(taskHandle));
    return 0; 
Error:
	if( DAQmxFailed(error) )
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		/*********************************************/       
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error)) printf("DAQmx Error: %s\n",errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	return 1;
}



int ni9263_play_4ch_ptp(TaskHandle taskHandle, double *data){
    /* play ptp values in data on the ni9263
    */
    // cDAQ9181 ethernet
	int error = 0;
    char errBuff[2048]={'\0'};
    int32 numSampsPerChan = 1;
    int32 *sampsPerChanWritten;
    sampsPerChanWritten = NULL; 
    bool32 autoStart = 1; 
    float64 timeOut = 10.0; 
    float64 timeToWait = 10.0; 
    DAQmxErrChk(DAQmxWriteAnalogF64(taskHandle, numSampsPerChan, autoStart, \
        timeOut, DAQmx_Val_GroupByScanNumber, data, sampsPerChanWritten, NULL));
    // DAQmxErrChk(DAQmxWaitUntilTaskDone(taskHandle, timeToWait));
	return 0; 	
Error:
	if( DAQmxFailed(error) )
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if( taskHandle!=0 ) {
		/*********************************************/       
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if( DAQmxFailed(error) ) printf("DAQmx Error: %s\n",errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	free(data); 
	return 1;
}


int testPTP(){
    float64 rate = 10.0; 
    float64 data[4];
    data[0] = 0.; 
    data[1] = 0.; 
    data[2] = 0.; 
    data[3] = 0.; 
    char ans; 
    TaskHandle taskHandle = ni9263_play_4ch_init_ptp(10.0, data);
    DISP("initialization done... taskHandle is defined now. ")
    int cond = True;
    while (cond == True) {
        PRINTLINE
        printf("Continue: yes (* \\ n), no (n) ? \n"); 
        ans = getchar();
        printf("\n"); 
        if (ans == 'n') {
            cond = False; 
        }
        else {
            printf("Send a pulse: yes (y), no (* \\ y) ? "); 
            ans = getchar(); 
            printf("\n"); 
            if (ans == 'y') {
                data[0] = 0.1; 
                data[1] = 0.1; 
                data[2] = 0.1; 
                data[3] = 0.1; 
            }
            else {
                data[0] = 0.; 
                data[1] = 0.; 
                data[2] = 0.; 
                data[3] = 0.; 
            }
            ni9263_play_4ch_ptp(taskHandle, data);
        }
        PRINTLINE
    }
    return 0; 
}

int testSinus(){
    int32 nObs = 6000000;
    double *x1 = malloc(nObs * sizeof(double)); 
    double *x2 = malloc(nObs * sizeof(double)); 
    double *x3 = malloc(nObs * sizeof(double)); 
    double *x4 = malloc(nObs * sizeof(double)); 
    double fS = 100000; 
    double f1 = 0.25; 
    double a1 = 0.05; 
    double phi1 = 0; 
    double f2 = 0.5;  
    double a2 = 0.050; 
    double phi2 = PI / 2;
    double f3 = 1;  
    double a3 = 0.050; 
    double phi3 = PI / 2;
    double f4 = 2; 
    double a4 = 0.050; 
    double phi4 = PI / 2;
    int resPlay; 
    mySinus(x1, nObs, fS, f1, a1, phi1); 
    mySinus(x2, nObs, fS, f2, a2, phi2); 
    mySinus(x3, nObs, fS, f3, a3, phi3); 
    mySinus(x4, nObs, fS, f4, a4, phi4); 
    printSignal(x1, nObs); 
    printSignal(x2, nObs);
    printSignal(x3, nObs);
    printSignal(x4, nObs);
    PRINTLINE
    printf("Trying to send data to the CNA\n"); 
    resPlay = ni9263_play_4ch(x1, x2, x3, x4, nObs, 100000, 2000000); 
    printf("resPlay: %6i\n", resPlay); 
    PRINTLINE
    free(x1); 
    free(x2); 
    free(x3); 
    free(x4); 
    return 0; 
}

int testSignal(){
    int32 nObs = 10;
    double *x1 = malloc(nObs * sizeof(double)); 
    double *x2 = malloc(nObs * sizeof(double)); 
    double *x3 = malloc(nObs * sizeof(double)); 
    double *x4 = malloc(nObs * sizeof(double)); 
    int resPlay; 
    x1[0] = 0.01; 
    x1[1] = 0.02;
    x1[2] = 0.03, 
    x1[3] = 0.04, 
    x1[4] = 0.01; 
    x1[5] = 0.02; 
    x1[6] = 0.03; 
    x1[7] = 0.04; 
    x1[8] = 0.01; 
    x1[9] = 0.02; 
    x2[0] = 0.01; 
    x2[1] = 0.02;
    x2[2] = 0.03, 
    x2[3] = 0.04; 
    x2[4] = 0.01; 
    x2[5] = 0.02; 
    x2[6] = 0.03; 
    x2[7] = 0.04; 
    x2[8] = 0.01; 
    x2[9] = 0.02; 
    x3[0] = 0.01; 
    x3[1] = 0.02;
    x3[2] = 0.03, 
    x3[3] = 0.04; 
    x3[4] = 0.01; 
    x3[5] = 0.02; 
    x3[6] = 0.03; 
    x3[7] = 0.01; 
    x3[8] = 0.02; 
    x3[9] = 0.03; 
    x4[0] = 0.01; 
    x4[1] = 0.02;
    x4[2] = 0.03, 
    x4[3] = 0.04; 
    x4[4] = 0.01; 
    x4[5] = 0.02; 
    x4[6] = 0.03; 
    x4[7] = 0.04; 
    x4[8] = 0.01; 
    x4[9] = 0.01; 
    printSignal(x1, nObs); 
    printSignal(x2, nObs);
    printSignal(x3, nObs);
    printSignal(x4, nObs);
    PRINTLINE
    printf("Trying to send data to the CNA\n"); 
    float64 rate = 10.0;
    uInt64 nSamplePerChannel = 10;
    resPlay = ni9263_play_4ch(x1, x2, x3, x4, nObs, rate, nSamplePerChannel); 
    printf("resPlay: %6i\n", resPlay); 
    PRINTLINE
    free(x1); 
    free(x2); 
    free(x3); 
    free(x4); 
    return 0; 
}

int main() {
    testPTP(); 
    return 0; 
}

