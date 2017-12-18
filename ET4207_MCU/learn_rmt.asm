
;==========================================
;TimerA中断
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
End_Learn:
			NOP
			NOP
			RETFIE






			



;==========================================
;采集输入rmt时间信号
;========================================== 
Learn_rmt:
			SETDP	02h
;			CLRF	flag
			CLRF	flag1
			CLRF	_LEARN_FLAG

			BCF     TCCONA,CAPSEL	
			BSF     RMTCTR,RX_EN				
			BCF     RMTCTR,TX_EN				;RX_EN=1,TX_EN=0 接收模式
			CLRF    PWMCON
			BSF		PWMCON,PWM_SEL				;PWM OUT = 1为高电平，进入学习模式

			CLRF    TSETAL
            CLRF    TSETAH
			BCF		flag1,bLearnEnd
			BSF		flag1,isHighLow
			MOVLW	5
			ADDWF	new_nHighLevel_L,f
			
		;	CALL	DELAY150ms
		;	CALL	DELAY150ms
		;	CALL	DELAY150ms

			MOVLW	ADDRESS_PartIndexCount
			MOVWF	p_PartIndexCount
			MOVLW	ADDRESS_Sample
			MOVWF	p_Sample
			MOVLW	ADDRESS_Index
			MOVWF	p_Index
			CLRF	nIndexCount_h
			CLRF	nIndexCount_l
;============================================
;获取载波频率，存放在Freq寄存器
;============================================
		
			BCF		INTE,TMAIE
			BCF		INTE,GIE
			BSF		I2CCON,I2C_PT15				;PT1.5 OD(开漏功能)打开
			BSF		I2CCON,I2C_EN				;enable I2C port(PT1_4(SCL), PT1_3(SDA))
			;BCF		WDTCTR,3
	;		BTFSC	flag,isLearnRec
	;		GOTO	learn_rec_st1
	        BCF		PWMCON,TCOUTA_DIR			;TIMER UP 16bit型
	    	MOVLW	00000110b					;timera时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONA
			CLRF	TSETAL
			CLRF	TSETAH
			BCF		TCCONA,TCRSTA
		
			
	;		GOTO    learn_init_st1
			/*
learn_rec_st1:
			MOVLW	01000110b					
			MOVWF	TCCONA
			CLRF	TSETAL
			CLRF	TSETAH
			BCF		TCCONA,TCRSTA	
learn_init_st1:	
*/		
			MOVLW	00000110b					;timerb时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONB
			CLRF	TSETB
			BCF		TCCONB,TCRSTB
			CLRF	INTF
			BCF		INTE,TMAIE
			BSF		INTE,TMBIE
			BSF     INTE,CAPIE ;ADD BY SUNSIBING to enable RMT capture interrupt
			BSF		INTE,I2CIE
			BCF		INTE,GIE
			BSF		TCCONA,TCENA ;ADD BY SUNSIBING
			BSF     TCCONA,CAPDEB

	        CLRF	temp

			MOVLW	08h
			MOVWF	WDTCTR
			CLRWDT
	
;			BTFSS	PT1,SDA
;			GOTO	IIC
		;	BTFSC	flag1,bLearnEnd
	;		GOTO	LearnStop



WaitCapSignal:
			BTFSC	INTF,I2CIF
			GOTO	LearnStop			;I2C 信号，等待中断。
			BTFSS	INTF,CAPIF			;等待第1个载波
			GOTO	WaitCapSignal

			MOVLW	0dh
			MOVWF	WDTCTR
			CLRWDT

			MOVFW	TCOUTAL
			MOVWF	FreqL
			MOVFW	TCOUTAH
			MOVWF	FreqH
			CLRF	INTF

			BTFSS	INTF,CAPIF			;等待第2个载波
			GOTO	$-1	
			MOVFW	TCOUTAL
			MOVWF	FreqL1
			MOVFW	TCOUTAH
			MOVWF	FreqH1

			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第3个载波
			GOTO	$-1
			MOVFW	TCOUTAL
			MOVWF	FreqL2
			MOVFW	TCOUTAH
			MOVWF	FreqH2
		;	BCF		TCCONA,TCRSTA
			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第4个载波
			GOTO	$-1
			MOVFW	TCOUTAL
			MOVWF	FreqL3
			MOVFW	TCOUTAH
			MOVWF	FreqH3
		;	BCF		TCCONA,TCRSTA
			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第5个载波
			GOTO	$-1
			MOVFW	TCOUTAL
			MOVWF	FreqL4
			MOVFW	TCOUTAH
			MOVWF	FreqH4
