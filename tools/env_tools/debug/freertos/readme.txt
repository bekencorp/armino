; --------------------------------------------------------------------------------
; @Title: Readme for FreeRTOS on ARM
; @Description: 
;   This file includes necessary patches for FreeRTOS to support Task aware
;   tracing.
;   
; @Keywords: awareness, RTOS, FreeRTOS
; @Author: AME
; @Copyright: (C) 1989-2019 Lauterbach GmbH, licensed for use with TRACE32(R) only
; --------------------------------------------------------------------------------
; $Id: readme.txt 14136 2019-03-15 14:47:46Z amerkle $

= Task aware Trace =
== CortexA/R without DataTrace ==
Applicable for Cortex-A5/A8/A9/A15/A17/A32.
Applicable for Cortex-A7/R4/R5/R7/R8/R52 (without DataTrace)

Description: A function T32_Magic_Update is used to write the pxCurrectTCB value to the CONTEXTIDR (C15:0x10d).

<portasm.s>
+	.type T32_Magic_Update, %function
+	T32_Magic_Update:
+		/* Write the Magic to the ContextID Register */
+		LDR	R0, pxCurrentTCBConst
+		LDR	R0, [R0]
+		MCR	P15, 0x0,R0,C13,C0,0x1
+		BX	R14
	...
	.type FreeRTOS_SWI_Handler, %function
	FreeRTOS_SWI_Handler:
		/* Save the context of the current task and select a new task to run. */
		portSAVE_CONTEXT
		LDR R0, vTaskSwitchContextConst
		BLX	R0
+		BL	T32_Magic_Update
		portRESTORE_CONTEXT
	...
	switch_before_exit:
		...
		/* Call the function that selects the new task to execute.
		vTaskSwitchContext() if vTaskSwitchContext() uses LDRD or STRD
		instructions, or 8 byte aligned stack allocated data.  LR does not need
		saving as a new LR will be loaded by portRESTORE_CONTEXT anyway. */
		LDR	R0, vTaskSwitchContextConst
		BLX	R0
+		BL	T32_Magic_Update
		/* Restore the context of, and branch to, the task selected to execute
		next. */
		portRESTORE_CONTEXT
	...
</portasm.s>
Setting within TRACE32:
	ETM.ContextID 32
	ETM.TraceON
	ETM.ON
