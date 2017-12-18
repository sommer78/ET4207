;===================================
; ET4207 program
;===================================
; jiangs
; 2016.1.5
; �ֻ�ң����
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
include consumerir.asm
include i2c_cmd_protocol.asm
include peripheral.asm
include interrupt.asm




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
		BCF     RMTCTR,TX_EN			 ;RX_EN=0,TX_EN=0 ��ֹ���䣬��ֹ����-->���͹���
		BSF     RMTCTR,PWM_CURT
		BSF     RMTCTR,PWM_CURT1		;��������1	
		BSF     RMTCTR,PWM_CURT0		;��������0

		BSF		I2CCON,I2C_PT15				;PT1.5 OD(��©����)��
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
	;	BsF		FLAG,isLearnRec
		SLEEP
	;	GOTO	Learn_rmt
		NOP
		NOP
		MOVLW	0dh
		MOVWF	WDTCTR
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
		BCF		flag1,isCmdByte
			
		BTFSC	flag1,isCmdEnd
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