#ifndef _IRDA_H_
#define _IRDA_H_

#define IRDA_DEBUG

#ifdef IRDA_DEBUG
#define IRDA_PRT      os_printf
#define IRDA_WARN     warning_prf
#define IRDA_FATAL    fatal_prf
#else
#define IRDA_PRT      null_prf
#define IRDA_WARN     null_prf
#define IRDA_FATAL    null_prf
#endif

#if (CONFIG_SOC_BK7271)
#define IRDA_BASE                     (0x00802760)
#else
#define IRDA_BASE                     (0x00802400)
#endif

#define IRDA_CTRL                     (IRDA_BASE + 0 * 4)
#define IRDA_NEC_EN                   (0x01UL << 0)
#define IRDA_POLARITY                 (0x01UL << 1)
#define UART_RX_ENABLE                (0x01UL << 2)
#define UART_LEN_POSI                 (3)
#define UART_LEN_MASK                 (0x03)
#define UART_PAR_EN                   (0x01UL << 5)
#define UART_PAR_MODE                 (0x01UL << 6)
#define UART_STOP_LEN                 (0x01UL << 7)
#define CLK_DIVID_POSI                (8)
#define CLK_DIVID_MASK                (0xFFFF)

#define IRDA_INT_MASK                 (IRDA_BASE + 1 * 4)
#define IRDA_END_INT_MASK             (0x01UL << 0)
#define IRDA_RIGHT_INT_MASK           (0x01UL << 1)
#define IRDA_REPEAT_INT_MASK          (0x01UL << 2)
#define FIFO_WREN_INT_MASK            (0x01UL << 3)
#define PARITY_ERR_INT_MASK           (0x01UL << 4)
#define STOP_ERR_INT_MASK             (0x01UL << 5)
#define INT_MASK_EN					(0x3f)

#define IRDA_INT                      (IRDA_BASE + 2 * 4)
#define IRDA_END_INT                  (0x01UL << 0)
#define IRDA_RIGHT_INT                (0x01UL << 1)
#define IRDA_REPEAT_INT               (0x01UL << 2)
#define FIFO_WREN_INT                 (0x01UL << 3)
#define PARITY_ERR_INT                (0x01UL << 4)
#define STOP_ERR_INT                  (0x01UL << 5)

#define RX_FIFO_DOUT                  (IRDA_BASE + 3 * 4)

#define IRDA_KEY_HTIMER_CHNAL 4 /*timer channle*/

#endif
