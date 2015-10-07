/*
 * Condition.cpp
 *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file implements Condition class defined in Condition.h
 */
#include "Mutex.h"
#include "Condition.h"

Condition::Condition()
{
  pthread_cond_init(&mCond, NULL);
}

Condition::~Condition()
{
  pthread_cond_destroy(&mCond);
}

void Condition::wait()
{
  pthread_cond_wait(&mCond, &mMutex);
}

void Condition::notify()
{
  pthread_cond_signal(&mCond);
}



