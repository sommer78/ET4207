;==========================================
;采集输入rmt时间信号
;========================================== 
LEARN_RMT_ZIP2:
			
			CLRF	_LEARN_FLAG
			SETDP	02h
			CLRF	FLAG
			BSF     RMTCTR,RX_EN				
			BCF     RMTCTR,TX_EN				;RX_EN=1,TX_EN=0 接收模式
			CLRF    PWMCON
			BSF		PWMCON,PWM_SEL				;PWM OUT = 1为高电平，进入学习模式
		;	BSF		state_flag,isLearnP14
			CLRF    TSETAL
           		CLRF    TSETAH
			BCF		FLAG,bLearnEnd
			BSF		FLAG,isHighLow
		
			MOVLW	5
			ADDWF	new_nHighLevel_L,f
			

			MOVLW	ADDRESS_PartIndexCount
			;MOVWF	p_PartIndexCount
			;MOVLW	ADDRESS_Sample
			MOVWF	p_Sample
			MOVLW	ADDRESS_Sample
		;	MOVLW	ADDRESS_Index
			MOVWF	p_Index
		;	MOVLW	100H
		;	MOVWF	p_PartIndexCount
			CLRF	nIndexCount_l
			CLRF	nIndexCount_H
			CLRF	total_sample
			_GREEN_SET
		;	GOTO	LRNZ_Collection_Loop
;============================================
;获取载波频率，存放在Freq寄存器
;============================================
			BCF		INTE,TMAIE
			BCF		INTE,GIE
	       	BCF		PWMCON,TCOUTA_DIR			;TIMER UP 16bit型
LRNZ_INPUT_SELECT:
			BTFSS	state_flag,isLearnP14
			GOTO	LRNZ_INPUT_1
	    	MOVLW	01110110b					;	INPUT RMT timera时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONA
			GOTO	LRNZ_INPUT_END_1
LRNZ_INPUT_1:
			MOVLW	00100110B				; INPUT PT14
			MOVWF	TCCONA
LRNZ_INPUT_END_1:
			NOP
			CLRF	TSETAL
			CLRF	TSETAH
			BCF		TCCONA,TCRSTA
			MOVLW	00000110b					;timerb时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONB
			CLRF	TSETB
			BCF		TCCONB,TCRSTB

			CLRF	INTF
			BCF		INTE,TMAIE
			BSF		INTE,TMBIE
			BSF     INTE,CAPIE ;ADD BY SUNSIBING to enable RMT capture interrupt
			BSF		INTE,I2CIF
			BCF		INTE,GIE
			BSF		TCCONA,TCENA ;ADD BY SUNSIBING
			BSF     TCCONA,CAPDEB

			MOVLW	78H
			MOVWF	TSETB
LEARNZ_CAP_WAIT:
			_WDT_DIS

			BTFSC	state_flag,isLearnEnd
			GOTO	LEARN_RMT_ZIP2_OUT
			BTFSS	INTF,CAPIF			;等待第1个载波
			GOTO	$-3

			_WDT_DIS			;	_WDT_EN  sommer
			CLRWDT

			MOVFW	TCOUTAL
			MOVWF	FreqL
			MOVFW	TCOUTAH
			MOVWF	FreqH
			CLRF	INTF
			BSF		TCCONB,TCENB
			BCF		TCCONB,TCRSTB

			BTFSC	INTF,TMBIF
			GOTO	LEARNZ_CAP_WAIT
			BTFSS	INTF,CAPIF			;等待第2个载波
			GOTO	$-3
						
			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第3个载波
			GOTO	$-1
			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第4个载波
			GOTO	$-1
			CLRF	INTF
			BTFSS	INTF,CAPIF			;等待第5个载波
			GOTO	$-1
			MOVFW	FreqL
			SUBWF	TCOUTAL,W
			MOVWF	FreqL
			MOVFW	FreqH
			SUBWFC	TCOUTAH,W
			MOVWF	FreqH
			RRF		FreqH,F
			RRF		FreqL,F
			COMF	FreqL,W
			MOVWF	TSETB
			CLRF	INTF
			
			BTFSS	state_flag,isLearnP14
			GOTO	LRNZ_INPUT_2
	    	MOVLW	01010100b					;timera时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONA
			GOTO	LRNZ_INPUT_END_2
