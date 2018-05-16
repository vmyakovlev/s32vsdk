/*****************************************************************************
* 
* NXP Confidential Proprietary
*
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

/*!*********************************************************************************
 *  @file apexdnn_sn_public.hpp
 *  @brief ApexCV Neural Networks library.
 **********************************************************************************/

/** 
 * \defgroup UserAPI
 * This is the group of enum, structure and functions needs to be exposed to APEX-DNN library user 
 */

#ifndef APEXDNNSNPUBLIC_HPP
#define APEXDNNSNPUBLIC_HPP

#include "acf_process_apu.h"

/*!*********************************************************************************
 * \ingroup UserAPI
 * \brief APEX-DNN supported element-wise operation
 *
 * apexdnnEltWiseOpType_t is a enumerated type used to declares all APEX-DNN library supported element-wise operation.
 ***********************************************************************************/
typedef enum
{
  APEXDNN_ELTWISE_OP_UNKNOWN = -1 /*!< Undefined type */,
  APEXDNN_ELTWISE_OP_MUL     =  0 /*!< Element-wise multiplication */,
  APEXDNN_ELTWISE_OP_ADD     =  1 /*!< Element-wise addition */
} apexdnnEltWiseOpType_t;

/*!*********************************************************************************
 * \ingroup UserAPI
 * \brief APEX-DNN supported layer and module types
 *
 * apexdnnLayerType_t is a enumerated type used to declares all APEX-DNN library supported layers and modules.
 * We use term "LAYER" if one only one operation, such as convolution, max pooling or ReLu, is executed; we use term "MODULE"
 * if multiple operations are executed.
 ***********************************************************************************/
typedef enum
{
   APEXDNN_LAYER_UNKNOWN           = -1  /*!< Undefined type */,
   APEXDNN_MODULE_TYPE_CONV3X3MPS1 =  0  /*!< Module includes 3x3 convolution filter, followed by max pooling filter then squeeze 1x1 convolution filter */,
   APEXDNN_MODULE_TYPE_E1E3MPS1    =  1  /*!< Module includes expand 1x1 convolution filter and expand 3x3 convolution filter followed by max pooling filter then squeeze 1x1 convolution filter */,
   APEXDNN_MODULE_TYPE_E1E3MP      =  2  /*!< Module includes expand 1x1 convolution filter and expand 3x3 convolution filter followed by max pooling filter */,
   APEXDNN_MODULE_TYPE_E1E3S1      =  3  /*!< Module includes expand 1x1 convolution filter and expand 3x3 convolution filter followed by squeeze 1x1 convolution filter */,
   APEXDNN_MODULE_TYPE_E1E3        =  4  /*!< Module includes expand 1x1 convolution filter and expand 3x3 convolution filter */,
   APEXDNN_MODULE_TYPE_E1AP        =  5  /*!< Module includes expand 1x1 convolution filter and expand 3x3 convolution filter */,

   APEXDNN_MODULE_TYPE_ELTMULCRED   = 6  /*!< Layer to do element wise operation. Only support multiplication for now. */, 

}  apexdnnLayerType_t;

/*!*********************************************************************************
 * \ingroup UserAPI
 * \brief APEX-DNN status
 *
 apexdnnStatus_t is a enumerated type used to declare function returned status.
 ***********************************************************************************/
typedef enum
{
   APEXDNN_STATUS_SUCCESS         = 0  /*!< The operation completed successfully */,
   APEXDNN_STATUS_BAD_PARAM       = 1  /*!< An incorrect value or parameter was passed to the function */,
   APEXDNN_STATUS_ALLOC_FAILED    = 2  /*!< Memory allocation failed */,
   APEXDNN_STATUS_INTERNAL_ERROR  = 3  /*!< An internal apexdnn executation failed */,
   APEXDNN_STATUS_WORKSPACE_ERROR = 4  /*!< Workspace hasn't been allocated yet */,
   APEXDNN_STATUS_HASNOT_VERIFY_GRAPH = 5 /*!< Network graph hasn't been verified yet */
} apexdnnStatus_t;

