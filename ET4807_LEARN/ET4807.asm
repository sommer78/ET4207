;===================================
; ET4207 program
;===================================
; jiangs
; 2015.5.13
; 手机遥控器
;===================================
include ET4807.inc
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
include i2c_cmd_protocol.asm
include peripheral.asm
include interrupt.asm

MAIN:
		NOP
		NOP
		NOP
		BTFSC	STATUS,TO
		GOTO	WDTC_STATUS
		MOVLW	06h
		MOVWF	WDTCTR
		CLRWDT
MAIN_START:
		CAll	SysIni
		CALL	RamsClearALL
		CALL	IniVersion
		CALL	NOP1000
		BSF		I2CCON,I2C_PT15				;PT1.5 OD(开漏功能)打开
		BSF		I2CCON,I2C_EN				;enable I2C port(PT1_4(SCL), PT1_3(SDA))
		BCF		INTE,TMBIE
		BSF		INTE, I2CIE ;enable I2C interrput
		BSF		INTE, GIE ;enable globle interrupt 


LoopMode:
;		CLRF	flag1
;		MOVLW	04h
;		MOVWF	WDTCTR
;		CLRWDT
;		_ET4807_OUT_HIGH_
;		_ET4807_OUT_LOW_
;		GOTO	LoopMode	

MainStart:
		CLRWDT	
		GOTO	Learn_rmt
		GOTO	LoopMode
	

	

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