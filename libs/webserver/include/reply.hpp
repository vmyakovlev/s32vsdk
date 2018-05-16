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

#ifndef WEBSOCKETHTTPREPLY_HPP
#define WEBSOCKETHTTPREPLY_HPP

#include <vector>
#include <boost/asio.hpp>
#include "header.hpp"

namespace websocket {
    namespace http {

        /// A http reply to be sent to a client.
        struct reply
        {
            /// The status of the reply.
            enum status_type
            {
                switching_protocols = 101,
                bad_request = 400,
                internal_server_error = 500,
            } status;

            /// The headers to be included in the reply.
            std::vector<header> headers;

            /// Convert the reply into a vector of buffers. The buffers do not own the
            /// underlying memory blocks, therefore the reply object must remain valid and
            /// not be changed until the write operation has completed.
            std::vector<boost::asio::const_buffer> to_buffers();

            /// Get a stock reply.
            static reply stock_reply(status_type status);
        };

    } // namespace http
} // namespace websocket

#endif /* WEBSOCKETHTTPREPLY_HPP */