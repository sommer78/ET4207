
;==========================================
;RECEIVER LEARN SUBROUTINE
;OUTPUT 
;========================================== 

RECEIVER_START:
		SETDP	02h
		CLRF	state_flag
		CLRF	FLAG
		BCF		PT1EN,REC
		BSF	FLAG,isRecMode
REC_SET_REG:
		MOVLW	ADDRESS_PartIndexCount
	;	MOVWF	p_PartIndexCount
	;	MOVLW	ADDRESS_Sample
	;	MOVWF	FRS0
		MOVWF	p_Sample
		;MOVLW	ADDRESS_Index
	;	MOVLW	ADDRESS_Sample
		MOVLW	00H
		MOVWF	p_Index
		CLRF	nIndexCount_h
		CLRF	nIndexCount_l
		MOVLW	0E8H
		MOVWF	p_PartIndexCount
	;	GOTO	REC_WAIT_Sub

		BCF		state_flag,isLearnEnd
	   	MOVLW	00000011b					;timera时钟源,内部时钟源mclk(111-5MHz)
		MOVWF	TCCONA
		CLRF	TSETAL
		CLRF	TSETAH
		CLRF	nIndexCount_l
		CLRF	nIndexCount_H
		BCF	INTE,TMAIE
		BSF	INTE,I2CIF
		BSF	INTE,GIE
	   	BCF	PWMCON,TCOUTA_DIR			;TIMER UP 16bit型
		_GREEN_SET
		_ET4207_BUSY_
		
REC_WAIT_FIRST_DOWN_EDGE:
		BTFSC	state_flag,isLearnEnd
		GOTO	REC_LEARN_OUT
		_WDT_DIS			;	_WDT_EN  sommer
		BTFSC	PT1,REC
		GOTO	REC_WAIT_FIRST_DOWN_EDGE
		_WDT_EN			;	_WDT_EN  sommer
		_GREEN_CLR

		MOVLW	00000110b					;timerb时钟源,内部时钟源mclk(111-5MHz)
		MOVWF	TCCONB
		BSF		TCCONB,TCENB
		BCF		TCCONB,TCRSTB
		MOVLW	78H
		MOVWF	TSETB
		BCF		TCCONA,TCRSTA
		BSF		TCCONA,TCENA
		BSF		INTE,TMAIE
		BSF		INTE,TMBIE
		BSF		INTE,GIE
REC_WAIT_Sub:
		CLRWDT
	;	BTFSC	FLAG,bLearnEnd	
	;	GOTO	REC_LEARN_END
		BTFSS	FLAG,isPulseEnd
		GOTO	REC_WAIT_Sub
		BCF		FLAG,isPulseEnd
		/*
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
*/
	
	;	BTFSC	state_flag,stopPush
	;	GOTO	RECL_Compare_ST
		SETDP	02h
		MOVFW	p_PartIndexCount
		MOVWF	FRS1
		MOVFW	nHighLevel_H
		MOVWF	IND1

		INCF	p_PartIndexCount,F
	;	MOVLW	0E8H
	;	SUBWF	p_PartIndexCount,W

		MOVFW	p_PartIndexCount
		MOVWF	FRS1
		MOVFW	nHighLevel_L
		MOVWF	IND1
		INCF	p_PartIndexCount,F
		MOVFW	p_PartIndexCount
		MOVWF	FRS1
		MOVFW	nLowLevel_H
		MOVWF	IND1
		INCF	p_PartIndexCount,F
		MOVFW	p_PartIndexCount
		MOVWF	FRS1
		MOVFW	nLowLevel_L
		MOVWF	IND1
		INCF	p_PartIndexCount,F
		
		MOVLW	0E8H
		MOVWF	p_PartIndexCount

	;	MOVLW	0E8H
	;	SUBWF	p_PartIndexCount,W
;		BTFSC	STATUS,C
	;	BSF		state_flag,stopPush
	;	GOTO	RECL_DATA_OUT

		GOTO	RECL_Compare_ST



RECL_Compare_ST:
		CLRF	Index_Bit
		GOTO	RECL_Compare_ST1
RECL_Compare_Next:
		INCF	Index_Bit,F		
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RECL_Compare_ST1:
		MOVFW	total_sample
		SUBLW	0
		BTFSC	STATUS,Z
		GOTO	RECL_PushNewSample	
		MOVFW	Index_Bit
		SUBWF	total_sample,W
		BTFSC	STATUS,Z
		GOTO	RECL_PushNewSample	
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
		
