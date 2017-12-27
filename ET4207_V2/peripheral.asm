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

;=================================================================		
;CLR ALL RAM TO 00H
;======================================================================
RamsClearALL:
		MOVLW		20h
		GOTO		ClearRams
LEARN_RAM_CLR:
		SETDP       00h
		MOVLW		40h
		GOTO		ClearRams

ClearRams:
		MOVWF		FRS0
ClearRamLoop_l:
		CLRF		IND0
		INCFSZ		FRS0,f
		GOTO		ClearRamLoop_l
		MOVLW		00h
		MOVWF		FRS0
		SETDP       01h
ClearRamLoop_h:
		CLRF		IND0
        INCFSZ		FRS0,f
        GOTO		ClearRamLoop_h
        SETDP       00h
		MOVLW	042H
		MOVWF	nVersion1
		MOVLW	007H
		MOVWF	nVersion2
		MOVLW	018H
		MOVWF	nVersion3
		MOVLW	001H
		MOVWF	nVersion4
		RETURN

;=================================================================		
;INIT VALUE FREOM 01 TO 0FFH
;======================================================================

InitValue:
		SETDP	00h
		CLRF	ncount
		MOVLW	40h
		MOVWF	FRS0
		MOVLW	01h
		MOVWF	nWriteByteCount_h
		MOVLW	0c0h
		MOVWF	nWriteByteCount_L
INIT_RAM1:
		MOVFW	NCOUNT
		MOVWF	IND0
		INCF	NCOUNT,F
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		DECFSZ	nWriteByteCount_h,F
		GOTO	INIT_RAM1
		DECFSZ	nWriteByteCount_L,F
		GOTO	INIT_RAM1
		NOP
		RETURN

