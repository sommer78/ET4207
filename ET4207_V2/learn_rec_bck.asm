
;==========================================
;RECEIVER LEARN SUBROUTINE
;OUTPUT 
;========================================== 

RECEIVER_START:
		SETDP	02h
		CLRF	state_flag
		CLRF	FLAG
		BCF		PT1EN,REC
		BSF	FLAG,isRecMode
REC_SET_REG:
		MOVLW	ADDRESS_PartIndexCount
	;	MOVWF	p_PartIndexCount
	;	MOVLW	ADDRESS_Sample
		MOVWF	FRS0
		MOVWF	p_Sample
		;MOVLW	ADDRESS_Index
		MOVLW	ADDRESS_Sample
		MOVWF	p_Index
		CLRF	nIndexCount_h
		CLRF	nIndexCount_l
		MOVLW	00H
		MOVWF	p_PartIndexCount
		GOTO	REC_WAIT_Sub

		BCF		state_flag,isLearnEnd
	   	MOVLW	00000011b					;timera时钟源,内部时钟源mclk(111-5MHz)
		MOVWF	TCCONA
		CLRF	TSETAL
		CLRF	TSETAH
		CLRF	nIndexCount_l
		CLRF	nIndexCount_H
		BCF	INTE,TMAIE
		BSF	INTE,I2CIF
		BSF	INTE,GIE
	   	BCF	PWMCON,TCOUTA_DIR			;TIMER UP 16bit型
		_GREEN_SET
		_ET4207_BUSY_
		
REC_WAIT_FIRST_DOWN_EDGE:
		BTFSC	state_flag,isLearnEnd
		GOTO	REC_LEARN_OUT
		_WDT_DIS			;	_WDT_EN  sommer
		BTFSC	PT1,REC
		GOTO	REC_WAIT_FIRST_DOWN_EDGE

		_GREEN_CLR

		MOVLW	00000110b					;timerb时钟源,内部时钟源mclk(111-5MHz)
		MOVWF	TCCONB
		BSF		TCCONB,TCENB
		BCF		TCCONB,TCRSTB
		MOVLW	78H
		MOVWF	TSETB
		BCF		TCCONA,TCRSTA
		BSF		TCCONA,TCENA
		BSF		INTE,TMAIE
		BSF		INTE,TMBIE
		BSF		INTE,GIE

REC_WAIT_Sub:
		SETDP	02h
		MOVFW	p_PartIndexCount
		MOVWF	FRS1
		MOVFW	IND1
		MOVWF	nHighLevel_H
		INCF	p_PartIndexCount,F
		MOVFW	p_PartIndexCount
		MOVWF	FRS1
		MOVFW	IND1
		MOVWF	nHighLevel_L
		INCF	p_PartIndexCount,F
		MOVFW	p_PartIndexCount
		MOVWF	FRS1
		MOVFW	IND1
		MOVWF	nLowLevel_H
		INCF	p_PartIndexCount,F
		MOVFW	p_PartIndexCount
		MOVWF	FRS1
		MOVFW	IND1
		MOVWF	nLowLevel_L
	
		INCF	p_PartIndexCount,F
	
		MOVLW	098H
		SUBWF	p_PartIndexCount,W
		BTFSC	STATUS,C
		GOTO	REC_LEARN_OUT
		GOTO	RECL_Compare_ST
/*
REC_WAIT_Sub:
		BTFSC	FLAG,bLearnEnd	
		GOTO	REC_LEARN_DATA_OUT
		BTFSS	FLAG,isPulseEnd
		GOTO	REC_WAIT_Sub
		BCF		FLAG,isPulseEnd
		NOP
		
		MOVFW	nHighLevel_H
		MOVWF	IND0
		INCF	FRS0,F
		MOVFW	nHighLevel_L
		MOVWF	IND0
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		MOVFW	nLowLevel_H
		MOVWF	IND0
		INCF	FRS0,F
		MOVFW	nLowLevel_L
		MOVWF	IND0
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		INCF	nIndexCount_l,F
	
		GOTO	REC_WAIT_Sub
*/
	;	GOTO	RECL_Compare_ST



REC_LEARN_END:
		NOP
		GOTO	RECL_PushNewSample
