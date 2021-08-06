#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <pthread.h>

//Semaphore class that includes a constructor, destructor, wait, and signal functions
//also contains private variables including a mutex, condition, and a count that can
//be initialized or changed by the functions mentioned

class Semaphore 
{
public:

	Semaphore(int count);
	~Semaphore();

	void wait();
	void signal();

private:
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int count;

};

#endif



