/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
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
#ifndef CBASEPROCINIT_H
#define CBASEPROCINIT_H

#include <iostream>
#include <string>

using namespace std;


#ifdef APEX2_EMULATE
#include "acf_lib.hpp"
using namespace APEX2;
#else
#include <acf_process_apu.h>
#endif

#include "common_time_measure.h"
#include "common_stringify_macros.h"



/// Base class for composite processes (i.e. having more than one process). It defines the basic interface.
class CBase_CompositeProc {

private:
	int64_t m_apex_ticks0;		/// ticks before starting the run 
	int64_t m_apex_ticks1;		/// ticks after end of run
	bool	m_addToGlobal;		/// if true, then add the tick differences to the global variables


protected:
	int32_t m_errVal;

public:
	int64_t m_apexTotalTicks;

public:
	CBase_CompositeProc()
		: m_apex_ticks0(0)
        , m_apex_ticks1(0)
        , m_addToGlobal(false)
        , m_errVal(0)
        , m_apexTotalTicks(0)
	{}
	virtual ~CBase_CompositeProc(){}
	virtual int32_t Connect() = 0;
	virtual int32_t Start() = 0;
	virtual int32_t Wait() = 0;
	virtual int32_t Exec() = 0;
	virtual std::string ErrorMessage() = 0;

	virtual int32_t Exec(std::string
	                 #ifdef APEX2_EMULATE
	                   graphName
                         #endif
			) {

#ifdef APEX2_EMULATE
		std::cout << "\nExecuting graph: " << graphName << std::endl;
#endif

#ifdef DEF_TIME_MEASURE
		m_apex_ticks1 = 0;
		m_apex_ticks0 = FSL_Ticks();
#endif		
		m_errVal |= Start();
		m_errVal |= Wait();


#ifdef DEF_TIME_MEASURE
		m_apex_ticks1 = FSL_Ticks();
		m_apexTotalTicks += m_apex_ticks1 - m_apex_ticks0; 
#endif


#ifdef APEX2_EMULATE
		if (m_errVal) {
			std::cout << graphName << "Execution error" << std::endl;
			std::cout << ErrorMessage() << std::endl;
			char c;
			std::cin >> c;

		}
#endif

		return m_errVal;
	}

	int32_t Create_and_exec() {
		int lretVal = Connect();

		if (lretVal == 0)
			lretVal = Exec();

		return m_errVal;
	}

	virtual int32_t Error() {
		return m_errVal;
	}

};


/// Template base class for a single process. Defines the interface and basic operations
/// ACF_Proc is the template for one of the generated process classes
template <class ACF_Proc>
class CBase_ProcInit : public ACF_Proc {
protected:
	int32_t m_errVal;

#ifdef DEF_TIME_MEASURE
	uint64_t m_apex_ticks0;		/// ticks before starting the run 
	uint64_t m_apex_ticks1;		/// ticks after end of run
	bool	m_addToGlobal;		/// if true, then add the tick differences to the global variables

#ifndef APEX2_EMULATE
public:
	AcfProfilingInfo m_profInfo;
	uint64_t m_apexTotalTicks;
	uint64_t m_apexLoadTicks;
#endif

#endif

public:
        CBase_ProcInit()
		:
#ifdef APEX2_EMULATE
		ACF_Proc((APEX_ID)APEX_APEX0)
#else
		ACF_Proc(0)
#endif
		, m_errVal(false)

#ifdef DEF_TIME_MEASURE
        , m_apex_ticks0(0)
        , m_apex_ticks1(0)
        , m_addToGlobal(true)
				
#ifndef APEX2_EMULATE
		, m_apexTotalTicks(0)
		, m_apexLoadTicks(0)
#endif
#endif
	{
	}
	
	CBase_ProcInit(int apex_id)
		:
#ifdef APEX2_EMULATE
		ACF_Proc((APEX_ID)apex_id)
#else
		ACF_Proc(apex_id)
#endif
		, m_errVal(false)

#ifdef DEF_TIME_MEASURE
        , m_apex_ticks0(0)
        , m_apex_ticks1(0)
		, m_addToGlobal(true)
#ifndef APEX2_EMULATE
		, m_apexTotalTicks(0)
		, m_apexLoadTicks(0)
#endif

#endif
	{
	}

	CBase_ProcInit(bool addGlobal, int apex_id)
		:
#ifdef APEX2_EMULATE
		ACF_Proc((APEX_ID)apex_id)
#else
		ACF_Proc(apex_id)
#endif
		, m_errVal(false)

#ifdef DEF_TIME_MEASURE
		, m_addToGlobal(addGlobal)
		, m_apex_ticks1(0)
#ifndef APEX2_EMULATE
		, m_apexTotalTicks(0)
		, m_apexLoadTicks(0)
#endif

#endif
	{
	}

