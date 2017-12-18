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
;	版本号首地址给read程序
;=======================================================================       
ReadVersion:
		MOVLW	025h
		MOVWF	FRS0
		SETDP	00h
;		MOVFW	IND0
		GOTO	I2cInterruptEnd 

;=======================================================================
;	read首地址给read程序
;=======================================================================   
ReadCode:

		CALL	GetAddressIndex	
		MOVFW	IND0		
		GOTO	I2cInterruptEnd 
ReadEnd:
	;	BSF		flag1,isCmdEnd
	;	NOP

;		MOVLW	40H
;		MOVWF	FRS0
;		MOVFW	IND0
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
		NOP
		GOTO	I2cInterruptEnd	
		
StartRMTLearn:
		BSF		flag1,isCmdEnd
		BCF		flag1,bLearnEnd
	
		MOVLW	40H
		MOVWF	_WRITE_CMD_DATA
		GOTO	I2cInterruptEnd		
		
StartRECLearn:
		BSF		flag1,isCmdEnd
		BCF		flag1,bLearnEnd
	
		MOVLW	45H
		MOVWF	_WRITE_CMD_DATA
		GOTO	I2cInterruptEnd			
StopRMTLearn:
		BSF		flag1,bLearnEnd

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
			