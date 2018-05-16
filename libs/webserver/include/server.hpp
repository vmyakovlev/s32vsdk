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

#ifndef WEBSOCKETAPPLICATIONSCHATSERVER_HPP
#define WEBSOCKETAPPLICATIONSCHATSERVER_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "room.hpp"
#include "session.hpp"
#include "request_handler.hpp"

namespace websocket {
    namespace applications {
		namespace auditorium {

        /// The top-level class of the chat server.
        class server
            : private boost::noncopyable
        {
		public:
            /// get instance of server
            static server* getInstance();

            /// Run the server's io_service loop.
            void run();

            /// The chat room.
            room room_;

            /// Setup server
            static void server_setup(const char *address, const char *port);
            
            /// Server status
            bool isRunning;

        private:

            /// Singleton instance
            static server* instance;

            /// Construct the server to listen on the specified TCP address and port, and
            /// serve up files from the given directory.
            explicit server(const std::string& address, const std::string& port);

            /// Server address
            static const char *address_;

            /// Server port
            static const char *port_;

            /// Initiate an asynchronous accept operation.
            void start_accept();

            /// Handle completion of an asynchronous accept operation.
            void handle_accept(const boost::system::error_code& e);

            /// Handle a request to stop the server.
            void handle_stop();

            /// The io_service used to perform asynchronous operations.
            boost::asio::io_service io_service_;

            /// The signal_set is used to register for process termination notifications.
            boost::asio::signal_set signals_;

            /// Acceptor used to listen for incoming connections.
            boost::asio::ip::tcp::acceptor acceptor_;

            /// The next connection to be accepted.
            session_ptr new_session_;

            /// The handler for all incoming requests.
            http::request_handler request_handler_;

			/// Running Thread
			boost::thread run_t_;

			/// Function for run thread
			void run_thread();
        };

        } // namespace chat
    } // namespace applications
} // namespace websocket


#endif /* WEBSOCKETAPPLICATIONSCHATSERVER_HPP */
