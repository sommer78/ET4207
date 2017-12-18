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
;	�汾�ų�ʼ��
;=======================================================================       

IniVersion:
		MOVLW	02AH
		MOVWF	nVersion1
		MOVLW	007H
		MOVWF	nVersion2
		MOVLW	000H
		MOVWF	nVersion3
		MOVLW	001H
		MOVWF	nVersion4
		RETURN
;=======================================================================
;	�汾���׵�ַ��read����
;=======================================================================       
ReadVersion:
		CALL	IniVersion
		MOVLW	025h
		MOVWF	FRS0
		SETDP	00h
;		MOVFW	IND0
		GOTO	I2cInterruptEnd 

;=======================================================================
;	read�׵�ַ��read����
;=======================================================================   
ReadCode:
		CALL	GetAddressIndex	
		MOVFW	IND0		
		GOTO	I2cInterruptEnd 
ReadEnd:

		GOTO	I2cInterruptEnd 


;=======================================================================
;	write index 
;=======================================================================   
WriteCode:
	;	INCF	ncount,f
	;	MOVFW	TEMP
	;	ANDLW	0FH
	;	MOVWF	TEMP
	;	CALL	RomTab
	;	MOVLW	40H
	;	MOVWF	FRS0
	/*	
		MOVLW	6
		SUBWF	TEMP,W
		BTFSC	STATUS,C
		GOTO	WriteCode1
		SETDP	00h
		GOTO	I2cInterruptEnd 	
WriteCode1:
		SETDP	01h
;		MOVFW	IND0
*/
		CALL	GetAddressIndex	
		GOTO	I2cInterruptEnd 
		
		
		
WriteEnd:
		BSF		flag1,isCmdEnd
		SETDP	00h
		GOTO	I2cInterruptEnd	
		
StartRMTLearn:
		BSF		flag1,isCmdEnd
		BCF		flag1,bLearnEnd
		BCF		flag,isLearnRec
		MOVLW	40H
		MOVWF	_WRITE_CMD_DATA
		GOTO	I2cInterruptEnd		
		
StartRECLearn:
		BSF		flag1,isCmdEnd
		BCF		flag1,bLearnEnd
		BSF		flag,isLearnRec
	
		MOVLW	40H
		MOVWF	_WRITE_CMD_DATA
		GOTO	I2cInterruptEnd			
StopRMTLearn:
		BSF		flag1,bLearnEnd
		GOTO	I2cInterruptEnd	
SetCurrent:
		MOVFW	TEMP
		BTFSC	TEMP,0
		GOTO	$+3
		BCF		RMTCTR,PWM_CURT0
		GOTO	$+2
		BSF		RMTCTR,PWM_CURT0
		BTFSC	TEMP,1
		GOTO	$+3
		BCF		RMTCTR,PWM_CURT1
		GOTO	$+2
		BSF		RMTCTR,PWM_CURT1
		BTFSC	TEMP,2
		GOTO	$+3
		BCF		RMTCTR,PWM_CURT
		GOTO	$+2
		BSF		RMTCTR,PWM_CURT

		GOTO	I2cInterruptEnd	

SetLearn:
		MOVFW	TEMP
		BTFSC	TEMP,0
		GOTO	$+3
		BCF		RMTCTR,SENS_CURT0
		GOTO	$+2
		BSF		RMTCTR,SENS_CURT0
		BTFSC	TEMP,1
		GOTO	$+3
		BCF		RMTCTR,SENS_CURT1
		GOTO	$+2
		BSF		RMTCTR,SENS_CURT1


		GOTO	I2cInterruptEnd	
GetAddressIndex:
		MOVFW	TEMP
		ANDLW	0FH
		MOVWF	TEMP
		CALL	RomTab
		MOVWF	FRS0
		MOVLW	6
		SUBWF	TEMP,W
		BTFSC	STATUS,C
		GOTO	GetAddressIndex1
		SETDP	00h	
		GOTO	GetAddressIndex2 	
GetAddressIndex1:
		SETDP	01h			
GetAddressIndex2:		
		RETURN



			