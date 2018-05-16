/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

/**
 * @file           webserver.hpp
 * @brief          Allow communication using websockets, messages, images streaming
 */

/*==================================================================================================
 *                                        INCLUDE FILES
 * ==================================================================================================*/

#ifndef __STANDALONE__
#include "server.hpp"
using namespace websocket::applications::auditorium;
#endif /* __STANDALONE__ */

#include <string>

/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
 * ==================================================================================================*/

void webserver_run();

void webserver_setup(const char *address, const char *port);

bool webserver_isRunning();

void webserver_setOnInitEventListener(void (*listener)(std::string *name, std::string *value));

void webserver_setOnUpdateEventListener(void (*listener)(std::string *name, std::string *value));

void webserver_setOnCommandEventListener(void (*listener)(std::string *command));

std::string webserver_getStringValueByName(std::string name, std::string default_val);

bool webserver_getBoolValueByName(std::string name, bool default_val);

int webserver_getIntValueByName(std::string name, int default_val);

double webserver_getDoubleValueByName(std::string name, double default_val);

void webserver_sendOutSharedValues();

void webserver_streamBuffer(char* buffer, int size, std::string header);

void webserver_sendCommand(std::string command);

/**
 * @brief         Add value to send buffer
 * @details       Prepare values before it is send
 * 
 * @param         name   Name of HTML DOM 
 * @param         value  Value of HTML DOM 
 * 
 * @return        void
 */
template <class T> void webserver_addOutValue(std::string name, T value)
{
#ifndef __STANDALONE__
  server::getInstance()->room_.addOutValue(name,value);
#endif /* __STANDALONE__ */
}

#endif /* WEBSERVER_HPP */