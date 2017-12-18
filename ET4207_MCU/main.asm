;===================================
; ET4207 program
;===================================
; jiangs
; 2015.5.13
; 手机遥控器
;===================================
include ET4207.inc
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
include consumerir.asm
include i2c_cmd_protocol.asm
include peripheral.asm

MAIN:
		NOP
		NOP
		NOP
		BTFSC	STATUS,TO
		GOTO	WDTC_STATUS
		MOVLW	0eh
		MOVWF	WDTCTR
		CLRWDT
MAIN_START:
		CAll	SysIni
		CALL	RamsClearALL
		CALL	IniVersion
	



SleepMode:
		CALL	NOP1000
		BCF     RMTCTR,SENS_CURT1
		BCF     RMTCTR,SENS_CURT0
		BCF     RMTCTR,RX_EN				
		BCF     RMTCTR,TX_EN			 ;RX_EN=0,TX_EN=0 禁止发射，禁止接收-->降低功耗
		BSF     RMTCTR,PWM_CURT
		BSF     RMTCTR,PWM_CURT1		;电流设置1	
		BSF     RMTCTR,PWM_CURT0		;电流设置0

		BSF		I2CCON,I2C_PT15				;PT1.5 OD(开漏功能)打开
		BSF		I2CCON,I2C_EN				;enable I2C port(PT1_4(SCL), PT1_3(SDA))
		BCF		INTE,TMBIE
SleepMode_1:		
		;MOVLW	06h
		;MOVWF	WDTCTR
		;CLRWDT
		CLRF	flag1
		MOVLW	04h
		MOVWF	WDTCTR
		CLRWDT


		_ET4207_NOT_BUSY_

		BCF		INTE,I2CIE ;disable I2C interrput
		BCF		INTE,GIE
		CLRF	INTF
		BCF		flag1,isCmdByte
		SLEEP
		NOP
		NOP
		MOVLW	04h
		MOVWF	WDTCTR
		_ET4207_BUSY_
	


I2C:		
			;MOVFW	pPackPointer
			;MOVWF	FRS0
		BSF		INTE, I2CIE ;enable I2C interrput
		BSF		INTE, GIE ;enable globle interrupt 
I2cWatingForStop:
	;	BTFSC   FLAG, isError  ;if cmd is error goto sleep mode
;		GOTO	SleepMode_1
		BTFSS   I2CCON, I2C_STO  ;if i2c_sto=1, deal with data, other wait
		GOTO	I2cWatingForStop
		BCF		INTE,I2CIE ;DISABLE I2C interrput
		BCF		INTE,GIE
		BCF		flag1,isCmdByte
			
		BTFSC	flag1,isCmdEnd
		GOTO	MainStart

		GOTO	SleepMode_1
	
MainStart:
		MOVFW	_WRITE_CMD_DATA
		SUBLW	40H
		BTFSC	STATUS,Z
		GOTO	LEARN_START
		MOVFW	_WRITE_CMD_DATA
		SUBLW	45H
		BTFSC	STATUS,Z
		GOTO	REC_LEARN_START
		MOVFW	_WRITE_CMD_DATA
		SUBLW	054H
		BTFSC	STATUS,Z
		CALL	CONSUMERIR_REMOTE
		NOP
		GOTO	SleepMode	
LEARN_START:
	;	BSF		TCCONA,CAPSEL
		GOTO	LEARN_RMT
	

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
		GOTO	START

WDTC_STATUS:
		NOP
		CLRF	STATUS
		GOTO	MAIN_START



	 

;=======================================================================
;  I2C 中断处理函数

;=======================================================================     

Interrupt_0ch:
		BTFSS	INTF, I2CIF
		GOTO	TIMBInterrupt
		BTFSC	I2CCON, I2C_RW ;i2c data write or read?
		GOTO	I2cReadData
;--------- IIC write start
		BTFSS	I2CCON, I2C_READY ;i2c data is ready?
		GOTO	I2cInterruptEnd
		BTFSS	flag1,isCmdByte
		GOTO	CmdDeal
		GOTO	I2cWriteData

I2cReadData:
		BTFSC	I2CCON, I2C_READY ;i2c data is ready?
		GOTO	I2cReadDataEnd
	;	INCFSZ	FRS0,F
	;	GOTO	$+2
	;	SETDP	01h
		
		INCF	FRS0,F
		MOVFW	IND0
		BSF		I2CCON, I2C_READY
		CLRF	INTF
		RETFIE

I2cReadDataEnd:
		CLRF	INTF
		RETFIE
;--------- IIC write get value
I2cWriteData:
		MOVI2CW
		MOVWF	IND0
		INCF	FRS0,F
	;	INCFSZ	FRS0,F 
	;	GOTO	$+2
	;	SETDP	01h
		CLRF	INTF
		RETFIE
CmdDeal:
		BSF		flag1,isCmdByte
		MOVI2CW
		MOVWF	temp
		SWAPF	temp,w
		ANDLW	0FH	
		ADDPCW
		GOTO	ERROR				; 0x00
		GOTO	WriteCode				; 0x01
		GOTO	WriteEnd				; 0x02
		GOTO	StartRECLearn				; 0x03
		GOTO	StartRMTLearn		; 0x04
		GOTO	StopRMTLearn				; 0x05
		GOTO	ReadVersion			; 0x06
		GOTO	ReadCode				; 0x07
		GOTO	ERROR				; 0x08
		GOTO	ERROR				; 0x09
		GOTO	ERROR				; 0x0a
		GOTO	ERROR				; 0x0b
		GOTO	ReadVersion		; 0x0c
		GOTO	ReadCode			; 0x0d
		GOTO	ERROR			; 0x0e
		GOTO	ERROR			; 0x0f

ERROR:
		_DIS_TIMERA_
		_DIS_InterruptTIMERA_
		_DIS_InterruptI2C_
		
		BCF		INTE,GIE
		CLRF	INTF
		BSF		flag,isError
		RETFIE
I2cInterruptEnd:
		CLRF	INTF
		RETFIE
TIMBInterrupt:
		CLRF	INTF
		BCF		TCCONA,TCRSTA
		BSF		TCCONA,TCENA
		BCF		TCCONB,TCENB
		BCF		TCCONB,TCRSTB
		BCF		flag1,isHighLow
		RETFIE

InitValue:
		SETDP	00h
		CLRF	ncount
		MOVLW	40h
		MOVWF	FRS0
		MOVLW	01h
		MOVWF nWriteByteCount_h
		MOVLW	0c0h
		MOVWF nWriteByteCount_L
INIT_RAM1:
		MOVFW	NCOUNT
		MOVWF	IND0
		INCF	NCOUNT,F
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		DECFSZ	nWriteByteCount_h,F
		GOTO	INIT_RAM1
		DECFSZ	nWriteByteCount_L,F
		GOTO	INIT_RAM1
		NOP
		RETURN
		END                