	virtual ~CBase_ProcInit(){}
	virtual int32_t Connect() = 0;
	virtual int32_t Exec() {

#ifdef APEX2_EMULATE
		cout << "\nExecuting graph: " << ACF_Proc::RetProcessIdentifier() << endl;
#endif


#ifdef DEF_TIME_MEASURE
		m_apex_ticks1 = 0;
		m_apex_ticks0 = FSL_Ticks();
#endif		
			m_errVal |= ACF_Proc::Start();
			m_errVal |= ACF_Proc::Wait();


#ifdef DEF_TIME_MEASURE
		m_apex_ticks1 = FSL_Ticks();
#endif

#ifdef DEF_TIME_MEASURE
#ifdef APEX2_EMULATE
		if (m_addToGlobal) {
			total_apex_ticks += getRunTicks();
			total_load_ticks += getLoadTicks();
		}		
#else
		m_profInfo = ACF_Proc::RetAcfProfilingInfo();
		uint64_t loadTcks = m_profInfo.host_start + m_profInfo.apu_idle + m_profInfo.apu_init;
		if (m_addToGlobal) {
			total_load_ticks += loadTcks;
			total_apex_ticks += m_profInfo.apu_processing;
			m_apexTotalTicks += m_profInfo.apu_processing;
			m_apexLoadTicks += loadTcks;

		}
		else {
			m_apex_ticks1 = m_apex_ticks0 + m_profInfo.apu_processing;
			m_apexTotalTicks += m_profInfo.apu_processing;
			m_apexLoadTicks += loadTcks;
		}
#endif		
#endif

#ifdef APEX2_EMULATE
		if (m_errVal) {
			std::cout << ACF_Proc::RetProcessIdentifier() << " execution error" << endl;
			std::cout << ErrorMessage() << endl;
			char c;
			std::cin >> c;
		}
#endif
		return m_errVal;
	}

	virtual int32_t Start() {

#ifdef APEX2_EMULATE
		cout << "\nStarting graph: " << ACF_Proc::RetProcessIdentifier() << endl;
#endif


#ifdef DEF_TIME_MEASURE
		m_apex_ticks0 = 0;
		m_apex_ticks1 = 0;
#endif		

		m_errVal |= ACF_Proc::Start();

#ifdef DEF_TIME_MEASURE
		m_apex_ticks0 = FSL_Ticks();
		total_load_ticks += getLoadTicks();
#endif			
		return m_errVal;
	}

	


	virtual int32_t Wait() {
		if (!m_errVal) {
			m_errVal |= ACF_Proc::Wait();


#ifdef DEF_TIME_MEASURE			
			m_apex_ticks1 = FSL_Ticks();			
#endif
			
			
		}
#ifdef DEF_TIME_MEASURE
#ifdef APEX2_EMULATE		
		if (m_addToGlobal) {
			total_apex_ticks += getRunTicks();
			//std::cout << "\nWait_End(): TIME MEASURED FOR APEX " << apex_ticks << ", and TOTAL : " << total_apex_ticks << " at time stamp "<< FSL_Ticks() << endl;
		}
		

#else
		m_profInfo = ACF_Proc::RetAcfProfilingInfo();
		uint64_t loadTcks = m_profInfo.host_start + m_profInfo.apu_idle + m_profInfo.apu_init;
		if (m_addToGlobal) {
			total_load_ticks += loadTcks;
			total_apex_ticks += m_profInfo.apu_processing;
			m_apexTotalTicks += m_profInfo.apu_processing;
			m_apexLoadTicks += loadTcks;
		}
		else {
			m_apex_ticks1 = m_apex_ticks0 + m_profInfo.apu_processing;
			m_apexTotalTicks += m_profInfo.apu_processing;
			m_apexLoadTicks += loadTcks;
		}
#endif

#endif		


#ifdef APEX2_EMULATE
		if (m_errVal) {
			std::cout << ACF_Proc::RetProcessIdentifier() << ": execution error" << endl;
			std::cout << ErrorMessage() << endl;
			char c;
			std::cin >> c;

		}
#endif
		return m_errVal;

	}


	virtual std::string ErrorMessage(){
#ifdef APEX2_EMULATE
		return ACF_Proc::ErrorMessage();
#else
		return ("");
#endif
	}

	virtual int32_t Error() {
		return m_errVal;
	}



	uint64_t getLoadTicks() {
#if defined(DEF_TIME_MEASURE) && !defined(APEX2_EMULATE)
		return m_profInfo.host_start + m_profInfo.apu_idle + m_profInfo.apu_init;
#else 
		return (int64_t)0;
#endif
	}

	uint64_t getRunTicks() {
#ifdef DEF_TIME_MEASURE
		return m_apex_ticks1 - m_apex_ticks0;
#else 
		return (int64_t)0;
#endif

	}
};

#endif /* CBASEPROCINIT_H */

