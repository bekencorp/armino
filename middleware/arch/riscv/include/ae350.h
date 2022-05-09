/*
 * Copyright (c) 2012-2017 Andes Technology Corporation
 * All rights reserved.
 *
 */

#ifndef __AE350_H__
#define __AE350_H__

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * System clock
 ****************************************************************************/
#define KHz                     1000
#define MHz                     1000000

#define OSCFREQ                 (20 * MHz)
#define CPUFREQ                 (60 * MHz)
#define RTCFREQ                 (32768)
#define HCLKFREQ                (CPUFREQ)
#define PCLKFREQ                (CPUFREQ)
#define UCLKFREQ                (OSCFREQ)

/*****************************************************************************
 * PLIC Interrupt source definition
 ****************************************************************************/
#define IRQ_RTCPERIOD_SOURCE    1
#define IRQ_RTCALARM_SOURCE     2
#define IRQ_PIT_SOURCE          3
#define IRQ_SPI1_SOURCE         4
#define IRQ_SPI2_SOURCE         5
#define IRQ_I2C_SOURCE          6
#define IRQ_GPIO_SOURCE         7
#define IRQ_UART1_SOURCE        8
#define IRQ_UART2_SOURCE        9
#define IRQ_DMA_SOURCE          10
#define IRQ_SWINT_SOURCE        12
#define IRQ_AC97_SOURCE         17
#define IRQ_SDC_SOURCE          18
#define IRQ_MAC_SOURCE          19
#define IRQ_LCDC_SOURCE         20
#define IRQ_TOUCH_SOURCE        25
#define IRQ_STANDBY_SOURCE      26
#define IRQ_WAKEUP_SOURCE       27

#ifndef __ASSEMBLER__

/*****************************************************************************
 * Device Specific Peripheral Registers structures
 ****************************************************************************/

#define __I                     volatile const	/* 'read only' permissions      */
#define __O                     volatile        /* 'write only' permissions     */
#define __IO                    volatile        /* 'read / write' permissions   */

/*****************************************************************************
 * PLMT - AE350
 ****************************************************************************/
typedef struct {
	__IO unsigned long long MTIME;          /* 0x00 Machine Time */
	__IO unsigned long long MTIMECMP;       /* 0x08 Machine Time Compare */
} PLMT_RegDef;

/*****************************************************************************
 * DMAC - AE350
 ****************************************************************************/
typedef struct {
	__IO unsigned int CTRL;                 /* DMA Channel Control Register */
	__IO unsigned int TRANSIZE;             /* DMA Channel Transfer Size Register */
	__IO unsigned int SRCADDRL;             /* DMA Channel Source Address(low part) Register */
	__IO unsigned int SRCADDRH;             /* DMA Channel Source Address(high part) Register */
	__IO unsigned int DSTADDRL;             /* DMA Channel Destination Address Register(low part) */
	__IO unsigned int DSTADDRH;             /* DMA Channel Destination Address Register(high part) */
	__IO unsigned int LLPL;                 /* DMA Channel Linked List Pointer Register(low part) */
	__IO unsigned int LLPH;                 /* DMA Channel Linked List Pointer Register(high part) */
} DMA_CHANNEL_REG;

typedef struct {
	__I  unsigned int IDREV;                /* 0x00 ID and Revision Register */
	     unsigned int RESERVED0[3];         /* 0x04 ~ 0x0C Reserved */
	__I  unsigned int DMACFG;               /* 0x10 DMA Configure Register */
	     unsigned int RESERVED1[3];         /* 0x14 ~ 0x1C Reserved */
	__IO unsigned int DMACTRL;              /* 0x20 DMA Control Register */
	__O  unsigned int CHABORT;              /* 0x24 DMA Channel Abort Register */
	     unsigned int RESERVED2[2];         /* 0x28 ~ 0x2C Reserved */
	__IO unsigned int INTSTATUS;            /* 0x30 Interrupt Status Register */
	__I  unsigned int CHEN;			/* 0x34 Channel Enable Register*/
	     unsigned int RESERVED3[2];         /* 0x38 ~ 0x3C Reserved */
	DMA_CHANNEL_REG   CHANNEL[9];           /* 0x40 ~ 0x54 Channel #n Registers */
} DMA_RegDef;

