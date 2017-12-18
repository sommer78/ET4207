;_command		equ		7fh
_register		equ		80h
_pwm_curt		equ		81h




RegChange:
			MOVFW	_register
			ANDLW	0f0h
			SUBLW	00h
			BTFSS	STATUS,Z
			GOTO	RegChangeEnd
			MOVFW	_register
			ANDLW	0fh
			ADDPCW
			GOTO	PwmCurt				;0x00
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd


	PwmCurt:
			MOVLW	11011001b
			ANDWF	RMTCTR,f
			MOVFW	_pwm_curt
			ANDLW	0f0h
			SUBLW	00h
			BTFSS	STATUS,Z
			GOTO	RegChangeEnd
			MOVFW	_pwm_curt
			ANDLW	0fh
			ADDPCW
			GOTO	C_000_150mA
			GOTO	C_001_200mA
			GOTO	C_010_250mA
			GOTO	C_011_300mA
			GOTO	C_100_350mA
			GOTO	C_101_400mA
			GOTO	C_110_450mA
			GOTO	C_111_500mA
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd
			GOTO	RegChangeEnd



C_000_150mA:
			MOVLW	00000000b
			IORWF	RMTCTR,f
			GOTO	RegChangeEnd

C_001_200mA:
			MOVLW	00000010b
			IORWF	RMTCTR,f
			GOTO	RegChangeEnd

C_010_250mA:
			MOVLW	00000100b
			IORWF	RMTCTR,f
			GOTO	RegChangeEnd

C_011_300mA:
			MOVLW	00000110b
			IORWF	RMTCTR,f
			GOTO	RegChangeEnd

C_100_350mA:
			MOVLW	00100000b
			IORWF	RMTCTR,f
			GOTO	RegChangeEnd

C_101_400mA:
			MOVLW	00100010b
			IORWF	RMTCTR,f
			GOTO	RegChangeEnd

C_110_450mA:
			MOVLW	00100100b
			IORWF	RMTCTR,f
			GOTO	RegChangeEnd

C_111_500mA:
			MOVLW	00100110b
			IORWF	RMTCTR,f
			GOTO	RegChangeEnd


RegChangeEnd:
			CLRF	_command
			CLRF	_register
			CLRF	_pwm_curt
			GOTO	SleepMode







