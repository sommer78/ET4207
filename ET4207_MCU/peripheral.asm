;===================================
;
;===================================


;//////////////////////////////////////////////////////////////////
;初始化中断
;//////////////////////////////////////////////////////////////////         
Initintp: BSF	TCCONB,TCENB             ;
		  BCF	TCCONB,CCLKB_SEL0		 ;
		  BCF	TCCONB,CCLKB_SEL1
		  BCF	TCCONB,CCLKB_SEL2
		  CLRF	INTE
		  CLRF	INTF
          BSF	INTE,GIE                   ;总中断使能
		  RETURN   
;////////////////////////////////////////////////////////////////// 																												;
SysIni:
		;MOVWF		MCK 									;MCK=5M, 指令周期 MCK/4，timer时钟源MCK/4（4分频）
															;Instruction Cycle 110-MCK/4 010-mck/12.5
		MOVLW		00000010b					;timer时钟源
		MOVWF		TCCONA							;000:tcc(PT1.1)
															;001:~tcc
															;010:timeclk_clk/4
															;011:timeclk_clk/8
															;100:timeclk_clk/16
															;101:timeclk_clk/32
															;110:timeclk_clk/64
															;111:timeclk_clk/128
															;

    
															;---------------
															; 00 = PT20 is triggered at negative edge
	
		CLRF		PWMCON									;{0,0,0,PWEN,0,PWCS[2:0]}
															;PDCS	PDM Pulse Width
															;000	1/MCK	*
															;001	2/MCK
															;010	4/MCK
															;011	8/MCK
															;100	16/MCK
															;101	32/MCK
															;110	64/MCK
															;111	128/MCK
															;---------------------------------
		CLRF		INTE
		CLRF		INTF
;----------------------------------------------------
		BCF			INTE,0								;enable PT10 interruptreturn

		BCF			TCCONA,7	;
		MOVLW	11111111b
		MOVWF	PT1SEL
		MOVLW	11111111b
		MOVWF	PT2SEL
	
		MOVLW	11101111b
		MOVWF	PT1EN
		MOVLW	00000000b
		MOVWF	PT1PU
		MOVLW	11110111b
		MOVWF	PT1

		
	
		MOVLW	11111110b
		MOVWF	PT2EN
		MOVLW	00000000b
		MOVWF	PT2PU
		MOVLW	00000000b
		MOVWF	PT2	
		BSF		I2CCON,I2C_EN ;enable I2C port(PT1_4(SCL), PT1_3(SDA))
	
		RETURN
      
;----------------------------------------------------------------  

		                          
;=======================================================================
;  delay sub program
;  2MS时间基准
;=======================================================================                 
DELAY_2MS:
		MOVLW		48 
		MOVWF		DELAYCT0
DLOOP2MS:		
		CALL		DELAY_40us
		DECFSZ		DELAYCT0,f
		GOTO		DLOOP2MS	
		RETURN
;=======================================================================
;  delay sub program
;  发送时40us时间基准
;=======================================================================                 
DELAY_40us:
		MOVLW		16
		NOP 
		MOVWF		DELAYCT1
DLOOP0:
		DECFSZ		DELAYCT1,f
		GOTO		DLOOP0	
		RETURN

;=======================================================================
;  delay sub program
;  采集电平40us时间基准
;======================================================================= 
DELAY_40us_l:
		MOVLW		13 
		MOVWF		DELAYCT1
DLOOP1:
		DECFSZ		DELAYCT1,f
		GOTO		DLOOP1	
		RETURN

;=======================================================