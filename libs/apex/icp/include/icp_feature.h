/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2011-2015 All right reserved.
* Copyright (c) 2015-2016 Freescale Semiconductor
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

/*!**********************************************************************************
*  @file icp_feature.h
*  @brief ICP Feature Descriptor
************************************************************************************/

#ifndef ICPFEATURE_H
#define ICPFEATURE_H
#include "icp_types.h"

namespace icp {
/*!**********************************************************************************
*  \brief ICP Feature structure.
*  This structure is used by the ICP_FeatureDesc class to store the position (x and y),
*  and the strength of a feature.
*  
************************************************************************************/
typedef struct Feature {
   ICP_Point_16S  position; /*!< The x and y location of the feature. */
   int16_t        strength; /*!< The strength of the feature. */
   int16_t        reserve;  /*!< Used for memory alignment. */
} Feature;

/*!*********************************************************************************
*  \brief ICP Feature Descriptor
*
*  ICP_FeatureDesc is a container class designed to encapsulate a contiguous region 
*  of data of type ICP_Feature.  It does not allocate or deallocate memory; it 
*  simply standardizes the representation of a contiguous memory region so it can 
*  be used by framework level services.
*  ICP_Feature is a structure that contains the position of a feature and its strength.
*
*  The memory must be allocated using OAL for the number of features you want to store.
*  i.e. If you want to be able to store 30 features:
*  \code{.cpp}
*           void *lBuffOal = OAL_MemoryAllocFlag(sizeof(ICP_Feature)*30,
*                            OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_MEMORY_FLAG_CONTIGUOUS);
*           ICP_FeatureDesc Desc(OAL_MemoryReturnAddress(lBuffOal, ACCESS_NCH_NB),
*                                OAL_MemoryReturnAddress(lBuffOal, ACCESS_PHY),
*                                30);
*  \endcode
*
***********************************************************************************/
class FeatureDescriptor
{
public:
   
/*!*********************************************************************************
*  \brief Default constructor.
*
***********************************************************************************/
   FeatureDescriptor();
   
/*!*********************************************************************************
*  \brief Constructor
*
*  Constructor that initializes a contiguous data region.  Note that
*  the data region must be physically contiguous in memory (not just
*  contiguous from the OS point of view).
***********************************************************************************/
   FeatureDescriptor(void* const lpData,       /*!< Pointer to contiguous data region. */
                     void* const lpDataPhys,   /*!< Physical pointer to contiguous data region (for HW use). */
                     int32_t maxElements       /*!< The maximum number of features. */
                  );

   ~FeatureDescriptor();

/*!*********************************************************************************
*  \brief Return Data Pointer
*  \return A void data pointer to the contiguous data region
*
*  Returns a void data pointer to the contiguous data region.
***********************************************************************************/
   void* GetDataPtr() const;

/*!*********************************************************************************
*  \brief Returns the 'physical' Data Pointer
*  \return A void 'physical' data pointer to the contiguous data region.
*
*  Returns a void 'physical' data pointer to the contiguous data region.
***********************************************************************************/
   void* GetDataPtrPhys() const;
/*!*********************************************************************************
*  \brief Returns the span of a single feature
*  \return The span of a feature
*
*  Returns the number of bytes a single ICP_Feature occupies
***********************************************************************************/
   int32_t GetSpan() const;

/*!*********************************************************************************
*  \brief Returns the maximum number of features
*  \return The number of features the descriptor can hold
*
*  Returns the maximum number of features the descriptor can hold. 
***********************************************************************************/
   int32_t GetSize() const;

/*!*********************************************************************************
*  \brief Return the number of features available
*  \return The number of features currently stored
*
*  Returns the number of features currently stored in the descriptor. This value
*  is incremented each time a feature is added with the ICP_FeatureDesc::Add function.
*  If the features are manually added/removed, the count can be updated using
*  ICP_FeatureDesc::SetCount. Only accurate if class functions are used to add/remove
*  features, or if kept accurate by updating count.
***********************************************************************************/
   int32_t GetCount() const;

/*!*********************************************************************************
*  \brief Set the number of features available
*  \return The result of the operation (zero on success).
*
*  This sets the number of features available in the descriptor. The count is limited
*  to the range [0, Max Elements];
***********************************************************************************/
   int32_t SetCount(int32_t count);

/*!*********************************************************************************
*  \brief Add a feature
*  \return The result of the operation (zero on success).
*
*  This adds a feature to the descriptor. This should only be used if the count of the
*  number of features available is kept accurate. This will check the number of
*  features against the maximum number of features, if it is not at full capacity,
*  the new feature will be added and the count is incremented.
***********************************************************************************/
   int32_t Add(int16_t x, int16_t y, int16_t strength=0);

/*!*********************************************************************************
*  \brief Remove a feature at an index
*  \return The result of the operation (zero on success).
*
*  This removes a feature from the descriptor at the specified index. The remaining
*  features will be shifted to fill the space. This should only be used if the count
*  is kept updated.
***********************************************************************************/
   int32_t Remove(int32_t ind);

/*!*********************************************************************************
*  \brief Get a feature at a specified index
*  \return The ICP_Feature at the specified index
*
*  This will return a feature at index _ind_. If the index is greater than the 
*  maximum size of the descriptor, the feature at index 0 is returned.
***********************************************************************************/
   Feature& GetFeature(int32_t ind) const;

/*!*********************************************************************************
*  \brief Set a feature
*  \return The result of the operation (zero on success).
*
*  This will modify the feature at index _ind_ to contain the specified position and
*  strength. The strength will default to 0 if not specified.
***********************************************************************************/
   int32_t Set(int32_t ind, int16_t x, int16_t y, int16_t strength=0); 

/*!*********************************************************************************
*  \brief Operator [].
*  \return ICP_Feature at index
*
*  This will return a feature at the specified index. If the index is greater than
*  the maximum size of the descriptor, the feature at index 0 is returned.
***********************************************************************************/
   Feature& operator[](int32_t ind);
   const Feature& operator[](const int32_t ind) const;

/*!*********************************************************************************
*  \brief Initialize the descriptor.
*
*  This will initialize the descriptor with a contiguous data region.  Note that
*  the data region must be physically contiguous in memory (not just
*  contiguous from the OS point of view).
***********************************************************************************/
   void Init(void* const lpData,       /*!< Pointer to contiguous data region. */
             void* const lpDataPhys,   /*!< Physical pointer to contiguous data region (for HW use). */
             int32_t maxElements       /*!< The maximum number of features. */
            );
private:
   void* mpData;           /*!< Pointer to contiguous data region. */
   void* mpDataPhys;       /*!< Physical pointer to contiguous data region (for HW use). */
   int32_t mMaxElements;   /*!< The maximum number of features. */
   int32_t mNumElements;   /*!< The number of features available. */
   
   // int32_t VerifyPhysPtrAlignment();
};

} // namespace icp
#endif /* ICPFEATURE_H */
