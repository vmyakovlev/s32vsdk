/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
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
/**
* \file filter.h
* \brief Implementation of Filter class 
* \author Anca Dima
* \version
* \date 11-February-2014
****************************************************************************/

#ifndef CFILTER_HPP
#define CFILTER_HPP

#include <string.h>
#include "utils_2d_matrix.hpp"
#include "utils_filter.h"
using namespace MatrixNS;

namespace Convolution {
	template <unsigned short xW, unsigned short yW, class  baseType>
	CFilter<xW, yW, baseType>::CFilter()
		: MatrixNS::C2DMatrix<baseType>(xW, yW)
		, m_scalingFact(0)
		, m_norm(0)
		, m_isSymX(false)
		, m_isSymY(false)
		, m_isAntiSymX(false)
		, m_isAntiSymY(false)
	{}

	template <unsigned short xW, unsigned short yW, class  baseType>
	CFilter<xW, yW, baseType>::CFilter(const baseType dat[yW][xW])
		: MatrixNS::C2DMatrix<baseType>(xW, yW)
		, m_scalingFact(1)
		, m_norm(0)
		, m_isSymX(false)
		, m_isSymY(false)
		, m_isAntiSymX(false)
		, m_isAntiSymY(false)
	{
		CopyData(dat);
		m_norm = CalcNorm();
		ComputeSymmetry();
	}


	template <unsigned short xW, unsigned short yW, class  baseType>
	CFilter<xW, yW, baseType>::CFilter(baseType scalingFact, const baseType dat[yW][xW])
		: MatrixNS::C2DMatrix<baseType>(xW, yW)
		, m_scalingFact(scalingFact)
		, m_norm(0)
		, m_isSymX(false)
		, m_isSymY(false)
		, m_isAntiSymX(false)
		, m_isAntiSymY(false)

	{
		CopyData(dat);
		m_norm = CalcNorm();

		ComputeSymmetry();
	}


	template <unsigned short xW, unsigned short yW, class  baseType>
	CFilter<xW, yW, baseType>::CFilter(const CFilter& toBeCopied)
		: MatrixNS::C2DMatrix<baseType>(toBeCopied)
		, m_scalingFact(toBeCopied.m_scalingFact)
		, m_norm(toBeCopied.m_norm)
		, m_isSymX(toBeCopied.m_isSymX)
		, m_isSymY(toBeCopied.m_isSymY)
		, m_isAntiSymX(toBeCopied.m_isAntiSymX)
		, m_isAntiSymY(toBeCopied.m_isAntiSymY)
	{
	}

	template <unsigned short xW, unsigned short yW, class  baseType>
	void CFilter<xW, yW, baseType>::CopyData(const baseType dat[yW][xW]) {
		size_t rowSz = (size_t) C2DMatrix<baseType>::width() * sizeof(baseType);
		int h = C2DMatrix<baseType>::height();
		baseType **d = C2DMatrix<baseType>::data();
		for (int i = 0; i < h; ++i) {
			memcpy(d[i], dat[i], rowSz);
		}
	}

	template <unsigned short xW, unsigned short yW, class  baseType>
	baseType CFilter<xW, yW, baseType>::CalcNorm() const 
	{
		int nrNonZero = 0;
		baseType norm = (baseType) C2DMatrix<baseType>::SumElems(nrNonZero);

		norm = norm != 0 ? abs(norm) : 1;
		return norm;
	}
	

	template <unsigned short xW, unsigned short yW, class  baseType>
	void CFilter<xW, yW, baseType>::ComputeSymmetry() 
	{
		int halfFiltWidth = xW >> 1;
		int halfFiltHeight = yW >> 1;
		baseType** d = C2DMatrix<baseType>::data();
		if (halfFiltWidth > 0) {
			// test if filter is symmetric/antisymmetric in X direction and set the corresponding member boolean
			bool* whichTested = NULL;
			int factor = 1;
			if (d[0][0] == d[0][xW - 1]){
				m_isSymX = true;
				whichTested = &m_isSymX;
			}
			else {
				if (d[0][0] == -d[0][xW - 1]) {
					m_isAntiSymX = true;
					whichTested = &m_isAntiSymX;
					factor = -1;
				}
			}
			if (whichTested != 0) {
				bool &crtTested = *whichTested;
				for (int y = 0; crtTested && y < yW; ++y) {
					for (int f = 0, b = xW - 1; crtTested && f < halfFiltWidth; ++f, --b){
						crtTested &= (d[y][f] == factor*d[y][b]);
					}
				}
			}
		}


		if (halfFiltHeight > 0) {
			// test if filter is symmetric/antisymmetric in X direction and set the corresponding member boolean
			bool* whichTested = NULL;
			int factor = 1;
			if (d[0][0] == d[yW - 1][0]){
				m_isSymY = true;
				whichTested = &m_isSymY;
			}
			else {
				if (d[0][0] == -d[yW - 1][0]) {
					m_isAntiSymY = true;
					whichTested = &m_isAntiSymY;
					factor = -1;
				}
			}
			if (whichTested != 0) {
				bool &crtTested = *whichTested;
				for (int f = 0, b = yW - 1; crtTested && f < halfFiltHeight; ++f, --b) {
					for (int x = 0; crtTested && x < xW; ++x){
						crtTested &= (d[f][x] == factor*d[b][x]);
					}
				}
			}
		}
	}

	template <unsigned short xW, unsigned short yW, class  baseType>
		CFilter<xW, yW, baseType>& CFilter<xW, yW, baseType>::operator += (const CFilter<xW, yW, baseType>& arOther) 
	{
		*((MatrixNS::C2DMatrix<baseType>*) this) += arOther; 
		m_norm = CalcNorm(); 
		return *this; 
	}

	template <unsigned short xW, unsigned short yW, class  baseType>
	CFilter<xW, yW, baseType>& CFilter<xW, yW, baseType>::operator -= (const CFilter<xW, yW, baseType>& arOther)
	{
		*((MatrixNS::C2DMatrix<baseType>*) this) -= arOther;
		m_norm = CalcNorm();
		return *this;
	}

	template <unsigned short xW, unsigned short yW, class  baseType>
	CFilter<xW, yW, baseType>& CFilter<xW, yW, baseType>::operator *= (const CFilter<xW, yW, baseType>& arOther)
	{
		*((MatrixNS::C2DMatrix<baseType>*) this) *= arOther;
		m_norm = CalcNorm();
		return *this;
	}


	/*
#define CASS_OP_DEF(OPERATOR) template <unsigned short xW, unsigned short yW, class  baseType>\
	CFilter<xW, yW, baseType>& CFilter<xW, yW, baseType>::operator##OPERATOR(const CFilter<xW, yW, baseType>& arOther) \
	{\
	*((MatrixNS::C2DMatrix<baseType>*) this) OPERATOR arOther; \
	m_norm = CalcNorm(); \
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
}

#endif /* CFILTER_HPP */
