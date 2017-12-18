
DELAY150ms:
	CALL	NOP50000
	CALL	NOP50000
	CALL	NOP50000
	RETURN
;=================================
;  delay100ms sub program
;  time : 100*125*10*T(inst)
;  (100*1ms, it's about 100ms)
;=================================
DELAY100ms:
	CALL	NOP50000
	CALL	NOP50000
	CALL	NOP20000
	CALL	NOP5000
	RETURN



;=======================================================
;ÑÓÊ±º¯Êý
;=======================================================
NOP50000:       CALL    NOP10000
NOP40000:       CALL    NOP10000
NOP30000:       CALL    NOP10000
NOP20000:       CALL    NOP10000
NOP10000:		CALL	NOP500
NOP9500:		CALL	NOP500
NOP9000:		CALL	NOP500
NOP8500:		CALL	NOP500
NOP8000:		CALL	NOP500
NOP7500:		CALL	NOP500
NOP7000:		CALL	NOP500
NOP6500:		CALL	NOP500
NOP6000:		CALL	NOP500
NOP5500:		CALL	NOP500
NOP5000:		CALL	NOP500
NOP4500:		CALL	NOP500
NOP4000:		CALL	NOP500
NOP3500:		CALL	NOP500
NOP3000:		CALL	NOP500
NOP2500:		CALL	NOP500
NOP2000:		CALL	NOP500
NOP1500:		CALL	NOP500
NOP1000:		CALL	NOP500
				CALL	NOP450
	    		GOTO	NOP48

NOP650:			CALL	NOP50		;2
NOP600:			CALL	NOP50		;3
NOP550:	    	CALL	NOP50		;4
NOP500:	    	CALL	NOP50		;5
NOP450:	    	CALL	NOP50		;6
NOP400:	    	CALL	NOP50		;7
NOP350:	    	CALL	NOP50		;8
NOP300:	    	CALL	NOP50		;9
NOP250:	    	CALL	NOP50		;10
NOP200:	    	CALL	NOP50		;11
NOP150:	    	CALL	NOP50		;12
NOP100:	    	CALL	NOP50		;13
				GOTO	NOP48		;14

NOP51:	    	NOP			;15
NOP50:	    	NOP			;16
NOP49:	    	NOP			;17
NOP48:	    	NOP			;18




NOP47:	    	NOP			;19
NOP46:	    	NOP			;20

NOP44:	    	NOP			;22
NOP43:	    	NOP			;23
NOP42:	    	NOP			;24
NOP41:	    	NOP			;25
NOP40:	    	NOP			;26
NOP39:	    	NOP			;27
NOP38:	    	NOP			;28
NOP37:	    	NOP			;29
NOP36:	    	NOP			;30
NOP35:	    	NOP			;31
NOP34:	    	NOP			;32
NOP33:	    	NOP			;33
NOP32:	    	NOP			;34
NOP31:	    	NOP			;35
NOP30:	    	NOP			;36
NOP29:	    	NOP			;37
NOP28:	    	NOP			;38
NOP27:	    	NOP			;39
NOP26:	    	NOP			;40
NOP25:	    	NOP			;41
NOP24:	    	NOP			;42
NOP23:	    	NOP			;43
NOP22:	    	NOP			;44
NOP21:	    	NOP			;45
NOP20:	    	NOP			;46
NOP19:	    	NOP			;47
NOP18:	    	NOP			;48
NOP17:	   		NOP			;49
NOP16:	    	NOP			;50
NOP15:	    	NOP			;51
NOP14:	    	NOP			;52
NOP13:	    	NOP			;53
NOP12:	    	NOP			;54
NOP11:	    	NOP			;55
NOP10:	    	NOP			;56
NOP9:	    	NOP			;57
NOP8:	    	NOP			;58
NOP7:	    	NOP			;59
NOP6:	    	NOP			;60
NOP5:	    	NOP			;61
NOP4:	    	NOP			;62
;NOP3:	    	NOP
;NOP2:	    	NOP
;NOP1:	    	NOP
;NOP0:	    	NOP
;				NOP
				
				RETURN		;63
;=======================================================