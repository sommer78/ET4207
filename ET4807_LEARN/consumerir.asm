


ANY_RAM_CLEAR:
		MOVWF	FRS0
		CLRF	IND0
		INCFSZ	FRS0,f
		GOTO	$-2
		SETDP	01h
		MOVLW	00h
		MOVWF	FRS0
		CLRF	IND0
        INCFSZ	FRS0,f
        GOTO	$-2
		SETDP	00h
		RETURN


;CONSUMERIR remote sender CONSUMERIR protol
CONSUMERIR_REMOTE:
			BSF		PWMCON,PWM_PORT_SEL_1
			BSF		PWMCON,TCOUTA_DIR			;TCOUTA_DIR写1是8位递减计数
			BCF		STATUS,C
			RRF		_FREQ,w
			MOVWF	TSETAL						;所有码型都是1/2占空比
			MOVFW	_FREQ
			MOVWF	TSETAH
			BCF     RMTCTR,RX_EN				;RX_EN写0，接收模块关闭。写1，打开接收模块
			BSF     RMTCTR,TX_EN				;RX_EN=0,TX_EN=1 发射模式
			BCF		PWMCON,PWM_SEL				;PWM_SEL写0，PWM OUT MODE

			MOVLW	00000010b					;timer时钟源,内部时钟源mclk
			MOVWF	TCCONA                      
			BCF     TCCONA, TCRSTA              ;TCRSTA写0 TIMEA复位
			BSF		TCCONA,TCENA                ;TCENA写1  TIMEA开始工作

			CLRF	INTF

			BCF		INTE,TMAIE
			BCF		INTE,GIE

			MOVFW	_FREQ
			SUBLW	128
			MOVWF	DELAYCT1
			MOVLW	9
			ADDWF	DELAYCT1,f

			MOVFW	_nPairs_h
			MOVWF	_nPairs_h_temp
			MOVFW	_nPairs_l
			MOVWF	_nPairs_l_temp

			MOVLW	01h
			SUBWF	_nPairs_l_temp,f
			MOVLW	00h
			SUBWFC	_nPairs_h_temp,f
			
			SETDP	00h
			MOVLW	_index
			ADDLW	POINTER0
			MOVWF	FRS0
	CONSUMERIR_DecodeLoop:
			MOVFW	IND0
			MOVWF	_temp1
			RRF		_temp1,f
			RRF		_temp1,f
			RRF		_temp1,f
			RRF		_temp1,w
			;RRF	_temp1,f
			;MOVFW	_temp1
			ANDLW	0fh
			MOVWF	_Index_1
			;SUBLW	00h
			;BTFSC	STATUS,Z
			;GOTO	CONSUMERIR_Decode_end
			;MOVFW	_Index_1
			BCF		STATUS,C
			RLF		_Index_1,f
			BCF		STATUS,C
			RLF		_Index_1,f
			MOVFW	_Index_1
			ADDLW	POINTER1
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
			;需不需要判断高字节和低字节是否全为0?
			;BCF     TCCONA, TCRSTA
			BSF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_High_l,f
			GOTO	$-2
			;MOVFW	_High_h
			;SUBLW	00h
			;BTFSC	STATUS,Z
			;GOTO	$+3
			DECFSZ	_High_h,f
			GOTO	$-4
			;BCF     TCCONA, TCRSTA
			BCF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_Low_l,f
			GOTO	$-2
			;MOVFW	_Low_h
			;SUBLW	00h
			;BTFSC	STATUS,Z
			;GOTO	$+3
			DECFSZ	_Low_h,f
			GOTO	$-4

			MOVLW	01h
			SUBWF	_nPairs_l_temp,f
			MOVLW	00h
			SUBWFC	_nPairs_h_temp,f
			BTFSS	STATUS,C
			GOTO	CONSUMERIR_Decode_end

			MOVFW	IND0
			ANDLW	0fh
			MOVWF	_Index_1
			;SUBLW	00h				;还需要判断是否到了最大长度
			;BTFSC	STATUS,Z
			;GOTO	CONSUMERIR_Decode_end
			;CALL	Remote_Sender
			;MOVFW	_Index_1
			BCF		STATUS,C
			RLF		_Index_1,f
			BCF		STATUS,C
			RLF		_Index_1,f
			MOVFW	_Index_1
			ADDLW	POINTER1
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
			;需不需要判断高字节和低字节是否全为0?
			;BCF     TCCONA, TCRSTA
			BSF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_High_l,f
			GOTO	$-2
			;MOVFW	_High_h
			;SUBLW	00h
			;BTFSC	STATUS,Z
			;GOTO	$+3
			DECFSZ	_High_h,f
			GOTO	$-4
			;BCF     TCCONA, TCRSTA
			BCF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_Low_l,f
			GOTO	$-2
			;MOVFW	_Low_h
			;SUBLW	00h
			;BTFSC	STATUS,Z
			;GOTO	$+3
			DECFSZ	_Low_h,f
			GOTO	$-4

			MOVLW	01h
			SUBWF	_nPairs_l_temp,f
			MOVLW	00h
			SUBWFC	_nPairs_h_temp,f
			BTFSS	STATUS,C
			GOTO	CONSUMERIR_Decode_end

			INCFSZ	FRS0,F
			GOTO	CONSUMERIR_DecodeLoop
			SETDP	01h
			GOTO	CONSUMERIR_DecodeLoop
CONSUMERIR_Decode_end:
			RETURN
			




;Base time including carrier
Timer_Delay_Base:
			MOVFW	DELAYCT1
			ADDPCW
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
			RETURN
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
			RETURN


xCal_crc:
			MOVFW	_LENGTH_h
			MOVWF	_LENGTH_h_temp
			MOVFW	_LENGTH_l
			MOVWF	_LENGTH_l_temp
			MOVLW	10
			SUBWF	_LENGTH_l_temp,f
			MOVLW	0
			SUBWFC	_LENGTH_h_temp,f
			CLRF	_crc_temp
			SETDP	00h
			MOVLW	POINTER0
			MOVWF	FRS0
xCal_crc_loop:
			MOVFW	IND0
			XORWF	_crc_temp,f
			;INCF	FRS0,f
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			MOVLW	8
			MOVWF	_temp1
xCal_crc_loop_loop_8times:
			MOVFW	_crc_temp
			ANDLW	01h
			SUBLW	00h
			BTFSC	STATUS,Z
			GOTO	xCal_crc_loop_8times_reasult_0
			BCF		STATUS,C
			RRF		_crc_temp,w
			XORLW	8ch
			MOVWF	_crc_temp
			GOTO	xCal_crc_loop_loop_8times_end
xCal_crc_loop_8times_reasult_0:
			BCF		STATUS,C
			RRF		_crc_temp,f
xCal_crc_loop_loop_8times_end:
			DECFSZ	_temp1,f
			GOTO	xCal_crc_loop_loop_8times
			MOVLW	01h
			SUBWF	_LENGTH_l_temp,f
			MOVLW	00h
			SUBWFC	_LENGTH_h_temp,f
			/*
			MOVLW	118
			SUBWF	_LENGTH_l_temp,w
			MOVLW	0
			SUBWFC	_LENGTH_h_temp,w
			BTFSC	STATUS,C
			SETDP	01h
			*/
			MOVFW	_LENGTH_h_temp
			SUBLW	00h
			BTFSS	STATUS,Z
			GOTO	xCal_crc_loop
			MOVFW	_LENGTH_l_temp
			SUBLW	00h
			BTFSS	STATUS,Z
			GOTO	xCal_crc_loop
			RETURN


			
	