/*****************************************************************************
 * SMU - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned int SYSTEMVER;            /* 0x00 SYSTEM ID and Revision Register */
	     unsigned int RESERVED0[3];         /* 0x04 ~ 0x0C Reserved */
	__IO unsigned int WRSR;                 /* 0x10 Wakeup and Reset Status Register */
	__IO unsigned int SMUCR;                /* 0x14 SMU Command Register */
	     unsigned int RESERVED1[1];         /* 0x18 Reserved */
	__IO unsigned int WRMASK;               /* 0x1C Wake up Mask Register */
	__IO unsigned int CER;                  /* 0x20 Clock Enable Register */
	__IO unsigned int CRR;                  /* 0x24 Clock Ratio Register */
	     unsigned int RESERVED2[6];         /* 0x28 ~ 0x3C Reserved Register */
	__IO unsigned int SCRATCH;              /* 0x40 Scratch Register */
	     unsigned int RESERVED3[3];         /* 0x44 ~ 0x4C Reserved */
	__IO unsigned int RESET_VECTOR;         /* 0x50 CPU Reset Vector Register */
} SMU_RegDef;

/*****************************************************************************
 * UARTx - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned int IDREV;                /* 0x00 ID and Revision Register */
	     unsigned int RESERVED0[3];         /* 0x04 ~ 0x0C Reserved */
	__I  unsigned int CFG;                  /* 0x10 Hardware Configure Register */
	__IO unsigned int OSCR;                 /* 0x14 Over Sample Control Register */
	     unsigned int RESERVED1[2];         /* 0x18 ~ 0x1C Reserved */
	union {
		__IO unsigned int RBR;          /* 0x20 Receiver Buffer Register */
		__O  unsigned int THR;          /* 0x20 Transmitter Holding Register */
		__IO unsigned int DLL;          /* 0x20 Divisor Latch LSB */
	};
	union {
		__IO unsigned int IER;          /* 0x24 Interrupt Enable Register */
		__IO unsigned int DLM;          /* 0x24 Divisor Latch MSB */
	};
	union {
		__IO unsigned int IIR;          /* 0x28 Interrupt Identification Register */
		__O  unsigned int FCR;          /* 0x28 FIFO Control Register */
	};
	__IO unsigned int LCR;                  /* 0x2C Line Control Register */
	__IO unsigned int MCR;                  /* 0x30 Modem Control Register */
	__IO unsigned int LSR;                  /* 0x34 Line Status Register */
	__IO unsigned int MSR;                  /* 0x38 Modem Status Register */
	__IO unsigned int SCR;                  /* 0x3C Scratch Register */
} UART_RegDef;

/*****************************************************************************
 * PIT - AE350
 ****************************************************************************/
typedef struct {
	__IO unsigned int CTRL;                 /* PIT Channel Control Register */
	__IO unsigned int RELOAD;               /* PIT Channel Reload Register */
	__IO unsigned int COUNTER;              /* PIT Channel Counter Register */
	__IO unsigned int RESERVED[1];
} PIT_CHANNEL_REG;

typedef struct {
	__I  unsigned int IDREV;                /* 0x00 ID and Revision Register */
	     unsigned int RESERVED[3];          /* 0x04 ~ 0x0C Reserved */
	__I  unsigned int CFG;                  /* 0x10 Configuration Register */
	__IO unsigned int INTEN;                /* 0x14 Interrupt Enable Register */
	__IO unsigned int INTST;                /* 0x18 Interrupt Status Register */
	__IO unsigned int CHNEN;                /* 0x1C Channel Enable Register */
	PIT_CHANNEL_REG   CHANNEL[4];           /* 0x20 ~ 0x50 Channel #n Registers */
} PIT_RegDef;