RECL_Compare_ST:
		CLRF	Index_Bit
		GOTO	RECL_Compare_ST1
RECL_Compare_Next:
		INCF	Index_Bit,F		
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RECL_Compare_ST1:
		MOVFW	total_sample
		SUBLW	0
		BTFSC	STATUS,Z
		GOTO	RECL_PushNewSample	
		MOVFW	Index_Bit
		SUBWF	total_sample,W
		BTFSC	STATUS,Z
		GOTO	RECL_PushNewSample		
		MOVFW	Index_Bit
		MOVWF	p_Sample	
		BCF		STATUS,C
		RLF		p_Sample,F
		BCF		STATUS,C
		RLF		p_Sample,W
		ADDLW	ADDRESS_PartIndexCount
		MOVWF	FRS0
		MOVFW	IND0
		MOVWF	Sample0_nHighLevel_H
		INCF	FRS0,f
		MOVFW	IND0
		MOVWF	Sample0_nHighLevel_L
		INCF	FRS0,f
		MOVFW	IND0
		MOVWF	Sample0_nLowLevel_H
		INCF	FRS0,f
		MOVFW	IND0
		MOVWF	Sample0_nLowLevel_L
		

		GOTO	RECL_Compare

RECL_Compare:					;==比较采样值跟样本值的大小==============
		MOVFW	Sample0_nHighLevel_L
		SUBWF	nHighLevel_L,w
		MOVWF	DELTA_ABS_L
		MOVFW	Sample0_nHighLevel_H
		SUBWFC	nHighLevel_H,w
		MOVWF	DELTA_ABS_H
		CALL	Neg
		MOVLW	_DELTA_REC_High_L_
		SUBWF	DELTA_ABS_L,w
		MOVLW	_DELTA_REC_High_H_
		SUBWFC	DELTA_ABS_H,w
		BTFSC	status,c
		GOTO	RECL_Compare_Next
		MOVFW	Sample0_nLowLevel_L
		SUBWF	nLowLevel_L,w
		MOVWF	DELTA_ABS_L
		MOVFW	Sample0_nLowLevel_H
		SUBWFC	nLowLevel_H,w
		MOVWF	DELTA_ABS_H
		CALL	Neg
		MOVLW	_DELTA_REC_Low_L_
		SUBWF	DELTA_ABS_L,w
		MOVLW	_DELTA_REC_Low_H_
		SUBWFC	DELTA_ABS_H,w
		BTFSC	status,c
		GOTO	RECL_Compare_Next
;--------------------------------------------------
;PUSH SAMPLE INDEX TO DATA STACKS
;--------------------------------------------------
RECL_PUSH_DATA_INDEX:
		SETDP	02H		
			BTFSC	FLAG,isHalf
			GOTO	RECL_PUSH_DATA_INDEX_LOW	
			MOVFW	p_Index
			MOVWF	FRS1
			SWAPF	Index_Bit,w
			MOVWF	IND1
			BSF		FLAG,isHalf
			INCFSZ	nIndexCount_l,F
			GOTO	$+2
			INCF	nIndexCount_H,F
			BTFSC	FLAG,bLearnEnd	
			GOTO	RECL_END
			GOTO	REC_WAIT_Sub
RECL_PUSH_DATA_INDEX_LOW:
			BCF		FLAG,isHalf	
			MOVFW	p_Index
			MOVWF	FRS1
			MOVFW	Index_Bit
			IORWF	IND1,F
			INCF	p_Index,f
		
			MOVLW	248
			SUBWF	p_Index,W
			BTFSC	STATUS,C
			GOTO	RECL_DATA_OUT
			INCFSZ	nIndexCount_l,F
			GOTO	$+2
			INCF	nIndexCount_H,F
			BTFSC	FLAG,bLearnEnd	
			GOTO	RECL_END
			GOTO	REC_WAIT_Sub

	
			;GOTO	StoreNewSample
