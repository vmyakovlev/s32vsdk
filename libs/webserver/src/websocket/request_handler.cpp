#include "request_handler.hpp"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/sha1.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include "header.hpp"
#include "reply.hpp"
#include "request.hpp"

namespace websocket {
    namespace http {

        void request_handler::handle_request(const request& req, reply& rep)
        {
            std::string key;
            for (std::vector<header>::const_iterator i = req.headers.cbegin();
                i != req.headers.cend(); ++i)
            {
                if (i->name == "Sec-WebSocket-Key")
                {
                    key = i->value;
                    break;
                }
            }

            if (key.empty())
            {
                rep = reply::stock_reply(reply::bad_request);
                return;
            }

            const std::string magic_guid("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
            std::string buffer(key + magic_guid);
            std::string accept(to_base64(to_sha1(buffer)));

            rep.status = reply::switching_protocols;
            rep.headers.resize(3);
            rep.headers[0].name = "Upgrade";
            rep.headers[0].value = "websocket";
            rep.headers[1].name = "Connection";
            rep.headers[1].value = "Upgrade";
            rep.headers[2].name = "Sec-WebSocket-Accept";
            rep.headers[2].value = accept;
        }

        std::vector<unsigned char> request_handler::to_sha1(const std::string& s)
        {
            boost::uuids::detail::sha1 cipher;
            cipher.process_bytes(s.c_str(), s.length());
            boost::uint32_t digest[5];
            cipher.get_digest(digest);

            std::vector<unsigned char> hash(20, 0);
            for (std::size_t i = 0; i < hash.size(); ++i)
            {
                hash[i] = (unsigned char)(digest[i >> 2] >> 8 * (3 - (i & 0x03)));
            }

            return hash;
        }

        std::string request_handler::to_base64(const std::vector<unsigned char>& data)
        {
            using namespace boost::archive::iterators;

            typedef 
                insert_linebreaks<         // insert line breaks every 72 characters
                    base64_from_binary<    // convert binary values ot base64 characters
                        transform_width<   // retrieve 6 bit integers from a sequence of 8 bit bytes
                            const char *,
                            6,
                            8
                        >
                    > 
                    ,72
                >        
                base64_iterator;

            std::vector<unsigned char> buff(data);
            size_t number_of_padd_chars = (buff.size() % 3 != 0) ? 3 - (buff.size() % 3) : 0;            
            buff.insert(buff.end(), number_of_padd_chars, '\0');

            base64_iterator begin(&buff[0]), end(&buff[0] + data.size()); 
            std::string result(begin, end);
            result.insert(result.end(), number_of_padd_chars, '=');

            return result;
        }

    } // namespace http
} // namespace websocket