/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 *
 * Stupid bit of code that does the bare minimum to make os_printf work.
 *
 * The source was modified to use esp_gpio library.
 */

#include <esp_sdo.h>
#include <user_interface.h>
#include <osapi.h>


static void ICACHE_FLASH_ATTR
stdout_uart_txd(char c)
{
  // Wait until there is room in the FIFO
  while (((READ_PERI_REG(UART_STATUS(0)) >> UART_TXFIFO_CNT_S) & UART_TXFIFO_CNT) >= 126);

  // Send the character.
  WRITE_PERI_REG(UART_FIFO(0), c);
}

static void ICACHE_FLASH_ATTR
stdout_put_char(char c)
{
  // Convert \n -> \r\n
  if (c == '\n') stdout_uart_txd('\r');
  stdout_uart_txd(c);
}

void ICACHE_FLASH_ATTR
stdout_init(uart_baud_rate br)
{
  // Enable TxD pin.
  PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);

  // Set baud rate and other serial parameters to 115200,n,8,1.
  uart_div_modify(UART0, (uint32) (UART_CLK_FREQ / br));

  WRITE_PERI_REG(UART_CONF0(UART0),
                 (STICK_PARITY_DIS) | (ONE_STOP_BIT << UART_STOP_BIT_NUM_S) | (EIGHT_BITS << UART_BIT_NUM_S));

  // Reset TX & RX fifo.
  SET_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST);
  CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST);

  // Clear pending interrupts.
  WRITE_PERI_REG(UART_INT_CLR(UART0), 0xffff);

  // Install our own put char handler.
  os_install_putc1((void *) stdout_put_char);
}