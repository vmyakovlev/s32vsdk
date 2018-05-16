#include "room.hpp"

using namespace boost::chrono;

namespace websocket {
    namespace applications {
		namespace auditorium {

            void room::setOnInitEventListener(const SharedValuesListener listener)
            {
                onInit.connect(listener);
            }
            
            void room::setOnUpdateEventListener(const SharedValuesListener listener)
            {
                onUpdate.connect(listener);
            }
            
            void room::setOnCommandEventListener(const CommandListener listener)
            {
                onCommand.connect(listener);
            }
                  
            void room::join(participant_ptr participant)
            {
                participants_.insert(participant);

                update_participants();

                welcome_message(participant);
            }

            void room::leave(participant_ptr participant)
            {
                participants_.erase(participant);

                update_participants();
            }

            void room::received(const dataframe& msg, participant_ptr source)
            {
                std::string message(msg.payload.begin(), msg.payload.end());
                std::string respond = "shared:";
                
                int index_of_doudot = message.find(':');
                if(index_of_doudot < 0)
                {
                    printf("Wrong input format, please use <command>:<data>\n");
                    return;
                }
                  
                std::string command = message.substr(0, index_of_doudot);
                message = message.substr(index_of_doudot + 1);
                
                if(command.compare("command") == 0)
                {
                    onCommand(&message);
                    return;
                }

                bool update = command.compare("shared_update") == 0 ? true : false;
                bool init = shared_properties.size() == 0 ? true : false;
                
                while (message.size() > 0)
                {
                    int index = message.find('&');
                    if (index < 0) break;

                    std::string name = message.substr(0, index);
                    message = message.substr(index + 1);

                    index = message.find('&');
                    std::string value = message.substr(0, index);
                    message = message.substr(index + 1);

                    
                   std::vector<shared_prop>::iterator item = getSharedProp(&name);
                    
                    /// if shared variable exists update
                    if(item != shared_properties.end())
                    {
                        if(update)
                        {
                          onUpdate(&name,&value);
                          item->value = value;
                        }
                    }
                    else
                    {
                        onInit(&name,&value);
                        shared_properties.push_back({name, value});
                    }
                    
                    respond += name + "&";
                    respond += value + "&";                 
                }
                if(init)
                {
                    dataframe frm;
                    std::copy(respond.begin(), respond.end(), std::back_inserter(frm.payload));
                    deliver(frm);
                }
                else if(update)
                {
                    dataframe frm;
                    std::copy(respond.begin(), respond.end(), std::back_inserter(frm.payload));
                    deliver_except(frm, source);
                }
            }
            
            std::vector<shared_prop>::iterator room::getSharedProp(std::string *name)
            {
                  std::vector<shared_prop>::iterator item = std::find_if(
                          shared_properties.begin(),
                          shared_properties.end(),
                          boost::bind(&shared_prop::name, _1) == *name);
                  return item;
            }

            std::string room::getStringValueByName(std::string name, std::string default_val)
            {
                  std::vector<shared_prop>::iterator item = getSharedProp(&name);
                  if(item == shared_properties.end()) return default_val;
                  return item->value;
            }
            
            bool room::getBoolValueByName(std::string name, bool default_val)
            {
                  std::vector<shared_prop>::iterator item = getSharedProp(&name);
                  if(item == shared_properties.end()) return default_val;
                  return item->value.compare("true") == 0 ? true : false;
            }
            
            int room::getIntValueByName(std::string name, int default_val)
            {
                  std::vector<shared_prop>::iterator item = getSharedProp(&name);
                  if(item == shared_properties.end()) return default_val;
                  if(item->value.compare("") == 0) return default_val;
                  return  boost::lexical_cast<int>(item->value);
            }
            
            double room::getDoubleValueByName(std::string name, double default_val)
            {
                  std::vector<shared_prop>::iterator item = getSharedProp(&name);
                  if(item == shared_properties.end()) return default_val;
                  if(item->value.compare("") == 0) return default_val;
                  return  boost::lexical_cast<double>(item->value);
            }

            void room::addOutValue(std::string name, bool value)
            {
                  this->mtx_shared.lock();
                  if(value) out_shared_properties.push_back({name, "true"});
                  else out_shared_properties.push_back({name, "false"});
                  this->mtx_shared.unlock();
            }

            void room::deliver(const dataframe& msg)
            {
                std::for_each(participants_.begin(), participants_.end(),
                    boost::bind(&participant::deliver, _1, boost::ref(msg)));
            }
            
            void room::deliver_except(const dataframe& msg, participant_ptr participant)
            {
                participants_.erase(participant);
                
                std::for_each(participants_.begin(), participants_.end(),
                    boost::bind(&participant::deliver, _1, boost::ref(msg)));
                
                participants_.insert(participant);
            }

            void room::update_participants()
            {
                std::string header = "connected:" + boost::lexical_cast<std::string>(participants_.size());

                dataframe frm;
                std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));

                std::for_each(participants_.begin(), participants_.end(),
                    boost::bind(&participant::deliver, _1, boost::ref(frm)));
            }

            void room::welcome_message(participant_ptr participant)
            {
            	std::string message = "shared:";
            	for (unsigned int i = 0; i < shared_properties.size(); i++) {
            		message += shared_properties.at(i).name + "&";
            		message += shared_properties.at(i).value + "&";
		}

            	dataframe frm;
            	std::copy(message.begin(), message.end(), std::back_inserter(frm.payload));
            	participant->deliver(frm);
            }
            
            void room::sendOutSharedValues()
            {      
                std::string message = "shared:";
                for (unsigned int i = 0; i < out_shared_properties.size(); i++) {
                        message += out_shared_properties.at(i).name + "&";
                        message += out_shared_properties.at(i).value + "&";
                }
                
                out_shared_properties.clear();
                
                this->mtx_send.lock();
                
                dataframe frm;
                std::copy(message.begin(), message.end(), std::back_inserter(frm.payload));
                std::for_each(participants_.begin(), participants_.end(),
                    boost::bind(&participant::deliver, _1, boost::ref(frm)));
                
                this->mtx_send.unlock();
            }

            void room::streamBuffer(char* buffer, int size , std::string header)
            {			
                dataframe frm;
        
                frm.stream = buffer;
                frm.size = size;
                
                header += "&";
                
                this->mtx_send.lock();
                
                std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));            
                std::for_each(participants_.begin(), participants_.end(),
                        boost::bind(&participant::deliver, _1, boost::ref(frm)));
                
                this->mtx_send.unlock();
            }
            
            void room::sendCommand(std::string command)
            {
                dataframe frm;
                
                std::string header = "command:" + command;
                
                this->mtx_send.lock();
                
                std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));               
                std::for_each(participants_.begin(), participants_.end(),
                        boost::bind(&participant::deliver, _1, boost::ref(frm)));
                
                this->mtx_send.unlock();
            }
			

        } // chat
    } // namespace applications
} // namespace websocket
