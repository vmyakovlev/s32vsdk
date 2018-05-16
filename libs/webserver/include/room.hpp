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

#ifndef WEBSOCKETAPPLICATIONSCHATROOM_HPP
#define WEBSOCKETAPPLICATIONSCHATROOM_HPP

#include <string>
#include <algorithm>
#include <deque>
#include <set>
#include <boost/lexical_cast.hpp>
#include <boost/chrono/thread_clock.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include "dataframe.hpp"
#include "participant.hpp"


namespace websocket {
    namespace applications {
		namespace auditorium {
            
            /// Delegate Shared Values, function template
            typedef boost::signals2::signal<void (std::string *name, std::string *value)> SharedValues;
            
            /// Delegate Command, function template
            typedef boost::signals2::signal<void (std::string *command)> Command;
            
            /// Queue for messages
            typedef std::deque<dataframe> message_queue;
            
            /// Struct for sharing properties with clients 
            typedef struct shared_prop
            {
               std::string name;
               std::string value;
               
            }shared_prop;
            
            /// The room class.
            class room
            {
            public:              
                /// OnInitListener
                typedef SharedValues::slot_type SharedValuesListener;            
                void setOnInitEventListener(const SharedValuesListener listener);
                
                /// OnUpdateListener
                void setOnUpdateEventListener(const SharedValuesListener listener);
                
                /// OnCommandListener
                typedef Command::slot_type CommandListener;
                void setOnCommandEventListener(const CommandListener listener);
                
                /// Join the room.
                void join(participant_ptr participant);

                /// Leave the room.
                void leave(participant_ptr participant);

                /// Deliver a chat message to all participant in the room.
                void received(const dataframe& msg, participant_ptr source);

                /// Get string value by name
                std::string getStringValueByName(std::string name, std::string default_val);
                
                /// Get Bool value by name
                bool getBoolValueByName(std::string name, bool default_val);
                
                /// Get Int value by name
                int getIntValueByName(std::string name, int default_val);

                /// Get Double value by name
                double getDoubleValueByName(std::string name, double default_val);
                
                /// Prepare out shared bool value
                void addOutValue(std::string name, bool value);
                
                /// Template function for share output values
                template <class T> void addOutValue(std::string name, T value)
                {
                      this->mtx_shared.lock();
                      out_shared_properties.push_back({name, boost::lexical_cast<std::string>(value)});
                      this->mtx_shared.unlock();
                }
                
                /// Function that sends message to all clients with prepared shared variables
                void sendOutSharedValues();

                /// Stream image or your own buffer
                void streamBuffer(char* buffer, int size, std::string header);
                
                /// Send your own command
                void sendCommand(std::string command);

            private:
                /// Vector of shared properties
                std::vector<shared_prop> shared_properties;
                
                /// Vector of output shared properties, if something from app has to be visualized
                std::vector<shared_prop> out_shared_properties;
                
                /// OnInit function
                SharedValues onInit;
                
                /// OnUpdate function
                SharedValues onUpdate;
                
                /// OnCommand function
                Command onCommand;
                
                /// Boost mutex due to thread safe
                boost::mutex mtx_send;
                
                /// Boost mutex due to thread safe
                boost::mutex mtx_shared;
              
                /// Deliver a message to all participant in the room.
                void deliver(const dataframe& msg);
                
                /// Deliver a message to all participant in the room exept sender
                void deliver_except(const dataframe& msg, participant_ptr participant);

                /// Update a number of connected participants. 
                void update_participants();
                
                /// Get shared_prop by name
                std::vector<shared_prop>::iterator getSharedProp(std::string *name);
                
                /// Set of participants
                std::set<participant_ptr> participants_;

                /// Send shared properties
                void welcome_message(participant_ptr participant);
            };

        } // chat
    } // namespace applications
} // namespace websocket

#endif /* WEBSOCKETAPPLICATIONSCHATROOM_HPP */
