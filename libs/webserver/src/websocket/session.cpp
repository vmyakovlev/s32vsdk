#include "session.hpp"
#include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/uuid/sha1.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>

namespace websocket {
    namespace applications {
		namespace auditorium {

			//room parametr is shared thru all sessions
            session::session(boost::asio::io_service& io_service, room& room)
                : state_(undefined_state), strand_(io_service),
                socket_(io_service), room_(room)
            {
            }

            boost::asio::ip::tcp::socket& session::socket()
            {
                return socket_;
            }

            void session::start()
            {
                state_ = reading_handshake;          
                socket_.async_read_some(boost::asio::buffer(buff_),
                    strand_.wrap(
                    boost::bind(&session::handle_read, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)));
            }

            void session::handle_read(const boost::system::error_code& error,
                std::size_t bytes_transferred)
            {
                if (!error)
                {
                    if (state_ == reading_handshake)
                    {
                        boost::tribool result;
                        boost::tie(result, boost::tuples::ignore) = req_parser_.parse(
                            req_, buff_.data(), buff_.data() + bytes_transferred);

                        if (result)
                        {
                            http::request_handler::handle_request(req_, reply_);

                            state_ = writing_handshake;
                            boost::asio::async_write(socket_, reply_.to_buffers(),
                                strand_.wrap(
                                boost::bind(&session::handle_write, shared_from_this(),
                                boost::asio::placeholders::error)));
                        }
                        else if (!result)
                        {
                            state_ = bad_request;                        
                            reply_ = http::reply::stock_reply(http::reply::bad_request);
                            boost::asio::async_write(socket_, reply_.to_buffers(),
                                strand_.wrap(
                                boost::bind(&session::handle_write, shared_from_this(),
                                boost::asio::placeholders::error)));
                        }
                        else
                        {
                            socket_.async_read_some(boost::asio::buffer(buff_),
                                strand_.wrap(
                                boost::bind(&session::handle_read, shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)));
                        }
                    }
                    else if (state_ == pumping_messages)
                    {
                        boost::tribool result;
                        boost::tie(result, boost::tuples::ignore) = frame_parser_.parse(
                            read_frame_, buff_.data(), buff_.data() + bytes_transferred);

                        if (result)
                        {
                            if (read_frame_.opcode == dataframe::text_frame)
                            {
                                room_.received(read_frame_, shared_from_this());

                                read_frame_.payload.clear();
                                frame_parser_.reset();

                                socket_.async_read_some(boost::asio::buffer(buff_),
                                    strand_.wrap(
                                    boost::bind(&session::handle_read, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred)));
                            }
                            else if (read_frame_.opcode == dataframe::pong ||
                                read_frame_.opcode == dataframe::ping)
                            {
                                read_frame_.payload.clear();
                                frame_parser_.reset();

                                socket_.async_read_some(boost::asio::buffer(buff_),
                                    strand_.wrap(
                                    boost::bind(&session::handle_read, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred)));
                            }
                            else
                            {
                                room_.leave(shared_from_this());
                            }
                        }
                        else if (!result)
                        {
                            room_.leave(shared_from_this());
                        }
                        else
                        {
                            socket_.async_read_some(boost::asio::buffer(buff_),
                                strand_.wrap(
                                boost::bind(&session::handle_read, shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)));
                        }
                    }
                }
                else
                {
                    if (error != boost::asio::error::eof)
                        std::cerr << error.message() << std::endl;

                    room_.leave(shared_from_this());
                }

                // If an error occurs then no new asynchronous operations are started. This
                // means that all shared_ptr references to the connection object will
                // disappear and the object will be destroyed automatically after this
                // handler returns. The connection class's destructor closes the socket.
            }

            void session::handle_write(const boost::system::error_code& error)
            {
                if (!error)
                {
                    if (state_ == writing_handshake)
                    {
                        state_ = pumping_messages;

                        room_.join(shared_from_this());

                        socket_.async_read_some(boost::asio::buffer(buff_),
                                    strand_.wrap(
                                    boost::bind(&session::handle_read, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred)));
                    }
                    else if (state_ == pumping_messages)
                    {
                        if (!write_msgs_.empty())
                        {
                            write_msgs_.pop_front();
                            if (!write_msgs_.empty())
                            {
                                boost::asio::async_write(socket_,
                                    write_msgs_.front().to_buffers(),
                                    strand_.wrap(boost::bind(&session::handle_write, shared_from_this(),
                                    boost::asio::placeholders::error)));
                            }
                        }
                    }
                }
                else
                {
                    if (error != boost::asio::error::eof)
                        std::cerr << error.message() << std::endl;

                    room_.leave(shared_from_this());
                }
            }

            void session::deliver(dataframe msg)
            {
                bool write_in_progress = !write_msgs_.empty();
                write_msgs_.push_back(msg);
                if (!write_in_progress)
                {
                    boost::asio::async_write(socket_,
                        write_msgs_.front().to_buffers(),
                        strand_.wrap(boost::bind(&session::handle_write, shared_from_this(),
                        boost::asio::placeholders::error)));
                }
            }

            std::string session::get_id()
            {
                return socket_.is_open() ? "user" + boost::lexical_cast<std::string>(socket_.remote_endpoint().port()) : "user0";
            }

        } // namespace chat
    } // namespace applications
} // namespace websocket