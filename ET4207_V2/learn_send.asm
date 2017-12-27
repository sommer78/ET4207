
LEARN_SEND:
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
LEARN_SEND_ST:
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
			SUBLW	128
			MOVWF	DELAYCT1
			MOVLW	9
			ADDWF	DELAYCT1,f

			MOVFW	n_Index
			MOVWF	_nPairs_l_temp
			BcF		state_flag,isSendEnd
		;	MOVLW	01h
		;	SUBWF	_nPairs_l_temp,f
		;	MOVLW	00h
		;	SUBWFC	_nPairs_h_temp,f
			
			SETDP	00h
			MOVLW	ADDRESS_PartIndexCount
			MOVWF	FRS0
LEARN_SEND_Loop:
			MOVFW	IND0
			
			MOVWF	_High_h
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			MOVFW	IND0
			MOVWF	_High_l
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			MOVFW	IND0
			MOVWF	_Low_h
		;	SUBLW	0FFH
		;	BTFSS	STATUS,Z
		;	GOTO	$+2
		;	MOVLW	10H
		;	MOVWF	_Low_h	
		;	BSF		state_flag,isSendEnd
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			MOVFW	IND0
			MOVWF	_Low_l
			

			INCF	_High_h,f
		;	BCF		STATUS,C
		;	RRF		_Low_h,F
		;	RRF		_Low_l,F
			INCF	_Low_h,f
			;�費��Ҫ�жϸ��ֽں͵��ֽ��Ƿ�ȫΪ0?
			;BCF     TCCONA, TCRSTA
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

			BTFSC	state_flag,isSendEnd
			GOTO	LEARN_SEND_end
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			
			DECFSZ	_nPairs_l_temp,F
		
			GOTO	LEARN_SEND_Loop
LEARN_SEND_end:
			NOP
			RETURN