INIT_Learn:
	ADDPCW
	RETLW 001H ; 0 
        RETLW 040H ; 1 
        RETLW 005H ; 2 
        RETLW 00dH ; 3 
        RETLW 000H ; 4 
        RETLW 014H ; 5 
        RETLW 001H ; 6 
        RETLW 0ddH ; 7 
        RETLW 000H ; 8 
        RETLW 014H ; 9 
        RETLW 000H ; 10 
        RETLW 094H ; 11 
        RETLW 000H ; 12 
        RETLW 014H ; 13 
        RETLW 000H ; 14 
        RETLW 095H ; 15 
        RETLW 000H ; 16 
        RETLW 014H ; 17 
        RETLW 000H ; 18 
        RETLW 095H ; 19 
        RETLW 000H ; 20 
        RETLW 014H ; 21 
        RETLW 000H ; 22 
        RETLW 095H ; 23 
        RETLW 000H ; 24 
        RETLW 014H ; 25 
        RETLW 000H ; 26 
        RETLW 095H ; 27 
        RETLW 000H ; 28 
        RETLW 014H ; 29 
        RETLW 000H ; 30 
        RETLW 095H ; 31 
        RETLW 000H ; 32 
        RETLW 014H ; 33 
        RETLW 000H ; 34 
        RETLW 095H ; 35 
        RETLW 000H ; 36 
        RETLW 014H ; 37 
        RETLW 005H ; 38 
        RETLW 00dH ; 39 
        RETLW 000H ; 40 
        RETLW 014H ; 41 
        RETLW 000H ; 42 
        RETLW 094H ; 43 
        RETLW 000H ; 44 
        RETLW 014H ; 45 
        RETLW 001H ; 46 
        RETLW 0ddH ; 47 
        RETLW 000H ; 48 
        RETLW 014H ; 49 
        RETLW 001H ; 50 
        RETLW 0ddH ; 51 
        RETLW 000H ; 52 
        RETLW 014H ; 53 
        RETLW 001H ; 54 
        RETLW 0ddH ; 55 
        RETLW 000H ; 56 
        RETLW 014H ; 57 
        RETLW 000H ; 58 
        RETLW 095H ; 59 
        RETLW 000H ; 60 
        RETLW 014H ; 61 
        RETLW 000H ; 62 
        RETLW 095H ; 63 
        RETLW 000H ; 64 
        RETLW 014H ; 65 
        RETLW 000H ; 66 
        RETLW 095H ; 67 
        RETLW 000H ; 68 
        RETLW 014H ; 69 
        RETLW 000H ; 70 
        RETLW 095H ; 71 
        RETLW 000H ; 72 
        RETLW 014H ; 73 
        RETLW 01dH ; 74 
        RETLW 076H ; 75 
        RETLW 001H ; 76 
        RETLW 040H ; 77 
        RETLW 005H ; 78 
        RETLW 00dH ; 79 
        RETLW 000H ; 80 
        RETLW 014H ; 81 
        RETLW 001H ; 82 
        RETLW 0deH ; 83 
        RETLW 000H ; 84 
        RETLW 014H ; 85 
        RETLW 000H ; 86 
        RETLW 094H ; 87 
        RETLW 000H ; 88 
        RETLW 014H ; 89 
        RETLW 000H ; 90 
        RETLW 094H ; 91 
        RETLW 000H ; 92 
        RETLW 014H ; 93 
        RETLW 000H ; 94 
        RETLW 094H ; 95 
        RETLW 000H ; 96 
        RETLW 014H ; 97 
        RETLW 000H ; 98 
        RETLW 094H ; 99 
        RETLW 000H ; 100 
        RETLW 014H ; 101 
        RETLW 000H ; 102 
        RETLW 094H ; 103 
        RETLW 000H ; 104 
        RETLW 014H ; 105 
        RETLW 000H ; 106 
        RETLW 094H ; 107 
        RETLW 000H ; 108 
        RETLW 014H ; 109 
        RETLW 000H ; 110 
        RETLW 094H ; 111 
        RETLW 000H ; 112 
        RETLW 014H ; 113 
        RETLW 005H ; 114 
        RETLW 00dH ; 115 
        RETLW 000H ; 116 
        RETLW 014H ; 117 
        RETLW 000H ; 118 
        RETLW 095H ; 119 
        RETLW 000H ; 120 
        RETLW 014H ; 121 
        RETLW 001H ; 122 
        RETLW 0deH ; 123 
        RETLW 000H ; 124 
        RETLW 014H ; 125 
        RETLW 001H ; 126 
        RETLW 0deH ; 127 
        RETLW 000H ; 128 
        RETLW 014H ; 129 
        RETLW 001H ; 130 
        RETLW 0deH ; 131 
        RETLW 000H ; 132 
        RETLW 014H ; 133 
        RETLW 000H ; 134 
        RETLW 094H ; 135 
        RETLW 000H ; 136 
        RETLW 014H ; 137 
        RETLW 000H ; 138 
        RETLW 094H ; 139 
        RETLW 000H ; 140 
        RETLW 014H ; 141 
        RETLW 000H ; 142 
        RETLW 094H ; 143 
        RETLW 000H ; 144 
        RETLW 014H ; 145 
        RETLW 000H ; 146 
        RETLW 094H ; 147 
        RETLW 000H ; 148 
        RETLW 014H ; 149 
        RETLW 01dH ; 150 
        RETLW 075H ; 151 
        RETLW 001H ; 152 
        RETLW 040H ; 153 
        RETLW 005H ; 154 
        RETLW 00dH ; 155 
        RETLW 000H ; 156 
        RETLW 014H ; 157 
        RETLW 001H ; 158 
        RETLW 0ddH ; 159 
        RETLW 000H ; 160 
        RETLW 014H ; 161 
        RETLW 000H ; 162 
        RETLW 094H ; 163 
        RETLW 000H ; 164 
        RETLW 014H ; 165 
        RETLW 000H ; 166 
        RETLW 094H ; 167 
        RETLW 000H ; 168 
        RETLW 014H ; 169 
        RETLW 000H ; 170 
        RETLW 094H ; 171 
        RETLW 000H ; 172 
        RETLW 014H ; 173 
        RETLW 000H ; 174 
        RETLW 095H ; 175 
        RETLW 000H ; 176 
        RETLW 014H ; 177 
        RETLW 000H ; 178 
        RETLW 095H ; 179 
        RETLW 000H ; 180 
        RETLW 014H ; 181 
        RETLW 000H ; 182 
        RETLW 095H ; 183 
        RETLW 000H ; 184 
        RETLW 014H ; 185 
        RETLW 000H ; 186 
        RETLW 095H ; 187 
        RETLW 000H ; 188 
        RETLW 014H ; 189 
        RETLW 005H ; 190 
        RETLW 00eH ; 191 
        RETLW 000H ; 192 
        RETLW 014H ; 193 
        RETLW 000H ; 194 
        RETLW 094H ; 195 
        RETLW 000H ; 196 
        RETLW 014H ; 197 
        RETLW 001H ; 198 
        RETLW 0ddH ; 199 
        RETLW 000H ; 200 
        RETLW 014H ; 201 
        RETLW 001H ; 202 
        RETLW 0ddH ; 203 
        RETLW 000H ; 204 
        RETLW 014H ; 205 
        RETLW 001H ; 206 
        RETLW 0ddH ; 207 
        RETLW 000H ; 208 
        RETLW 014H ; 209 
        RETLW 000H ; 210 
        RETLW 094H ; 211 
        RETLW 000H ; 212 
        RETLW 014H ; 213 
        RETLW 000H ; 214 
        RETLW 094H ; 215 
        RETLW 000H ; 216 
        RETLW 014H ; 217 
        RETLW 000H ; 218 
        RETLW 095H ; 219 
        RETLW 000H ; 220 
        RETLW 014H ; 221 
        RETLW 000H ; 222 
        RETLW 095H ; 223 
        RETLW 000H ; 224 
        RETLW 014H ; 225 
        RETLW 01dH ; 226 
        RETLW 076H ; 227 
        RETLW 001H ; 228 
        RETLW 040H ; 229 
        RETLW 005H ; 230 
        RETLW 00dH ; 231 
        RETLW 000H ; 232 
        RETLW 014H ; 233 
        RETLW 001H ; 234 
        RETLW 0ddH ; 235 
        RETLW 000H ; 236 
        RETLW 014H ; 237 
        RETLW 000H ; 238 
        RETLW 094H ; 239 
        RETLW 000H ; 240 
        RETLW 014H ; 241 
        RETLW 000H ; 242 
        RETLW 094H ; 243 
        RETLW 000H ; 244 
        RETLW 014H ; 245 
        RETLW 000H ; 246 
        RETLW 094H ; 247 
        RETLW 000H ; 248 
        RETLW 014H ; 249 
        RETLW 000H ; 250 
        RETLW 094H ; 251 
        RETLW 000H ; 252 
        RETLW 014H ; 253 
        RETLW 000H ; 254 
        RETLW 094H ; 255 
        RETLW 000H ; 256 
        RETLW 014H ; 257 
        RETLW 000H ; 258 
        RETLW 094H ; 259 
        RETLW 000H ; 260 
        RETLW 014H ; 261 
        RETLW 000H ; 262 
        RETLW 094H ; 263 
        RETLW 000H ; 264 
        RETLW 014H ; 265 
        RETLW 005H ; 266 
        RETLW 00eH ; 267 
        RETLW 000H ; 268 
        RETLW 014H ; 269 
        RETLW 000H ; 270 
        RETLW 095H ; 271 
        RETLW 000H ; 272 
        RETLW 014H ; 273 
        RETLW 001H ; 274 
        RETLW 0ddH ; 275 
        RETLW 000H ; 276 
        RETLW 014H ; 277 
        RETLW 001H ; 278 
        RETLW 0ddH ; 279 
        RETLW 000H ; 280 
        RETLW 014H ; 281 
        RETLW 001H ; 282 
        RETLW 0ddH ; 283 
        RETLW 000H ; 284 
        RETLW 014H ; 285 
        RETLW 000H ; 286 
        RETLW 094H ; 287 

Push_Learn_Ram:
	SETDP	02h
	MOVLW	80
	MOVWF	nWriteByteCount_h
	MOVLW	100H
	MOVWF	FRS1
	MOVLW	0
	MOVWF	ncount
Push_Learn_Ram1:	
	MOVFW	ncount
	CALL	INIT_Learn
	MOVWF	IND1
	INCF	ncount,F
	INCF	FRS1,F
	DECFSZ	nWriteByteCount_h,F
	GOTO	Push_Learn_Ram1
	NOP
	RETURN
	
        