/*****************************************************************************
 * RTC - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned int IDREV;                /* 0x00 ID and Revision Register */
	     unsigned int RESERVED[3];          /* 0x04 ~ 0x0C Reserved */
	__IO unsigned int CNTR;                 /* 0x10 Counter Register */
	__IO unsigned int ALARM;                /* 0x14 Alarm Register */
	__IO unsigned int CTRL;                 /* 0x18 Control Register */
	__IO unsigned int STATUS;               /* 0x1C Status Register */
} RTC_RegDef;

/*****************************************************************************
 * GPIO - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned int IDREV;                /* 0x00 ID and revision register */
	     unsigned int RESERVED0[3];         /* 0x04 ~ 0x0c Reserved */
	__I  unsigned int CFG;                  /* 0x10 Configuration register */
	     unsigned int RESERVED1[3];         /* 0x14 ~ 0x1c Reserved */
	__I  unsigned int DATAIN;               /* 0x20 Channel data-in register */
	__IO unsigned int DATAOUT;              /* 0x24 Channel data-out register */
	__IO unsigned int CHANNELDIR;           /* 0x28 Channel direction register */
	__O  unsigned int DOUTCLEAR;            /* 0x2c Channel data-out clear register */
	__O  unsigned int DOUTSET;              /* 0x30 Channel data-out set register */
	     unsigned int RESERVED2[3];         /* 0x34 ~ 0x3c Reserved */
	__IO unsigned int PULLEN;               /* 0x40 Pull enable register */
	__IO unsigned int PULLTYPE;             /* 0x44 Pull type register */
	     unsigned int RESERVED3[2];         /* 0x48 ~ 0x4c Reserved */
	__IO unsigned int INTREN;               /* 0x50 Interrupt enable register */
	__IO unsigned int INTRMODE0;            /* 0x54 Interrupt mode register (0~7) */
	__IO unsigned int INTRMODE1;            /* 0x58 Interrupt mode register (8~15) */
	__IO unsigned int INTRMODE2;            /* 0x5c Interrupt mode register (16~23) */
	__IO unsigned int INTRMODE3;            /* 0x60 Interrupt mode register (24~31) */
	__IO unsigned int INTRSTATUS;           /* 0x64 Interrupt status register */
             unsigned int RESERVED4[2];         /* 0x68 ~ 0x6c Reserved */
	__IO unsigned int DEBOUNCEEN;           /* 0x70 De-bounce enable register */
	__IO unsigned int DEBOUNCECTRL;         /* 0x74 De-bounce control register */
} GPIO_RegDef;

/*****************************************************************************
 * I2C - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned int IDREV;                /* 0x00 ID and Revision Register */
	     unsigned int RESERVED[3];          /* 0x04 ~ 0x0C Reserved */
	__I  unsigned int CFG;                  /* 0x10 Configuration Register */
	__IO unsigned int INTEN;                /* 0x14 Interrupt Enable Register */
	__IO unsigned int STATUS;               /* 0x18 Status Register */
	__IO unsigned int ADDR;                 /* 0x1C Address Register */
	__IO unsigned int DATA;                 /* 0x20 Data Register */
	__IO unsigned int CTRL;                 /* 0x24 Control Register */
	__IO unsigned int CMD;                  /* 0x28 Command Register */
	__IO unsigned int SETUP;                /* 0x2C Setup Register */
} I2C_RegDef;

/*****************************************************************************
 * SPI - AE350
 ****************************************************************************/
