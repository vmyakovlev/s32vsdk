/*****************************************************************************
* 
* NXP Confidential Proprietary
*
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

#ifndef APEX_H
#define APEX_H

/////////////////////////////////////////////////////////////////////////
/// \addtogroup apex
/// apex includes methods related to high level initialization/de-initialization
/// of APEX hardware and driver modules.  This is the only user/application facing APEX
/// driver module.
/// @{

#define APEX_APEX0 0
#define APEX_APEX1 1

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////
/// Reset all APEX hardware blocks and initialize/create APEX driver specific entities.
///
/// \return
/// 0 if successful, 1 if failure.
/////////////////////////////////////////////////////////////////////////////////
int APEX_Init();

/////////////////////////////////////////////////////////////////////////////////
/// Cleanup/destroy APEX driver specific entities.
///
/// \return
/// 0 if successful, 1 if failure.
/////////////////////////////////////////////////////////////////////////////////
int APEX_Deinit();

/////////////////////////////////////////////////////////////////////////////////
/// Returns the state of the APEX initialization flag.
///
/// \return
/// Returns 1 if APEX has been successfully initialized with ACF_Init(), and
/// returns 0 otherwise.
/////////////////////////////////////////////////////////////////////////////////
int RetApexInitFlag();

//deprecated
int ACF_Init();
int ACF_Deinit();

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif /* APEX_H */