RECL_Compare:					;==比较采样值跟样本值的大小==============
		MOVFW	Sample0_nHighLevel_L
		SUBWF	nHighLevel_L,w
		MOVWF	DELTA_ABS_L
		MOVFW	Sample0_nHighLevel_H
		SUBWFC	nHighLevel_H,w
		MOVWF	DELTA_ABS_H
		CALL	Neg
		MOVLW	_DELTA_REC_High_L_
		SUBWF	DELTA_ABS_L,w
		MOVLW	_DELTA_REC_High_H_
		SUBWFC	DELTA_ABS_H,w
		BTFSC	status,c
		GOTO	RECL_Compare_Next
		MOVFW	Sample0_nLowLevel_L
		SUBWF	nLowLevel_L,w
		MOVWF	DELTA_ABS_L
		MOVFW	Sample0_nLowLevel_H
		SUBWFC	nLowLevel_H,w
		MOVWF	DELTA_ABS_H
		CALL	Neg
		MOVLW	_DELTA_REC_Low_L_
		SUBWF	DELTA_ABS_L,w
		MOVLW	_DELTA_REC_Low_H_
		SUBWFC	DELTA_ABS_H,w
		BTFSC	status,c
		GOTO	RECL_Compare_Next
;--------------------------------------------------
;PUSH SAMPLE INDEX TO DATA STACKS
;--------------------------------------------------
RECL_PUSH_DATA_INDEX:
		SETDP	02H		
		BTFSC	FLAG,isHalf
		GOTO	RECL_PUSH_DATA_INDEX_LOW	
		MOVFW	p_Index
		MOVWF	FRS1
		SWAPF	Index_Bit,w
		MOVWF	IND1
		BSF		FLAG,isHalf
		INCFSZ	nIndexCount_l,F
		GOTO	$+2
		INCF	nIndexCount_H,F
		BTFSC	FLAG,bLearnEnd	
		GOTO	RECL_END
		GOTO	REC_WAIT_Sub
RECL_PUSH_DATA_INDEX_LOW:
		BCF		FLAG,isHalf	
		MOVFW	p_Index
		MOVWF	FRS1
		MOVFW	Index_Bit
		IORWF	IND1,F
		INCF	p_Index,f
		
	;	MOVLW	248
	;	SUBWF	p_Index,W
	;	BTFSC	STATUS,C
	;	GOTO	RECL_DATA_OUT

		INCFSZ	nIndexCount_l,F
		GOTO	$+2
		INCF	nIndexCount_H,F
		BTFSC	FLAG,bLearnEnd	
		GOTO	RECL_END
		GOTO	REC_WAIT_Sub

	
			;GOTO	StoreNewSample
;-------------------------------------------------------
;PUSH PULSE TO SAMPLE STACKS
;------------------------------------------------------
RECL_PushNewSample:		
			
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
	
		INCF	FRS0,f
		MOVFW	nHighLevel_L
		MOVWF	IND0
	
		INCF	FRS0,f
		MOVFW	nLowLevel_H
		MOVWF	IND0
	
		INCF	FRS0,f
		MOVFW	nLowLevel_L
		MOVWF	IND0
	
		INCF	total_sample,F
		MOVLW	16
		SUBWF	total_sample,W
		BTFSC	STATUS,C
		GOTO	RECL_SAMPLE_OUT
		GOTO	RECL_PUSH_DATA_INDEX

RECL_DATA_OUT:
		BSF		_LEARN_FLAG,L_DATA
		GOTO	RECL_END	

RECL_SAMPLE_OUT:
	   	BSF		_LEARN_FLAG,L_SAMPLE
		GOTO	RECL_END		
RECL_END:

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
RECL_ZIPDATA_PUSH:
			
		MOVFW	IND1
		MOVWF	IND0
		INCF	FRS1,F
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	03h
		DECFSZ  p_Index,F
		GOTO	RECL_ZIPDATA_PUSH
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
		MOVLW	41H
		MOVWF	LEARN_TYPE
		_GREEN_CLR

		GOTO	Learn_OK	
REC_LEARN_OUT:
		BSF		_LEARN_FLAG,L_OUT
		SETDP	00h
		GOTO	SleepMode

		