;			BCF		TCCONA,TCRSTA
			MOVFW	FreqL
			SUBWF	TCOUTAL,W
			MOVWF	FreqL
			MOVFW	FreqH
			SUBWFC	TCOUTAH,W
			MOVWF	FreqH
			CLRF	STATUS
			RRF		FreqH,F
			RRF		FreqL,F
			COMF	FreqL,W
			MOVWF	TSETB
			CLRF	INTF
			MOVFW	FreqH
			SUBLW	00H
			BTFSS	STATUS,Z
			GOTO	LearnErrorOut
			MOVLW	00000100b
			MOVWF	TCCONA
		
			;BSF     TCCONA,CAPDEB
			BSF		TCCONB,TCENB
			BSF     INTE,TMAIE
			BSF		INTE,GIE

			BTFSC	flag1,isHighLow
			GOTO	$-1
			BTFSS	flag1,isHighLow
			GOTO	$-1
			BTFSC	flag1,isHighLow
			GOTO	$-1

			
			CLRF	nPartIndexCount
			INCF	nPartIndexCount,f
			CLRF	Sample0_nHighLevel_H
			CLRF	Sample0_nHighLevel_L
			CLRF	Sample0_nLowLevel_H
			CLRF	Sample0_nLowLevel_L
			MOVFW	p_Sample
			MOVWF	FRS1
			MOVFW	nHighLevel_H
			MOVWF	IND1
			MOVWF	Sample1_nHighLevel_H
			INCF	FRS1,f
			MOVFW	nHighLevel_L
			MOVWF	IND1
			MOVWF	Sample1_nHighLevel_L
			INCF	FRS1,f
			MOVFW	nLowLevel_H
			MOVWF	IND1
			MOVWF	Sample1_nLowLevel_H
			INCF	FRS1,f
			MOVFW	nLowLevel_L
			MOVWF	IND1
			MOVWF	Sample1_nLowLevel_L
			;;;;;;;Index移位存放程序;;;;;;;;;;;;;;
			BSF		status,c	;;;;;;与其他不同
			RRF		Index_Bit,f
			BTFSS	status,c
			GOTO	$+3
			BSF		Index_Bit,7
			INCF	p_Index,f
			MOVFW	p_Index
			MOVWF	FRS0
			MOVFW	Index_Bit
			IORWF	IND0,f
			INCFSZ	nIndexCount_l,f
			GOTO	$+2
			INCF	nIndexCount_h,f
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	SampleCompare_loop:
			BTFSC	flag1,bLearnEnd
			GOTO	Learn_next
			BTFSS	flag1,isHighLow
			GOTO	$-1
			BTFSC	flag1,bLearnEnd
			GOTO	SampleCompare_0
			BTFSC	flag1,isHighLow
			GOTO	$-1
	SampleCompare_0:
			MOVFW	Sample0_nHighLevel_L
			SUBWF	nHighLevel_L,w
			MOVWF	DELTA_ABS_L
			MOVFW	Sample0_nHighLevel_H
			SUBWFC	nHighLevel_H,w
			MOVWF	DELTA_ABS_H
			CALL	Neg
			MOVLW	_DELTA_HighLevel_L_
			SUBWF	DELTA_ABS_L,w
			MOVLW	_DELTA_HighLevel_H_
			SUBWFC	DELTA_ABS_H,w
			BTFSC	status,c
			GOTO	SampleCompare_1
			MOVFW	Sample0_nLowLevel_L
			SUBWF	nLowLevel_L,w
			MOVWF	DELTA_ABS_L
			MOVFW	Sample0_nLowLevel_H
			SUBWFC	nLowLevel_H,w
			MOVWF	DELTA_ABS_H
			CALL	Neg
			MOVLW	_DELTA_LowLevel_L_
			SUBWF	DELTA_ABS_L,w
			MOVLW	_DELTA_LowLevel_H_
			SUBWFC	DELTA_ABS_H,w
			BTFSC	status,c
			GOTO	SampleCompare_1
			;;;;;;;Index移位存放程序;;;;;;;;;;;;;;
			BCF		status,c
			RRF		Index_Bit,f
			BTFSS	status,c
			GOTO	$+3
			BSF		Index_Bit,7
			INCF	p_Index,f
			;MOVFW	p_Index
			;MOVWF	FRS1
			;MOVFW	Index_Bit
			;IORWF	IND1,f
			INCFSZ	nIndexCount_l,f
			GOTO	$+2
			INCF	nIndexCount_h,f
			INCFSZ	nPartIndexCount,f
			GOTO	SampleCompare_loop
			DECF	nPartIndexCount,f
			GOTO	StoreNewSample
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	SampleCompare_1:
			MOVFW	Sample1_nHighLevel_L
			SUBWF	nHighLevel_L,w
			MOVWF	DELTA_ABS_L
			MOVFW	Sample1_nHighLevel_H
			SUBWFC	nHighLevel_H,w
			MOVWF	DELTA_ABS_H
			CALL	Neg
			MOVLW	_DELTA_HighLevel_L_
			SUBWF	DELTA_ABS_L,w
			MOVLW	_DELTA_HighLevel_H_
			SUBWFC	DELTA_ABS_H,w
			BTFSC	status,c
			GOTO	StoreNewSample
			MOVFW	Sample1_nLowLevel_L
			SUBWF	nLowLevel_L,w
			MOVWF	DELTA_ABS_L
			MOVFW	Sample1_nLowLevel_H
			SUBWFC	nLowLevel_H,w
			MOVWF	DELTA_ABS_H
			CALL	Neg
			MOVLW	_DELTA_LowLevel_L_
			SUBWF	DELTA_ABS_L,w
			MOVLW	_DELTA_LowLevel_H_
			SUBWFC	DELTA_ABS_H,w
			BTFSC	status,c
			GOTO	StoreNewSample
			;;;;;;;Index移位存放程序;;;;;;;;;;;;;;
			BCF		status,c
			RRF		Index_Bit,f
			BTFSS	status,c
			GOTO	$+3
			BSF		Index_Bit,7
			INCF	p_Index,f
			MOVFW	p_Index
			MOVWF	FRS0
			MOVFW	Index_Bit
			IORWF	IND0,f
			INCFSZ	nIndexCount_l,f
			GOTO	$+2
			INCF	nIndexCount_h,f
			INCFSZ	nPartIndexCount,f
			GOTO	SampleCompare_loop
			DECF	nPartIndexCount,f
			;GOTO	StoreNewSample
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	StoreNewSample:
			MOVFW	p_PartIndexCount
			MOVWF	FRS0
			MOVFW	nPartIndexCount
			MOVWF	IND0
			INCF	p_PartIndexCount,f
			CLRF	nPartIndexCount
			INCF	nPartIndexCount,f
			MOVLW	4
			ADDWF	p_Sample,f
			MOVFW	Sample1_nHighLevel_H
			MOVWF	Sample0_nHighLevel_H
			MOVFW	Sample1_nHighLevel_L
			MOVWF	Sample0_nHighLevel_L
			MOVFW	Sample1_nLowLevel_H
			MOVWF	Sample0_nLowLevel_H
			MOVFW	Sample1_nLowLevel_L
			MOVWF	Sample0_nLowLevel_L
			MOVFW	p_Sample
			MOVWF	FRS1
			MOVFW	nHighLevel_H
			MOVWF	IND1
			MOVWF	Sample1_nHighLevel_H
			INCF	FRS1,f
			MOVFW	nHighLevel_L
			MOVWF	IND1
			MOVWF	Sample1_nHighLevel_L
			INCF	FRS1,f
			MOVFW	nLowLevel_H
			MOVWF	IND1
			MOVWF	Sample1_nLowLevel_H
			INCF	FRS1,f
			MOVFW	nLowLevel_L
			MOVWF	IND1
			MOVWF	Sample1_nLowLevel_L
			;;;;;;;Index移位存放程序;;;;;;;;;;;;;;
			BCF		status,c
			RRF		Index_Bit,f
			BTFSS	status,c
			GOTO	$+3
			BSF		Index_Bit,7
			INCF	p_Index,f
			MOVFW	p_Index
			MOVWF	FRS0
			MOVFW	Index_Bit
			IORWF	IND0,f
			INCFSZ	nIndexCount_l,f
			GOTO	$+2
			INCF	nIndexCount_h,f
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			GOTO	SampleCompare_loop
			
	

