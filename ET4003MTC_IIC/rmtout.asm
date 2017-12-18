	
RMTOUT_LEARN:
			BCF		TCCONA,TCENA
			BSF		PWMCON,TCOUTA_DIR			;TCOUTA_DIRд1��8λ�ݼ�����
			BCF     RMTCTR,RX_EN				;RX_ENд0������ģ��رա�д1���򿪽���ģ��
			BSF     RMTCTR,TX_EN				;RX_EN=0,TX_EN=1 ����ģʽ
			BCF		PWMCON,PWM_SEL				;PWM_SELд0��PWM OUT MODE
	    	MOVLW	00000111b					;timerʱ��Դ,�ڲ�ʱ��Դmclk(111-5MHz)
			MOVWF	TCCONA
			MOVFW	FreqH
			MOVWF	TSETAH
			MOVFW	FreqL
			MOVWF	TSETAL
			BSF		INTE,TMAIE
			BCF     INTE,CAPIE
			BCF		INTE,GIE
			CLRF	INTF

			SETDP	00h
			MOVLW	RAM_address
			MOVWF	FRS0
			MOVFW	IND0
			MOVWF	nTemp_H
			INCF	FRS0,F
			MOVFW	IND0
			MOVWF	nTemp_L
			INCF	FRS0,F

			CLRF	Count

			BCF     TCCONA,TCRSTA              ;TCRSTAд0 TIMEA��λ
			BSF		TCCONA,TCENA


			/*
			MOVLW	41h
			MOVWF	TSETAH
			MOVLW	20h
			MOVWF	TSETAL
			NOP
			NOP
			NOP
			BSF		PWMCON,PWMEN
			CALL	DELAY80ms
			BCF		PWMCON,PWMEN
			GOTO	$
			*/



	RmtoutLearnLoopHigh:
			BSF		PWMCON,PWMEN
			BTFSS	INTF,TMAIF
			GOTO	$-1
			CLRF	INTF
			MOVLW	1
			SUBWF	nTemp_L,F
			MOVLW	0
			SUBWFC	nTemp_H,F
			BTFSC	STATUS,C
			GOTO	RmtoutLearnLoopHigh
			INCF	Count,f
			MOVFW	Count
			SUBWF	Count_total,W
			BTFSC	STATUS,Z
			GOTO	RmtoutLearnEnd
			MOVFW	IND0
			MOVWF	nTemp_H
			INCF	FRS0,F
			MOVFW	IND0
			MOVWF	nTemp_L
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			;BCF		PT1,SCL
			BCF     TCCONA,TCRSTA
	RmtoutLearnLoopLow:
			BCF		PWMCON,PWMEN
			BTFSS	INTF,TMAIF
			GOTO	$-1
			CLRF	INTF
			MOVLW	1
			SUBWF	nTemp_L,F
			MOVLW	0
			SUBWFC	nTemp_H,F
			BTFSC	STATUS,C
			GOTO	RmtoutLearnLoopLow
			INCF	Count,f
			MOVFW	Count
			SUBWF	Count_total,W
			BTFSC	STATUS,Z
			GOTO	RmtoutLearnEnd
			MOVFW	IND0
			MOVWF	nTemp_H
			INCF	FRS0,F
			MOVFW	IND0
			MOVWF	nTemp_L
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	01h
			;BSF		PT1,SCL
			BCF     TCCONA,TCRSTA
			GOTO	RmtoutLearnLoopHigh


	RmtoutLearnEnd:
			BCF		PWMCON,PWMEN
		    RETURN




