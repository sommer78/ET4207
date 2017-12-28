
CONSUMERIR_START:
		MOVLW          0002H        
		SUBWFC         _LENGTH_h,w 
		BTFSC	STATUS,c
		GOTO	Send_len_err
		MOVFW	_LENGTH_h
		MOVWF	_CRC_LEN_H
		MOVFW	_LENGTH_l
		MOVWF	_CRC_LEN_L
		MOVLW	10
		SUBWF	_CRC_LEN_L,f
		MOVLW	0
		SUBWFC	_CRC_LEN_H,f
		SETDP	00h
		MOVLW	POINTER0
		MOVWF	FRS0
		CALL	ET_xCal_crc
	;	CALL	xCal_crc
		MOVFW	_CRC_CODE
		SUBWF	_CRC,w
		BTFSS	STATUS,Z
		GOTO	Send_crc_err
		GOTO	CONSUMERIR_REMOTE
TIANJIA_START:
		MOVLW          0002H        
		SUBWFC         _LENGTH_h,w 
		BTFSC	STATUS,c
		GOTO	Send_len_err
		MOVFW	_LENGTH_h
		MOVWF	_CRC_LEN_H
		MOVFW	_LENGTH_l
		MOVWF	_CRC_LEN_L
		MOVLW	10
		SUBWF	_CRC_LEN_L,f
		MOVLW	0
		SUBWFC	_CRC_LEN_H,f
		SETDP	00h
		MOVLW	POINTER0
		MOVWF	FRS0
		CALL	ET_xCal_crc
		MOVFW	_CRC_CODE
		SUBWF	_CRC,w
		BTFSS	STATUS,Z
		GOTO	Send_crc_err
TIANJIA000:
      CLRWDT 
	  MOVLW          003FH        
      ADDWF          _LENGTH_l,W   
	  MOVWF			nWriteByteCount_l  
	  MOVLW			0000H
	  ADDWFC		_LENGTH_h,F 
	  MOVWF			nWriteByteCount_h
	  BTFSC   		nWriteByteCount_h,0
	  GOTO			$+3
	  SETDP			00H
	  GOTO			$+2
	  SETDP			01H
	  MOVFW			nWriteByteCount_l
	  MOVWF			FRS0
	  MOVFW			IND0
	  MOVWF			tianjiacode_1

      MOVLW          0001H        
      SUBWF          _LENGTH_l,F     
      MOVLW          0000H        
      SUBWFC         _LENGTH_h,F        
      MOVFW          tianjiacode_1        
      MOVWF          tianjiacode_2        
      BTFSS          tianjiacode_2,7      
      GOTO               TIANJIA001    
      MOVFW          tianjiacode_2        
      XORLW          00D5H        
      MOVWF          tianjiacode_2  
TIANJIA001:      
      BCF            STATUS,C      
      RLF            tianjiacode_2,F      
      BTFSS          tianjiacode_1,7      
      GOTO                TIANJIA002   
      MOVFW          tianjiacode_2        
      IORLW          0001H        
      MOVWF          tianjiacode_2 
TIANJIA002:       
      MOVFW          _LENGTH_l        
      MOVWF          tianjia_temp1        
      MOVFW          _LENGTH_h        
      MOVWF          tianjia_temp2        
      MOVLW          000AH        
      SUBWF          tianjia_temp1,F      
      MOVLW          0000H        
      SUBWFC         tianjia_temp2,F      
      CLRF           nWriteByteCount_l        
      CLRF           nWriteByteCount_h        
      SETDP          0000H        
      MOVLW          004AH        
      MOVWF          FRS0   
TIANJIA005:
      MOVFW          nWriteByteCount_l        
      XORWF          tianjiacode_2,F      
      MOVFW          tianjiacode_2        
      MOVWF          tianjia_temp5        
      BCF            STATUS,C      
      RRF            tianjia_temp5,F      
      MOVFW          tianjiacode_2        
      BTFSS          tianjiacode_2,0      
      GOTO                TIANJIA003   
      MOVFW          tianjia_temp5        
      XORLW          00D5H        
      MOVWF          tianjia_temp5  
TIANJIA003:      
      MOVFW          IND0        
      XORWF          tianjia_temp5,W      
      MOVWF          IND0        
      INCF           tianjiacode_2,F      
      INCFSZ         FRS0,F      
      GOTO               TIANJIA004    
      SETDP          0001H   
TIANJIA004:    
      MOVLW          0001H        
      ADDWF          nWriteByteCount_l,F      
      MOVLW          0000H        
      ADDWFC         nWriteByteCount_h,F      
      MOVFW          tianjia_temp2        
      SUBWF          nWriteByteCount_h,W      
      BTFSS          STATUS,Z      
      GOTO           TIANJIA005    
      MOVFW          tianjia_temp1        
      SUBWF          nWriteByteCount_l,W      
      BTFSS          STATUS,Z      
      GOTO              TIANJIA005     
      CLRWDT   
TIANJIAEND:   


;CONSUMERIR remote sender CONSUMERIR protol
CONSUMERIR_REMOTE:
			CLRF		_FORM_h
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
			


Send_len_err:
			BSF	_FORM_h,0
			GOTO	SleepMode

Send_crc_err:
			_BLUE_SET
			BSF	_FORM_h,1
			_BLUE_CLR
			GOTO	SleepMode