Learn_next:
			MOVFW	p_PartIndexCount
			MOVWF	FRS0
			MOVFW	nPartIndexCount
			MOVWF	IND0
			INCF	p_PartIndexCount,f
			BCF		INTE,GIE
			BCF     INTE,TMAIE
			BCF     INTE,CAPIE
			BCF		TCCONA,TCENA
			NOP
			RRF		FreqH,F
			RRF		FreqL,F



;************************************************ 
;************************************************ 
;************************************************ 
;learn_decode:
			

;************************************************
;      remoter learn restone         ;
;************************************************   				
;remoter_code_restone:	
;		GOTO	learn_end

learn_end:
		MOVLW	ADDRESS_PartIndexCount
		SUBWF	p_PartIndexCount,w
		MOVWF	n_PartIndexCount

		MOVLW	4
		ADDWF	p_Sample,f
		MOVLW	ADDRESS_Sample
		SUBWF	p_Sample,w
		MOVWF	n_Sample

		INCF	p_Index,f
		MOVLW	ADDRESS_Index
		SUBWF	p_Index,w
		MOVWF	n_Index

		CLRF	_crc_learn

		MOVLW	0
		MOVWF	_LENGTH_h_learn
		MOVFW	n_PartIndexCount
		MOVWF	_LENGTH_l_learn
		SETDP	00h
		MOVLW	ADDRESS_PartIndexCount
		MOVWF	FRS0
		CALL	Learn_xCal_crc
		
		MOVLW	0
		MOVWF	_LENGTH_h_learn
		MOVFW	n_Sample
		MOVWF	_LENGTH_l_learn
		SETDP	01h
		MOVLW	ADDRESS_Sample
		MOVWF	FRS0
		CALL	Learn_xCal_crc
		MOVLW	0
		MOVWF	_LENGTH_h_learn
		MOVFW	n_Index
		MOVWF	_LENGTH_l_learn
		SETDP	00h
		MOVLW	ADDRESS_Index
		MOVWF	FRS0
		CALL	Learn_xCal_crc
		

		MOVFW	_crc_learn
		MOVWF	n_Crc
		MOVLW	01H
		MOVWF	_LEARN_FLAG

		;BCF		WDTCTR,3
		SETDP	00h
		GOTO	SleepMode
		
