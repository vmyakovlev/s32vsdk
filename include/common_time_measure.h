/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
#ifndef TIMEMEASURE_H
#define TIMEMEASURE_H

#include <stdint.h>
#include <vector>
#include "common_config_time_measure.h"

#ifdef DEF_TIME_MEASURE // define it if APEX PROCESSING time has to be measured
extern uint64_t total_apex_ticks;
extern uint64_t total_load_ticks;
#endif

uint64_t FSL_Ticks();
double FSL_GetFrequency();
double FSL_TicksToSeconds(uint64_t ticks);


#ifdef __STANDALONE__
//extern SEQ_Buf_t  producerless_buffer_1;
extern "C" {
	unsigned long get_uptime_microS(void);
}

#define GETTIME(tm)   (*(tm)=get_uptime_microS())
#else // ifdef __STANDALONE__
#ifndef APEX2_EMULATE
#include <sys/time.h>

#define GETTIME(tm) \
  { \
  struct timeval lTime; gettimeofday(&lTime,0); \
  *tm=(lTime.tv_sec*1000000+lTime.tv_usec);   \
  }
#else
#define GETTIME(tm) *tm = unsigned long(FSL_TicksToSeconds(FSL_Ticks())*1000000)
#endif

#endif // else from #ifdef __STANDALONE__

template <class T>
class MovingAvg
{
public:
  
  MovingAvg(unsigned int number_of_elements);
  
  ~MovingAvg();
  
  T GetNext(T actual_value);
private:
  
  unsigned int number_of_elements;
  
  std::vector<T> v;
};

template <class T>
T MovingAvg<T>::GetNext(T actual_value)
{
    v.insert(v.begin(),actual_value);
    if(v.size() > number_of_elements)
      v.pop_back();
    
    T sum = 0;
    
    for(unsigned int i = 0; i < v.size(); i++)
      sum += v[i];
    
    return sum/(T)v.size();
}

template <class T>
MovingAvg<T>::MovingAvg(unsigned int number_of_elements)
{
    this->number_of_elements = number_of_elements;
}

template <class T>
MovingAvg<T>::~MovingAvg(){}


#endif /* TIMEMEASURE_H */


