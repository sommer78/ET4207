;===================================
; ET4003 program
;===================================
; wangyf
; 2013.5.19
; ÊÖ»úÑ§Ï°ÍòÄÜÒ£¿ØÆ÷
;===================================
include ET4003.inc
include main.inc




BUFFER_ADDRESS		equ		80h
RamInitial_ADDRESS	equ		80h
buffer				equ		31h
nWriteBitCount		equ		32h
nWriteByteCount		equ		33h






_EN_InterruptTIMERA_	macro
					BSF		INTE,TMAIE
					endm

_DIS_InterruptTIMERA_	macro
					BCF		INTE,TMAIE
					endm


_EN_TIMERA_			macro
					BSF		TCCONA,TCENA
					endm

_RST_TIMERA_		macro
					BCF		TCCONA,TCRSTA
					endm

_DIS_TIMERA_		macro
					BCF		TCCONA,TCENA
					endm




;-----------------------------------
; Main program start point
;-----------------------------------
org	00h

START:
	BTFSS 1FFH, 7
	BTFSS 1FFH, 7 ;fpga test
	NOP
	GOTO	MAIN
	
org	06h
	;GOTO 	Interrupt_06h
	GOTO	MAIN
	
org 0ch
	GOTO	Interrupt_0ch

org	012h
	GOTO	MAIN


include peripheral.asm
;include	learn_rmt.asm
;include	rmtout.asm
include rmtout_consumeir.asm
include RegChange.asm
;include rmtout_version.asm




MAIN:
	NOP
	NOP
	NOP
	;BTFSC	STATUS,TO
	;GOTO	SUPER_LOOP
	;MOVLW	0eh
	;MOVWF	WDTCTR
	CLRWDT
	CALL	SysIni
	CALL	Initintp
	CALL	RamsClearALL

	BSF		PT1SEL,3
	BSF		PT2SEL,0

	MOVLW	11011001b
	ANDWF	RMTCTR,f
	MOVLW	00000110b
	IORWF	RMTCTR,f

	CLRF	temp
	MOVLW	7fh
	MOVWF	frs0
ram_initial:
	MOVFW	temp
	MOVWF	ind0
	INCFSZ	FRS0,F
	GOTO	$+2
	SETDP	01h
	INCFSZ	temp,f
	GOTO	ram_initial
	
	


SUPER_LOOP:
SleepMode:
		CALL	NOP1000
		BCF     RMTCTR,7
		BCF     RMTCTR,6
		BCF     RMTCTR,RX_EN				
		BCF     RMTCTR,TX_EN			 ;RX_EN=0,TX_EN=0 ½ûÖ¹·¢Éä£¬½ûÖ¹½ÓÊÕ-->½µµÍ¹¦ºÄ
		;BSF     RMTCTR,5

		BSF		I2CCON,7 ;enable I2C port(PT1_4(SCL), PT1_3(SDA))
		;BSF		INTE,1 ;enable I2C interrput
		
		;MOVLW	06h
		;MOVWF	WDTCTR
		;CLRWDT
		CLRF	flag1
		SETDP	00h
		;MOVLW	BUFFER_ADDRESS
		MOVLW	7fh
		MOVWF	pPackPointer
SleepMode_1:
		BCF		INTE,1 ;enable I2C interrput
		BCF		INTE,GIE
		CLRF	INTF
		BCF		flag1,isFirstByte
		SLEEP
		NOP
		NOP




I2C:		
			;MOVFW	pPackPointer
			;MOVWF	FRS0
			BSF		INTE, 1 ;enable I2C interrput
			BSF		INTE, GIE ;enable globle interrupt 
I2cWatingForStop:
			BTFSS   I2CCON, I2C_STO  ;if i2c_sto=1, deal with data, other wait
			GOTO	I2cWatingForStop
			BCF		INTE,1 ;enable I2C interrput
			BCF		INTE,GIE

			
			BTFSS	I2CCON, I2C_RW ;i2c data write =0 or read =1
			GOTO	$+6
			BCF		I2CCON, I2C_READY
			MOVLW	1
			SUBWF	FRS0,F
			BTFSS	STATUS,C
			SETDP	00h
			

			BTFSC	flag1,isLastPack
			GOTO	HandleData
			MOVFW	FRS0
			MOVWF	pPackPointer
			GOTO	SleepMode_1
			



HandleData:
			MOVFW	_command
			ANDLW	0f0h
			SUBLW	50h
			BTFSS	STATUS,Z
			GOTO	SleepMode
			MOVFW	_command
			ANDLW	0fh
			ADDPCW
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SEND_KITKAT_IR		;0x54
			GOTO	RegChange			;0x55
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			GOTO	SleepMode
			NOP
			NOP
			GOTO	SleepMode
		





;=================================================================		
;¼Ä´æÆ÷ÇåÁã
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


          
		  
DELAY150ms:
		CALL	NOP50000
		CALL	NOP50000
		CALL	NOP50000
		RETURN



Interrupt_0ch:
			BTFSC	I2CCON, I2C_RW ;i2c data write or read?
			GOTO	I2cReadData
			BTFSS	I2CCON, I2C_READY ;i2c data is ready?
			GOTO	I2cInterruptEnd
			BTFSS	flag1,isFirstByte
			GOTO	FirstByte
			GOTO	I2cWriteData
	I2cReadData:
			BTFSC	I2CCON, I2C_READY ;i2c data is ready?
			GOTO	I2cReadDataEnd
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			MOVFW	IND0
			BSF		I2CCON, I2C_READY
			CLRF	INTF
			RETFIE
	I2cReadDataEnd:
			MOVLW	7fh
			MOVWF	pPackPointer
			CLRF	INTF
			RETFIE

I2cWriteData:
		MOVI2CW
		MOVWF	IND0
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		CLRF	INTF
		RETFIE
FirstByte:
		MOVI2CW
		MOVWF	temp
		MOVLW	0fh
		ANDWF	temp,w
		SUBLW	00h
		BTFSS	STATUS,Z
		GOTO	$+5
		MOVLW	80h
		MOVWF	pPackPointer
		MOVFW	pPackPointer
		MOVWF	FRS0
		SWAPF	temp,w
		SUBWF	temp,w
		BTFSC	STATUS,Z
		BSF		flag1,isLastPack
		BSF		flag1,isFirstByte
		MOVFW	IND0
		CLRF	INTF
		RETFIE
I2cInterruptEnd:
		CLRF	INTF
		RETFIE


	
		END                