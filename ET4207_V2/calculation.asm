
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
;;È¡¾ø¶ÔÖµ
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
