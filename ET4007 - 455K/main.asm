;===================================
; ET4007 program
;===================================
; wangyf
; 2015.5.13
; 手机遥控器
;===================================
include ET4007.inc
include main.inc

org	00h
START:
	NOP
	GOTO	MAIN
org	06h
	GOTO 	Interrupt_06h
org 0ch
	GOTO	Interrupt_0ch
org	012h
	GOTO	Interrupt_EXT

include delay.asm
include	learn_rmt.asm
include rmtout_samsung.asm
include rmtout_version.asm

MAIN:
	NOP
	NOP
	NOP
;	MOVLW	01h
;	ADDWF	nWriteByteCount_l,f
;	MOVLW	00h
;	ADDWFC	nWriteByteCount_h,f	
;	GOTO     MAIN
	;BTFSC	STATUS,TO
	;GOTO	SUPER_LOOP
	MOVLW	06h
	MOVWF	WDTCTR
	CLRWDT
	CALL	RamsClearALL

SleepMode:
		CALL	NOP1000
		BCF     RMTCTR,7
		BCF     RMTCTR,6
		BCF     RMTCTR,RX_EN				
		BCF     RMTCTR,TX_EN			 ;RX_EN=0,TX_EN=0 禁止发射，禁止接收-->降低功耗
		BSF     RMTCTR,5
		BSF     RMTCTR,2
		BSF     RMTCTR,1

		BSF		I2CCON,2				;PT1.5 OD(开漏功能)打开

		BCF		INTE,TMBIE
		BCF		flag,repeat

		MOVLW	11111111b
		MOVWF	PT1SEL
		MOVLW	11011111b
		MOVWF	PT1MR
		MOVLW	11001111b
		MOVWF	PT1EN
		MOVLW	00000000b
		MOVWF	PT1PU
		MOVLW	11110111b
		MOVWF	PT1

		MOVLW	11111111b
		MOVWF	PT2SEL
		MOVLW	11111111b
		MOVWF	PT2MR
		MOVLW	11111111b
		MOVWF	PT2EN
		MOVLW	00000000b
		MOVWF	PT2PU
		MOVLW	11111110b
		MOVWF	PT2

		BSF		PWMCON,PWM_PORT_SEL_0

;-----------------------------------
; 变量初始化
;-----------------------------------


;-----------------------------------
		MOVLW	06h
		MOVWF	WDTCTR
		CLRWDT

		SETDP	00h
		MOVLW	BUFFER_ADDRESS
		MOVWF	FRS0

		_ET4007_NOT_BUSY_

		SLEEP
		NOP
		NOP

IIC:
		_ET4007_BUSY_
		
		BTFSC	PT1,SDA
		GOTO	ERROR
		
		CLRF	buffer
		CLRF	nWriteBitCount
		CLRF	nWriteByteCount
		CLRF	INTF
		BSF		INTE,GIE
		BTFSS	PT1,SCK
		GOTO	ERROR
		CLRWDT
		BTFSC	PT1,SCK				;下降沿
		GOTO	$-1
;-----------------------------------
; 开始接收地址字节
;-----------------------------------
ADDRESS_SCK_RISING_EDGE_LOOP:
		CLRWDT
		BTFSS	PT1,SCK				;上升沿
		GOTO	$-1
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		BTFSS	PT1,SDA
		GOTO	ADDRESS_READ_SDA_0
ADDRESS_READ_SDA_1:
		BSF		STATUS,C
		GOTO	ADDRESS_WIRTE_TO_BUFFER
ADDRESS_READ_SDA_0:
		BCF		STATUS,C
ADDRESS_WIRTE_TO_BUFFER:
		RLF		buffer,F
		CLRWDT
		BTFSC	PT1,SCK				;下降沿
		GOTO	$-1
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		INCF	nWriteBitCount,F
		BTFSS	nWriteBitCount,3	;判断是否到了8次
		GOTO	ADDRESS_SCK_RISING_EDGE_LOOP
;;;如果接收到的地址值不是035h，则继续休眠，等待START信号
		MOVFW	buffer
		SUBLW	035h
		BTFSS	STATUS,Z
		GOTO	SleepMode
		CLRF	buffer
		CLRF	nWriteBitCount
		_SET_SDA_OUTPUT_0_
		_SET_SDA_OUTPUT_
		CLRWDT
		BTFSS	PT1,SCK				;上升沿
		GOTO	$-1
		CLRWDT
		BTFSC	PT1,SCK				;下降沿
		GOTO	$-1
		_SET_SDA_INPUT_
