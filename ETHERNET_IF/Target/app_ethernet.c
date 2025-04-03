/**
  ******************************************************************************
  * File Name          : app_ethernet.c
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
/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "main.h"
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif
#include "app_ethernet.h"
#include "ethernetif.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/inet.h"
#include "tcp_server.h"
#include "tcp_client.h"

#if defined(USE_dp83848)
#include "dp83848/dp83848.h"
extern dp83848_Object_t DP83848;
#endif

extern ETH_HandleTypeDef heth;

bool Eth_initial =0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

 extern struct netif gnetif; /* network interface structure */
 static _ETH_Information eth_information_st;
/*****************************************************************************
  * @brief  get_current_net_information
  * @param  
  * @retval None
******************************************************************************/
_ETH_Information get_current_net_information(void)
{
	  _ETH_Information _eth_info;
      uint32_t _ip,_netmask,_gw;

      _eth_info.mode = 0;
      _ip       = ip4_addr_get_u32(netif_ip4_addr(&gnetif));//(uint32_t)gnetif.ip_addr;
	  _netmask  = ip4_addr_get_u32(netif_ip4_netmask(&gnetif));
	  _gw       = ip4_addr_get_u32(netif_ip4_gw(&gnetif));
         
      memcpy(_eth_info.ip , (inet_ntoa(_ip)) , sizeof(_eth_info.ip));
      memcpy(_eth_info.gw , (inet_ntoa(_gw)) , sizeof(_eth_info.gw));
      memcpy(_eth_info.netmask , (inet_ntoa(_netmask)) , sizeof(_eth_info.netmask));
         
  
      covertMacAddrToString(gnetif.hwaddr,_eth_info.mac);
      _eth_info.port = tcpserver_get_Local_port();
        
	return _eth_info;
}
/*****************************************************************************
  * @brief  setLocalNetParameter
  * @param  
  * @retval None
******************************************************************************/
void setLocalNetParameter(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;  
  uint8_t Mac[6];


  uint32_t ip32 = inet_addr(eth_information_st.ip); 
  uint32_t netmask32 = inet_addr(eth_information_st.netmask);
  uint32_t gw32 = inet_addr(eth_information_st.gw);
  
  covertMacAddrToHex(eth_information_st.mac , Mac);
  
  IP_ADDR4(&ipaddr,(uint8_t)(ip32&0xff),(uint8_t)(ip32>>8), (uint8_t)(ip32>>16),(uint8_t)(ip32>>24));
  IP_ADDR4(&netmask,(uint8_t)(netmask32&0xff),(uint8_t )(netmask32>>8), (uint8_t)(netmask32>>16),(uint8_t)(netmask32>>24));
  IP_ADDR4(&gw,(uint8_t)(gw32&0xff),(uint8_t)(gw32>>8), (uint8_t)(gw32>>16),(uint8_t)(gw32>>24));  
  
  netif_set_addr(&gnetif,&ipaddr, &netmask, &gw);
   
  gnetif.hwaddr[0] =  Mac[0];
  gnetif.hwaddr[1] =  Mac[1];
  gnetif.hwaddr[2] =  Mac[2];
  gnetif.hwaddr[3] =  Mac[3];
  gnetif.hwaddr[4] =  Mac[4];
  gnetif.hwaddr[5] =  Mac[5];  
  
  heth.Instance->MACA0HR = (((uint32_t)(Mac[5]) << 8) | (uint32_t)Mac[4]);
  /* Set MAC addr bits 0 to 31 */
  heth.Instance->MACA0LR = (((uint32_t)(Mac[3]) << 24) | ((uint32_t)(Mac[2]) << 16) |
                             ((uint32_t)(Mac[1]) << 8) | (uint32_t)Mac[0]);
  
  
  tcpserver_set_Local_port(eth_information_st.port) ;

  
  HAL_ETH_Stop_IT(&heth);
  netif_set_down(&gnetif);
  netif_set_link_down(&gnetif);
}

/*****************************************************************************
  * @brief  setLocalIP
  * @param  
  * @retval None
******************************************************************************/
void setLocalIP(char *ip)
{
   memset(eth_information_st.ip,0,sizeof(eth_information_st.ip));
   memcpy(eth_information_st.ip , ip ,sizeof(eth_information_st.ip));
}
/*****************************************************************************
  * @brief  setLocalSubNetMask
  * @param  
  * @retval None
******************************************************************************/
void setLocalSubNetMask(char *netmask)
{
    memset(eth_information_st.netmask,0,sizeof(eth_information_st.netmask));
    memcpy(eth_information_st.netmask , netmask , sizeof(eth_information_st.netmask));
}
/*****************************************************************************
  * @brief  setLocalGateWay
  * @param  
  * @retval None
******************************************************************************/
void setLocalGateWay(char *gateway)
{
   memset(eth_information_st.gw,0,sizeof(eth_information_st.gw));
   memcpy(eth_information_st.gw , gateway ,sizeof(eth_information_st.gw));
}
/*****************************************************************************
  * @brief  setLocalMacAddress
  * @param  
  * @retval None
******************************************************************************/
void setLocalMacAddress(char *Mac)
{
    memset(eth_information_st.mac,0,sizeof(eth_information_st.mac));
    memcpy(eth_information_st.mac , Mac ,sizeof(eth_information_st.mac));   
}
/*****************************************************************************
  * @brief  setLocalNetPort
  * @param  
  * @retval None
******************************************************************************/
void setLocalNetPort(uint16_t port)
{
    //tcpserver_set_Local_port(port) ;
  eth_information_st.port =port;
}
/*****************************************************************************
  * @brief  covertMacAddrToHex
  * @param  
  * @retval None
******************************************************************************/
void covertMacAddrToHex(char *MacChar , uint8_t *Machex)
{ 
  char* ptr;

  Machex[0] = strtol( strtok(MacChar,":"), &ptr, 16 );
  for( uint8_t i = 1; i < 6; i++ )
  {
    Machex[i] = strtol( strtok( NULL,":"), &ptr, 16 );
  }
}
/*****************************************************************************
  * @brief  covertMacAddrToString
  * @param  
  * @retval None
******************************************************************************/
void covertMacAddrToString(uint8_t *MacHex , char *MacStr)
{ 
 char macStr[18];
 
 memset(macStr,0,sizeof(macStr));
 snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
         MacHex[0], MacHex[1], MacHex[2], MacHex[3], MacHex[4], MacHex[5]);
 
 memcpy(MacStr,macStr,sizeof(macStr));
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
