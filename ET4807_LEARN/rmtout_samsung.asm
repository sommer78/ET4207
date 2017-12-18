;_CODE			equ		80h
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





;new interface
SEND_KITKAT_IR:
		CLRF	flag
		SETDP	00h
		MOVLW	80h
		MOVWF	FRS0
RMTOUT_KITKAT_1_DATA:
;-----------------------------------
;
;-----------------------------------
KITKAT_1_SCK_RISING_EDGE_LOOP:
		BTFSS	PT1,SCK				;上升沿
		GOTO	KITKAT_1_SCK_RISING_EDGE_LOOP
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
		BTFSS	PT1,SDA
		GOTO	KITKAT_1_READ_SDA_0
KITKAT_1_READ_SDA_1:
		BSF		STATUS,C
		GOTO	KITKAT_1_WIRTE_TO_BUFFER
KITKAT_1_READ_SDA_0:
		BCF		STATUS,C
KITKAT_1_WIRTE_TO_BUFFER:
		RLF		buffer,F
KITKAT_1_SCK_FALLING_EDGE_LOOP:
		BTFSC	PT1,SCK				;下降沿
		GOTO	KITKAT_1_SCK_FALLING_EDGE_LOOP
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_
		INCF	nWriteBitCount,F
		BTFSS	nWriteBitCount,3	;判断是否到了8次
		GOTO	KITKAT_1_SCK_RISING_EDGE_LOOP
;;;将buffer的值送入RAM指定区域
		MOVFW	buffer
		MOVWF	IND0
		INCF	FRS0,F
		CLRF	buffer
		CLRF	nWriteBitCount
		_SET_SDA_OUTPUT_
		_SET_SDA_OUTPUT_0_
KITKAT_1_SCK_9TH_RISING_EDGE_LOOP:
		BTFSS	PT1,SCK				;上升沿
		GOTO	KITKAT_1_SCK_9TH_RISING_EDGE_LOOP
		_RST_TIMERA_
KITKAT_1_SCK_9TH_FALLING_EDGE_LOOP:
		BTFSC	PT1,SCK				;下降沿
		GOTO	KITKAT_1_SCK_9TH_FALLING_EDGE_LOOP
		_RST_TIMERA_
		_SET_SDA_INPUT_
		INCF	nWriteByteCount,F
		MOVFW	nWriteByteCount
		SUBLW	2
		BTFSS	STATUS,Z
		GOTO	KITKAT_1_SCK_RISING_EDGE_LOOP
		NOP

		/*
		MOVFW	_CODE
		SUBLW	80h
		BTFSS	STATUS,Z
		GOTO	SEND_KITKAT_IR_LENGTH_END_FAIL
		*/

		MOVLW	03h
		ADDWF	nWriteByteCount_l,f
		MOVLW	00h
		ADDWFC	nWriteByteCount_h,f
;-----------------------------------
;
;-----------------------------------
KITKAT_SCK_RISING_EDGE_LOOP:
		BTFSS	PT1,SCK				;上升沿
		GOTO	KITKAT_SCK_RISING_EDGE_LOOP
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
		BTFSS	PT1,SDA
		GOTO	KITKAT_READ_SDA_0
KITKAT_READ_SDA_1:
		BSF		STATUS,C
		GOTO	KITKAT_WIRTE_TO_BUFFER
KITKAT_READ_SDA_0:
		BCF		STATUS,C
KITKAT_WIRTE_TO_BUFFER:
		RLF		buffer,F
KITKAT_SCK_FALLING_EDGE_LOOP:
		BTFSC	PT1,SCK				;下降沿
		GOTO	KITKAT_SCK_FALLING_EDGE_LOOP
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_
		INCF	nWriteBitCount,F
		BTFSS	nWriteBitCount,3	;判断是否到了8次
		GOTO	KITKAT_SCK_RISING_EDGE_LOOP
;;;将buffer的值送入RAM指定区域
		MOVFW	buffer
		MOVWF	IND0
		;INCF	FRS0,F
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		CLRF	buffer
		CLRF	nWriteBitCount
		_SET_SDA_OUTPUT_
		_SET_SDA_OUTPUT_0_
KITKAT_SCK_9TH_RISING_EDGE_LOOP:
		BTFSS	PT1,SCK				;上升沿
		GOTO	KITKAT_SCK_9TH_RISING_EDGE_LOOP
		_RST_TIMERA_
