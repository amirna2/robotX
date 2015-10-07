/*
 * Thread.h
 *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file defines a Thread class as a wrapper around posix pthreads
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <bits/local_lim.h>
#include <pthread.h>
#include "TestBotTypes.h"

#define MIN_STACK_SIZE PTHREAD_STACK_MIN

class Thread
{
public:
  Thread();
  Thread(bool detach, int32_t size);

  virtual ~Thread();
  uint32_t tid() const;
  void start(void *arg = NULL);
  void join();
  int32_t yield();
  virtual void run() = 0;

protected:
  pthread_attr_t mAttr;
  bool  mStarted;
  void* mArg;
  static void *exec(void *thr);

private:
  pthread_t mId;
  void setAttributes(bool detach, int size);
  Thread(const Thread& arg);
  Thread& operator=(const Thread& rhs);
};

#endif /* THREAD_H_ */
