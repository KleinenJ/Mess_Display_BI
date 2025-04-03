/**
  ******************************************************************************
  * File Name          : app_ethernet.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_ETHERNET_H
#define __APP_ETHERNET_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/netif.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
   
   /*Static IP ADDRESS*/
#define IP_ADDR0   EDT_IP_ADDR0
#define IP_ADDR1   EDT_IP_ADDR1
#define IP_ADDR2   EDT_IP_ADDR2
#define IP_ADDR3   EDT_IP_ADDR3
   
/*NETMASK*/
#define NETMASK_ADDR0   EDT_NETMASK_ADDR0
#define NETMASK_ADDR1   EDT_NETMASK_ADDR1
#define NETMASK_ADDR2   EDT_NETMASK_ADDR2
#define NETMASK_ADDR3   EDT_NETMASK_ADDR3

/*Gateway Address*/
#define GW_ADDR0   EDT_GW_ADDR0
#define GW_ADDR1   EDT_GW_ADDR1
#define GW_ADDR2   EDT_GW_ADDR2
#define GW_ADDR3   EDT_GW_ADDR3
   
#define TCP_PORT (uint16_t)EDT_TCPIP_PORT   
   
/* ethernet connect remote tcp server IP config*/
#define REMOTE_SERVER_IP_ADDR0        EDT_REMOTE_SERVER_IP_ADDR0
#define REMOTE_SERVER_IP_ADDR1        EDT_REMOTE_SERVER_IP_ADDR1
#define REMOTE_SERVER_IP_ADDR2        EDT_REMOTE_SERVER_IP_ADDR2
#define REMOTE_SERVER_IP_ADDR3        EDT_REMOTE_SERVER_IP_ADDR3
      
#define REMOTE_SERVER_PORT            (uint16_t)EDT_REMOTE_SERVER_PORT
   
typedef enum{
    IP_CONNECT_NONE = 0,
	STATIC_IP_MODE  = 1,
	DHCP_MODE       = 2,
}ETH_SETTING_MODE;

#pragma pack(1) /// force alignment to 1 byte
typedef struct __eth_info__{
	uint8_t  mode;
	char ip[15];
	char netmask[15];
	char gw[15];
    char mac[18];
    uint16_t port;
}_ETH_Information;
#pragma pack()  /// set alignment back to default

extern bool Eth_initial ;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void User_notification(struct netif *netif) ;
_ETH_Information get_current_net_information(void);

void setLocalNetParameter(void);
void setLocalIP(char *ip);
void setLocalSubNetMask(char *mask);
void setLocalGateWay(char *gateway);
void setLocalMacAddress(char *Mac);
void setLocalNetPort(uint16_t port);
void covertMacAddrToHex(char *MacChar , uint8_t *Machex);
void covertMacAddrToString(uint8_t *MacHex , char *MacStr);

#ifdef __cplusplus
}
#endif

#endif /* __APP_ETHERNET_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

