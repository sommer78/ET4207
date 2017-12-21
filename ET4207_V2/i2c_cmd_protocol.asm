RomTab:
		ADDPCW
		RETLW	040h		;0
		RETLW	060h		;1
		RETLW	080h		;2
		RETLW	0a0h		;3
		RETLW	0c0h		;4			
		RETLW	0e0h		;5
		RETLW	000h		;6
		RETLW	020h		;7
		RETLW	040h		;8
		RETLW	060h		;9
		RETLW	080h		;10
		RETLW	0a0h		;11
		RETLW	0c0h		;12
		RETLW	0e0h		;13
		RETLW	000h		;3
		RETLW	000h
		RETLW	000h
		RETLW	000h
		
;=======================================================================
;	版本号初始化
;=======================================================================       

IniVersion:
		MOVLW	042H
		MOVWF	nVersion1
		MOVLW	007H
		MOVWF	nVersion2
		MOVLW	018H
		MOVWF	nVersion3
		MOVLW	001H
		MOVWF	nVersion4
		RETURN
;=======================================================================
;	版本号首地址给read程序
;=======================================================================       
ReadVersion:
	;	CALL	IniVersion
		MOVLW	nVersion1
		MOVWF	FRS0
		SETDP	01h
		MOVFW	IND0
		GOTO	I2cInterruptEnd 

;=======================================================================
;	read首地址给read程序
;=======================================================================   
ReadCode:
		GOTO	GetAddressIndex	
	;	MOVFW	IND0		
	;	GOTO	I2cInterruptEnd 

ReadEnd:
		GOTO	I2cInterruptEnd 


;=======================================================================
;	write index 
;=======================================================================   
WriteCode:
	;	INCF	ncount,f
	;	MOVFW	I2C_CMD
	;	ANDLW	0FH
	;	MOVWF	I2C_CMD
	;	CALL	RomTab
	;	MOVLW	40H
	;	MOVWF	FRS0
	/*	
		MOVLW	6
		SUBWF	I2C_CMD,W
		BTFSC	STATUS,C
		GOTO	WriteCode1
		SETDP	00h
		GOTO	I2cInterruptEnd 	
WriteCode1:
		SETDP	01h
;		MOVFW	IND0
*/
		GOTO	GetAddressIndex	
WriteEnd:
		BSF		FLAG,isCmdEnd
		SETDP	00h
		GOTO	I2cInterruptEnd	
		
StartRMTLearn:
		BSF		FLAG,isCmdEnd
		BCF		FLAG,bLearnEnd
		BCF		state_flag,isLearnP14
		MOVLW	40H
		MOVWF	_WRITE_CMD_DATA
		GOTO	I2cInterruptEnd		
		
StartRECLearn:
		BSF		FLAG,isCmdEnd
		BCF		FLAG,bLearnEnd
		BSF		state_flag,isLearnP14
	
		MOVLW	40H
		MOVWF	_WRITE_CMD_DATA
		GOTO	I2cInterruptEnd			
StopRMTLearn:
		BSF		FLAG,bLearnEnd
		GOTO	I2cInterruptEnd	
SetCurrent:
		MOVFW	I2C_CMD
		BTFSC	I2C_CMD,0
		GOTO	$+3
		BCF		RMTCTR,PWM_CURT0
		GOTO	$+2
		BSF		RMTCTR,PWM_CURT0
		BTFSC	I2C_CMD,1
		GOTO	$+3
		BCF		RMTCTR,PWM_CURT1
		GOTO	$+2
		BSF		RMTCTR,PWM_CURT1
		BTFSC	I2C_CMD,2
		GOTO	$+3
		BCF		RMTCTR,PWM_CURT
		GOTO	$+2
		BSF		RMTCTR,PWM_CURT

		GOTO	I2cInterruptEnd	

SetLearn:
		MOVFW	I2C_CMD
		BTFSC	I2C_CMD,0
		GOTO	$+3
		BCF		RMTCTR,SENS_CURT0
		GOTO	$+2
		BSF		RMTCTR,SENS_CURT0
		BTFSC	I2C_CMD,1
		GOTO	$+3
		BCF		RMTCTR,SENS_CURT1
		GOTO	$+2
		BSF		RMTCTR,SENS_CURT1


		GOTO	I2cInterruptEnd	

GetAddressIndex:
		MOVFW	I2C_CMD
		ANDLW	0FH
		MOVWF	I2C_CMD
		CALL	RomTab
		MOVWF	FRS0
		MOVLW	6
		SUBWF	I2C_CMD,W
		BTFSC	STATUS,C
		GOTO	GetAddressIndex1
		SETDP	00h	
		MOVFW	IND0
		GOTO	I2cInterruptEnd 
GetAddressIndex1:
		SETDP	01h	
		MOVFW	IND0			
		GOTO	I2cInterruptEnd 



			