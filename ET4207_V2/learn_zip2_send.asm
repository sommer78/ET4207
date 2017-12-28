LEARN_ZIP2_SEND:
		MOVLW          0002H        
		SUBWFC         _LENGTH_h_learn,w 
		BTFSC	STATUS,c
		GOTO	Send_len_err
		MOVFW	_LENGTH_h_learn
		MOVWF	_CRC_LEN_H
		MOVFW	_LENGTH_l_learn
		MOVWF	_CRC_LEN_L
		SETDP	00h
		MOVLW	ADDRESS_PartIndexCount
		MOVWF	FRS0
		CALL	ET_xCal_crc
	;	CALL	xCal_crc
		MOVFW	_CRC_CODE
		SUBWF	n_Crc,w
		BTFSS	STATUS,Z
		GOTO	Send_crc_err


;learn  remote sender protol
LEARN_ZIP2_SEND_ST:
		_RED_SET
		BCF	FLAG,isHalf
		BSF		PWMCON,PWM_PORT_SEL_1
		BSF		PWMCON,TCOUTA_DIR			;TCOUTA_DIRд1��8λ�ݼ�����
		BCF		STATUS,C
		RRF		FreqL,F
		BCF		STATUS,C
		RRF		FreqL,W
		MOVWF	TSETAL						;�������Ͷ���1/2ռ�ձ�
		MOVFW	FreqL
		MOVWF	TSETAH
		BCF     RMTCTR,RX_EN				;RX_ENд0������ģ��رա�д1���򿪽���ģ��
		BSF     RMTCTR,TX_EN				;RX_EN=0,TX_EN=1 ����ģʽ
		BCF		PWMCON,PWM_SEL				;PWM_SELд0��PWM OUT MODE

		MOVLW	00000010b					;timerʱ��Դ,�ڲ�ʱ��Դmclk
		MOVWF	TCCONA                      
		BCF     TCCONA, TCRSTA              ;TCRSTAд0 TIMEA��λ
		BSF		TCCONA,TCENA                ;TCENAд1  TIMEA��ʼ����

		CLRF	INTF

		BCF		INTE,TMAIE
		BCF		INTE,GIE

		MOVFW	FreqL
		SUBLW	137
		MOVWF	DELAYCT1
	;	MOVLW	9
	;	ADDWF	DELAYCT1,f

		MOVFW	n_Index
		MOVWF	_nPairs_l_temp
		BCF		state_flag,isSendEnd
		;	MOVLW	01h
		;	SUBWF	_nPairs_l_temp,f
		;	MOVLW	00h
		;	SUBWFC	_nPairs_h_temp,f
		INCF	nIndexCount_H,F		
		SETDP	00h
		MOVFW	total_sample	
		MOVWF	p_Sample	
		BCF		STATUS,C
		RLF		p_Sample,F
		RLF		p_Sample,W
		ADDLW	ADDRESS_PartIndexCount
		MOVWF	FRS0
LEARN_ZIP2_SEND_Loop:
		BTFSC	FLAG,isHalf
		GOTO	LEARN_ZIP2_LOWHALF	
		MOVFW	IND0
		MOVWF	Index_Bit
		SWAPF	Index_Bit,w
		ANDLW	0fh
		MOVWF	Index_Bit
		BSF		FLAG,isHalf
		GOTO	LEARN_ZIP2_HIGHLOW_END
LEARN_ZIP2_LOWHALF:
		MOVFW	IND0
		ANDLW	0fh
		MOVWF	Index_Bit
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		BCF		FLAG,isHalf
LEARN_ZIP2_HIGHLOW_END:
		BCF		STATUS,C
		RLF		Index_Bit,f
		BCF		STATUS,C
		RLF		Index_Bit,W
		ADDLW	ADDRESS_PartIndexCount
		MOVWF	FRS1
		MOVFW	IND1
		MOVWF	_High_h
		INCF	FRS1,f
		MOVFW	IND1
		MOVWF	_High_l
		INCF	FRS1,f
		MOVFW	IND1
		MOVWF	_Low_h
		INCF	FRS1,f
		MOVFW	IND1
		MOVWF	_Low_l
		INCF	_High_h,f
		INCF	_Low_h,f
		
	
		
;-----------send loop--------------------------- 	;�費��Ҫ�жϸ��ֽں͵��ֽ��Ƿ�ȫΪ0?			
		BSF		PWMCON,PWMEN
		CALL	Timer_Delay_Base
		DECFSZ	_High_l,f
		GOTO	$-2
		DECFSZ	_High_h,f
		GOTO	$-4
			
		BCF		PWMCON,PWMEN
		;	CALL	Timer_Delay_Low
		DECFSZ	_Low_l,f
		GOTO	$-2
		DECFSZ	_Low_h,f
		GOTO	$-4
;-----------send loop end--------------------------- 
		
		DECFSZ	nIndexCount_l,F
		GOTO	LEARN_ZIP2_SEND_Loop
		DECFSZ	nIndexCount_H,F
		GOTO	LEARN_ZIP2_SEND_Loop


	
			
	
		

		
LEARN_ZIP2_SEND_end:
		_RED_CLR
		NOP
		RETURN