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

/*  List of supported decoders */
#define DECODER_JPEG                    1
#define DECODER_H264                    2

/*  List of stream variants */
#define SINGLE_STREAM                   1
#define FOUR_STREAMS                    2

/*  Select desired decoder variant */
#define APP_CFG_DECODER_VARIANT         DECODER_JPEG

/*  Select variant based on number of streams */
#define APP_CFG_STREAM_VARIANT          FOUR_STREAMS

/*  If TRUE then application will be build considering the input to be a static image
    read from file. If FALSE then application is set up to get data from a network stream
    using the sm_app firmware. */
#define APP_CFG_USE_STATIC_INPUT        FALSE