LRNZ_INPUT_2:
			MOVLW	00000100B
			MOVWF	TCCONA
LRNZ_INPUT_END_2:
			BSF		TCCONB,TCENB
			BSF     INTE,TMAIE
			BSF		INTE,GIE

			BTFSC	FLAG,isHighLow
			GOTO	$-1
			BTFSS	FLAG,isHighLow
			GOTO	$-1
			BTFSC	FLAG,isHighLow
			GOTO	$-1

			
			GOTO	PushNewSample
		
		

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LRNZ_Collection_Loop:

			BTFSC	FLAG,bLearnEnd
			GOTO	LRNZ_END
			BTFSS	FLAG,isHighLow
			GOTO	$-1
			BTFSC	FLAG,bLearnEnd
			GOTO	LRNZ_Compare_ST
			BTFSC	FLAG,isHighLow
			GOTO	$-1
			GOTO	LRNZ_Compare_ST
/*
			SETDP	02h
			MOVFW	p_PartIndexCount
			MOVWF	FRS1
			MOVFW	IND1
			MOVWF	nHighLevel_H
			INCF	p_PartIndexCount,F
			MOVFW	p_PartIndexCount
			MOVWF	FRS1
			MOVFW	IND1
			MOVWF	nHighLevel_L
			INCF	p_PartIndexCount,F
			MOVFW	p_PartIndexCount
			MOVWF	FRS1
			MOVFW	IND1
			MOVWF	nLowLevel_H
			INCF	p_PartIndexCount,F
			MOVFW	p_PartIndexCount
			MOVWF	FRS1
			MOVFW	IND1
			MOVWF	nLowLevel_L
			INCF	p_PartIndexCount,F
			MOVLW	54H
			SUBWF	p_PartIndexCount,W
			BTFSC	STATUS,C
			GOTO	LRNZ_END
			GOTO	LRNZ_Compare_ST
			*/
LRNZ_Compare:					;==比较采样值跟样本值的大小==============
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
			GOTO	LRNZ_Compare_Next
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
			GOTO	LRNZ_Compare_Next
;--------------------------------------------------
;PUSH SAMPLE INDEX TO DATA STACKS
;--------------------------------------------------
PUSH_DATA_INDEX:		
			BTFSC	FLAG,isHalf
			GOTO	PUSH_DATA_INDEX_LOW	
			MOVFW	p_Index
			MOVWF	FRS1
			SWAPF	Index_Bit,w
			MOVWF	IND1
			BSF		FLAG,isHalf
			INCFSZ	nIndexCount_l,F
			GOTO	$+2
			INCF	nIndexCount_H,F
		
			GOTO	LRNZ_Collection_Loop
PUSH_DATA_INDEX_LOW:
			BCF		FLAG,isHalf	
			MOVFW	p_Index
			MOVWF	FRS1
			MOVFW	Index_Bit
			IORWF	IND1,F
			INCF	p_Index,f
		
			MOVLW	248
			SUBWF	p_Index,W
			BTFSC	STATUS,C
			GOTO	LRNZ_DATA_OUT
			INCFSZ	nIndexCount_l,F
			GOTO	$+2
			INCF	nIndexCount_H,F
			GOTO	LRNZ_Collection_Loop
LRNZ_Compare_ST:
			CLRF	Index_Bit
			GOTO	LRNZ_Compare_ST1
LRNZ_Compare_Next:
			INCF	Index_Bit,F		
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LRNZ_Compare_ST1:
		;	MOVFW	total_sample
		;	SUBLW	0
		;	BTFSC	STATUS,Z
		;	GOTO	PushNewSample	
			MOVFW	Index_Bit
			SUBWF	total_sample,W
			BTFSC	STATUS,Z
			GOTO	PushNewSample		
			MOVFW	Index_Bit
			MOVWF	p_Sample	
			BCF		STATUS,C
			RLF		p_Sample,F
			BCF		STATUS,C
			RLF		p_Sample,W
			ADDLW	ADDRESS_PartIndexCount
			MOVWF	FRS0
			MOVFW	IND0
			MOVWF	Sample0_nHighLevel_H
			INCF	FRS0,f
			MOVFW	IND0
			MOVWF	Sample0_nHighLevel_L
			INCF	FRS0,f
			MOVFW	IND0
			MOVWF	Sample0_nLowLevel_H
			INCF	FRS0,f
			MOVFW	IND0
			MOVWF	Sample0_nLowLevel_L
		

			GOTO	LRNZ_Compare
	
			;GOTO	StoreNewSample
