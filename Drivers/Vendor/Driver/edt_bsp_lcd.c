/**
  ******************************************************************************  
  * File Name          : edt_bsp_lcd.c
  * @author            : EDT Embedded Application Team
  * Description        : This file includes the driver for LCD/TFT module on
  *                      smart embedded display board.
  * @brief             : EDT <https://smartembeddeddisplay.com/>
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT(c) 2020 Emerging Display Technologies Corp.
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
#include "edt_bsp_lcd.h"

static void EDT_LL_FillBuffer(LTDC_HandleTypeDef *hBootLtdcHandler , \
        uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, \
          uint32_t OffLine, uint32_t ColorIndex) ;

/*******LCD Sleep function*****/
LCD_StructDef LCD = {.width = TFT_WIDTH, .height = TFT_HEIGHT, .lcdstate = false};
uint16_t Sleep_cunter=0;
uint16_t Sleep_Time=180; 
static bool SleepDetected;
//extern osThreadId LCDSleepTaskHandle;

static void EDT_LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params);
/******************************************************************************
  * @brief  EDT_LCD_ClockConfig
  * @param  hltdc  : LTDC_HandleTypeDef  hltdc
  * @param  Params : NULL
  * @retval NONE
  * @note   This API is called by EDT_LCD_Init()
  *         Being __weak it can be overwritten by the application
*******************************************************************************/
void EDT_LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params)
{
  static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;
  /* TFT LCD clock configuration */
  periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  periph_clk_init_struct.PLL3.PLL3M = TFT_PLL3M;
  periph_clk_init_struct.PLL3.PLL3N = TFT_PLL3N;
  periph_clk_init_struct.PLL3.PLL3P = TFT_PLL3P;
  periph_clk_init_struct.PLL3.PLL3Q = TFT_PLL3Q;
  periph_clk_init_struct.PLL3.PLL3R = TFT_PLL3R;

  HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
}
/******************************************************************************
  * @brief  EDT_LCD_Init
  * @param  NONE
  * @retval NONE
  * @note   EDT DISPLAY Initial
*******************************************************************************/
uint8_t EDT_LCD_Init(void)
{  
  /* De-Initialize LTDC */
  HAL_LTDC_DeInit(&hltdc);
 
  /* Select the used LCD */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity         = HS_POLARITY;
  hltdc.Init.VSPolarity         = VS_POLARITY;
  hltdc.Init.DEPolarity         = DE_POLARITY;
  hltdc.Init.PCPolarity         = PC_POLARITY;
  /* Timing Configuration */  
  hltdc.Init.HorizontalSync     = (TFT_HSYNC - 1);
  hltdc.Init.VerticalSync       = (TFT_VSYNC - 1);
  hltdc.Init.AccumulatedHBP     = (TFT_HSYNC + TFT_HBP - 1);
  hltdc.Init.AccumulatedVBP     = (TFT_VSYNC + TFT_VBP - 1);
  hltdc.Init.AccumulatedActiveH = (TFT_HEIGHT + TFT_VSYNC + TFT_VBP - 1);
  hltdc.Init.AccumulatedActiveW = (TFT_WIDTH + TFT_HSYNC + TFT_HBP - 1);
  hltdc.Init.TotalHeigh         = (TFT_HEIGHT + TFT_VSYNC + TFT_VBP + TFT_VFP - 1);
  hltdc.Init.TotalWidth         = (TFT_WIDTH + TFT_HSYNC + TFT_HBP + TFT_HFP - 1);
  EDT_LCD_ClockConfig(&hltdc,NULL);
//  LCD.width = TFT_WIDTH;
//  LCD.height = TFT_HEIGHT;
  EDT_LCD_SetSize(&LCD, TFT_WIDTH, TFT_HEIGHT);
  /* Background value */  
  hltdc.Init.Backcolor.Blue     = 0;
  hltdc.Init.Backcolor.Green    = 0;
  hltdc.Init.Backcolor.Red      = 0;
   
  LCD.lcdstate = false;

  if(HAL_LTDC_GetState(&hltdc) == HAL_LTDC_STATE_RESET)
  {   
  } 
  HAL_LTDC_Init(&hltdc);    

  HAL_LTDC_SetPitch(&hltdc, LCD.width, 0);
  EDT_LCD_Clear(&hltdc ,0 );   

  return LCD_OK;  
}
/******************************************************************************
  * @brief  EDT_LCD_DeInit
  * @param  NONE
  * @retval NONE
  * @note   EDT DISPLAY DeInitial
*******************************************************************************/
uint8_t EDT_LCD_DeInit(void)
{ 
  /* Initialize the hltdc Instance parameter */
  
 /* Disable LTDC block */
  __HAL_LTDC_DISABLE(&hltdc);

  /* DeInit the LTDC */
  HAL_LTDC_DeInit(&hltdc);
  
  return LCD_OK;
}
/*************************************************************
  * @brief  Initializes the LCD layer in RGB565 format (16 bits per pixel) or 
            RGB888(24 bits per pixel) depending on definitionof USE_BPP
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
***************************************************************/
void EDT_LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address)
{
    LCD_LayerCfgTypeDef  layer_cfg;
   
    /* Layer Init */
    layer_cfg.WindowX0 = 0;
    layer_cfg.WindowX1 = EDT_LCD_GetXSize();
    layer_cfg.WindowY0 = 0;
    layer_cfg.WindowY1 = EDT_LCD_GetYSize();

    layer_cfg.FBStartAdress = FB_Address;
    layer_cfg.Alpha = 255;
    layer_cfg.Alpha0 = 0;
    layer_cfg.Backcolor.Blue = 0x0;
    layer_cfg.Backcolor.Green = 0;
    layer_cfg.Backcolor.Red = 0;
    layer_cfg.ImageWidth  = EDT_LCD_GetXSize();
    layer_cfg.ImageHeight = EDT_LCD_GetYSize();
#if !defined(USE_BPP) || USE_BPP==16
    layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

#elif USE_BPP==24
    layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
#else
#error Unknown USE_BPP
#endif
    HAL_LTDC_ConfigLayer(&hltdc, &layer_cfg, LayerIndex);    
}
/******************************************************************************
  * @brief  EDT_LCD_GetXSize
  * @param  NONE
  * @retval DISPLAY Width uint32_t
  * @note   EDT DISPLAY Get Width Size
*******************************************************************************/
uint32_t EDT_LCD_GetXSize(void)
{
    return LCD.width;
}
/******************************************************************************
  * @brief  EDT_LCD_GetYSize
  * @param  NONE
  * @retval DISPLAY Height uint32_t
  * @note   EDT DISPLAY Get Height Size
*******************************************************************************/
uint32_t EDT_LCD_GetYSize(void)
{
    return LCD.height;
}
/******************************************************************************
  * @brief  EDT_LCD_SetSize
  * @param  NONE
  * @retval NONE
  * @note   EDT Set DISPLAY Size
*******************************************************************************/
void EDT_LCD_SetSize(LCD_StructDef * lcd, uint16_t width, uint16_t height)
{
  lcd->width = width;
  lcd->height = height;
}
/******************************************************************************
  * @brief  EDT_LCD_DisplayOn
  * @param  NONE
  * @retval NONE
  * @note   EDT DISPLAY ON Ctrl hltdc/ RESET PIN
*******************************************************************************/
void EDT_LCD_DisplayOn(void)
{
  HAL_GPIO_WritePin( LCD_STBYB_GPIO_Port, LCD_STBYB_Pin, GPIO_PIN_SET );
    HAL_Delay( 10 ); 
  HAL_GPIO_WritePin( LCD_POWER_RESET_GPIO_Port, LCD_POWER_RESET_Pin, GPIO_PIN_SET );
    HAL_Delay( 100 );  

  EDT_LCD_BL_ON();
  EDT_LCD_SetDisplayStatus( true );
}
/******************************************************************************
  * @brief  EDT_LCD_DisplayOff
  * @param  NONE
  * @retval NONE
  * @note   EDT DISPLAY ON Ctrl hltdc/RESET PIN
*******************************************************************************/
void EDT_LCD_DisplayOff(void)
{
  EDT_LCD_BL_OFF();

  HAL_GPIO_WritePin( LCD_POWER_RESET_GPIO_Port, LCD_POWER_RESET_Pin, GPIO_PIN_RESET );
  HAL_Delay( 10 );  
  HAL_GPIO_WritePin( LCD_STBYB_GPIO_Port, LCD_STBYB_Pin, GPIO_PIN_RESET );
  HAL_Delay( 10 );  
  HAL_GPIO_WritePin( LCD_POWER_RESET_GPIO_Port, LCD_POWER_RESET_Pin, GPIO_PIN_SET );
  HAL_Delay( 250 );  
  HAL_GPIO_WritePin( LCD_POWER_RESET_GPIO_Port, LCD_POWER_RESET_Pin, GPIO_PIN_RESET );

  EDT_LCD_SetDisplayStatus( false );
}
/******************************************************************************
  * @brief  EDT_LCD_SetDisplayStatus
  * @param  lcdstatus  : true :display enabled / false :display disable
  * @retval NONE
  * @note   EDT DISPLAY ON Ctrl hltdc/RESET PIN
*******************************************************************************/
void EDT_LCD_SetDisplayStatus(bool lcdstatus)
{
  LCD.lcdstate = lcdstatus;
}
/******************************************************************************
  * @brief  EDT_LCD_DisplayOff
  * @param  NONE
  * @retval lcdstatus  : true :display enabled / false :display disable
  * @note   EDT DISPLAY ON Ctrl hltdc/RESET PIN
*******************************************************************************/
bool EDT_LCD_GetDisplayStatus(void)
{
 return LCD.lcdstate ;
}

