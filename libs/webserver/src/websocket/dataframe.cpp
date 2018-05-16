#include "dataframe.hpp"
#include "dataframe_parser.hpp"

namespace websocket {

    // For details on the base framing protocol check the following link
    // http://tools.ietf.org/html/rfc6455#section-5.2

    dataframe::dataframe() : fin(true),
        opcode(text_frame), mask(false), fin_opcode(0),
        mask_payload_len(0), payload_len(0), extended_payload_len16(0), extended_payload_len64(0) {
          stream = NULL;
          size = 0;
        }

    std::vector<boost::asio::const_buffer> dataframe::to_buffers()
    {
        std::vector<boost::asio::const_buffer> buffers;

        if (fin)
        {
            fin_opcode |= 0x80;
        }
        
        if (stream != NULL)
        {
            fin_opcode |= 0x2;
        }
        else
        {
            switch (opcode)
            {
                case continuation_frame:    fin_opcode |= 0x0; break;
                case text_frame:            fin_opcode |= 0x1; break;
                case binary_frame:          fin_opcode |= 0x2; break;
                case connection_close:      fin_opcode |= 0x8; break;
                case ping:                  fin_opcode |= 0x9; break;
                case pong:                  fin_opcode |= 0xA; break;
                default:                    fin_opcode |= 0xF; break;
            }
        }

        buffers.push_back(boost::asio::buffer(static_cast<const void*>(&fin_opcode), sizeof(fin_opcode)));

        if(stream ==NULL)
        {
            if (payload.size() < 126)
            {
                mask_payload_len = static_cast<boost::uint8_t>(payload.size());
                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&mask_payload_len), sizeof(mask_payload_len)));
                buffers.push_back(boost::asio::buffer(payload));
            }
            else if (payload.size() < 65536)
            {
                mask_payload_len = 126;
                extended_payload_len16 = static_cast<boost::uint16_t>(payload.size());
                extended_payload_len16 = dataframe_parser::hton16(extended_payload_len16);

                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&mask_payload_len), sizeof(mask_payload_len)));
                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&extended_payload_len16), sizeof(extended_payload_len16)));
                buffers.push_back(boost::asio::buffer(payload));
            }
            else
            {
                mask_payload_len = 127;
                extended_payload_len64 = payload.size();
                extended_payload_len64 = dataframe_parser::hton64(extended_payload_len64);

                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&mask_payload_len), sizeof(mask_payload_len)));
                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&extended_payload_len64), sizeof(extended_payload_len64)));
                buffers.push_back(boost::asio::buffer(payload));
            }
        }
        else 
        {
            if(size + payload.size() < 126)
            {
                mask_payload_len = static_cast<boost::uint8_t>(size + payload.size());
                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&mask_payload_len), sizeof(mask_payload_len)));
                buffers.push_back(boost::asio::buffer(payload));
                buffers.push_back(boost::asio::buffer(stream, size));
            }
            else if (size + payload.size() < 65536)
            {
                mask_payload_len = 126;
                extended_payload_len16 = static_cast<boost::uint16_t>(size + payload.size());
                extended_payload_len16 = dataframe_parser::hton16(extended_payload_len16);

                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&mask_payload_len), sizeof(mask_payload_len)));
                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&extended_payload_len16), sizeof(extended_payload_len16)));
                buffers.push_back(boost::asio::buffer(payload));
                buffers.push_back(boost::asio::buffer(stream, size));
            }
            else
            {
                mask_payload_len = 127;
                extended_payload_len64 = size + payload.size();
                extended_payload_len64 = dataframe_parser::hton64(extended_payload_len64);

                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&mask_payload_len), sizeof(mask_payload_len)));
                buffers.push_back(boost::asio::buffer(static_cast<const void*>(&extended_payload_len64), sizeof(extended_payload_len64)));
                buffers.push_back(boost::asio::buffer(payload));
                buffers.push_back(boost::asio::buffer(stream, size));
            }
        }

        

        return buffers;
    }

}