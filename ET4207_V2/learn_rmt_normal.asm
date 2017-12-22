;==========================================
;采集输入rmt时间信号
;========================================== 
LEARN_RMT_NORMAL:
			
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
			MOVWF	new_nHighLevel_L
			

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
	        BCF		PWMCON,TCOUTA_DIR			;TIMER UP 16bit型
LRNN_INPUT_SELECT:
			BTFSS	state_flag,isLearnP14
			GOTO	LRNN_INPUT_1
	    	MOVLW	01110110b					;	INPUT RMT timera时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONA
			GOTO	LRNN_INPUT_END_1
LRNN_INPUT_1:
			MOVLW	00100110B				; INPUT PT14
			MOVWF	TCCONA
LRNN_INPUT_END_1:
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
LEARNN_CAP_WAIT:
			_WDT_DIS

			BTFSC	FLAG,bLearnEnd
			GOTO	Learn_OUT
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
			BSF		TCCONB,TCRSTB

			BTFSC	INTF,TMBIF
			GOTO	LEARNN_CAP_WAIT
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
			GOTO	LRNN_INPUT_2
	    	MOVLW	01010100b					;timera时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONA
			GOTO	LRNN_INPUT_END_2
LRNN_INPUT_2:
			MOVLW	00000100B
			MOVWF	TCCONA
LRNN_INPUT_END_2:
			BSF		TCCONB,TCENB
			BSF     INTE,TMAIE
			BSF		INTE,GIE

			BTFSC	FLAG,isHighLow
			GOTO	$-1
			BTFSS	FLAG,isHighLow
			GOTO	$-1
			BTFSC	FLAG,isHighLow
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


SampleNCompare_loop:
			BTFSC	FLAG,bLearnEnd
			GOTO	Learn_OUT
			BTFSS	FLAG,isHighLow
			GOTO	$-3
			BTFSC	FLAG,bLearnEnd
			GOTO	Learn_OUT
			BTFSC	FLAG,isHighLow
			GOTO	$-3
SampleNCompare_0:
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
			GOTO	SampleNCompare_1
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
			GOTO	SampleNCompare_1
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
			GOTO	SampleNCompare_loop
			DECF	nPartIndexCount,f
			GOTO	StoreNewSample
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SampleNCompare_1:
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
			GOTO	SampleNCompare_loop
			DECF	nPartIndexCount,f
	
;---------------------------------------------------
StoreNNewSample:
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
			GOTO	SampleNCompare_loop


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
;      remoter learn restone         ;
;************************************************   				

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
	


		
			CLRF	_LENGTH_h_learn
			CLRF	_LENGTH_l_learn

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
			GOTO	Learn_OK	
