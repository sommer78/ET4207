;==========================================
;采集输入rmt时间信号
;========================================== 
LEARN_RMT_NORMAL:
			
			CLRF	_LEARN_FLAG
			SETDP	00h
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
			MOVWF	FRS0
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
		;	BCF	TCCONA,1			;改变采样率
			BTFSS	state_flag,isLearnP14
			GOTO	LRNn_INPUT_2
	    	MOVLW	01010100b					;timera时钟源,内部时钟源mclk(111-5MHz)
			MOVWF	TCCONA
			GOTO	LRNn_INPUT_END_2
LRNn_INPUT_2:
			MOVLW	00000100B
			MOVWF	TCCONA
LRNn_INPUT_END_2:	
			BSF		TCCONB,TCENB
			BSF     INTE,TMAIE
			BSF		INTE,GIE

			BTFSC	FLAG,isHighLow
			GOTO	$-1
			BTFSS	FLAG,isHighLow
			GOTO	$-1
			BTFSC	FLAG,isHighLow
			GOTO	$-1

			
	
	
	    	MOVFW	nHighLevel_H
			MOVWF	IND0
			INCF	_LENGTH_L_learn,F
			INCF	FRS0,F
	  	
			MOVFW	nHighLevel_L
			MOVWF	IND0
			INCF	_LENGTH_L_learn,F
			INCF	FRS0,F
		
			MOVFW	nLowLevel_H
			MOVWF	IND0
	    	INCF	_LENGTH_L_learn,F
			INCF	FRS0,F
			
			MOVFW	nLowLevel_L
			MOVWF	IND0
			INCF	_LENGTH_L_learn,F
			INCF	FRS0,F
		
			INCF	nIndexCount_l,f

NWAIT_loop:
			BTFSC	FLAG,bLearnEnd
			GOTO	NLearn_end
			BTFSS	FLAG,isHighLow
			GOTO	$-1
			BTFSC	FLAG,bLearnEnd
			GOTO	N_PUSH
			BTFSC	FLAG,isHighLow
			GOTO	$-1
N_PUSH:
		
			MOVFW	nHighLevel_H
			MOVWF	IND0
			INCF	_LENGTH_L_learn,F
			INCFSZ	FRS0,F
	  		GOTO	$+3
			SETDP	01h
			INCF	_LENGTH_H_learn,F
			MOVFW	nHighLevel_L
			MOVWF	IND0
			INCF	_LENGTH_L_learn,F
			INCFSZ	FRS0,F
			GOTO	$+3
			SETDP	01h
			INCF	_LENGTH_H_learn,F
			MOVFW	nLowLevel_H
			MOVWF	IND0
	    	INCF	_LENGTH_L_learn,F
			INCFSZ	FRS0,F
			GOTO	$+3
			SETDP	01h
			INCF	_LENGTH_H_learn,F
			MOVFW	nLowLevel_L
			MOVWF	IND0
			INCF	_LENGTH_L_learn,F
			INCFSZ	FRS0,F
			GOTO	$+3
			SETDP	01h
			INCF	_LENGTH_H_learn,F

			INCF	nIndexCount_l,f
			MOVLW	106
			SUBWF	nIndexCount_l,W
			BTFSC	STATUS,C
			GOTO	NLearn_len_end
		
			GOTO	NWAIT_loop
NLearn_len_end:
		   	BSF		_LEARN_FLAG,L_LEN	
NLearn_end:
		
			BCF		INTE,GIE
			BCF     INTE,TMAIE
			BCF     INTE,CAPIE
			BCF		TCCONA,TCENA
			NOP
			RRF		FreqH,F
			RRF		FreqL,F


;************************************************
;      normal remoter learn restone         ;
;************************************************   				

			
			MOVFW	nIndexCount_l
			MOVWF	n_index

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
			MOVLW	30H
			MOVWF	LEARN_TYPE
			GOTO	Learn_OK	
