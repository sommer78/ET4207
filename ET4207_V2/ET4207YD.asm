;===================================
; ET4207 program
;===================================
; jiangs
; 2016.1.5
; 手机遥控器
;===================================
include ET4007.inc
include et4207_include.inc

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
include interrupt.asm
include calculation.asm
include learn_new.asm
include data_match.asm

MAIN:
		NOP
		NOP
		NOP
		BTFSC	STATUS,TO
		GOTO	WDTC_STATUS
		_WDT_DIS

MAIN_START:
		CAll	SysIni
		CALL	RamsClearALL
	;	CALL	IniVersion
	



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
	
		CLRF	FLAG
		_WDT_DIS
		CLRWDT


		_ET4207_NOT_BUSY_

		BCF		INTE,I2CIE ;disable I2C interrput
		BCF		INTE,GIE
		CLRF	INTF
		BCF		FLAG,isCmdByte
		SLEEP
	;	GOTO	Learn_rmt
		NOP
		NOP
		_WDT_DIS				;_WDT_EN
		_ET4207_BUSY_
	


;I2C:		
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
		BCF		FLAG,isCmdByte
			
		BTFSC	FLAG,isCmdEnd
		GOTO	MainStart

		GOTO	SleepMode_1
	
MainStart:
		MOVFW	_WRITE_CMD_DATA
		SUBLW	40H
		BTFSC	STATUS,Z
		GOTO	RMT_LEARN_START
		MOVFW	_WRITE_CMD_DATA
		SUBLW	054H
		BTFSC	STATUS,Z
		CALL	CONSUMERIR_START
		MOVFW	_WRITE_CMD_DATA
		SUBLW	055H
		BTFSC	STATUS,Z
		CALL	TIANJIA_START
		NOP
		GOTO	SleepMode	
RMT_LEARN_START:
	;	BSF		TCCONA,CAPSEL
		CALL	LEARN_RAM_CLR
		GOTO	LEARN_RMT
	





WDTC_STATUS:
		NOP
		CLRF	STATUS
		GOTO	MAIN_START



	 


		END                