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
/**
* \file c_2d_matrix.hpp
* \brief Implementation of  template C2DMatrix class
* \author Anca Dima
* \version
* \date 11-February-2014
****************************************************************************/

#ifndef C2DMATRIX_HPP
#define C2DMATRIX_HPP
#include <assert.h>
#include <stdlib.h>
#include <assert.h>
#include "utils_2d_matrix.h"

namespace MatrixNS{

	template <class baseType>
	C2DMatrix<baseType>::C2DMatrix()
		: m_nx(0)
		, m_ny(0)
		, m_isBound(false)
		, m_data(NULL)
	{}

	template <class baseType>
	C2DMatrix<baseType>::C2DMatrix(int xW, int yW)
		: m_nx(xW)
		, m_ny(yW)
		, m_isBound(false)
		, m_data(NULL)
	{
		AllocateMem(xW, yW);
	}

	template <class baseType>
	C2DMatrix<baseType>::C2DMatrix(const C2DMatrix<baseType>& toBeCopied)
		: m_nx(toBeCopied.m_nx)
		, m_ny(toBeCopied.m_ny)
		, m_isBound(false)
		, m_data(NULL)
	{
		CpyData(toBeCopied.dataVect());		
	}

	template <class baseType>
	C2DMatrix<baseType>::~C2DMatrix() 
	{
		DeallocateMem();
	}

	template <class baseType>
	void C2DMatrix<baseType>::DeallocateMem() 
	{
		if (m_data == NULL)
			return;

		if (!m_isBound) { // delete all the data
			delete[] m_data[0]; //it's a contigous memory chunk				
		}
		
		delete[] m_data;		
		m_data = NULL;
		m_nx = 0;
		m_ny = 0;
	}

	template <class baseType>
	void C2DMatrix<baseType>::Init(baseType val) 
	{
		if (!m_data && m_nx + m_ny > 0) {			
			AllocateMem(m_nx, m_ny);
		}

		memset(m_data[0], val, size());
	}


	template <class baseType>
	void C2DMatrix<baseType>::AllocateMem(int xW, int yW) 
	{
		if (m_data != 0) {
			if (m_nx == xW && m_ny == yW && !m_isBound) {
				return;
			}
			else {
				DeallocateMem();
			}
		}

		m_nx = xW;
		m_ny = yW;

		m_data = new baseType*[m_ny];
		m_data[0] = new baseType[nrElems()]; // it's a contigous memory chunk, with pointers to each row
		baseType* ptr = m_data[0];
		for (int j = 0; j < m_ny; ++j, ptr += m_nx){
			m_data[j] = ptr;
		}
	}
		
	template <class baseType>
	void C2DMatrix<baseType>::CpyData(const baseType* dataToBeCopied)
	{
		size_t sz = size();
		
		assert(sz > 0);
		if (m_data == NULL)
			AllocateMem(m_nx, m_ny);

		memcpy(m_data[0], dataToBeCopied, sz);
	}

	template <class baseType>
	void C2DMatrix<baseType>::CpyData(const baseType** dataToBeCopied)
	{
		size_t rowSz = (m_nx *sizeof(baseType));
			
		assert(rowSz > 0);

		if (m_data == NULL)
			AllocateMem(m_nx, m_ny);

		for (int i = 0; i < m_ny; ++i) {
			memcpy(m_data[i], dataToBeCopied[i], rowSz);
		}			
	}

	template <class baseType>
	void C2DMatrix<baseType>::BindData(baseType* dataToBeBound) {
		size_t matrixSz = size();

		assert(matrixSz > 0); 
		m_isBound = true;

		if (m_data[0] != NULL) {
			delete [] m_data[0];
		}

		m_data[0] = dataToBeBound; // it's a contigous memory chunk, with pointers to each row
		baseType* ptr = m_data[0];
		for (int j = 0; j < m_ny; ++j, ptr += m_nx){
			m_data[j] = ptr;
		}
	}


	template <class baseType>
	baseType* C2DMatrix<baseType>::operator [] (int idx) 
	{
		assert(idx >= 0 && idx < m_ny); //TBD: throw an exception if  not
		if (m_data != NULL)
			return m_data[idx];
		else
			return NULL;
	}


	template <class baseType>
	const baseType* C2DMatrix<baseType>::operator [] (int idx) const
	{
		assert(idx >= 0 && idx < m_ny); //TBD: throw an exception if  not
		if (m_data != NULL)
			return m_data[idx];
		else
			return NULL;
	}

	template <class baseType>
	float C2DMatrix<baseType>::SumElems(int& nrNonZeroElems) const
	{
		
		float sum = 0; // In order not to cause overflow
		nrNonZeroElems = 0;
		
		if (m_data == 0)
			return 0;

		for (baseType j = 0; j < m_ny; ++j){
			for (baseType i = 0; i < m_nx; ++i){
				if (m_data[j][i] != 0){
					++nrNonZeroElems;
					sum += m_data[j][i];
				}
			}
		}

		return sum;
	}

	template <class baseType>
	C2DMatrix<baseType>& C2DMatrix<baseType>::operator += (const C2DMatrix<baseType>& arOther) 
	{
		for (int i = 0; i < m_ny; ++i)
		{
			for (int j = 0; j < m_nx; ++j)
			{
				(*this)[i][j] += arOther[i][j]; 
			}
		}
		return *this; 
	}

	template <class baseType>
	C2DMatrix<baseType>& C2DMatrix<baseType>::operator *= (const C2DMatrix<baseType>& arOther)
	{
		for (int i = 0; i < m_ny; ++i)
		{
			for (int j = 0; j < m_nx; ++j)
			{
				(*this)[i][j] *= arOther[i][j];
			}
		}
		return *this;
	}

	template <class baseType>
	C2DMatrix<baseType>& C2DMatrix<baseType>::operator -= (const C2DMatrix<baseType>& arOther)
	{
		for (int i = 0; i < m_ny; ++i)
		{
			for (int j = 0; j < m_nx; ++j)
			{
				(*this)[i][j] -= arOther[i][j];
			}
		}
		return *this;
	}

	/*
#define CASS_OP_DEF(OPERATOR) template <class baseType>\
	C2DMatrix<baseType>& C2DMatrix<baseType>::operator##OPERATOR(const C2DMatrix<baseType>& arOther) \
	{\
	for (int i = 0; i < m_ny; ++i)\
	{\
	for (int j = 0; j < m_nx; ++j)\
	{\
	(*this)[i][j] OPERATOR arOther[i][j]; \
	}\
	}\
	return *this; \
	}\


	CASS_OP_DEF(+= )
	CASS_OP_DEF(-= )
	CASS_OP_DEF(*= )
	//CASS_OP_DEF(/= )
	CASS_OP_DEF(%= )
	CASS_OP_DEF(&= )
	CASS_OP_DEF(|= )
	CASS_OP_DEF(^= )

#undef CASS_OP_DEC
#undef CASS_OP_DEF
*/
}// end of namespace
#endif /* C2DMATRIX_HPP */
