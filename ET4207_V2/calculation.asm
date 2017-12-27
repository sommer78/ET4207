






/*


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
		
			MOVFW	_LENGTH_h_temp
			SUBLW	00h
			BTFSS	STATUS,Z
			GOTO	xCal_crc_loop
			MOVFW	_LENGTH_l_temp
			SUBLW	00h
			BTFSS	STATUS,Z
			GOTO	xCal_crc_loop
			RETURN
*/
;--------------------
;;取绝对值
;-----------------------

;==============================================================
;crc CALCULATION
;INPUT FRS0
;RETURN _CRC_CODE
;==============================================================

ET_xCal_crc:
		CLRF	_CRC_CODE
ET_xCal_crc_loop:
		MOVFW	IND0
		XORWF	_CRC_CODE,f
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		MOVLW	8
		MOVWF	_CRC_COUNT
ET_xCal_crc_loop_loop_8times:
		MOVFW	_CRC_CODE
		ANDLW	01h
		SUBLW	00h
		BTFSC	STATUS,Z
		GOTO	ET_xCal_crc_loop_8times_reasult_0
		BCF	STATUS,C
		RRF	_CRC_CODE,w
		XORLW	8ch
		MOVWF	_CRC_CODE
		GOTO	ET_xCal_crc_loop_loop_8times_end
ET_xCal_crc_loop_8times_reasult_0:
		BCF		STATUS,C
		RRF		_CRC_CODE,f
ET_xCal_crc_loop_loop_8times_end:
		DECFSZ	_CRC_COUNT,f
		GOTO	ET_xCal_crc_loop_loop_8times
		MOVLW	01h
		SUBWF	_CRC_LEN_L,f
		MOVLW	00h
		SUBWFC	_CRC_LEN_H,f
		MOVFW	_CRC_LEN_H
		SUBLW	00h
		BTFSS	STATUS,Z
		GOTO	ET_xCal_crc_loop
		MOVFW	_CRC_LEN_L
		SUBLW	00h
		BTFSS	STATUS,Z
		GOTO	ET_xCal_crc_loop
		RETURN
;====================================================
;;取绝对值
;====================================================
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
