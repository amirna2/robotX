/*
 * Condition.h
 *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file defines a Condition class as a wrapper around posix pthread conditions
 */

#ifndef CONDITION_H_
#define CONDITION_H_

#include "Mutex.h"

class Condition : public Mutex
{
public:
  Condition();
  virtual ~Condition();
  void wait();
  void notify();

protected:
  pthread_cond_t mCond;

  // Prevent copying or assignment
  //Condition(const Condition& arg);
  //Condition& operator=(const Condition& rhs);

};


#endif /* CONDITION_H_ */