/******************************************************************************
  * @brief  EDT_LCD_Reset
  * @param  NONE
  * @retval NONE
  * @note   EDT DISPLAY RESET Control PIN
*******************************************************************************/
void EDT_LCD_Reset(void)
{ 

  EDT_LCD_BL_OFF();
  
  HAL_GPIO_WritePin( LCD_STBYB_GPIO_Port, LCD_STBYB_Pin, GPIO_PIN_RESET );
  HAL_GPIO_WritePin( LCD_POWER_RESET_GPIO_Port, LCD_POWER_RESET_Pin, GPIO_PIN_RESET );
  HAL_Delay( 100 ); 
  HAL_GPIO_WritePin( LCD_POWER_RESET_GPIO_Port, LCD_POWER_RESET_Pin, GPIO_PIN_SET );
//  HAL_Delay( 50 ); 
   
  EDT_LCD_DisplayOff();
//  EDT_LCD_DisplayOn();
//  EDT_LCD_SetDisplayStatus(true);
}
/*****************************************************************
  * @brief  Sleep Function
  * @param  
  * @param  
  * @retval None
  * @note   turn off  LCD display and backlight
*****************************************************************/
void StartLCDSleepTask(void * argument)
{
  while(1) {
    #if defined( USE_LCD_SleepFunction )  
      EDT_LCD_Sleep_Function();
    #endif
      vTaskDelay( TEMP_REFRESH_PERIOD );
    }   
}
/************************************************/
void EDT_LCD_Sleep_Time(uint16_t value)
{
  Sleep_Time = value;
}
/************************************************/
void EDT_LCD_Sleep_Function(void)
{  
  Sleep_cunter++; 

  if ( Sleep_cunter >= Sleep_Time ) {
    if ( LCD.lcdstate == true ) {
      Sleep_cunter = 0;
      EDT_LCD_DisplayOff();
    }
    SleepDetected = true; 
  }

  if ( EDT_TS_GetDetected() ) {
    Sleep_cunter = 0;
    EDT_TS_SetDetected( false );
    if ( LCD.lcdstate == false ) {
      EDT_LCD_DisplayOn();   
    }
  } 
}