REC_LEARN_DATA_OUT:
		BSF		_LEARN_FLAG,L_OUT
		MOVFW	nHighLevel_H
		MOVWF	IND0
		INCF	FRS0,F
		MOVFW	nHighLevel_L
		MOVWF	IND0
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		MOVFW	nLowLevel_H
		MOVWF	IND0
		INCF	FRS0,F
		MOVFW	nLowLevel_L
		MOVWF	IND0
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		INCF	nIndexCount_l,F
		SETDP	00h
		GOTO	SleepMode



;==========================================
;RECEIVER LEARN SUBROUTINE
;OUTPUT 
;========================================== 

RECEIVER_START_NOZIP:
		SETDP	02h
		CLRF	state_flag
		CLRF	FLAG
		BCF		PT1EN,REC
		BSF		FLAG,isRecMode
REC_NZ_SET_REG:
		MOVLW	ADDRESS_PartIndexCount
	;	MOVWF	p_PartIndexCount
	;	MOVLW	ADDRESS_Sample
		MOVWF	FRS0
		MOVWF	p_Sample

		CLRF	nIndexCount_h
		CLRF	nIndexCount_l

		BCF		state_flag,isLearnEnd
	   	MOVLW	00000011b					;timera时钟源,内部时钟源mclk(111-5MHz)
		MOVWF	TCCONA
		CLRF	TSETAL
		CLRF	TSETAH
		CLRF	nIndexCount_l
		CLRF	nIndexCount_H
		BCF	INTE,TMAIE
		BSF	INTE,I2CIF
		BSF	INTE,GIE
	   	BCF	PWMCON,TCOUTA_DIR			;TIMER UP 16bit型
		_GREEN_SET
		_ET4207_BUSY_
		
REC_NZ_WAIT_FIRST_DOWN_EDGE:
		BTFSC	state_flag,isLearnEnd
		GOTO	REC_LEARN_OUT
		_WDT_DIS			;	_WDT_EN  sommer
		BTFSC	PT1,REC
		GOTO	REC_NZ_WAIT_FIRST_DOWN_EDGE
		_WDT_DIS			;	_WDT_EN  sommer
		_GREEN_CLR

		MOVLW	00000110b					;timerb时钟源,内部时钟源mclk(111-5MHz)
		MOVWF	TCCONB
		BSF		TCCONB,TCENB
		BCF		TCCONB,TCRSTB
		MOVLW	78H
		MOVWF	TSETB
		BCF		TCCONA,TCRSTA
		BSF		TCCONA,TCENA
		BSF		INTE,TMAIE
		BSF		INTE,TMBIE
		BSF		INTE,GIE
REC_NZ_WAIT_Sub:
		CLRWDT
		BTFSC	FLAG,bLearnEnd	
		GOTO	REC_NZ_LEARN_END
		BTFSS	FLAG,isPulseEnd
		GOTO	REC_NZ_WAIT_Sub
		BCF		FLAG,isPulseEnd

		MOVFW	nHighLevel_H
		MOVWF	IND0
		INCF	FRS0,F
		MOVFW	nHighLevel_L
		MOVWF	IND0
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		MOVFW	nLowLevel_H
		MOVWF	IND0
		INCF	FRS0,F
		MOVFW	nLowLevel_L
		MOVWF	IND0
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		INCF	nIndexCount_l,F
		MOVLW	106
		SUBWF	nIndexCount_l,W
		BTFSC	STATUS,C
		GOTO	RECL_NZ_DATA_OUT
		GOTO	REC_NZ_WAIT_Sub



		


RECL_NZ_DATA_OUT:
		BSF		_LEARN_FLAG,L_DATA
		GOTO	REC_NZ_LEARN_END	
	
REC_NZ_LEARN_END:

		BCF		INTE,GIE
		BCF     INTE,TMAIE
		BCF		INTE,TMBIE
		BCF		TCCONA,TCENA
		NOP
		MOVFW	nIndexCount_l
		MOVWF	_LENGTH_L_learn
		BCF		STATUS,C
		RLF		_LENGTH_L_learn,F
		RLF		_LENGTH_h_learn,F
		RLF		_LENGTH_L_learn,F
		RLF		_LENGTH_h_learn,F

	
	    
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
		MOVLW	40H
		MOVWF	LEARN_TYPE
		_GREEN_CLR

		GOTO	Learn_OK	


		


