/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright 2016 NXP 
 * All Rights Reserved
 *
 *****************************************************************************
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
 **************************************************************************************************/


/*==================================================================================================
 *                                        INCLUDE FILES
 * ==================================================================================================*/
/**
 * @file           webserver.cpp
 * @brief          Allow communication using websockets
 */  

#include "webserver.hpp"

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 * ==================================================================================================*/

/**
 * @brief         run webserver
 */
void webserver_run()
{
#ifndef __STANDALONE__
   server::getInstance()->run();
#endif /* __STANDALONE__ */
}

/**
 * @brief         Setup webserver
 * 
 * @param         address   IP address (IPV 4 or 6)
 * @param         port      Server port
 */
void webserver_setup(const char *address, const char *port)
{
#ifndef __STANDALONE__
   server::server_setup(address, port);
#endif /* __STANDALONE__ */
}

/**
 * @brief         check if webserver is running
 * 
 * @return        webserver is running
 */
bool webserver_isRunning()
{
#ifdef __STANDALONE__
   return false;
#else
   return server::getInstance()->isRunning;
#endif /* __STANDALONE__ */
}

/**
 * @brief         Set listener for init event
 * @details       If server notify new value this event is triggered
 * 
 * @param         void   pointer to function event handler, function prototype needs include same parameters (std::string *name, std::string *value)
 */
void webserver_setOnInitEventListener(void (*listener)(std::string *name, std::string *value))
{
#ifndef __STANDALONE__
   server::getInstance()->room_.setOnInitEventListener(listener);
#endif /* __STANDALONE__ */
}

/**
 * @brief         Set listener for update event
 * @details       If server notify change of value this event is triggered
 * 
 * @param         void   pointer to function event handler, function prototype needs include same parameters (std::string *name, std::string *value)
 */
void webserver_setOnUpdateEventListener(void (*listener)(std::string *name, std::string *value))
{
#ifndef __STANDALONE__
   server::getInstance()->room_.setOnUpdateEventListener(listener);
#endif /* __STANDALONE__ */
}

/**
 * @brief         Set listener for command event
 * @details       If server received command message this event is triggered
 * 
 * @param         void   pointer to function event handler, function prototype needs include same parameters (std::string *command)
 */
void webserver_setOnCommandEventListener(void (*listener)(std::string *command))
{
#ifndef __STANDALONE__  
   server::getInstance()->room_.setOnCommandEventListener(listener);
#endif /* __STANDALONE__ */
}

/**
 * @brief         Get String value
 * @details       Get String value from webserver 
 * 
 * @param         name          Name of HTML DOM 
 * @param         default_val   If server did not find the value by name default value is returned   
 * 
 * @return        String value of DOM
 */
std::string webserver_getStringValueByName(std::string name, std::string default_val)
{
#ifdef __STANDALONE__ 
  return default_val;
#else
  if(server::getInstance()->isRunning)
    return server::getInstance()->room_.getStringValueByName(name, default_val);
  else return default_val;
#endif /* __STANDALONE__ */ 
}

/**
 * @brief         Get bool value
 * @details       Get bool value from webserver 
 * 
 * @param         name          Name of HTML DOM 
 * @param         default_val   If server did not find the value by name default value is returned   
 * 
 * @return        bool value of DOM
 */
bool webserver_getBoolValueByName(std::string name, bool default_val)
{
#ifdef __STANDALONE__ 
  return default_val;
#else
  if(server::getInstance()->isRunning)
    return server::getInstance()->room_.getBoolValueByName(name, default_val);
  else return default_val;
#endif /* __STANDALONE__ */ 
}

/**
 * @brief         Get int value
 * @details       Get int value from webserver 
 * 
 * @param         name          Name of HTML DOM 
 * @param         default_val   If server did not find the value by name default value is returned   
 * 
 * @return        int value of DOM
 */
int webserver_getIntValueByName(std::string name, int default_val)
{
#ifdef __STANDALONE__ 
  return default_val;
#else
  if(server::getInstance()->isRunning)
   return server::getInstance()->room_.getIntValueByName(name, default_val);
  else return default_val;
#endif /* __STANDALONE__ */
}

/**
 * @brief         Get double value
 * @details       Get double value from webserver 
 * 
 * @param         name          Name of HTML DOM 
 * @param         default_val   If server did not find the value by name default value is returned   
 * 
 * @return        double value of DOM
 */
double webserver_getDoubleValueByName(std::string name, double default_val)
{
#ifdef __STANDALONE__ 
  return default_val;
#else
  if(server::getInstance()->isRunning)
   return server::getInstance()->room_.getDoubleValueByName(name, default_val);
  else return default_val;
#endif /* __STANDALONE__ */
}

/**
 * @brief         Send buffered values to all clients
 * @details       This function send all values which were stored by function "webserver_addOutValue" in once
 */
void webserver_sendOutSharedValues()
{
#ifndef __STANDALONE__ 
  if(server::getInstance()->isRunning)
   server::getInstance()->room_.sendOutSharedValues();
#endif /* __STANDALONE__ */
}

/**
 * @brief         Stream buffer to clients
 * @details       This function allows to stream buffer of bytes, it can be used for streaming images, or some user defined buffer
 * 
 * @param         buffer   byte array of data
 * @param         size     size of byte array
 * @param         header   header, this is inserted before buffer (some user data which can describe content of buffer)
 */
void webserver_streamBuffer(char* buffer, int size, std::string header)
{
#ifndef __STANDALONE__  
  if(server::getInstance()->isRunning)
   server::getInstance()->room_.streamBuffer(buffer, size, header);
#endif /* __STANDALONE__ */
}

/**
 * @brief         Send Command
 * @details       This can be used when some user own communication is needed, at the client side onCommand event is triggered
 * 
 * @param         command   user own command
 */
void webserver_sendCommand(std::string command)
{
#ifndef __STANDALONE__
  if(server::getInstance()->isRunning)
   server::getInstance()->room_.sendCommand(command);
#endif
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

/** @} */


