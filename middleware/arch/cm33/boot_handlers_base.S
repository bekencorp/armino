/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

//TODO: copy from bootrom

.section .vector_table
    .long    __MSPTop              /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */

 .text
 .global Reset_Handler
Reset_Handler:
	ldr r0, =entry_main
	bx  r0