;-------------------------------------------------------
;PUSH PULSE TO SAMPLE STACKS
;------------------------------------------------------
PushNewSample:		
			
			MOVFW	total_sample	
			MOVWF	p_Sample	
			BCF		STATUS,C
			RLF		p_Sample,F
			BCF		STATUS,C
			RLF		p_Sample,W
			ADDLW	ADDRESS_PartIndexCount
			MOVWF	FRS0
			MOVFW	nHighLevel_H
			MOVWF	IND0
			MOVWF	Sample0_nHighLevel_H
			INCF	FRS0,f
			MOVFW	nHighLevel_L
			MOVWF	IND0
			MOVWF	Sample0_nHighLevel_L
			INCF	FRS0,f
			MOVFW	nLowLevel_H
			MOVWF	IND0
			MOVWF	Sample0_nLowLevel_H
			INCF	FRS0,f
			MOVFW	nLowLevel_L
			MOVWF	IND0
			MOVWF	Sample0_nLowLevel_L
			INCF	total_sample,F
			MOVLW	16
			SUBWF	total_sample,W
			BTFSC	STATUS,C
			GOTO	LRNZ_SAMPLE_OUT
			GOTO	PUSH_DATA_INDEX

LRNZ_DATA_OUT:
			BSF		_LEARN_FLAG,L_DATA
			GOTO	LRNZ_END	

LRNZ_SAMPLE_OUT:
		   	BSF		_LEARN_FLAG,L_SAMPLE
			GOTO	LRNZ_END		
LRNZ_END:

			BCF		INTE,GIE
			BCF     INTE,TMAIE
			BCF     INTE,CAPIE
			BCF		TCCONA,TCENA
			NOP
			RRF		FreqH,F
			RRF		FreqL,F


			;MOVFW	FRS1
			INCF	FRS1,W
			MOVWF	p_Index
			;INCF	p_Sample,F
			MOVWF	_LENGTH_l_learn
			;INCF	_LENGTH_l_learn,F
			MOVFW	total_sample
			MOVWF	p_Sample
			MOVWF	n_Sample
			
			BCF		STATUS,C
			RLF		p_Sample,F
			RLF		p_Sample,F
			
			SETDP	02h
			MOVLW	ADDRESS_PartIndexCount
			ADDWF   p_Sample,W

			MOVWF	FRS0
			MOVFW	n_Sample
			MOVWF	p_Sample
			MOVLW	ADDRESS_Sample
			MOVWF	FRS1
LRNZ_ZIPDATA_PUSH:
			
			MOVFW	IND1
			MOVWF	IND0
			INCF	FRS1,F
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	03h
			DECFSZ  p_Index,F
			GOTO	LRNZ_ZIPDATA_PUSH
			NOP	
			CLRF	_LENGTH_h_learn
			MOVFW	total_sample
			MOVWF	p_Sample	
			BCF		STATUS,C
			RLF		p_Sample,F
			RLF		p_Sample,W
			ADDWF	_LENGTH_l_learn,F
			BTFSS	STATUS,C
			GOTO	$+2
			INCF	_LENGTH_h_learn,F
		    
			MOVFW	_LENGTH_h_learn
			MOVWF	_CRC_LEN_H
			MOVFW	_LENGTH_L_learn
			MOVWF	_CRC_LEN_L
			SETDP	00h
			MOVLW	ADDRESS_PartIndexCount
			MOVWF	FRS0
			CALL	ET_xCal_crc
			MOVFW	_CRC_CODE
			MOVWF	n_Crc
			MOVLW	32H
			MOVWF	LEARN_TYPE
			_GREEN_CLR

			GOTO	Learn_OK	

LEARN_RMT_ZIP2_OUT:
		BSF		_LEARN_FLAG,L_OUT
		SETDP	00h
		GOTO	SleepMode


