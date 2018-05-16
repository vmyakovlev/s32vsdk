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

#ifndef WEBSOCKETAPPLICATIONSCHATPARTICIPANT_HPP
#define WEBSOCKETAPPLICATIONSCHATPARTICIPANT_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include "dataframe.hpp"

namespace websocket {
    namespace applications {
        namespace auditorium {

            class participant
            {
            public:
                virtual ~participant() {}
                virtual void deliver(dataframe msg) = 0;
                virtual std::string get_id() = 0;
            };

            typedef boost::shared_ptr<participant> participant_ptr;
            
        } // chat
    } // namespace applications
} // namespace websocket

#endif /* WEBSOCKETAPPLICATIONSCHATPARTICIPANT_HPP */