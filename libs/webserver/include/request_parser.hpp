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

#ifndef WEBSOCKETSERVERREQUESTPARSER_HPP
#define WEBSOCKETSERVERREQUESTPARSER_HPP

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace websocket {
    namespace http {

        struct request;

        /// Parser for incoming requests.
        class request_parser
        {
        public:
            /// Construct ready to parse the request method.
            request_parser();

            /// Reset to initial parser state.
            void reset();

            /// Parse some data. The tribool return value is true when a complete request
            /// has been parsed, false if the data is invalid, indeterminate when more
            /// data is required. The InputIterator return value indicates how much of the
            /// input has been consumed.
            template <typename InputIterator>
            boost::tuple<boost::tribool, InputIterator> parse(request& req,
                InputIterator begin, InputIterator end)
            {
                while (begin != end)
                {
                    boost::tribool result = consume(req, *begin++);
                    if (result || !result)
                        return boost::make_tuple(result, begin);
                }
                boost::tribool result = boost::indeterminate;
                return boost::make_tuple(result, begin);
            }

        private:
            /// Handle the next character of input.
            boost::tribool consume(request& req, char input);

            /// Check if a byte is an HTTP character.
            static bool is_char(int c);

            /// Check if a byte is an HTTP control character.
            static bool is_ctl(int c);

            /// Check if a byte is defined as an HTTP tspecial character.
            static bool is_tspecial(int c);

            /// Check if a byte is a digit.
            static bool is_digit(int c);

            /// The current state of the parser.
            enum state
            {
                method_start,
                method,
                uri,
                http_version_h,
                http_version_t_1,
                http_version_t_2,
                http_version_p,
                http_version_slash,
                http_version_major_start,
                http_version_major,
                http_version_minor_start,
                http_version_minor,
                expecting_newline_1,
                header_line_start,
                header_lws,
                header_name,
                space_before_header_value,
                header_value,
                expecting_newline_2,
                expecting_newline_3
            } state_;
        };

    } // namespace http
} // namespace websocket

#endif /* WEBSOCKETSERVERREQUESTPARSER_HPP */