/*!*********************************************************************************
 * \ingroup UserAPI
 * \brief APEX-DNN supported tensor data type
 *
 * apexdnnTensorDataType_t is a enumerated type used to declare supported data types.
 ***********************************************************************************/
typedef enum
{
   APEXDNN_DATA_8BIT  = 0 /*!< Fixed point 8-bit, the only one supported for now */,
   APEXDNN_DATA_16BIT = 1 /*!< Fixed point 16-bit, not supported yet */,
}  apexdnnTensorDataType_t;

typedef enum
{
   APEXDNN_TENSOR_UNKNOWN = -1  /*!< Unknown */,
   APEXDNN_TENSOR_NCHW    =  0  /*!< This tensor format specifies that the data layout as batches, channels, rows and columns. Column is the inner most dimension and batche is the outer most dimension */,
   APEXDNN_TENSOR_NHCW    =  1  /*!< This tensor format specifies that the data layout as batches, rows, channels and columns. Column is the inner most dimension and batche is the outer most dimension */,
   APEXDNN_TENSOR_NHWC    =  2  /*!< This tensor format specifies that the data layout as batches, rows, columns and channels. Channel is the inner most dimension and batche is the outer most dimension */,
}  apexdnnTensorFormat_t;

/*!*********************************************************************************
 * \ingroup UserAPI
 * \brief APEX-DNN supported memory the tensor will be allocated on
 *
 * apexdnnTensorMemory_t is a enumberated type used to declare on which memory partation the tensor will be allocated.
 ***********************************************************************************/
typedef enum
{
   APEXDNN_TENSOR_MEM_HEAP = 0  /*!< regular heap memory */,
   APEXDNN_TENSOR_MEM_OAL  = 1  /*!< OAL memory */,
}  apexdnnTensorMemory_t;

/*!*********************************************************************************
 * \ingroup UserAPI
 * \brief APEX-DNN supported pooling mode 
 *
 * apexdnnPoolingMode_t is a enumberated type used to declare differnet pooling mode.
 ***********************************************************************************/
typedef enum
{
   APEXDNN_POOLING_MAX  = 0  /*!< max pooling which is the only supported pooling mode for now */,
   APEXDNN_POOLING_AVG  = 1  /*!< average pooling */,
}  apexdnnPoolingMode_t;

/*!*********************************************************************************
 * \ingroup UserAPI
 * \brief APEX-DNN supported show level
 *
 * apexdnnShow_Lvl is a enumberated type used to declare different show level when user call Show routine on network, tensor or each layer/module for debugging purpose.
 ***********************************************************************************/
typedef enum
{
   APEXDNN_SHOW_SIMPLE  = 0  /*!< Basic level to display most critical information only */,
   APEXDNN_SHOW_VERBOSE = 1  /*!< Verbose level */,
   APEXDNN_SHOW_SUPER   = 2  /*!< Unused yet */,
} apexdnnShow_Lvl;


struct apexdnnWorkSpace;
struct apexdnnQuantParams;
struct apexdnnTensorDescriptor;
struct apexdnnConvolutionDescriptor;
struct apexdnnPoolingDescriptor;
struct apexdnnBaseLayer;
struct apexdnnConv3x3MPS1Module;
struct apexdnnE1E3MPS1Module;
struct apexdnnE1E3MPModule;
struct apexdnnE1E3S1Module;
struct apexdnnE1E3Module;
struct apexdnnE1APModule;
struct apexdnnEltMulCRedModule;
struct apexdnnNet;

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Create APEX-DNN library's workspace
 *
 *  This is an interface for creating the APEX-DNN library's workspace by allocating the 
 *  memory needed to hold its data structure.
 ******************************************************************************/
