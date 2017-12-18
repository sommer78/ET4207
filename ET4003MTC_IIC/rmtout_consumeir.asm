_command		equ		7fh
_LENGTH_h		equ		80h
_LENGTH_l		equ		81h
_FREQ			equ		82h
_nPairs_h		equ		83h
_nPairs_l		equ		84h
_FORM_h			equ		85h
_FORM_l			equ		86h
_REPEAT			equ		87h
_CRC			equ		88h
POINTER0		equ		89h
POINTER1		equ		89h
_High_h			equ		70h
_High_l			equ		71h
_Low_h			equ		72h
_Low_l			equ		73h
_temp1			equ     74h
_Index_1		equ		75h
_crc_temp		equ		76h
_LENGTH_h_temp	equ		77h
_LENGTH_l_temp	equ		78h
_nPairs_h_temp	equ		79h
_nPairs_l_temp	equ		7ah

_index			equ		64

nWriteByteCount_h		equ		34h
nWriteByteCount_l		equ		35h





SEND_KITKAT_IR:


		MOVFW	RMTCTR
		ANDLW	00100110b
		MOVWF	temp
		MOVLW	00000000b
		SUBWF	temp,w
		BTFSC	status,z
		GOTO	TEST_000_150mA
		MOVLW	00000010b
		SUBWF	temp,w
		BTFSC	status,z
		GOTO	TEST_001_200mA
		MOVLW	00000100b
		SUBWF	temp,w
		BTFSC	status,z
		GOTO	TEST_010_250mA
		MOVLW	00000110b
		SUBWF	temp,w
		BTFSC	status,z
		GOTO	TEST_011_300mA
		MOVLW	00100000b
		SUBWF	temp,w
		BTFSC	status,z
		GOTO	TEST_100_350mA
		MOVLW	00100010b
		SUBWF	temp,w
		BTFSC	status,z
		GOTO	TEST_101_400mA
		MOVLW	00100100b
		SUBWF	temp,w
		BTFSC	status,z
		GOTO	TEST_110_450mA
		MOVLW	00100110b
		SUBWF	temp,w
		BTFSC	status,z
		GOTO	TEST_111_500mA
		GOTO	SEND_KITKAT_IR_LENGTH_END_FAIL



	TEST_000_150mA:
			;DECF	_nPairs_l,f
			GOTO	SEND_KITKAT_IR_LENGTH_END

	TEST_001_200mA:
				DECF	_nPairs_l,f
				GOTO	SEND_KITKAT_IR_LENGTH_END
	
	TEST_010_250mA:
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				GOTO	SEND_KITKAT_IR_LENGTH_END
	
	TEST_011_300mA:
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				GOTO	SEND_KITKAT_IR_LENGTH_END
	
	TEST_100_350mA:
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				GOTO	SEND_KITKAT_IR_LENGTH_END
	
	TEST_101_400mA:
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				GOTO	SEND_KITKAT_IR_LENGTH_END
	
	TEST_110_450mA:
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				GOTO	SEND_KITKAT_IR_LENGTH_END
	
	TEST_111_500mA:
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				DECF	_nPairs_l,f
				GOTO	SEND_KITKAT_IR_LENGTH_END

















SEND_KITKAT_IR_LENGTH_END:
		BTFSS	flag,spc
		GOTO	$+3
		BCF		flag,spc
		GOTO	$+2
		BSF		flag,spc

		CALL	xCal_crc
		MOVFW	_crc_temp
		SUBWF	_CRC,w
		BTFSS	STATUS,Z
		GOTO	SEND_KITKAT_IR_LENGTH_END_FAIL

		CALL	SAMSUNG_REMOTE

	SEND_KITKAT_IR_LENGTH_END_FAIL:
		CLRF	nWriteByteCount_h
		CLRF	nWriteByteCount_l
		SETDP	00h
		MOVLW	70h
		CALL	ANY_RAM_CLEAR
		GOTO	SleepMode

;通信出错时怎么会跳转到学习部分?


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
			


;samsung remote sender samsung protol
SAMSUNG_REMOTE:
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
	SAMSUNG_DecodeLoop:
			SWAPF	IND0,W
			;MOVWF	_temp1
			ANDLW	0fh
			MOVWF	_Index_1
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
			BSF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_High_l,f
			GOTO	$-2
			DECFSZ	_High_h,f
			GOTO	$-4
			BCF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_Low_l,f
			GOTO	$-2
			DECFSZ	_Low_h,f
			GOTO	$-4

			MOVLW	01h
			SUBWF	_nPairs_l_temp,f
			MOVLW	00h
			SUBWFC	_nPairs_h_temp,f
			BTFSS	STATUS,C
			GOTO	SAMSUNG_Decode_end

			MOVFW	IND0
			ANDLW	0fh
			MOVWF	_Index_1
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
			BSF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_High_l,f
			GOTO	$-2
			DECFSZ	_High_h,f
			GOTO	$-4
			BCF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_Low_l,f
			GOTO	$-2
			DECFSZ	_Low_h,f
			GOTO	$-4

			MOVLW	01h
			SUBWF	_nPairs_l_temp,f
			MOVLW	00h
			SUBWFC	_nPairs_h_temp,f
			BTFSS	STATUS,C
			GOTO	SAMSUNG_Decode_end

			INCFSZ	FRS0,F
			GOTO	SAMSUNG_DecodeLoop
			SETDP	01h
			GOTO	SAMSUNG_DecodeLoop
SAMSUNG_Decode_end:
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


			
	








