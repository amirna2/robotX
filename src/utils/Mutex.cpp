/*
 * Mutex.cpp
 *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file provides an implementation of Mutex class define in Mutex.h
 */

#include "Mutex.h"

using namespace std;

Mutex::Mutex()
{
  pthread_mutexattr_init(&mAttr);
  pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&mMutex, &mAttr);
}

Mutex::~Mutex()
{
  pthread_mutex_destroy(&mMutex);
}

void Mutex::lock()
{
  pthread_mutex_lock(&mMutex);
}

void Mutex::unlock()
{
  pthread_mutex_unlock(&mMutex);
}