LearnStop:
		MOVI2CW
		SUBLW	50H				; STOP COMMAND
		BTFSS	STATUS,Z
		GOTO	Learn_rmt
		SETDP	00h
		MOVLW	02H
		MOVWF	_LEARN_FLAG
		GOTO	SleepMode

LearnErrorOut:
		MOVLW	08h
		MOVWF	WDTCTR
		NOP	
		GOTO	Learn_rmt


Learn_xCal_crc:
Learn_xCal_crc_loop:
		MOVFW	IND0
		XORWF	_crc_learn,f
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		MOVLW	8
		MOVWF	_learn1
	Learn_xCal_crc_loop_loop_8times:
		MOVFW	_crc_learn
		ANDLW	01h
		SUBLW	00h
		BTFSC	STATUS,Z
		GOTO	Learn_xCal_crc_loop_8times_reasult_0
		BCF		STATUS,C
		RRF		_crc_learn,w
		XORLW	8ch
		MOVWF	_crc_learn
		GOTO	Learn_xCal_crc_loop_loop_8times_end
	Learn_xCal_crc_loop_8times_reasult_0:
		BCF		STATUS,C
		RRF		_crc_learn,f
	Learn_xCal_crc_loop_loop_8times_end:
		DECFSZ	_learn1,f
		GOTO	Learn_xCal_crc_loop_loop_8times
		MOVLW	01h
		SUBWF	_LENGTH_l_learn,f
		MOVLW	00h
		SUBWFC	_LENGTH_h_learn,f
		MOVFW	_LENGTH_h_learn
		SUBLW	00h
		BTFSS	STATUS,Z
		GOTO	Learn_xCal_crc_loop
		MOVFW	_LENGTH_l_learn
		SUBLW	00h
		BTFSS	STATUS,Z
		GOTO	Learn_xCal_crc_loop
		RETURN

		


