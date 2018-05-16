/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
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
/**
* \file filter.h
* \brief Declaration of Filter class. Defines a filter object from statically declared 2D coefficients
* \author Anca Dima
* \version
* \date 11-February-2014
****************************************************************************/

#ifndef FILTER_H
#define FILTER_H
//#include <memory.h>
#include "utils_2d_matrix.hpp"


namespace Convolution {


	template <unsigned short xW, unsigned short yW, class  baseType>
	class CFilter : public MatrixNS::C2DMatrix<baseType> {

	protected:
		baseType m_scalingFact; //factor to divide with after convolution (is provided by the user)
		baseType m_norm;		// sum of all filter elemetns (can be used instead of the scaling factor)
		bool	 m_isSymX;		// true if filter is symmetric in X direction
		bool	 m_isSymY;		// true if filter is symmetric in Y direction
		bool	 m_isAntiSymX;	// true if filter is anti-symmetric in X direction
		bool	 m_isAntiSymY;  // true if filter is anti-symmetric in Y direction

	protected: 
		/****************************************************************************/
		/** Needed by the constructor to copy the filter coefficients from data into the memory
		* \param data - the filter coefficients to be set into the CFilter class
		* \note Precondition: memory should already be allocated
		*
		*****************************************************************************/
		void CopyData(const baseType data[yW][xW]);

	public:

		/****************************************************************************/
		/** Constructor
		*
		* \note Allocates yW rows, each xW columns wide. Does no initialization or setting to zero
		*
		*****************************************************************************/
		CFilter();


		/****************************************************************************/
		/** Constructor. Sets also filter coefficients. Computes the norm of the filter coefficients 
		* \param data - the filter coefficients to be set into the CFilter class
		* \note Allocates yW rows, each xW columns wide. Initializes internal data to the values in parameter "data"
		*
		*****************************************************************************/
		CFilter(const baseType data[yW][xW]);


		/****************************************************************************/
		/** Constructor. Sets filter coefficients and scaling factor
		* \param scalingFact - factor to divide the result, after convolving with the filter
		* \param data - the filter coefficients to be set into the CFilter class
		* \note Allocates yW rows, each xW columns wide. Initializes internal data to the values in parameter "data"
		*
		*****************************************************************************/
		CFilter(baseType scalingFact, const baseType data[yW][xW]);

		/****************************************************************************/
		/** Copy Constructor. 
		* \param toBeCopied - the filter to be copied
		* \note Allocates yW rows, each xW columns wide. 
		*
		*****************************************************************************/
		CFilter(const CFilter& toBeCopied);

		/****************************************************************************/
		/** Computes the sum of the filter coefficients. 
		* \note Can be used instead of the scaling factor
		*
		*****************************************************************************/
		baseType CalcNorm() const;

		/****************************************************************************/
		/** Returns a scaling factor set from outside.
		* \note If no scaling factor was set (i.e. it is 0) then the norm of the filter coefficients is returned
		*
		*****************************************************************************/
		inline baseType ScalingFact() const { return m_scalingFact == 0 ? m_norm : m_scalingFact; }


		/****************************************************************************/
		/** Computes internally, if filter coefficients are symmetric in X and Y direction.
		*
		*****************************************************************************/
		void ComputeSymmetry();



		/****************************************************************************/
		/** Getter
		****************************************************************************/
		inline unsigned short isSymX() const { return (unsigned short) m_isSymX; }
		inline unsigned short isSymY() const { return (unsigned short) m_isSymY; }
		inline unsigned short isAntiSymX() const { return (unsigned short) m_isAntiSymX; }
		inline unsigned short isAntiSymY() const { return (unsigned short) m_isAntiSymY; }

		
		/****************************************************************************/
		/** OPERATORS
		****************************************************************************/
		CFilter& operator += (const CFilter& arOther);
		CFilter& operator -= (const CFilter& arOther);
		CFilter& operator *= (const CFilter& arOther);
		/*
#define CASS_OP_DEC(OPERATOR) CFilter& operator##OPERATOR (const CFilter& arOther);

		CASS_OP_DEC(+= )
		CASS_OP_DEC(-= )
		CASS_OP_DEC(*= )
		//CASS_OP_DEC(/= )
		CASS_OP_DEC(%= )
		CASS_OP_DEC(&= )
		CASS_OP_DEC(|= )
		CASS_OP_DEC(^= )
	*/
	};

	extern const CFilter<3, 3, short> gaussFilter3x3;
	extern const CFilter<5, 5, short> gaussFilter5x5;
	extern const CFilter<3, 1, short> gradientXFilter;
	extern const CFilter<1, 3, short> gradientYFilter;
	extern const CFilter<3, 3, short> gradientXYFilter;
	extern const CFilter<3, 3, short> scharrXFilter;
	extern const CFilter<3, 3, short> scharrYFilter;

}

#endif /* FILTER_H */