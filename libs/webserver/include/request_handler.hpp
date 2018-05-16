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

#ifndef WEBSOCKETHTTPREQUESTHANDLER_HPP
#define WEBSOCKETHTTPREQUESTHANDLER_HPP

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

namespace websocket {
    namespace http {

        struct reply;
        struct request;

        /// The handler for incoming http requests.
        class request_handler
            : private boost::noncopyable
        {
        public:
            /// Handle a request and produce a reply.
            static void handle_request(const request& req, reply& rep);

        private:
            /// Encode data using the SHA1 algorithm.
            static std::vector<unsigned char> to_sha1(const std::string& s);

            /// Encode data using the Base64 algorithm.
            static std::string to_base64(const std::vector<unsigned char>& data);
        };

    } // namespace http
} // namespace websocket

#endif /* WEBSOCKETHTTPREQUESTHANDLER_HPP */