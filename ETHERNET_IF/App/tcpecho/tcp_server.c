/**
  ******************************************************************************
  * File Name          : tcp_server.c
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
#include "tcp_server.h"
#include "lwip/opt.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#if LWIP_NETCONN

struct netconn *connbackup =NULL;
struct netconn *tcp_server_handle=NULL;

#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/ip4.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"


/* Private typedef -----------------------------------------------------------*/
extern struct netif gnetif;
/* Private define ------------------------------------------------------------*/
#define TCPECHO_THREAD_PRIO  (osPriorityRealtime)

/* Private macro -------------------------------------------------------------*/
osThreadId tcpserver_id;
ip_addr_t client_ipaddr;
uint16_t remote_client_port;
uint16_t local_port;
uint8_t clientIP[4]={0};
TCPServer_Rx_Message TcpServer_Rx_St;
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void netCallback(struct netconn *pconn, enum netconn_evt event, u16_t len);

/*****************************************************************************
  * @brief  Can bus 2 Initial
  * @param  
  * @retval None
******************************************************************************/
static void tcpserver_thread(void *arg)
{
  struct netconn *conn=NULL, *newconn=NULL;
  err_t err;  
  struct netbuf *buf;
  void *data;
  u16_t len;


  conn = netconn_new_with_callback(NETCONN_TCP, netCallback);
    
  netconn_set_nonblocking(conn,NETCONN_FLAG_NON_BLOCKING);

  netconn_bind(conn, IP_ADDR_ANY, local_port);
  
  netconn_listen(conn);

  while (1) {

   if(netif_is_link_up(&gnetif))
    {
     if(conn!=NULL)
     {
       connbackup = conn;

       err = netconn_accept(conn, &newconn);

    /* Process the new connection. */
      if (err == ERR_OK) {
      
        tcp_server_handle = newconn;

        while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
         do {
             netbuf_data(buf, &data, &len);
         } while (netbuf_next(buf) >= 0);
         memcpy(TcpServer_Rx_St.ETH_Buf,data,len);
         TcpServer_Rx_St.ETH_EN=true;
         TcpServer_Rx_St.ETH_Size=len;
         netbuf_delete(buf);
        }
       newconn->pending_err=ERR_CLSD; //It is hacky way to be sure than callback will don't do treatment on a netconn closed and deleted
       netconn_close(newconn);
       netconn_delete(newconn);
       tcp_server_handle=NULL;
      }
      else
      {
       netconn_delete(newconn);
      }
     }/* conn!=NULL */
    }/* gnetif is alive */
     osDelay(1);
  }/* while */
}

/*****************************************************************************
  * @brief  netconn netCallback
  * @param
  * @retval None
******************************************************************************/
static void netCallback(struct netconn *conn, enum netconn_evt evt, u16_t len)
{

}
/*****************************************************************************
  * @brief  tcpserver_set_Local_port
  * @param  set local port
  * @retval None
******************************************************************************/
void tcpserver_set_Local_port(uint16_t port)
{  
   local_port = port ;   

      if(connbackup!=NULL)
      {
          connbackup->pcb.tcp->local_port = local_port;
          netconn_bind_if(connbackup,0);
          netconn_listen(connbackup); 
      }
}
/*****************************************************************************
  * @brief  tcpserver_get_Local_port
  * @param  
  * @retval get loacal port
******************************************************************************/
uint16_t tcpserver_get_Local_port(void)
{
  return local_port;
}
/*****************************************************************************
  * @brief  tcpserver_get_remote_tcpclient_ip
  * @param  
  * @retval None
******************************************************************************/
void tcpserver_get_remote_tcpclient_ip(char* remoteclient_ip)
{
  char client_address[15];
  char *remote_address;
  uint32_t remote_ip =0 ;  
  remote_client_port   = 0;
  ip_addr_t remote_client_ipaddr ;
  int i=0;
  
  remote_client_ipaddr.addr = 0 ;
  
  for(i=0;i<sizeof(client_address);i++)
        remoteclient_ip[i]=0;

  if(netif_is_link_up(&gnetif)){
    if((tcp_server_handle!=NULL)&&(tcp_server_handle->type ==NETCONN_TCP)&&(tcp_server_handle->state ==NETCONN_NONE))
     {
       netconn_peer(tcp_server_handle, &remote_client_ipaddr, &remote_client_port);

       remote_ip = (uint32_t)remote_client_ipaddr.addr;
    
       for(i=0;i<sizeof(client_address);i++)
         client_address[i]=0;
    
       remote_address = (inet_ntoa(remote_ip));

       for(i=0;i<sizeof(client_address);i++)
         client_address[i]=remote_address[i];
    
       for(i=0;i<sizeof(client_address);i++)
         remoteclient_ip[i]=client_address[i];
 
     }
   }
}
/*****************************************************************************
  * @brief  tcpserver_init
  * @param  
  * @retval None
******************************************************************************/
void
tcpserver_init(void)
{   
   if(tcpserver_id== NULL){ 
      tcpserver_id = sys_thread_new("tcpserver_thread", tcpserver_thread, NULL,  (configMINIMAL_STACK_SIZE*8), TCPECHO_THREAD_PRIO);
   } 
    if(tcpserver_id != NULL)
      osThreadSuspend (tcpserver_id);

   local_port = 7;

}
/*****************************************************************************
  * @brief  tcpserver_Start
  * @param  
  * @retval None
******************************************************************************/
void
tcpserver_Start(void)
{
	if(netif_is_link_up(&gnetif)){
       if(tcpserver_id!= NULL){
        osThreadResume (tcpserver_id);
      }
	}
}

/*****************************************************************************
  * @brief  tcpserver_Stop
  * @param  
  * @retval None
******************************************************************************/
void
tcpserver_Stop(void)
{     
	if(netif_is_link_up(&gnetif)){
       if(tcpserver_id!= NULL){
        osThreadSuspend (tcpserver_id);
       }
    }
}
/*****************************************************************************
  * @brief  send date from tcp client to tcp server
  * @param  
  * @retval -1: not finish 0:finish
******************************************************************************/
int8_t tcpserver_send_data_to_tcpclient(uint8_t *pdata,uint16_t len)
{
  err_t err = ERR_CONN ;
  struct netconn *conn = tcp_server_handle;
  if(conn!=NULL){
	  if(netif_is_link_up(&gnetif)){
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
  }
   return -1;
}
/*****************************************************************************
  * @brief  receive datat from tcp server to tcp server
  * @param  
  * @retval receive data length 
******************************************************************************/
uint16_t tcpserver_receive_data_from_tcpclient(uint8_t *pdata)
{
  uint16_t len;
  struct netconn *conn = tcp_server_handle;
  if(conn!=NULL){
	  if(netif_is_link_up(&gnetif)){
       if( TcpServer_Rx_St.ETH_EN){
         len =  TcpServer_Rx_St.ETH_Size;
         memcpy(pdata , TcpServer_Rx_St.ETH_Buf ,len);
         memset(&TcpServer_Rx_St,0,sizeof(TcpServer_Rx_St));
         return len;
        }
	  }
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
