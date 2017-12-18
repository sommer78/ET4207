



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


_EN_TIMERA_			macro
					BSF		TCCONA,TCENA
					endm

_RST_TIMERA_		macro
					BCF		TCCONA,TCRSTA
					endm

_DIS_TIMERA_		macro
					BCF		TCCONA,TCENA
					endm


_SET_SDA_OUTPUT_	macro
					BSF		PT1EN,SDA
					endm

_SET_SDA_OUTPUT_1_	macro
					BSF		PT1,SDA
					endm

_SET_SDA_OUTPUT_0_	macro
					BCF		PT1,SDA
					endm

_SET_SDA_INPUT_		macro
					BCF		PT1EN,SDA
					endm
					

_ET4007_BUSY_		macro
					BSF		PT2,BUSY
					endm

_ET4007_NOT_BUSY_	macro
					BCF		PT2,BUSY
					endm







_DELTA_HighLevel_H_		equ		0
_DELTA_HighLevel_L_		equ		3

_DELTA_LowLevel_H_		equ		0
_DELTA_LowLevel_L_		equ		50




SCK					equ		bPT14
SDA					equ		bPT15
BUSY				equ		bPT22
BUFFER_ADDRESS		equ		80h
RamInitial_ADDRESS	equ		80h
buffer				equ		31h
nWriteBitCount		equ		32h
nWriteByteCount		equ		33h






NegTemp			equ		20h

flag			equ		21h		
	repeat		equ	1		
	bSETDP		equ 3
	spc			equ	4
	isCarrierEnd		equ	5
	bRMTOUT_COMMON		equ	6		
	bLearnErr	equ	7


flag1			equ		22h
	bTIMERB_CALCULATE	equ	0
	bRMTOUT_LEARN		equ	1
	isHighLow			equ	2		;为1说明正处于高电平,为0处于低电平
	bLearnEnd			equ	3



;-------------------------------------------
; Value define for programing
;-------------------------------------------

DELAYCT0		equ	25h
DELAYCT1		equ	26h
DELAYCT2		equ	27h
;================================
; Memory define 
;===========================================
n_Crc				equ	4bh
n_PartIndexCount	equ	4ch
n_Sample			equ	4dh
n_Index				equ	4eh	;同一个地址
FreqH				equ 4eh	;同一个地址
FreqL				equ	4fh


nLowLevel_H			equ	2ah
nLowLevel_L			equ	2bh
nHighLevel_H		equ	2ch
nHighLevel_L		equ	2dh
new_nHighLevel_H	equ	2eh
new_nHighLevel_L	equ	2fh

TEMP				equ	30h



DELTA_ABS_L			equ	32h
DELTA_ABS_H			equ	33h


W_BACKUP			equ	34h
STATUS_BACKUP		equ	35h


ADDRESS_PartIndexCount		equ		50h			;存放 某2个样本的index数目
ADDRESS_Sample				equ		100h			;存放样本
ADDRESS_Index				equ		0c0h			;存放索引值

p_PartIndexCount	equ	36h
p_Sample			equ	37h
p_Index				equ	38h

nPartIndexCount		equ	39h

nIndexCount_h		equ	3bh
nIndexCount_l		equ	3ch

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


