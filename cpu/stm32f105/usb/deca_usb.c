
#include "deca_usb.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "serial-line.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined(__ICCARM__)      /*!< IAR Compiler */
#pragma data_alignment=4
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB_OTG_dev __ALIGN_END;

CDC_IF_Prop_TypeDef VCP_fops =
{
  DW_VCP_Init,
  DW_VCP_DeInit,
  DW_VCP_Ctrl,
  DW_VCP_DataTx,
  DW_VCP_DataRx
};

/* These are external variables imported from CDC core to be used for IN
   transfer management. */
extern uint8_t APP_Rx_Buffer[];   /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */
extern uint32_t APP_Rx_ptr_out;
extern uint32_t APP_Rx_length;

uint16_t
DW_VCP_Init(void)
{
  return USBD_OK;
}
uint16_t
DW_VCP_DeInit(void)
{
  return USBD_OK;
}
/**
 * @brief  DW_VCP_Ctrl
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
uint16_t
DW_VCP_Ctrl(uint32_t Cmd, uint8_t *Buf, uint32_t Len)
{
  return USBD_OK;
}
/*example functions to interface to USB VCOM */
/**
 * @brief  DW_VCP_DataTx - send data over USB (all or nothing)
 * @param  Buf: Buffer of data to be sent
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
 */
uint16_t
DW_VCP_DataTx(uint8_t *Buf, uint32_t Len)
{
  int in, out;
  int buf_len;

  out = APP_Rx_ptr_out;
  in = APP_Rx_ptr_in;

  if (out == APP_RX_DATA_SIZE) {
    out = 0;
  }

  if(out > in) /* rollback */
  { 
    buf_len = APP_RX_DATA_SIZE - out + in;
  }
  else 
  {
    buf_len = in - out;
  }

  if((buf_len + Len) >= APP_RX_DATA_SIZE - CDC_DATA_IN_PACKET_SIZE) {
    return USBD_FAIL;
  }


  int l;
  /* Get the data to be sent */
  for(l = 0; l < Len; l++) {
    APP_Rx_Buffer[in++] = Buf[l];
    /* Increment the in pointer */
    if(in >= APP_RX_DATA_SIZE) {
      in = 0;
    }
  }

  APP_Rx_ptr_in = in;

  return USBD_OK;
}

#if 0
// old function
uint16_t
DW_VCP_DataTx(uint8_t *Buf, uint32_t Len)
{
  int i = 0;

  if((APP_Rx_length + Len) >= APP_RX_DATA_SIZE) {
    return USBD_FAIL;
  }

  i = APP_Rx_ptr_in;

  int l;
  /* Get the data to be sent */
  for(l = 0; l < Len; l++) {
    APP_Rx_Buffer[i++] = Buf[l];
    /* Increment the in pointer */
    if(i >= APP_RX_DATA_SIZE) {
      i = 0;
    }
  }

  APP_Rx_ptr_in = i;

  return USBD_OK;
}
#endif


/**
 * @brief  DW_VCP_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         until exiting this function. If you exit this function before transfer
 *         is complete on CDC interface (i.e. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
 */
uint16_t
DW_VCP_DataRx(uint8_t *Buf, uint32_t Len)
{
  uint32_t i;

  for(i=0; i<Len; i++) {
    serial_line_input_byte(Buf[i]);
  }
  return USBD_OK;
}

int
usb_init(void)
{
  /* enable/initialise the USB functionality */
  USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);

  return 0;
}
