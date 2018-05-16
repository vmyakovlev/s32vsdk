/*
  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
  
  This file contains sample code only.
  
  THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

//***************************************************************************
//                         GLOBAL FUNCTION PROTOTYPES
//***************************************************************************
extern int SramInitAllocator(const char *szDrvName);
extern int SramAlloc(unsigned long *pSizeAddr);
extern int SramRelease(unsigned long Addr);
extern void SramReleaseAll(void);
extern void SramCloseAllocator(void);