typedef struct {
	__I  unsigned int IDREV;                /* 0x00 ID and revision register */
	     unsigned int RESERVED0[3];         /* 0x04 ~ 0x0c Reserved */
	__IO unsigned int TRANSFMT;             /* 0x10 SPI transfer format register */
	__IO unsigned int DIRECTIO;             /* 0x14 SPI direct IO control register */
	     unsigned int RESERVED1[2];         /* 0x18 ~ 0x1c Reserved */
	__IO unsigned int TRANSCTRL;            /* 0x20 SPI transfer control register */
	__IO unsigned int CMD;                  /* 0x24 SPI command register */
	__IO unsigned int ADDR;                 /* 0x28 SPI address register */
	__IO unsigned int DATA;                 /* 0x2c SPI data register */
	__IO unsigned int CTRL;                 /* 0x30 SPI conrtol register */
	__I  unsigned int STATUS;               /* 0x34 SPI status register */
	__IO unsigned int INTREN;               /* 0x38 SPI interrupt enable register */
	__O  unsigned int INTRST;               /* 0x3c SPI interrupt status register */
	__IO unsigned int TIMING;               /* 0x40 SPI interface timing register */
	     unsigned int RESERVED2[3];         /* 0x44 ~ 0x4c Reserved */
	__IO unsigned int MEMCTRL;              /* 0x50 SPI memory access control register */
	     unsigned int RESERVED3[3];         /* 0x54 ~ 0x5c Reserved */
	__IO unsigned int SLVST;                /* 0x60 SPI slave status register */
	__I  unsigned int SLVDATACNT;           /* 0x64 SPI slave data count register */
	     unsigned int RESERVED4[5];         /* 0x68 ~ 0x78 Reserved */
	__I  unsigned int CONFIG;               /* 0x7c Configuration register */
} SPI_RegDef;

/*****************************************************************************
 * Memory Map
 ****************************************************************************/

#define _IO_(addr)              (addr)

#define EILM_BASE               0x00000000
#define EDLM_BASE               0x00200000
#define SPIMEM_BASE             0x80000000

#define BMC_BASE                _IO_(0xC0000000)
#define AHBDEC_BASE             _IO_(0xE0000000)
#define MAC_BASE                _IO_(0xE0100000)
#define LCDC_BASE               _IO_(0xE0200000)
#define SMC_BASE                _IO_(0xE0400000)
#define PLIC_BASE               _IO_(0xE4000000)
#define PLMT_BASE               _IO_(0xE6000000)
#define PLIC_SW_BASE            _IO_(0xE6400000)
#define PLDM_BASE               _IO_(0xE6800000)
#define APBBRG_BASE             _IO_(0xF0000000)
#define SMU_BASE                _IO_(0xF0100000)
#define UART1_BASE              _IO_(0xF0200000)
#define UART2_BASE              _IO_(0xF0300000)
#define PIT_BASE                _IO_(0xF0400000)
#define WDT_BASE                _IO_(0xF0500000)
#define RTC_BASE                _IO_(0xF0600000)
#define GPIO_BASE               _IO_(0xF0700000)
#define I2C_BASE                _IO_(0xF0A00000)
#define SPI1_BASE               _IO_(0xF0B00000)
#define DMAC_BASE               _IO_(0xF0C00000)
#define AC97_BASE               _IO_(0xF0D00000)
#define SDC_BASE                _IO_(0xF0E00000)
#define SPI2_BASE               _IO_(0xF0F00000)

/*****************************************************************************
 * Peripheral device declaration
 ****************************************************************************/
#define AE350_PLMT              ((PLMT_RegDef *) PLMT_BASE)
#define AE350_DMA               ((DMA_RegDef *)  DMAC_BASE)
#define AE350_SMU               ((SMU_RegDef *)  SMU_BASE)
#define AE350_UART1             ((UART_RegDef *) UART1_BASE)
#define AE350_UART2             ((UART_RegDef *) UART2_BASE)
#define AE350_PIT               ((PIT_RegDef *)  PIT_BASE)
#define AE350_RTC               ((RTC_RegDef *)  RTC_BASE)
#define AE350_GPIO              ((GPIO_RegDef *) GPIO_BASE)
#define AE350_I2C               ((I2C_RegDef *)  I2C_BASE)
#define AE350_SPI1              ((SPI_RegDef *)  SPI1_BASE)
#define AE350_SPI2              ((SPI_RegDef *)  SPI2_BASE)

#endif	/* __ASSEMBLER__ */

#ifdef __cplusplus
}
#endif

#endif	/* __AE350_H__ */