bool EDT_Sleep_GetDetected(void) 
{
  return  SleepDetected;
}

void EDT_Sleep_SetDetected(bool bl) 
{
  SleepDetected = bl;
}
/******************************************************************************
  * @brief  SuspendLCDSleepTask
  * @param
  * @param
  * @note   Stop LCDSleep Task
  * @retval Create Task For LCDSleep
*******************************************************************************/
void EDT_Sleep_SuspendLCDSleepTask(void)
{
//    vTaskSuspend(LCDSleepTaskHandle);
    vTaskSuspend(osThreadGetId());
    Sleep_cunter=0;
}
/******************************************************************************
  * @brief  ResumeLCDSleepTask
  * @param
  * @param
  * @note   Start LCDSleep Task
  * @retval Create Task For LCDSleep
*******************************************************************************/
void EDT_Sleep_ResumeLCDSleepTask(void)
{
//    vTaskResume(LCDSleepTaskHandle);
    vTaskResume(osThreadGetId());
    Sleep_cunter=0;
}
/******************************************************************
  * @brief  Fills a buffer.
  * @param  LayerIndex: Layer index
  * @param  pDst: Pointer to destination buffer
  * @param  xSize: Buffer width
  * @param  ySize: Buffer height
  * @param  OffLine: Offset
  * @param  ColorIndex: Color index
  * @retval None
  ********************************************************************/
static void EDT_LL_FillBuffer(LTDC_HandleTypeDef *hBootLtdcHandler ,uint32_t LayerIndex, \
  void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex) 
{
  /* Register to memory mode with ARGB8888 as color Mode */ 
  hdma2d.Init.Mode         = DMA2D_R2M;
  if(hBootLtdcHandler->LayerCfg[0].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
  { /* RGB565 format */ 
    hdma2d.Init.ColorMode    = DMA2D_RGB565;
  }
  else
  { /* ARGB8888 format */
    hdma2d.Init.ColorMode    = DMA2D_ARGB8888;
  }
  hdma2d.Init.OutputOffset = OffLine;      
  
  hdma2d.Instance = DMA2D;
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, LayerIndex) == HAL_OK) 
    {
       SCB_CleanInvalidateDCache();
      if (HAL_DMA2D_Start(&hdma2d, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d, 10);
      }
    }
  } 
}
/****************************************************************************************
  * @brief  EDT_LCD_Clear
  * @param  hltdc  :LTDC_HandleTypeDef hltdc
  * @param  Color  :uint32_t  0xFFFFFFFF
  * @retval NONE
  * @note   
****************************************************************************************/
void EDT_LCD_Clear(LTDC_HandleTypeDef *hltdc , uint32_t Color)
{ 
  EDT_LL_FillBuffer(hltdc,0, (uint32_t *)(hltdc->LayerCfg[0].FBStartAdress), EDT_LCD_GetXSize(), EDT_LCD_GetYSize(), 0, Color);
}

 /*******(C) COPYRIGHT Emerging Display Technologies Corp. **** END OF FILE ***/
