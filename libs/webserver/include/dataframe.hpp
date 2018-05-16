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

#ifndef WEBSOCKETDATAFRAME_HPP
#define WEBSOCKETDATAFRAME_HPP

#include <vector>
#include <boost/cstdint.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace websocket {

    // A structure to hold websocket frame data. 
    struct dataframe
    {
        dataframe();

        bool fin;
        enum operation_code { continuation_frame, text_frame, binary_frame, connection_close, ping, pong, reserved } opcode;
        bool mask;
        boost::int8_t fin_opcode;
        boost::int8_t mask_payload_len;
        boost::int8_t payload_len;
        boost::uint16_t extended_payload_len16;
        boost::uint64_t extended_payload_len64;
        boost::array<boost::uint8_t, 4> masking_key;
        std::vector<boost::uint8_t> payload;
        char* stream;
        int size;
        
        /// Convert the dataframe into a vector of buffers. 
        std::vector<boost::asio::const_buffer> to_buffers();
    };

} // namespace websocket

#endif /* WEBSOCKETDATAFRAME_HPP */