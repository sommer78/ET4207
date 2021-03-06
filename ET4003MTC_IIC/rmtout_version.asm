


READ_VERSION:
		MOVLW	14
		MOVWF	80h
		MOVLW	3
		MOVWF	81h
		MOVLW	3
		MOVWF	82h
		MOVLW	1
		MOVWF	83h
;-----------------------------------
; 开始发送数据
;-----------------------------------
		_SET_SDA_OUTPUT_
VERSION_READ_RamInitial_ADDRESS:
		MOVLW	RamInitial_ADDRESS
		MOVWF	FRS0
VERSION_READ_RamInitial_ADDRESS_MSB:
		MOVFW	IND0
		MOVWF	buffer
		BTFSS	buffer,7
		GOTO	VERSION_SET_SDA_0_7
		_SET_SDA_OUTPUT_1_
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_7
VERSION_SET_SDA_0_7:
		_SET_SDA_OUTPUT_0_
VERSION_READ_SCK_RISING_EDGE_LOOP_7:
		BTFSS	PT1,SCK				;上升沿
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_7
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
VERSION_READ_SCK_FALLING_EDGE_LOOP_7:
		BTFSC	PT1,SCK				;下降沿
		GOTO	VERSION_READ_SCK_FALLING_EDGE_LOOP_7
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_

		BTFSS	buffer,6
		GOTO	VERSION_SET_SDA_0_6
		_SET_SDA_OUTPUT_1_
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_6
VERSION_SET_SDA_0_6:
		_SET_SDA_OUTPUT_0_
VERSION_READ_SCK_RISING_EDGE_LOOP_6:
		BTFSS	PT1,SCK				;上升沿
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_6
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
VERSION_READ_SCK_FALLING_EDGE_LOOP_6:
		BTFSC	PT1,SCK				;下降沿
		GOTO	VERSION_READ_SCK_FALLING_EDGE_LOOP_6
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_

		BTFSS	buffer,5
		GOTO	VERSION_SET_SDA_0_5
		_SET_SDA_OUTPUT_1_
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_5
VERSION_SET_SDA_0_5:
		_SET_SDA_OUTPUT_0_
VERSION_READ_SCK_RISING_EDGE_LOOP_5:
		BTFSS	PT1,SCK				;上升沿
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_5
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
VERSION_READ_SCK_FALLING_EDGE_LOOP_5:
		BTFSC	PT1,SCK				;下降沿
		GOTO	VERSION_READ_SCK_FALLING_EDGE_LOOP_5
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_

		BTFSS	buffer,4
		GOTO	VERSION_SET_SDA_0_4
		_SET_SDA_OUTPUT_1_
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_4
VERSION_SET_SDA_0_4:
		_SET_SDA_OUTPUT_0_
VERSION_READ_SCK_RISING_EDGE_LOOP_4:
		BTFSS	PT1,SCK				;上升沿
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_4
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
VERSION_READ_SCK_FALLING_EDGE_LOOP_4:
		BTFSC	PT1,SCK				;下降沿
		GOTO	VERSION_READ_SCK_FALLING_EDGE_LOOP_4
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_

		BTFSS	buffer,3
		GOTO	VERSION_SET_SDA_0_3
		_SET_SDA_OUTPUT_1_
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_3
VERSION_SET_SDA_0_3:
		_SET_SDA_OUTPUT_0_
VERSION_READ_SCK_RISING_EDGE_LOOP_3:
		BTFSS	PT1,SCK				;上升沿
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_3
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
VERSION_READ_SCK_FALLING_EDGE_LOOP_3:
		BTFSC	PT1,SCK				;下降沿
		GOTO	VERSION_READ_SCK_FALLING_EDGE_LOOP_3
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_

		BTFSS	buffer,2
		GOTO	VERSION_SET_SDA_0_2
		_SET_SDA_OUTPUT_1_
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_2
VERSION_SET_SDA_0_2:
		_SET_SDA_OUTPUT_0_
VERSION_READ_SCK_RISING_EDGE_LOOP_2:
		BTFSS	PT1,SCK				;上升沿
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_2
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
VERSION_READ_SCK_FALLING_EDGE_LOOP_2:
		BTFSC	PT1,SCK				;下降沿
		GOTO	VERSION_READ_SCK_FALLING_EDGE_LOOP_2
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_

		BTFSS	buffer,1
		GOTO	VERSION_SET_SDA_0_1
		_SET_SDA_OUTPUT_1_
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_1
VERSION_SET_SDA_0_1:
		_SET_SDA_OUTPUT_0_
VERSION_READ_SCK_RISING_EDGE_LOOP_1:
		BTFSS	PT1,SCK				;上升沿
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_1
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
VERSION_READ_SCK_FALLING_EDGE_LOOP_1:
		BTFSC	PT1,SCK				;下降沿
		GOTO	VERSION_READ_SCK_FALLING_EDGE_LOOP_1
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_

		BTFSS	buffer,0
		GOTO	VERSION_SET_SDA_0_0
		_SET_SDA_OUTPUT_1_
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_0
VERSION_SET_SDA_0_0:
		_SET_SDA_OUTPUT_0_
VERSION_READ_SCK_RISING_EDGE_LOOP_0:
		BTFSS	PT1,SCK				;上升沿
		GOTO	VERSION_READ_SCK_RISING_EDGE_LOOP_0
		_EN_InterruptEXT_			;上升沿之后打开外部中断，检测START信号
		_RST_TIMERA_
VERSION_READ_SCK_FALLING_EDGE_LOOP_0:
		BTFSC	PT1,SCK				;下降沿
		GOTO	VERSION_READ_SCK_FALLING_EDGE_LOOP_0
		_DIS_InterruptEXT_			;下降沿之后关闭外部中断
		_RST_TIMERA_

		_SET_SDA_INPUT_				;检查主机发过来的ACK
VERSION_READ_SCK_9TH_RISING_EDGE_LOOP:
		BTFSS	PT1,SCK				;上升沿
		GOTO	VERSION_READ_SCK_9TH_RISING_EDGE_LOOP
		_RST_TIMERA_
		BTFSC	PT1,SDA
		GOTO	ERROR
VERSION_READ_SCK_9TH_FALLING_EDGE_LOOP:
		BTFSC	PT1,SCK				;下降沿
		GOTO	VERSION_READ_SCK_9TH_FALLING_EDGE_LOOP
		_RST_TIMERA_
		_SET_SDA_OUTPUT_

		INCF	nWriteByteCount,F
		MOVFW	nWriteByteCount
		SUBLW	4
		BTFSC	STATUS,Z
		GOTO	VERSION_READ_RamInitial_ADDRESS_END
		INCF	FRS0,F
		GOTO	VERSION_READ_RamInitial_ADDRESS_MSB
VERSION_READ_RamInitial_ADDRESS_END:
		NOP
		NOP
		BCF		INTE,GIE
		NOP
		CALL	STOP_WAIT_FOR_STOP
		GOTO	SleepMode
