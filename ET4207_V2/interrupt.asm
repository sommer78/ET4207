;===================================
; ET4207 interrupt
;===================================
; jiangs
; 2016.1.5

;===================================



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
		GOTO	SetCurrent				; 0x08
		GOTO	SetLearn				; 0x09
		GOTO	ERROR				; 0x0a
		GOTO	ERROR				; 0x0b
		GOTO	ERROR			; 0x0c
		GOTO	ERROR			; 0x0d
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
;=======================================================================
;  TIMEB INTERRUPT
;=======================================================================     
TIMBInterrupt:
		CLRF	INTF
		BCF		TCCONA,TCRSTA
		BSF		TCCONA,TCENA
		BCF		TCCONB,TCENB
		BCF		TCCONB,TCRSTB
		BCF		flag1,isHighLow
		RETFIE

;=======================================================================
; GPIO DOWN TRIG 
;=======================================================================     
Interrupt_EXT:
		_DIS_InterruptEXT_
		GOTO	START


		
;==========================================
;TimerA CAPTURE INTERRUPT
;========================================== 

Interrupt_06h:
			BTFSC   INTF, CAPIF
			GOTO	Interrupt_Capture
Interrupt_TimerA:
			BCF		INTF,TMAIF
			MOVLW	0ffh
			MOVWF	nLowLevel_L
			MOVLW	0ffh
			MOVWF	nLowLevel_H
			MOVFW	new_nHighLevel_L
			MOVWF	nHighLevel_L
			MOVFW	new_nHighLevel_H
			MOVWF	nHighLevel_H
			BSF		flag1,isHighLow
			BSF		flag1,bLearnEnd
			RETFIE
Interrupt_Capture:
			BTFSC	flag1,isHighLow
			GOTO	Interrupt_Capture_Inc
			MOVFW	TCOUTAL
			MOVWF	nLowLevel_L
			MOVFW	TCOUTAH
			MOVWF	nLowLevel_H
			MOVFW	new_nHighLevel_L
			MOVWF	nHighLevel_L
			MOVFW	new_nHighLevel_H
			MOVWF	nHighLevel_H
			CLRF	new_nHighLevel_L
			CLRF	new_nHighLevel_H
			BSF		flag1,isHighLow
			BCF		TCCONA,TCENA
			BSF		TCCONB,TCENB
Interrupt_Capture_Inc:
			INCFSZ	new_nHighLevel_L,f
			GOTO	$+2
			INCF	new_nHighLevel_H,f
			BCF		TCCONB,TCRSTB
			CLRF	INTF
			RETFIE

