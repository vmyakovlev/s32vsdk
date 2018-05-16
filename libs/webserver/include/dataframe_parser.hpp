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

#ifndef WEBSOCKETDATAFRAMEPARSER_HPP
#define WEBSOCKETDATAFRAMEPARSER_HPP

#include <boost/cstdint.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace websocket {

        struct dataframe;

        /// Parser for incoming dataframes.
        class dataframe_parser
        {
        public:
            /// Construct ready to parse the dataframe.
            dataframe_parser();

            /// Reset to initial parser state.
            void reset();

            /// Parse some data. The tribool return value is true when a complete dataframe
            /// has been parsed, false if the data is invalid, indeterminate when more
            /// data is required. The InputIterator return value indicates how much of the
            /// input has been consumed.
            template <typename InputIterator>
            boost::tuple<boost::tribool, InputIterator> parse(dataframe& frame,
                InputIterator begin, InputIterator end)
            {
                while (begin != end)
                {
                    boost::tribool result = consume(frame, *begin++);
                    if (result || !result)
                        return boost::make_tuple(result, begin);
                }
                boost::tribool result = boost::indeterminate;
                return boost::make_tuple(result, begin);
            }

            /// Convert a uint16_t from the network byte order to the host byte order.
            static boost::uint16_t ntoh16(boost::uint16_t net16);

            /// Convert a uint16_t from the host byte order to the network byte order.
            static boost::uint16_t hton16(boost::uint16_t net16);

            /// Convert a uint64_t from the network byte order to the host byte order.
            static boost::uint64_t ntoh64(boost::uint64_t net64);

            /// Convert a uint64_t from the host byte order to the network byte order.
            static boost::uint64_t hton64(boost::uint64_t net64);

        private:
            /// Handle the next character of input.
            boost::tribool consume(dataframe& frame, boost::uint8_t input);

            /// Get a number of bits at the specified offset.
            boost::uint8_t get_bits(char b, boost::uint8_t offset, boost::uint8_t count);

            /// The current state of the parser.
            enum state
            {
                fin_opcode,
                mask_payload_len,
                extended_payload_len1,
                extended_payload_len2,
                extended_payload_len3,
                extended_payload_len4,
                extended_payload_len5,
                extended_payload_len6,
                extended_payload_len7,
                extended_payload_len8,
                masking_key1,
                masking_key2,
                masking_key3,
                masking_key4,
                payload
            } state_;
        };

} // namespace websocket

#endif /* WEBSOCKETDATAFRAMEPARSER_HPP */