;-------------------------------------------------------
;PUSH PULSE TO SAMPLE STACKS
;------------------------------------------------------
RECL_PushNewSample:		
			
			MOVFW	total_sample	
			MOVWF	p_Sample	
			BCF		STATUS,C
			RLF		p_Sample,F
			BCF		STATUS,C
			RLF		p_Sample,W
			ADDLW	ADDRESS_PartIndexCount
			MOVWF	FRS0
			MOVFW	nHighLevel_H
			MOVWF	IND0
			MOVWF	Sample0_nHighLevel_H
			INCF	FRS0,f
			MOVFW	nHighLevel_L
			MOVWF	IND0
			MOVWF	Sample0_nHighLevel_L
			INCF	FRS0,f
			MOVFW	nLowLevel_H
			MOVWF	IND0
			MOVWF	Sample0_nLowLevel_H
			INCF	FRS0,f
			MOVFW	nLowLevel_L
			MOVWF	IND0
			MOVWF	Sample0_nLowLevel_L
			INCF	total_sample,F
			MOVLW	16
			SUBWF	total_sample,W
			BTFSC	STATUS,C
			GOTO	RECL_SAMPLE_OUT
			GOTO	RECL_PUSH_DATA_INDEX

RECL_DATA_OUT:
			BSF		_LEARN_FLAG,L_DATA
			GOTO	RECL_END	

RECL_SAMPLE_OUT:
		   	BSF		_LEARN_FLAG,L_SAMPLE
			GOTO	RECL_END		
RECL_END:

			BCF		INTE,GIE
			BCF     INTE,TMAIE
			BCF     INTE,CAPIE
			BCF		TCCONA,TCENA
			NOP
			RRF		FreqH,F
			RRF		FreqL,F


			;MOVFW	FRS1
			INCF	FRS1,W
			MOVWF	p_Index
			;INCF	p_Sample,F
			MOVWF	_LENGTH_l_learn
			;INCF	_LENGTH_l_learn,F
			MOVFW	total_sample
			MOVWF	p_Sample
			MOVWF	n_Sample
			
			BCF		STATUS,C
			RLF		p_Sample,F
			RLF		p_Sample,F
			
			SETDP	02h
			MOVLW	ADDRESS_PartIndexCount
			ADDWF   p_Sample,W

			MOVWF	FRS0
			MOVFW	n_Sample
			MOVWF	p_Sample
			MOVLW	ADDRESS_Sample
			MOVWF	FRS1
RECL_ZIPDATA_PUSH:
			
			MOVFW	IND1
			MOVWF	IND0
			INCF	FRS1,F
			INCFSZ	FRS0,F
			GOTO	$+2
			SETDP	03h
			DECFSZ  p_Index,F
			GOTO	RECL_ZIPDATA_PUSH
			NOP	
			CLRF	_LENGTH_h_learn
			MOVFW	total_sample
			MOVWF	p_Sample	
			BCF		STATUS,C
			RLF		p_Sample,F
			RLF		p_Sample,W
			ADDWF	_LENGTH_l_learn,F
			BTFSS	STATUS,C
			GOTO	$+2
			INCF	_LENGTH_h_learn,F
		    
			MOVFW	_LENGTH_h_learn
			MOVWF	_CRC_LEN_H
			MOVFW	_LENGTH_L_learn
			MOVWF	_CRC_LEN_L
			SETDP	00h
			MOVLW	ADDRESS_PartIndexCount
			MOVWF	FRS0
			CALL	ET_xCal_crc
			MOVFW	_CRC_CODE
			MOVWF	n_Crc
			MOVLW	40H
			MOVWF	LEARN_TYPE
			_GREEN_CLR

			GOTO	Learn_OK	
REC_LEARN_OUT:
		BSF		_LEARN_FLAG,L_OUT
		SETDP	00h
		GOTO	SleepMode

		
REC_LEARN_DATA_OUT:
		BSF		_LEARN_FLAG,L_OUT
		MOVFW	nHighLevel_H
		MOVWF	IND0
		INCF	FRS0,F
		MOVFW	nHighLevel_L
		MOVWF	IND0
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		MOVFW	nLowLevel_H
		MOVWF	IND0
		INCF	FRS0,F
		MOVFW	nLowLevel_L
		MOVWF	IND0
		INCFSZ	FRS0,F
		GOTO	$+2
		SETDP	01h
		INCF	nIndexCount_l,F
		SETDP	00h
		GOTO	SleepMode
		
		
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
	
        