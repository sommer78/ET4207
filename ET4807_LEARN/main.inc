
_EN_InterruptEXT_	macro
					BSF		INTE,E0IE
					endm

_DIS_InterruptEXT_	macro
					BCF		INTE,E0IE
					endm


_EN_InterruptTIMERA_	macro
					BSF		INTE,TMAIE
					endm

_DIS_InterruptTIMERA_	macro
					BCF		INTE,TMAIE
					endm
_EN_InterruptI2C_	macro
					BSF		INTE,I2CIE
					endm

_DIS_InterruptI2C_	macro
					BCF		INTE,I2CIE
					endm

_EN_TIMERA_			macro
					BSF		TCCONA,TCENA
					endm

_RST_TIMERA_		macro
					BCF		TCCONA,TCRSTA
					endm

_DIS_TIMERA_		macro
					BCF		TCCONA,TCENA
					endm
					

_ET4207_BUSY_		macro
					BSF		PT1EN,BUSY
					BSF		PT1,BUSY
					endm

_ET4207_NOT_BUSY_	macro
					BSF		PT1EN,BUSY
					BCF		PT1,BUSY
					endm

_ET4807_OUT_HIGH_	macro
					BSF		PT2,OUT
					endm

_ET4807_OUT_LOW_	macro
					BCF		PT2,OUT
					endm
_ET4807_OUT_TRIG_	macro
					MOVLW	00000100B
					XORWF	PT2,F	
					endm

;-------------------------------------------
; Value define for gpio
;-------------------------------------------
BUSY				equ		bPT15
OUT					equ		bPT22
REC					equ		bPT14
;-------------------------------------------
; Value define for programing
;-------------------------------------------


_DELTA_HighLevel_H_		equ		0
_DELTA_HighLevel_L_		equ		3

_DELTA_LowLevel_H_		equ		0
_DELTA_LowLevel_L_		equ		50

ADDRESS_PartIndexCount		equ		50h			;存放 某2个样本的index数目
ADDRESS_Sample				equ		100h			;存放样本
ADDRESS_Index				equ		0c0h			;存放索引值

_index			equ		64

;-------------------------------------------
; Value define for programing
;-------------------------------------------

;BUFFER_ADDRESS		equ		80h


TEMP			equ		20h

flag			equ		21h	
	isError		equ	0		
;	bSETDP		equ 3
;	spc			equ	4
;	isCarrierEnd		equ	5
;	bRMTOUT_COMMON		equ	6		
;	bLearnErr	equ	7
;	isLearnRec		equ	2


flag1					equ		22h
	bTIMERB_CALCULATE	equ	0
	bRMTOUT_LEARN		equ	1
	isHighLow			equ	2		;为1说明正处于高电平,为0处于低电平
	bLearnEnd			equ	3
	isCmdByte		equ 4
	isCmdEnd		equ	5

	
;-------------------------------------------
; Delay time  
;-------------------------------------------

DELAYCT0		equ	23h
DELAYCT1		equ	24h
;-------------------------------------------
; version rom  
;-------------------------------------------

nVersion1		equ	25h
nVersion2		equ	26h
nVersion3		equ	27h
nVersion4		equ	28h

ncount			equ	29h
nWriteByteCount_h equ	2ah
nWriteByteCount_L equ	2bh

;================================
; Memory define 
;===========================================


nLowLevel_H			equ	2ah
nLowLevel_L			equ	2bh
nHighLevel_H		equ	2ch
nHighLevel_L		equ	2dh
new_nHighLevel_H	equ	2eh
new_nHighLevel_L	equ	2fh

DELTA_ABS_L			equ	30h
DELTA_ABS_H			equ	31h


;W_BACKUP			equ	34h
;STATUS_BACKUP		equ	35h

;pPackPointer			equ	37h



p_PartIndexCount	equ			32h
p_Sample			equ			33h
p_Index				equ			34h

nPartIndexCount		equ			35h

nIndexCount_h		equ			36h
nIndexCount_l		equ			37h

Sample0_nHighLevel_H	equ	3dh
Sample0_nHighLevel_L	equ	3eh
Sample0_nLowLevel_H		equ	3fh
Sample0_nLowLevel_L		equ	40h

Sample1_nHighLevel_H	equ	41h
Sample1_nHighLevel_L	equ	42h
Sample1_nLowLevel_H		equ	43h
Sample1_nLowLevel_L		equ	44h

Index_Bit				equ	45h

_crc_learn				equ	46h
_learn1					equ	47h
_LENGTH_h_learn			equ	48h
_LENGTH_l_learn			equ	49h
_LEARN_FLAG			EQU 4AH
n_Crc				equ	4bh
n_PartIndexCount	equ	4ch
n_Sample			equ	4dh
n_Index				equ	4eh	;同一个地址
FreqH				equ 4eh	;同一个地址
FreqL				equ	4fh

FreqH1				equ 60h	;同一个地址
FreqL1				equ	61h
FreqH2				equ 62h	;同一个地址
FreqL2				equ	63h
FreqH3				equ 64h	;同一个地址
FreqL3				equ	65h
FreqH4				equ 66h	;同一个地址
FreqL4				equ	67h

;;------------send ram-----------------------------------
_WRITE_CMD_DATA	EQU		40H
_LENGTH_h		equ		41h
_LENGTH_l		equ		42h
_FREQ			equ		43h
_nPairs_h		equ		44h
_nPairs_l		equ		45h
_FORM_h			equ		46h
_FORM_l			equ		47h
_REPEAT			equ		48h
_CRC			equ		49h
POINTER0		equ		4ah
POINTER1		equ		4ah
_High_h			equ		30h
_High_l			equ		31h
_Low_h			equ		32h
_Low_l			equ		33h
_temp1			equ     34h
_Index_1		equ		35h
_crc_temp		equ		36h
_LENGTH_h_temp	equ		37h
_LENGTH_l_temp	equ		38h
_nPairs_h_temp	equ		39h
_nPairs_l_temp	equ		3ah


;nWriteByteCount_h		equ		34h
;nWriteByteCount_l		equ		35h


