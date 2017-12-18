

;==========================================
;TimerA中断
;========================================== 



Interrupt_06h:
			BTFSC   INTF, CAPIF
			GOTO	Interrupt_Capture
Interrupt_TimerA:
	Low_Level:
			BCF		flag1,isHighLow
			CLRF	INTF
			INCFSZ	nLowLevel_L,f
			RETFIE
			INCF	nLowLevel_H,f
			BTFSS	nLowLevel_H,4
			RETFIE
			BSF		flag1,isHighLow
			BSF		flag1,bLearnEnd
			RETFIE

			

Interrupt_Capture:
	High_Level:
			INCFSZ	nHighLevel_L,f
			GOTO	$+2
			INCF	nHighLevel_H,f
			NOP
			NOP
			NOP
			NOP
			NOP
			BCF		TCCONA,TCRSTA
			BSF		flag1,isHighLow
			CLRF	INTF
			RETFIE








			



;==========================================
;采集输入rmt时间信号
;========================================== 
Learn_rmt:
			SETDP	00h
			MOVLW	RAM_address
			MOVWF	FRS0
			BSF     RMTCTR,RX_EN				
			BCF     RMTCTR,TX_EN				;RX_EN=1,TX_EN=0 接收模式
			CLRF    PWMCON
			BSF		PWMCON,PWM_SEL				;PWM OUT = 1为高电平，进入学习模式
			CLRF    TSETAL
            CLRF    TSETAH
			CLRF    nLowLevel_H
			CLRF    nLowLevel_L
			CLRF    nHighLevel_H
			CLRF    nHighLevel_L
			MOVLW	5
			ADDWF	nHighLevel_L,f
			CLRF	Count_total
			CLRF	flag
			CLRF	flag1
			BSF		flag1,isHighLow
			CALL	DELAY150ms
			CALL	DELAY150ms
			CALL	DELAY150ms
;============================================
;获取载波频率，存放在Freq寄存器
;============================================
			BCF		INTE,TMAIE
			BCF		INTE,GIE
			;BSF		WDTCTR,4
	        BCF		PWMCON,TCOUTA_DIR			;TIMER UP 16bit型
	    	MOVLW	00000111b					;timer时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONA
			CLRF	TSETAL
			CLRF	TSETAH
			CLRF	INTF
			BCF		TCCONA,TCRSTA 
			BCF		INTE,TMAIE
			BSF     INTE,CAPIE ;ADD BY SUNSIBING to enable RMT capture interrupt
			BCF		INTE,GIE
			BSF		TCCONA,TCENA ;ADD BY SUNSIBING
			BSF     TCCONA,CAPDEB
	        CLRF	temp
			BTFSS	INTF,CAPIF
			GOTO	$-1
		CarrierCount:
			MOVFW	TCOUTAL
			MOVWF	FreqL
			MOVFW	TCOUTAH
			MOVWF	FreqH
			CLRF	INTF
			;BCF		INTE,GIE
			BTFSS	INTF,CAPIF			;等待第2个载波
			GOTO	$-1
			CLRF	INTF
			;BCF		flag,isCarrierIn
			BTFSS	INTF,CAPIF			;等待第3个载波
			GOTO	$-1
			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第4个载波
			GOTO	$-1
			CLRF	INTF
			;BSF		INTE,GIE
			;BTFSS	flag,isCarrierIn	;等待第5个载波
			;GOTO	$-1
			BTFSS	INTF,CAPIF			;等待第5个载波
			GOTO	$-1
			;BCF		INTE,GIE
			MOVFW	FreqL
			SUBWF	TCOUTAL,W
			MOVWF	FreqL
			MOVFW	FreqH
			SUBWFC	TCOUTAH,W
			MOVWF	FreqH
			/*
			MOVFW	tempL
			MOVWF	FreqL
			MOVFW	tempH
			MOVWF	FreqH
			BTFSS	temp,2
			GOTO	$-1
			BCF		INTE,CAPIE
			MOVFW	FreqL
			SUBWF	tempL,W
			MOVWF	FreqL
			MOVFW	FreqH
			SUBWFC	tempH,W
			MOVWF	FreqH
			BSF		flag,isCarrierEnd
			*/
			BCF		STATUS,C
			RRF		FreqH,F
			RRF		FreqL,F
			BCF		STATUS,C
			RRF		FreqH,F
			RRF		FreqL,F
			COMF	FreqL,W
			MOVWF	TSETAL
			COMF	FreqH,W
			MOVWF	TSETAH
			BSF     flag,isCarrierEnd
			BCF		TCCONA,TCRSTA				;清除TCC TCOUTL
			;BSF		INTE,CAPIE
			BSF     INTE,TMAIE
			BSF		INTE,GIE
	WaitForLearnEnd:
			BTFSC	flag1,isHighLow
			GOTO	$-1
			MOVFW	nHighLevel_H
			MOVWF	IND0
			INCF	FRS0,F
			MOVFW	nHighLevel_L
			MOVWF	IND0
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			;CLRF	Count_total
			CLRF	nHighLevel_H
			CLRF	nHighLevel_L
			INCF	Count_total,f
			;BCF		STATUS,C
			MOVLW	192
			SUBWF	Count_total,W
			BTFSC	STATUS,C
			GOTO	Learn_next

			BTFSS	flag1,isHighLow
			GOTO	$-1
			BTFSC	flag1,bLearnEnd
			GOTO	Learn_next
			MOVFW	nLowLevel_H
			MOVWF	IND0
			INCF	FRS0,F
			MOVFW	nLowLevel_L
			MOVWF	IND0
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			;CLRF	Count_total
			CLRF	nLowLevel_H
			CLRF	nLowLevel_L
			INCF	Count_total,f
			;BCF		STATUS,C
			MOVLW	192
			SUBWF	Count_total,W
			BTFSC	STATUS,C
			GOTO	Learn_next
			GOTO	WaitForLearnEnd

Learn_next: 
			BCF		INTE,GIE
			BCF     INTE,TMAIE
			BCF     INTE,CAPIE
			BCF		TCCONA,TCENA
			;BCF     TCCONA,CAPDEB
			NOP



;************************************************ 
;************************************************ 
;************************************************ 
learn_decode:
			

;************************************************
;      remoter learn restone         ;
;************************************************   				
remoter_code_restone:	
		GOTO	learn_end

learn_end:
;;;;;;;;;FreqH的取值为82h,83h,84h,85h,86h时;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		MOVFW	FreqL
		MOVWF	FreqH
		BCF		Status,C
		RRF		FreqL,F

		BCF		WDTCTR,4

		GOTO	SleepMode





;********************************************************
;				取绝对值
;********************************************************
Neg:
          btfsc       Status,C
          return
		  movwf		  NegTemp
          comf        NegTemp,w
          addlw       1
          return

			
