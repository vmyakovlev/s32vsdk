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
* \file c_2d_matrix.h
* \brief Declaration of C2DMatrix class: container for 2D data of any type with convenience operators.
* \author Anca Dima
* \version
* \date 11-February-2014
****************************************************************************/

#ifndef C2DMATRIX_H
#define C2DMATRIX_H
//#include <memory.h>
#include <stdlib.h>

namespace MatrixNS{

	template <typename baseType>
	class C2DMatrix {


	protected:
		int			m_nx;
		int			m_ny;
		bool		m_isBound; // true if the data pointer points to memory which was allocated and is managed outside this class
		baseType	**m_data; // This is a vector of pointers to a contigous data vector

	public:
		
		/****************************************************************************/
		/**  Empty constructor. 
		*		
		* \note Does not allocate memory
		*
		*****************************************************************************/
		C2DMatrix();

		
		/****************************************************************************/
		/** Constructor
		*
		* \note Allocates yW rows, each xW columns wide. Does no initialization or setting to zero
		*
		*****************************************************************************/
		C2DMatrix(int xW, int yW);

		/****************************************************************************/
		/** Copy Constructor
		*
		* \note Allocates yW rows, each xW columns wide. Does no initialization or setting to zero
		*
		*****************************************************************************/
		C2DMatrix(const C2DMatrix& toBeCopied);

		/****************************************************************************/
		/** Destructor
		*
		* \note If data is bound, it does not deallocate the principal memory vector, but only the vector of pointers to rows
		*
		*****************************************************************************/
		virtual ~C2DMatrix();

		/****************************************************************************/
		/** Explicit memory deallocation
		*
		* \note If data is bound, it does not deallocate the principal memory vector, but only the vector of pointers to rows
		*
		*****************************************************************************/
		void DeallocateMem();

		/****************************************************************************/
		/** Explicit memory allocation
		* \param xW, yW: width and height of the matrix
		* \note If old data has a different size, it deallocates old memory.
		*
		*****************************************************************************/
		void AllocateMem(int xW, int yW);

		/****************************************************************************/
		/** Memory initialization with a given value
		* \param val the value which will be spread across the whole allocated memory
		* \note If memory is not yet allocated, this function will allocate it
		*
		*****************************************************************************/
		void Init(baseType val);

		
		/****************************************************************************/
		/** Copy data from a vector into current memory space
		* \param dataToBeCopied: the data vector from which we will copy.  
		* \warning dataToBeCopied should have the same size as "this" is expecting!
		* \note If memory is not yet allocated, this function will allocate it. 
		*
		*****************************************************************************/
		void CpyData(const baseType* dataToBeCopied);
		void CpyData(const baseType** dataToBeCopied);
		
		/****************************************************************************/
		/** Bind data from a vector into current object, without copying it. 
		*
		* \note If memory is already allocated, the old data vector will be deallocated. 
		* \warning vectors should have the same size
		*
		*****************************************************************************/
		void BindData(baseType* data);

		/****************************************************************************/
		/** OPERATORS
		*****************************************************************************/

		/****************************************************************************/
		/** Index operator. Gives access to matrix rows by index
		* \param: idx - the index of the row to be returned
		* \note Index should be in bounds. Check done only in debug mode
		*****************************************************************************/
		virtual baseType* operator [] (int idx);
		virtual const baseType* operator [] (int idx) const;

		/****************************************************************************/
		/** Sums up all elements of the matrix.
		* \param nrNonZeroElems: returns the number of non zero elements which were summed up
		* \note Result could be out of the range of the baseType. Therefore returns a float
		*
		*****************************************************************************/
		float SumElems(int& nrNonZero) const;

		/****************************************************************************/
		/** TBD: Other Operators: Sum of two Matrices, Difference of two Matrices, 
		* Unary Negation, Multiplication of two Matrices, Scaling, Min, Max, NrNonZeroElems
		* Mean, MeanOfNonZero, StdDev, StdDevOfNonZero, Power, etc...
		*****************************************************************************/
		C2DMatrix& operator += (const C2DMatrix& arOther);
		C2DMatrix& operator *= (const C2DMatrix& arOther);
		C2DMatrix& operator -= (const C2DMatrix& arOther);

		/* does not work with arm compiler....
#define CASS_OP_DEC(OPERATOR) C2DMatrix& operator##OPERATOR (const C2DMatrix& arOther);

		CASS_OP_DEC(+= )
		CASS_OP_DEC(-= )
		CASS_OP_DEC(*= )
		//CASS_OP_DEC(/= )
		CASS_OP_DEC(%= )
		CASS_OP_DEC(&= )
		CASS_OP_DEC(|= )
		CASS_OP_DEC(^= )
*/

		/****************************************************************************/
		/** Getter: width, height, size (in bytes), nrElems (= width*height), data, dataVect
		*****************************************************************************/
		inline int width() const { return m_nx; }
		inline int height() const { return m_ny; }
		inline size_t size() const {return size_t(m_nx*m_ny*sizeof(baseType));}
		inline int nrElems() const { return m_nx*m_ny; }

		inline virtual baseType** data()				{ return m_data;}
		inline virtual const baseType** data() const 	{ return (const baseType**) m_data; }
		inline virtual baseType* dataVect()				{ return m_data != NULL ? m_data[0] : NULL; }
		inline virtual const baseType* dataVect() const 	{ return (const baseType*) m_data != NULL ? m_data[0] : NULL; }

	};

}
#endif /* C2DMATRIX_H */