KITKAT_SCK_9TH_FALLING_EDGE_LOOP:
		BTFSC	PT1,SCK				;下降沿
		GOTO	KITKAT_SCK_9TH_FALLING_EDGE_LOOP
		_RST_TIMERA_
		_SET_SDA_INPUT_
		MOVLW	01h
		ADDWF	nWriteByteCount_l,f
		MOVLW	00h
		ADDWFC	nWriteByteCount_h,f
		/*
		MOVLW	128
		SUBWF	nWriteByteCount_l,w
		MOVLW	0
		SUBWFC	nWriteByteCount_h,w
		BTFSC	STATUS,C
		SETDP	01h
		*/
		MOVFW	_LENGTH_h
		SUBWF	nWriteByteCount_h,w
		BTFSS	STATUS,Z
		GOTO	KITKAT_SCK_RISING_EDGE_LOOP
		MOVFW	_LENGTH_l
		SUBWF	nWriteByteCount_l,w
		BTFSS	STATUS,Z
		GOTO	KITKAT_SCK_RISING_EDGE_LOOP
		GOTO	SEND_KITKAT_IR_LENGTH_END
		/*
		BTFSC	flag,bSETDP
		GOTO	KITKAT_WRITE_NEXT_PAGE
		INCF	nWriteByteCount_l,f
		MOVFW	nWriteByteCount_l
		SUBLW	128
		BTFSC	STATUS,Z
		BSF		flag,bSETDP
		MOVFW	_LENGTH_h
		SUBWF	nWriteByteCount_h,w
		BTFSS	STATUS,Z
		GOTO	KITKAT_SCK_RISING_EDGE_LOOP
		MOVFW	_LENGTH_l
		SUBWF	nWriteByteCount_l,w
		BTFSS	STATUS,Z
		GOTO	KITKAT_SCK_RISING_EDGE_LOOP
		GOTO	SEND_KITKAT_IR_LENGTH_END
	KITKAT_WRITE_NEXT_PAGE:
		SETDP	01h
		INCFSZ	nWriteByteCount_l,f
		GOTO	$+2
		INCF	nWriteByteCount_h,f
		MOVFW	nWriteByteCount_l
		SUBLW	80h
		BTFSS	STATUS,Z
		GOTO	KITKAT_WRITE_NEXT_PAGE_COMPARE
		MOVFW	nWriteByteCount_h
		SUBLW	01h
		BTFSC	STATUS,Z
		GOTO	SEND_KITKAT_IR_LENGTH_END_FAIL
	KITKAT_WRITE_NEXT_PAGE_COMPARE:
		MOVFW	_LENGTH_h
		SUBWF	nWriteByteCount_h,w
		BTFSS	STATUS,Z
		GOTO	KITKAT_SCK_RISING_EDGE_LOOP
		MOVFW	_LENGTH_l
		SUBWF	nWriteByteCount_l,w
		BTFSS	STATUS,Z
		GOTO	KITKAT_SCK_RISING_EDGE_LOOP
		GOTO	SEND_KITKAT_IR_LENGTH_END
		*/


SEND_KITKAT_IR_LENGTH_END:
		NOP
		BCF		INTE,GIE
		NOP
		CALL	STOP_WAIT_FOR_STOP

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
		/*
		SETDP	00h
		MOVLW	70h
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
		*/
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
			;GOTO	SAMSUNG_Decode_end
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
			GOTO	SAMSUNG_Decode_end

			MOVFW	IND0
			ANDLW	0fh
			MOVWF	_Index_1
			;SUBLW	00h				;还需要判断是否到了最大长度
			;BTFSC	STATUS,Z
			;GOTO	SAMSUNG_Decode_end
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
/*
;Base time including carrier
Timer_Delay_Base:
			MOVFW		DELAYCT1
			MOVWF		DELAYCT2
			NOP
			DECFSZ		DELAYCT2,f
			GOTO		$-1
			RETURN
*/
/*
;Base time including carrier
Timer_Delay_Base:
			MOVFW		_FREQ
			MOVWF		DELAYCT1
Timer_Delay_Base0:
			DECFSZ		DELAYCT1,f
			GOTO		Timer_Delay_Base0	
			RETURN
*/


/*
;Base time including carrier
Remote_Sender:
			MOVFW	_Index_1
			RLF		_Index_1,f
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
			;需不需要判断高字节和低字节是否全为0?
			;BCF     TCCONA, TCRSTA
			BSF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_High_l,f
			GOTO	$-2
			MOVFW	_High_h
			SUBLW	00h
			BTFSC	STATUS,Z
			GOTO	$+3
			DECFSZ	_High_h,f
			GOTO	$-8
			;BCF     TCCONA, TCRSTA
			BCF		PWMCON,PWMEN
			CALL	Timer_Delay_Base
			DECFSZ	_Low_l,f
			GOTO	$-2
			MOVFW	_Low_h
			SUBLW	00h
			BTFSC	STATUS,Z
			RETURN
			DECFSZ	_Low_h,f
			GOTO	$-8
			RETURN
*/


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


			
	