;********************************************************
;				取绝对值
;********************************************************
Neg:
		BTFSC		Status,C
		RETURN
		COMF		DELTA_ABS_L,f
		COMF		DELTA_ABS_H,f
		MOVLW		1
		ADDWF		DELTA_ABS_L,f
		MOVLW		0
		ADDWFC		DELTA_ABS_H,f
		RETURN

REC_LEARN_START:
			SETDP	02h
;			CLRF	flag
			CLRF	flag1
			CLRF	_LEARN_FLAG
			CLRF    TSETAL
            CLRF    TSETAH
			BCF		flag1,bLearnEnd
			BSF		flag1,isHighLow
			MOVLW	5
			ADDWF	new_nHighLevel_L,f
			

			MOVLW	ADDRESS_PartIndexCount
			MOVWF	p_PartIndexCount
			MOVLW	ADDRESS_Sample
			MOVWF	p_Sample
			MOVLW	ADDRESS_Index
			MOVWF	p_Index
			CLRF	nIndexCount_h
			CLRF	nIndexCount_l
			
;============================================
;获取载波频率，存放在Freq寄存器
;============================================
		
			BCF		INTE,TMAIE
			BCF		INTE,GIE
			BSF		I2CCON,I2C_PT15				;PT1.5 OD(开漏功能)打开
			BSF		I2CCON,I2C_EN				;enable I2C port(PT1_4(SCL), PT1_3(SDA))
			
			;BCF		WDTCTR,3
	;		BTFSC	flag,isLearnRec
	;		GOTO	learn_rec_st1
	        BCF		PWMCON,TCOUTA_DIR			;TIMER UP 16bit型
	    	MOVLW	00000110b					;timera时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONA
			CLRF	TSETAL
			CLRF	TSETAH
			BCF		TCCONA,TCRSTA
			BSF     TCCONA,CAPSEL
			MOVLW	00000110b					;timerb时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONB
			CLRF	TSETB
			BCF		TCCONB,TCRSTB
			CLRF	INTF
			BCF		INTE,TMAIE
			BSF		INTE,TMBIE
			BSF     INTE,CAPIE ;ADD BY SUNSIBING to enable RMT capture interrupt
			BSF		INTE,I2CIE
			BCF		INTE,GIE
			BSF		TCCONA,TCENA ;ADD BY SUNSIBING
			BSF     TCCONA,CAPDEB

	        CLRF	temp

			MOVLW	08h
			MOVWF	WDTCTR
			CLRWDT
RecWaitCapSignal:
			BTFSC	INTF,I2CIF
			GOTO	LearnStop			;I2C 信号，等待中断。
			BTFSS	INTF,CAPIF			;等待第1个载波
			GOTO	RecWaitCapSignal		
			MOVFW	TCOUTAL
			MOVWF	FreqL
			MOVFW	TCOUTAH
			MOVWF	FreqH
			CLRF	INTF

			BTFSS	INTF,CAPIF			;等待第2个载波
			GOTO	$-1	
			MOVFW	TCOUTAL
			MOVWF	FreqL1
			MOVFW	TCOUTAH
			MOVWF	FreqH1

			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第3个载波
			GOTO	$-1
			MOVFW	TCOUTAL
			MOVWF	FreqL2
			MOVFW	TCOUTAH
			MOVWF	FreqH2
		;	BCF		TCCONA,TCRSTA
			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第4个载波
			GOTO	$-1
			MOVFW	TCOUTAL
			MOVWF	FreqL3
			MOVFW	TCOUTAH
			MOVWF	FreqH3
		;	BCF		TCCONA,TCRSTA
			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第5个载波
			GOTO	$-1
			MOVFW	TCOUTAL
			MOVWF	FreqL4
			MOVFW	TCOUTAH
			MOVWF	FreqH4
			NOP
		GOTO	SleepMode
		
