;===================================
; ET4207 interrupt
;===================================
; jiangs
; 2017.12.15

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
		BTFSS	FLAG,isCmdByte
		GOTO	CmdDeal
		GOTO	I2cWriteData

I2cReadData:
		BTFSC	I2CCON, I2C_READY ;i2c data is ready?
		GOTO	I2cReadDataEnd
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		
	;	INCF	FRS0,F
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
	;	INCF	FRS0,F
		INCFSZ	FRS0,F 
		GOTO	$+2
		SETDP	01h
		CLRF	INTF
		RETFIE
CmdDeal:
		BSF		FLAG,isCmdByte
		MOVI2CW
		MOVWF	I2C_CMD
		SWAPF	I2C_CMD,w
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
		BSF		state_flag,isError
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
		BCF		FLAG,isHighLow
		RETFIE

/*
		BCF INTF,TMBIF
		
;		BTFSC F_LEARN,F_TIMB

		BSF F_LEARN,F_TIMB
		 BTFSC F_LEARN,F_HEAD 
		RETFIE 
MEASURE_HIGH_CYCLE:
        BSF F_LEARN,F_M_LOW
        BTFSC F_LEARN,F_M_SEL
 ;       GOTO PLUS_SAVE
         ;GOTO INT_TIMEB_RET
		 RETFIE
        
;       INCFSZ TF_COUNTL,F  ;用作测量码间隔
;         GOTO INT_TIMEB_RET
;	     INCF TF_COUNTH,F      
;         GOTO INT_TIMEB_RET

PLUS_SAVE:
        MOVWF W_BAK
		MOVFW STATUS
		MOVWF STATUS_BAK

         BSF F_LEARN,F_M_SEL
		MOVFW DAT0 ;载波个数存入缓存区
		MOVWF IND1
        INCF FRS1,F
		MOVFW DAT1
		MOVWF IND1
        INCF FRS1,F

        CLRF DAT0
		CLRF DAT1
	    BCF INTE,TMBIE ;zzzzzzzzzzzzzzzzzzzzzzzz
          CLRF  TSETB
          MOVLW 00001011B ;定时器B选32分频测量间隔 992us/6.4us=155
		  MOVWF TCCONB

    ;  BSF F_LEARN,F_TF
        CLRF TF_COUNTL
        CLRF TF_COUNTH
INT_TIMEB_RET:
		MOVFW STATUS_BAK
		MOVWF STATUS
	 	MOVFW W_BAK
		RETFIE

*/

;=======================================================================
; GPIO DOWN TRIG 
;=======================================================================     
Interrupt_EXT:
		_DIS_InterruptEXT_
		RETFIE


		
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
			BSF		FLAG,isHighLow
			BSF		FLAG,bLearnEnd
			RETFIE
Interrupt_Capture:
			BTFSC	FLAG,isHighLow
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
			BSF		FLAG,isHighLow
			BCF		TCCONA,TCENA
			BSF		TCCONB,TCENB
Interrupt_Capture_Inc:
			INCFSZ	new_nHighLevel_L,f
			GOTO	$+2
			INCF	new_nHighLevel_H,f
			BCF		TCCONB,TCRSTB
			CLRF	INTF
			RETFIE
	
/*
INT_TIMEA:
INT_TIMEA_CAPIF: ;脉冲上升沿中断

        BCF TCCONB,TCRSTB ;TIMEB重新计数，作为载波结束计算用

MEASURE_LOW_CYCLE:
        MOVWF W_BAK
		MOVFW STATUS
		MOVWF STATUS_BAK

        BCF F_LEARN,F_M_SEL ;上升沿中断，tmb下一步测是否高电平结束

        BTFSC F_LEARN,F_M_LOW
        GOTO PLUS_COUNT_L
PLUS_COUNT:
          BCF TCCONA,TCRSTA  ;TMA清零开始测 低电平
       INCFSZ DAT0,F
         GOTO INT_TIMEA_RET1
	     INCF DAT1,F      
INT_TIMEA_RET1:
		MOVFW STATUS_BAK
		MOVWF STATUS
	 	MOVFW W_BAK
		 BCF INTF,CAPIF
       RETFIE  ;25*0.8=20US 小于20us会少算一个周期
    
PLUS_COUNT_L:	      
  		  MOVLW 156 ;206
          MOVWF TSETB
          MOVLW 00001000B ;111B  ;定时器B选1分频
		  MOVWF TCCONB
   ;      BCF F_LEARN,F_M_SEL ;上升沿中断，tmb下一步测是否高电平结束
		 BCF F_LEARN,F_TIMB ;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	     BSF INTE,TMBIE    ;>>>>>>>>>>>>>>>>>>>>>>>>>>>>.
;	        BTFSS F_LEARN,F_M_LOW
;         GOTO PLUS_COUNT
          BCF F_LEARN,F_M_LOW
		MOVFW TCOUTAL
		MOVWF IND1
         INCF FRS1,F
		MOVFW TCOUTAH
		MOVWF IND1
         INCF FRS1,F
		MOVFW STATUS_BAK
		MOVWF STATUS
	 	MOVFW W_BAK
		 BCF INTF,CAPIF
       RETFIE  ;25*0.8=20US 小于20us会少算一个周期
*/






 