apexdnnStatus_t apexdnnCreateWorkSpace(
                         apexdnnWorkSpace** Workspace /*!< Workspace handle */,
                         ACF_APU_CFG mApuConfig       /*!< Apu configuration, only support ACF_APU_CFG__APU_0_CU_0_63_SMEM_0_3 for now */,
                         int         mApexID          /*!< APEX ID */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Destroy APEX-DNN library's workspace and free up the workspace reserved memory
 *
 *  This is an interface for destroying the APEX-DNN library's workspace and free up the reserved memory.
 ******************************************************************************/
apexdnnStatus_t apexdnnDestroyWorkSpace(
                         apexdnnWorkSpace* Workspace  /*!< Work space */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Create APEX-DNN library's 4-D tensor
 *
 * This is an interface for creating the 4-D tensor descriptor by allocating the memory needed to hold its data structure. The tensor
 * can have padding on each dimension when the stride is greather than product of the dimension and the stride of the
 * next dimension (stride[n] > stride[n+1] * dim[n+1]).
 ******************************************************************************/
apexdnnStatus_t apexdnnCreate4dTensorDescriptor(   
                         apexdnnTensorDescriptor** TensorDesc /*!< Pointer point to the tensor's handle */,
                         apexdnnTensorDataType_t   DataType   /*!< Tensor data type, only 8-bit is supported for now */,
                         apexdnnTensorFormat_t     Format     /*!< Tensor layout format */,
                         apexdnnTensorMemory_t     Memory     /*!< Tensor memory */,
                         int dim0                             /*!< Size of outer most dimension */,
                         int dim1                             /*!< Size of second dimension */,
                         int dim2                             /*!< Size of third dimension */,
                         int dim3                             /*!< Size of inner most dimension */,
                         int stride0                          /*!< Outer most dimension's stride */,
                         int stride1                          /*!< Second dimension's stride */,
                         int stride2                          /*!< Third dimension's stride */,
                         int stride3                          /*!< Inner most dimension's stride */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Create APEX-DNN library's 4-D compact tensor
 *
 * This is an interface for creating the 4-D tensor descriptor by allocating the memory needed to hold its data structure.
 * This will create a compact tensor, ie. the stride of one dimension equal to the product of the dimension and the stride of the
 * next dimension (stride[n] = stride[n+1] * dim[n+1]).
 ******************************************************************************/
apexdnnStatus_t apexdnnCreate4dTensorDescriptor(   
                         apexdnnTensorDescriptor** TensorDesc /*!< Pointer point to the tensor's handle */,
                         apexdnnTensorDataType_t   DataType   /*!< Tensor data type, only 8-bit is supported for now */,
                         apexdnnTensorFormat_t     Format     /*!< Tensor layout format */,
                         apexdnnTensorMemory_t     Memory     /*!< Tensor memory */,
                         int dim0                             /*!< Size of outer most dimension */,
                         int dim1                             /*!< Size of second dimension */,
                         int dim2                             /*!< Size of third dimension */,
                         int dim3                             /*!< Size of inner most dimension */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Create APEX-DNN library's virtual 4-D compact tensor
 *
 * This is an interface for creating the virtual 4-D tensor descriptor but not really allocate the memory
 * This will create a compact virtual tensor, ie. the stride of one dimension equal to the product of the dimension and the stride of the
 * next dimension (stride[n] = stride[n+1] * dim[n+1]).
 ******************************************************************************/
apexdnnStatus_t apexdnnCreateVirtual4dTensorDescriptor(
                         apexdnnTensorDescriptor** TensorDesc /*!< Pointer point to the tensor's handle */,
                         apexdnnTensorDataType_t   DataType   /*!< Tensor data type, only 8-bit is supported for now */,
                         apexdnnTensorFormat_t     Format     /*!< Tensor layout format */,
                         apexdnnTensorMemory_t     Memory     /*!< Tensor memory */,
                         int dim0                             /*!< Size of outer most dimension */,
                         int dim1                             /*!< Size of second dimension */,
                         int dim2                             /*!< Size of third dimension */,
                         int dim3                             /*!< Size of inner most dimension */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Return tensor's data pointer
 *
 * This is an interface for returning tensor's data pointer.
 ******************************************************************************/
void* apexdnnRetTensorDataPtr(
                         apexdnnTensorDescriptor* TensorDesc /*!< Tensor descriptor */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Return tensor's data format 
 *
 * This is an interface for returning tensor's data format.
 ******************************************************************************/
apexdnnTensorFormat_t apexdnnRetTensorFormat(
                         apexdnnTensorDescriptor* TensorDesc  /*!< Tensor descriptor */
                     );

apexdnnTensorDataType_t apexdnnRetTensorDataType(
                         apexdnnTensorDescriptor* TensorDesc
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Return tensor's buffer memory 
 *
 * This is an interface for returning tensor's buffer memory.
 ******************************************************************************/
apexdnnTensorMemory_t apexdnnRetTensorMemory(
                         apexdnnTensorDescriptor* TensorDesc  /*!< Tensor descriptor */
                     );


/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Return tensor's i-th dimension 
 *
 * This is an interface for returning tensor's i-th dimension.
 ******************************************************************************/
int apexdnnRetTensorDim(
                         apexdnnTensorDescriptor* TensorDesc  /*!< Tensor descriptor */,
                         int i                                /*!< Dimension index */
                     );

int apexdnnRetTensorStride(
                         apexdnnTensorDescriptor* TensorDesc  /*!< Tensor descriptor */,
                         int i                                /*!< Stride index */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief transform 4-D tensor's data buffer into different format. 
 *
 * This is an utilit to transform tensor from one format to another. Two tensors have to have separated buffer allocated. Don't support inplace transformation for now.
 * This is unoptimized routine yet for functionality only. Only support following transformation:
 * From N-C-H-W to N-H-C-w
 * From N-H-C-W to N-C-H-W
 * From N-C-H-W to N-H-W-C
 ******************************************************************************/
apexdnnStatus_t apexdnnTransform4dTensorDescriptor(
                         apexdnnTensorDescriptor *XTensorDesc /* Source tensor descriptor */,
                         apexdnnTensorDescriptor *YTensorDesc /* Destination tensor descriptor */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Compre two 4-D tensor's data buffer content. 
 *
 * This is an utilit to compare two tensors buffer content. The comparison won't care the tensors' format and only compare the pixel values at corresponding coordinate.
 * Only support the tensors of N-C-H-W and N-H-C-W format.
 ******************************************************************************/
apexdnnStatus_t apexdnnCompare4dTensorDescriptor(
                         apexdnnTensorDescriptor *XTensorDesc /* Tensor descriptor to compare */,
                         apexdnnTensorDescriptor *YTensorDesc /* Tensor descriptor to compare */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Randomize the tensor.
 *
 * This is an utilit to fill in tensor with random value. Mainly for testing / debugging purpose.
 ******************************************************************************/
apexdnnStatus_t apexdnnRandomize4dTensorDescriptor(
                         apexdnnTensorDescriptor *TensorDesc /* Tensor descriptor to compare */,
                         int Seed                            /* Random seed. -1: Don't reset seed. >=0: Reset seed by given value */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief display tensor descriptor
 *
 * This is an interface to display tensor descriptor for debugging purpose
 ******************************************************************************/
void apexdnnTensorDescriptorShow(
                         apexdnnTensorDescriptor* Tensor    /*!< Tensor descriptor needs to be displayed */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief destroy tensor descriptor
 *
 * This is an interface to destroy tensor descriptor. Tensor's memory will be freed.
 ******************************************************************************/
apexdnnStatus_t apexdnnDestroyTensorDescriptor(
                         apexdnnTensorDescriptor* TensorDesc /*!< Tensor descriptor needs to be destroyed */
                     );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Create Conv3x3MPS1 convolution network module
 *
 * This is an interface to create a convolution network module which includes 3x3 convolution filter, followed by
 * max pooling filter then squeeze 1x1 convolution filter.
 ******************************************************************************/
apexdnnStatus_t apexdnnCreateConv3x3MPS1Module(
                          apexdnnConv3x3MPS1Module** Module   /*!< Pointer which points to the module's handle */,
                          apexdnnTensorDataType_t    DataType /*!< Data type, only support 8-bit for now */,
                          int wConv3x3Dim0                    /*!< 3x3 convolution filter's number of output channels */,
                          int wConv3x3Dim1                    /*!< 3x3 convolution filter's number of input channels */,
                          int Conv3x3PadH                     /*!< 3x3 convolution filter's vertical padding */,
                          int Conv3x3PadW                     /*!< 3x3 convolution filter's horizontal padding */,
                          int Conv3x3StrideH                  /*!< 3x3 convolution filter's vertical stride */,
                          int Conv3x3StrideW                  /*!< 3x3 convolution filter's horizontal stride */,
                          int MPWindowH                       /*!< Maxpooling filter's vertical window size */,
                          int MPWindowW                       /*!< Maxpooling filter's horizontal window size */,
                          int MPPadH                          /*!< Maxpooling filter's vertical padding */,
                          int MPPadW                          /*!< Maxpooling filter's horizontal padding */,
                          int MPStrideH                       /*!< Maxpooling filter's vertical stride */,
                          int MPStrideW                       /*!< Maxpooling filter's horizontal stride */,
                          int mS1Dim0                         /*!< 1x1 convolution filter's number of output channels */,
                          int ConvS1StrideH                   /*!< 1x1 convolution filter's vertical stride */,
                          int COnvS1StrideW                   /*!< 1x1 convolution filter's horizontal stride */
                      );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Create E1E3MPS1 convolution network module
 *
 * This is an interface to create a convolution network module which includes expand 1x1 convolution filter and
 * expand 3x3 convolution filter followed by max pooling filter then squeeze 1x1 convolution filter.
 ******************************************************************************/
apexdnnStatus_t apexdnnCreateE1E3MPS1Module(
                               apexdnnE1E3MPS1Module** Module    /*!< Pointer which points to module's handle */,
                               apexdnnTensorDataType_t DataType  /*!< Data type, only support 8-bit for now */,
                               int wE1Dim0                       /*!< 1x1 convolution filter's number of output channels */,
                               int wE1Dim1                       /*!< 1x1 convolution filter's number of input channels */,
                               int E1StrideH                     /*!< 1x1 convolution filter's vertical stride */,
                               int E1StrideW                     /*!< 1x1 convolution filter's horizontal stride */,
                               int wE3Dim0                       /*!< 3x3 convolution filter's number of output channels */,
                               int wE3Dim1                       /*!< 3x3 convolution filter's number of input channels */,
                               int E3PadH                        /*!< 3x3 convolution filter's vertical padding */,
                               int E3PadW                        /*!< 3x3 convolution filter's horizontal padding */,
                               int E3StrideH                     /*!< 3x3 convolution filter's vertical stride */,
                               int E3StrideW                     /*!< 3x3 convolution filter's horizontal stride */,
                               int MPWindowH                     /*!< Maxpooling filter's vertical window size */,
                               int MPWindowW                     /*!< 3x3 convolution filter's horizontal stride */,
                               int MPPadH                        /*!< Maxpooling filter's vertical padding */,
                               int MPPadW                        /*!< Maxpooling filter's horizontal padding */,
                               int MPStrideH                     /*!< Maxpooling filter's vertical stride */,
                               int MPStrideW                     /*!< Maxpooling filter's horizontal stride */,
                               int wS1Dim0                       /*!< 1x1 convolution filter's number of output channels */,
                               int S1StrideH                     /*!< 1x1 convolution filter's vertical stride */,
                               int S1StrideW                     /*!< 1x1 convolution filter's horizontal stride */
                           );
/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Create E1E3MP convolution network module
 *
 * This is an interface to create a convolution network module which includes expand 1x1 convolution filter and
 * expand 3x3 convolution filter followed by max pooling filter.
 ******************************************************************************/
apexdnnStatus_t apexdnnCreateE1E3MPModule(
                               apexdnnE1E3MPModule** Module     /*!< Pointer which points to module's handle */,
                               apexdnnTensorDataType_t DataType /*!< Data type, only support 8-bit for now */,
                               int wE1Dim0                      /*!< 1x1 convolution filter's number of output channels */,
                               int wE1Dim1                      /*!< 1x1 convolution filter's number of input channels */,
                               int E1StrideH                    /*!< 1x1 convolution filter's vertical stride */,
                               int E1StrideW                    /*!< 1x1 convolution filter's horizontal stride */,
                               int wE3Dim0                      /*!< 3x3 convolution filter's number of output channels */,
                               int wE3Dim1                      /*!< 3x3 convolution filter's number of input channels */,
                               int E3PadH                       /*!< 3x3 convolution filter's vertical padding */,
                               int E3PadW                       /*!< 3x3 convolution filter's horizontal padding */,
                               int E3StrideH                    /*!< 3x3 convolution filter's vertical stride */,
                               int E3StrideW                    /*!< 3x3 convolution filter's horizontal stride */,
                               int MPWindowH                    /*!< Maxpooling filter's vertical window size */,
                               int MPWindowW                    /*!< 3x3 convolution filter's horizontal stride */,
                               int MPPadH                       /*!< Maxpooling filter's vertical padding */,
                               int MPPadW                       /*!< Maxpooling filter's horizontal padding */,
                               int MPStrideH                    /*!< Maxpooling filter's vertical stride */,
                               int MPStrideW                    /*!< Maxpooling filter's horizontal stride */
                           );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Create E1E3S1 convolution network module
 *
 * This is an interface to create a convolution network module which includes expand 1x1 convolution filter and
 * expand 3x3 convolution filter followed by squeeze 1x1 convolution filter.
 ******************************************************************************/
apexdnnStatus_t apexdnnCreateE1E3S1Module(   
                                apexdnnE1E3S1Module**   Module    /*!< Pointer which points to the module's handle */,
                                apexdnnTensorDataType_t DataType  /*!< Data type, only support 8-bit for now */,
                                int wE1Dim0                       /*!< 1x1 convolution filter's number of output channels */,
                                int wE1Dim1                       /*!< 1x1 convolution filter's number of input channels */,
                                int E1StrideH                     /*!< 1x1 convolution filter's vertical stride */,
                                int E1StrideW                     /*!< 1x1 convolution filter's horizontal stride */,
                                int wE3Dim0                       /*!< 3x3 convolution filter's number of output channels */,
                                int wE3Dim1                       /*!< 3x3 convolution filter's number of input channels */,
                                int E3PadH                        /*!< 3x3 convolution filter's vertical padding */,
                                int E3PadW                        /*!< 3x3 convolution filter's horizontal padding */,
                                int E3StrideH                     /*!< 3x3 convolution filter's vertical stride */,
                                int E3StrideW                     /*!< 3x3 convolution filter's horizontal stride */,
                                int wS1Dim0                       /*!< 1x1 convolution filter's number of output channels */,
                                int S1StrideH                     /*!< 1x1 convolution filter's vertical stride */,
                                int S1StrideW                     /*!< 1x1 convolution filter's horizontal stride */
                           );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Create E1E3 convolution network module
 *
 * This is an interface to create a convolution network module which includes expand 1x1 convolution filter and
 * expand 3x3 convolution filter.
 ******************************************************************************/
apexdnnStatus_t apexdnnCreateE1E3Module(
                                apexdnnE1E3Module**     Module    /*!< Pointer which points to the module's handle */,
                                apexdnnTensorDataType_t DataType  /*!< Data type, only support 8-bit for now */,
                                int wE1Dim0                       /*!< 1x1 convolution filter's number of output channels */,
                                int wE1Dim1                       /*!< 1x1 convolution filter's number of input channels */,
                                int E1StrideH                     /*!< 1x1 convolution filter's vertical stride */,
                                int E1StrideW                     /*!< 1x1 convolution filter's horizontal stride */,
                                int wE3Dim0                       /*!< 3x3 convolution filter's number of output channels */,
                                int wE3Dim1                       /*!< 3x3 convolution filter's number of input channels */,
                                int E3PadH                        /*!< 3x3 convolution filter's vertical padding */,
                                int E3PadW                        /*!< 3x3 convolution filter's horizontal padding */,
                                int E3StrideH                     /*!< 3x3 convolution filter's vertical stride */,
                                int E3StrideW                     /*!< 3x3 convolution filter's horizontal stride */
                           );

apexdnnStatus_t apexdnnCreateE1APModule(
                                apexdnnE1APModule**     Module    /*!< Pointer which points to the module's handle */,
                                apexdnnTensorDataType_t DataType  /*!< Data type, only support 8-bit for now */,
                                int wE1Dim0                       /*!< 1x1 convolution filter's number of output channels */,
                                int wE1Dim1                       /*!< 1x1 convolution filter's number of input channels */,
                                int E1StrideH                     /*!< 1x1 convolution filter's vertical stride */,
                                int E1StrideW                     /*!< 1x1 convolution filter's horizontal stride */
                           );

apexdnnStatus_t apexdnnCreateEltMulCRedModule(
                                apexdnnEltMulCRedModule** Module,
                                apexdnnTensorDataType_t   DataType,
                                int DimC,
                                int DimH,
                                int DimW
                           );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Create convolution neural network
 *
 *  This is an interface for creating an empty convolution neural network by allocating the memory needed to hold its data structure.
 ******************************************************************************/
apexdnnStatus_t apexdnnCreateEmptyNet(
                                apexdnnNet** Net    /*!< Pointer which points to the network's handle */,
                                bool BatchProc = 0  /*!< Batch processing enablement. 0: Disabled. 1: Enabled */
                            );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Destroy convolution neural network
 *
 *  This is an interface for destroying an convolution neural network. All the layers and related tensors will be destroied and
 *  allocated memory will be freed.
 ******************************************************************************/
apexdnnStatus_t apexdnnDestroyNet(
                                apexdnnNet* Net                               /*!< Network to be destroied */,
                                apexdnnTensorDescriptor*  NetInputTensorDesc  /*!< Network's input tensor to be destroied */
                            );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Get total number of layers in the network. 
 *
 *  This is an interface for retrieving the total number of layers in the network.
 ******************************************************************************/
int apexdnnRetNetNumofLayers(
                                apexdnnNet* Net /*!< Network */
                            );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Get each layer's output tensor
 *
 *  This is an interface for retrieving the output tensor of each network's layer.
 ******************************************************************************/
apexdnnTensorDescriptor* apexdnnRetNetLayerOutputTensorDesc(
                                apexdnnNet* Net /*!< Network */, 
                                int i           /*!< Layer index */
                            );
/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Get each layer's type 
 *
 *  This is an interface for retrieving the type of each network's layer.
 ******************************************************************************/
apexdnnLayerType_t apexdnnRetNetLayerType(
                                apexdnnNet* Net /*!< Network */, 
                                int i           /*!< Layer index */
                            );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Add layer or module into network
 *
 *  This is an interface to add the layer into network.
 ******************************************************************************/
apexdnnStatus_t apexdnnNetAppendLayer(
                                apexdnnNet* Net /*!< Network */,
                                void* Layer     /*!< Created layer needs to add into network */
                            );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Fill network model (weights, bias, quantization parameters) into network.
 *
 *  This is an interface to fill in network model, including each filter's weight, bias and quantization parameters.
 ******************************************************************************/
apexdnnStatus_t apexdnnNetFillModel(
                                apexdnnNet* Net /*!< Network */,
                                int8_t *Model   /*!< Model buffer which hold read in fixed point model file produced by offline tool */
                            );
                                
/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Verify network
 *
 *  This is an interface to verify the network. It won't really perform forward calculation instead it will go through the network,
 *  check the consistency and allocate intermediate tensor for each layer or module's output based on input tensor's resolution.
 *  This only needs to execute once if no input resolution change.
 *  The output tensor if provided will be connect to last layer. Otherwise it will be internally allocated.
 *  Thie is for host CPU forwarding, i.e. assume the following forward calculation call will be "apexdnnNetForwardCpu". 
 ******************************************************************************/
apexdnnStatus_t apexdnnNetVerifyGraphCpu(
                                apexdnnNet* Net                                /*!< Network */,
                                apexdnnTensorDescriptor*  NetInputTensorDesc   /*!< Network input tesnor */,
                                apexdnnTensorDescriptor** NetOutputTensorDesc  /*!< Network output tesnor */
                            );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Forward network
 *
 *  This is an interface to forward the network on host CPU.
 *  This serve as reference model, i.e. very straight forward implementation without any level optimization.
 ******************************************************************************/
apexdnnStatus_t apexdnnNetForwardCpu(
                                apexdnnWorkSpace* Workspace                   /*!< Work space handle */,
                                apexdnnNet* Net                               /*!< Network */,
                                apexdnnTensorDescriptor*  NetInputTensorDesc  /*!< Network input tesnor */
                            );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Verify network
 *
 *  This is an interface to verify the network. It won't really perform forward calculation instead it will go through the network,
 *  check the consistency and allocate intermediate tensor for each layer or module's output based on input tensor's resolution.
 *  This only needs to execute once if no input resolution change.
 *  The output tensor if provided will be connect to last layer. Otherwise it will be internally allocated.
 *  Thie is for host CPU forwarding, i.e. assume the following forward calculation call will be "apexdnnNetForwardApex". 
 ******************************************************************************/
apexdnnStatus_t apexdnnNetVerifyGraphApex(
                                apexdnnWorkSpace* Workspace                    /*!< Work space handle */,
                                apexdnnNet* Net                                /*!< Network */,
                                apexdnnTensorDescriptor*  NetInputTensorDesc   /*!< Network input tesnor */,
                                apexdnnTensorDescriptor** NetOutputTensorDesc  /*!< Network output tesnor */
                            );

/*!****************************************************************************
 * \ingroup UserAPI
 *  \brief Forward network
 *
 *  This is an interface to forward the network on APEX.
 ******************************************************************************/
apexdnnStatus_t apexdnnNetForwardApex(
                                apexdnnWorkSpace* Workspace                    /*!< Work space handle */,
                                apexdnnNet* Net                                /*!< Network */,
                                apexdnnTensorDescriptor*  NetInputTensorDesc   /*!< Network input tesnor */,
                                int Profiling = 0                              /*!< Enable ACF profiling. Profiling information will be printed onto console. 0: Disabled. 1: Enabled */
                            );

apexdnnStatus_t apexdnnNetEstMemUsageApex(
                                apexdnnNet* Net,
                                 apexdnnTensorDescriptor*  NetInputTensorDesc
                            );

/*!****************************************************************************
 * \ingroup UserAPI
 * \brief Display Netowork
 *
 * This is a debug utility function to show the network's configuration.
 ******************************************************************************/
void apexdnnNetShow(
                                apexdnnNet* Net                                /*!< Network */, 
                                apexdnnShow_Lvl Lvl = APEXDNN_SHOW_SIMPLE      /*!< Show level */
                            );




#endif /* APEXDNNSNPUBLIC_HPP */