;-----------------------------------
; 开始接收Control字节
;-----------------------------------
CONTROL_SCK_RISING_EDGE_LOOP:
		CLRWDT
		BTFSS	PT1,SCK				;上升沿
		GOTO	$-1
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		BTFSS	PT1,SDA
		GOTO	CONTROL_READ_SDA_0
CONTROL_READ_SDA_1:
		BSF		STATUS,C
		GOTO	CONTROL_WIRTE_TO_BUFFER
CONTROL_READ_SDA_0:
		BCF		STATUS,C
CONTROL_WIRTE_TO_BUFFER:
		RLF		buffer,F
		CLRWDT
		BTFSC	PT1,SCK				;下降沿
		GOTO	$-1
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		INCF	nWriteBitCount,F
		BTFSS	nWriteBitCount,3	;判断是否到了8次
		GOTO	CONTROL_SCK_RISING_EDGE_LOOP
		MOVFW	buffer
		ANDLW	0f0h
		SUBLW	050h
		BTFSS	STATUS,Z
		GOTO	ERROR			;如果接收到的control值的高4位不是0x5，则跳转到ERROR，等待START信号(连ACK也不发了)
		CLRF	nWriteBitCount
		_SET_SDA_OUTPUT_0_
		_SET_SDA_OUTPUT_
		CLRWDT
		BTFSS	PT1,SCK				;上升沿
		GOTO	$-1
		CLRWDT
		BTFSC	PT1,SCK				;下降沿
		GOTO	$-1
		_SET_SDA_INPUT_
		MOVFW	buffer
		CLRF	buffer
		ANDLW	0fh
		ADDPCW
		GOTO	ERROR
		GOTO	ERROR
		GOTO	ERROR
		GOTO	ERROR
		GOTO	SEND_KITKAT_IR	;0x54
		GOTO	ERROR
		GOTO	ERROR
		GOTO	START_LEARN		;0x57
		GOTO	ERROR
		GOTO	ERROR
		GOTO	ERROR
		GOTO	ERROR
		GOTO	READ_VERSION	;0x5c
		GOTO	READ_CODE		;0x5d
		GOTO	STOP_LEARN		;0x5e
		GOTO	SEND_REPEAT		;0x5f



START_LEARN:
		_DIS_InterruptEXT_
		CALL	STOP_WAIT_FOR_STOP
		SETDP	00h
		MOVLW	20h
		CALL	ANY_RAM_CLEAR
		GOTO	Learn_rmt

STOP_LEARN:
		_DIS_InterruptEXT_
		BCF		INTE,GIE
		CALL	STOP_WAIT_FOR_STOP
		GOTO	SleepMode

SEND_REPEAT:
		_DIS_InterruptEXT_
		CALL	STOP_WAIT_FOR_STOP
		BSF		flag,repeat
		BCF		INTE,GIE
		NOP
		;CALL	RMTOUT_COMMON
		GOTO	SleepMode



;=================================================================		
;寄存器清零
;======================================================================
RamsClearALL:
		MOVLW		20h
		CALL		ClearRams
		RETURN
ClearRams:
		MOVWF		FRS0
ClearRamLoop_l:
		CLRF		IND0
		INCFSZ		FRS0,f
		GOTO		ClearRamLoop_l

		MOVLW		00h
		MOVWF		FRS0
		SETDP       01h
ClearRamLoop_h:
		CLRF		IND0
        INCFSZ		FRS0,f
        GOTO		ClearRamLoop_h

        SETDP       00h
		RETURN




Interrupt_EXT:
		_DIS_InterruptEXT_
		GOTO	IIC




ERROR:
		_DIS_TIMERA_
		_DIS_InterruptTIMERA_
		_DIS_InterruptEXT_
		BCF		INTE,GIE
		GOTO	SleepMode
	


STOP_WAIT_FOR_STOP:
		BCF		INTE,GIE
		_DIS_InterruptEXT_
		_SET_SDA_INPUT_
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		BTFSS	PT1,SCK
		GOTO	STOP_WAIT_FOR_STOP
	STOP_WAIT_FOR_STOP_SDA:
		BTFSS	PT1,SDA
		GOTO	STOP_WAIT_FOR_STOP_SDA
		RETURN


		
	
		END                