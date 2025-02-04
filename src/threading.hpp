/*
* Copyright (c) 2008, AMT – The Association For Manufacturing Technology (“AMT”)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the AMT nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
* BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
* AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
* RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
* (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
* WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
* LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. 

* LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
* PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
* OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
* CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
* WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
* THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
* SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#ifndef THREADING_HPP
#define THREADING_HPP

#ifdef THREADED

#ifndef WIN32
#include <pthread.h>
#endif

class MTCMutex 
{
public:
  MTCMutex() {
#ifdef WIN32
  InitializeCriticalSection(&mLock);
#else
  pthread_mutex_init(&mLock, NULL);
#endif
  }
  
  ~MTCMutex() {
#ifdef WIN32
  DeleteCriticalSection(&mLock);
#else
  pthread_mutex_destroy(&mLock);
#endif  
  }

  void lock() {
#ifdef WIN32
  EnterCriticalSection(&mLock);
#else
  pthread_mutex_lock(&mLock);
#endif
  }
  
  void unlock() {
#ifdef WIN32
  LeaveCriticalSection(&mLock);
#else
  pthread_mutex_unlock(&mLock);
#endif    
  }
  
protected:
#ifdef WIN32
  CRITICAL_SECTION mLock;
#else
  pthread_mutex_t mLock;
#endif

private:
  MTCMutex(const MTCMutex& aMutex) {
    // No copy constructor is possible
  }
};

#else // Not threaded

class MTCMutex {
public:
    void lock() {}

    void unlock() {}
};

#endif

class MTCAutoLock {
public:
    MTCAutoLock(MTCMutex &aMutex) : mMutex(&aMutex) {
        mMutex->lock();
    }

    ~MTCAutoLock() {
        mMutex->unlock();
    }

protected:
    MTCMutex *mMutex;

private:
    MTCAutoLock() : mMutex(NULL) {}
};

#endif