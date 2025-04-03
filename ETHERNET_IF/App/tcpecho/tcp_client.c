/**
  ******************************************************************************
  * File Name          : tcp_client.c
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tcp_client.h"
#include "lwip/opt.h"

#if LWIP_NETCONN

static struct netconn *tcp_client_handle = NULL;

#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/inet.h"
/* Private typedef -----------------------------------------------------------*/
extern struct netif gnetif;
/* Private define ------------------------------------------------------------*/
#define TCPCLIENT_THREAD_PRIO  (osPriorityRealtime)

osThreadId client_tcpecho_id;
static void tcpclient_thread(void *arg);

uint8_t serverIP[4]={0};
uint16_t remote_server_port = 0 ;

TCPClient_Rx_Message TcpClient_Rx_St;  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*****************************************************************************
  * @brief  tcpclient_thread
  * @param  
  * @retval None
******************************************************************************/
static void tcpclient_thread(void *arg)
{ 
         struct netconn *client_conn;
         ip_addr_t DestIPaddr;
         err_t err;
         struct netbuf *rx_net_buf;

         LWIP_UNUSED_ARG(arg);
        while (1)
          {
          if(gnetif.ip_addr.addr !=0){
          client_conn = netconn_new(NETCONN_TCP);
           if(client_conn !=NULL){
             netconn_bind(client_conn, IP_ADDR_ANY, 5001);
             IP4_ADDR(&DestIPaddr, serverIP[0], serverIP[1], serverIP[2], serverIP[3]);

             err = netconn_connect(client_conn, &DestIPaddr, remote_server_port);
             if(err == ERR_OK){
                tcp_client_handle = client_conn;
                void *data;
                u16_t len;
                  while ((err = netconn_recv(client_conn, &rx_net_buf)) == ERR_OK) {
                  do {
                      netbuf_data(rx_net_buf, &data, &len);

                      TcpClient_Rx_St.ETH_EN=true;
                      TcpClient_Rx_St.ETH_Size=len;
                      memcpy(TcpClient_Rx_St.ETH_Buf,data,len);

                  } while (netbuf_next(rx_net_buf) >= 0);
                  netbuf_delete(rx_net_buf);
              }
              netconn_close(client_conn);
              netconn_delete(client_conn);
              tcp_client_handle = NULL;
            }
            else
            {
              netconn_close(client_conn);
              netconn_delete(client_conn);
              tcp_client_handle = NULL;
            }
          }
         }
         osDelay(100);
        }
}
/*****************************************************************************
  * @brief  initial thread for build local tcp client to connecting remote tcp server
  * @param  
  * @retval None
******************************************************************************/
void tcpclient_init(void)
{ 	
  if(client_tcpecho_id==NULL){
    client_tcpecho_id =sys_thread_new("tcpclient_thread", tcpclient_thread, NULL,  (configMINIMAL_STACK_SIZE*8), TCPCLIENT_THREAD_PRIO);
  }

  if(client_tcpecho_id != NULL){
      osThreadSuspend (client_tcpecho_id);
      memset(tcp_client_handle,0,sizeof(struct netconn));
  }
}

/*****************************************************************************
  * @brief  start connect remote TCP Server thread
  * @param  
  * @retval None
******************************************************************************/
void tcpclient_connect(char *ip ,uint16_t port)
{ 
   tcpclient_set_remote_tcpserver_ip(ip,port);

  if(client_tcpecho_id!=NULL){
           osThreadResume (client_tcpecho_id);
  }
}
/*****************************************************************************
  * @brief  stop connect remote TCP server thread
  * @param  
  * @retval None
******************************************************************************/
void tcpclient_disconnect(void)
{
  if(client_tcpecho_id!=NULL){   
           osThreadSuspend (client_tcpecho_id);
  }
}
/*****************************************************************************
  * @brief    setting connect remote tcp server id
  * @param    ip string ex. :"192.168.1.2"
  * @param    port ex. : 5001
  * @example  set_remote_tcp_server_id("192.168.1.2",5001);
  * @retval   None
******************************************************************************/
void tcpclient_set_remote_tcpserver_ip(char *ip ,uint16_t port)
{
  uint32_t id = inet_addr(ip);
  
	serverIP[0] = id;
	serverIP[1] = id>>8;
	serverIP[2] = id>>16;
	serverIP[3] = id>>24;
        remote_server_port = port;      
}
/*****************************************************************************
  * @brief    get current connectiong remote server id 
  * @param  
  * @retval   remote_ip :return current connecting remote server id
  * @example  0x0301A8C0 ip0=0xC0 ip1=0xA8 ip2=0x01 ip3=0x03
******************************************************************************/
void tcpclient_get_remote_tcpserver_ip(char *remoteClientIP)
{
  uint32_t remote_ip   =0 ;  
  char server_address[15];
  ip_addr_t server_ipaddr;
  
  if(gnetif.ip_addr.addr !=0){
   if((tcp_client_handle->type == NETCONN_TCP)&&(tcp_client_handle!=NULL)){
     netconn_getaddr(tcp_client_handle, &server_ipaddr, &remote_server_port, 0);  
     if(server_ipaddr.addr!=0){
     remote_ip = (uint32_t)server_ipaddr.addr;    
     memset(server_address,0,sizeof(server_address));
     memcpy(server_address , (inet_ntoa(remote_ip)) , sizeof(server_address));
     memcpy(remoteClientIP,server_address, sizeof(server_address));
     }   
    }else  if(tcp_client_handle->type == NETCONN_INVALID)
    {
    	memset(remoteClientIP,0, sizeof(server_address));
    }
   } 
}
/*****************************************************************************
  * @brief  get current connecting remote server state
  * @param
  * @retval 0: unconnected 1:connected
******************************************************************************/
bool tcpclient_get_connect_state(void)
{
  struct netconn *conn = tcp_client_handle;

   if(gnetif.ip_addr.addr !=0){
    if((conn->type == NETCONN_TCP)&&(conn!=NULL)){
      if(conn !=NULL){
        return true;
      }else{
        return false;
      }
     }
    else{
      return false;
    }
   }
  return false;
}
/*****************************************************************************
  * @brief  send date from tcp client to tcp server
  * @param  
  * @retval -1: not finish 0:finish
******************************************************************************/
int8_t tcpclient_send_data_to_tcpserver(uint8_t *pdata,uint16_t len)
{
  err_t err = ERR_CONN ;
  struct netconn *conn = tcp_client_handle;
  if(gnetif.ip_addr.addr !=0){
    if((conn->type == NETCONN_TCP)&&(conn!=NULL)){
      if(conn ==NULL){
       return -1 ;
      }   
      err = netconn_write(conn, pdata, len, NETCONN_COPY);
        if(err !=ERR_OK){
          return -1 ;
        }else{
          return 0;
        }
     }
   }
   return -1;
}
/*****************************************************************************
  * @brief  receive datat from tcp server to tcp server
  * @param  
  * @retval receive data length 
******************************************************************************/
uint16_t tcpclient_receive_data_from_tcpserver(uint8_t *pdata)
{
  uint16_t len;
  if( TcpClient_Rx_St.ETH_EN)
  {
      len =  TcpClient_Rx_St.ETH_Size;
      memcpy(pdata , TcpClient_Rx_St.ETH_Buf ,len);
      memset(&TcpClient_Rx_St,0,sizeof(TcpClient_Rx_St));
      return len;
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
