/**
  ******************************************************************************
  * File Name          : tcp_server.h
  * @author            : EDT Embedded Application Team
  * Description        : This file includes the initial function of hardware on
  *                      smart embedded display board.
  * @brief             : EDT <https://smartembeddeddisplay.com/>
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT(c) 2024 Emerging Display Technologies Corp.
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of Emerging Display Technologies Corp. nor the names of
  *      its contributors may be used to endorse or promote products derived from
  *      this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32h7xx_hal.h"   
#include <stdbool.h>
#include <string.h>
#include "lwip/netif.h"

typedef struct _tcpserver_Rx_Message{
  bool ETH_EN;
  uint8_t ETH_Buf[128];
  uint32_t ETH_Size;
}TCPServer_Rx_Message;

extern TCPServer_Rx_Message TcpServer_Rx_St;

void tcpserver_init(void);
void tcpserver_Start(void);
void tcpserver_Stop(void);
void tcpserver_get_remote_tcpclient_ip(char* client_ip);
void tcpserver_set_Local_port(uint16_t port);
uint16_t tcpserver_get_Local_port(void);
int8_t tcpserver_send_data_to_tcpclient(uint8_t *pdata,uint16_t len);
uint16_t tcpserver_receive_data_from_tcpclient(uint8_t *pdata);

#ifdef __cplusplus
}
#endif

#endif /* __TCP_SERVER_H__ */
