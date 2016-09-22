#ifndef __TOM_UTILS_FUNCTIONS__
#define __TOM_UTILS_FUNCTIONS__

	#include <csignal>
	#include <sys/time.h>
	#include <cstdint>
	#include <cstdlib>
	#include <cstdio>
	#include <thread>
	
	#define sigev_notify_thread_id _sigev_un._tid
	
	//--	print a buffer in HEX
	void printBuffer(const uint8_t* data, size_t len){
		for (int i=0;i<len;i++){
			printf("%02x ", data[i]);
		}
		printf("\n");
	}

	//--	block and wait for the presence of a signal
	//--	return FALSE if the waiting is interrupted by a SIGINT
	bool waitSignal(int waitingSignal){
		sigset_t sig;
		sigemptyset(&sig);
		sigaddset(&sig, waitingSignal);
		sigaddset(&sig, SIGINT);	//--	interupt case
		
		int caughtSignal;
		sigwait(&sig, &caughtSignal);	
		return waitingSignal == caughtSignal;
	}

	//--	timeoutSignal and waitingSignal must be differrent, otherwise the behavior is undefined
	//--	return TRUE if caught signal is correct, otherwise return FALSE
	bool waitSignal(int waitingSignal, int timeoutSignal, int timeout){
		sigset_t sig;
		sigemptyset(&sig);
		sigaddset(&sig, waitingSignal);
		sigaddset(&sig, timeoutSignal);
		sigaddset(&sig, SIGINT);	
		
		timer_t timer;
		struct sigevent evt;
		evt.sigev_notify = SIGEV_SIGNAL;
		evt.sigev_signo = timeoutSignal;
		evt.sigev_notify_thread_id = pthread_self();
		timer_create(CLOCK_REALTIME, &evt, &timer);
		
		struct itimerspec time;
		time.it_interval.tv_sec=0;
		time.it_interval.tv_nsec=0;	
		time.it_value.tv_sec=timeout/1000;
		time.it_value.tv_nsec=(timeout - time.it_value.tv_sec*1000)*1000000;	
		timer_settime(timer, 0, &time, NULL);
		
		//--	wait for either timeoutSignal or watingSignal, or SIGINT
		int caughtSignal;
		sigwait(&sig, &caughtSignal);
		
		return caughtSignal == waitingSignal;	
	}
	
#endif
