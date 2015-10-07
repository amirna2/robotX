/*
 * Mutex.h
 *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file defines a Mutex class as a wrapper around posix pthread_mutex
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include "pthread.h"

class Mutex
{
public:
  Mutex();
  virtual ~Mutex();
  void lock();
  void unlock();

protected:
  pthread_mutex_t mMutex;
  pthread_mutexattr_t mAttr;
};

#endif /* MUTEX_H_ */
