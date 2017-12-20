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

;--------------------
;;È¡¾ø¶ÔÖµ
;-----------------------
ABS:  
       COMF   TMPL,F
       COMF   TMPH,F
       MOVLW  1
	   ADDWF TMPL,F
	   MOVLW 0
	   ADDWFC TMPH,F
	   RETURN


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




