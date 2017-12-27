
;-------------------------------------------
; Value define for gpio
;-------------------------------------------
BUSY				equ		bPT15
REC					equ		bPT14
LED					EQU		bPT23
;-------------------------------------------
; macro  define for programing
;-------------------------------------------

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
					BCF		PT1,BUSY
					endm

_ET4207_NOT_BUSY_	macro
					BCF		PT1EN,BUSY
					BSF		PT1,BUSY
					endm

_LED_CLR			macro
					BSF		PT2,LED
					endm
_LED_SET			macro
					BCF		PT2,LED
					endm
_WDT_EN				macro
					BSF		WDTCTR,WDTEN
					endm
_WDT_DIS			macro
					BCF		WDTCTR,WDTEN
					endm

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


I2C_CMD			equ		20h
	isP14		equ	3
	
state_flag			equ		21h	
	isError		equ	0		
	isLearnP14	equ	1
	isSendEnd	equ 2


FLAG					equ		22h
	bTIMERB_CALCULATE	equ	0
	bRMTOUT_LEARN		equ	1
	isHighLow			equ	2		;为1说明正处于高电平,为0处于低电平
	bLearnEnd			equ	3
	isCmdByte		equ 4
	isCmdEnd		equ	5
	isSend			equ 	6
	isHalf			equ	7
;-------------------------------------------
; Delay time  
;-------------------------------------------

DELAYCT0		equ	23h
DELAYCT1		equ	24h

;-------------------------------------------
; LEARN MODE 0 
;-------------------------------------------

LEARN_MODE		equ 25h
;-------------------------------------------
; CRC RAM
;-------------------------------------------
_CRC_LEN_H		equ 26h
_CRC_LEN_L		equ 27h	
_CRC_CODE		equ 28h	
_CRC_COUNT		equ	29h
;-------------------------------------------
; version rom  
;-------------------------------------------

nVersion1		equ	1f8h
nVersion2		equ	1f9h
nVersion3		equ	1fah
nVersion4		equ	1fbh

ncount			equ	29h
nWriteByteCount_h equ	26h
nWriteByteCount_L equ	27h

;================================
; Memory define 
;===========================================

;===========================================
; Learn ram
;===========================================
nLowLevel_H			equ	2ah
nLowLevel_L			equ	2bh
nHighLevel_H		equ	2ch
nHighLevel_L		equ	2dh
new_nHighLevel_H	equ	2eh
new_nHighLevel_L	equ	2fh

DELTA_ABS_L			equ	30h
DELTA_ABS_H			equ	31h



p_PartIndexCount	equ			32h
p_Sample			equ			33h
p_Index				equ			34h

nPartIndexCount		equ			35h

nIndexCount_h		equ			42h
nIndexCount_l		equ			43h
/*
Sample0_nHighLevel_H	equ	3dh
Sample0_nHighLevel_L	equ	3eh
Sample0_nLowLevel_H		equ	3fh
Sample0_nLowLevel_L		equ	40h

Sample1_nHighLevel_H	equ	41h
Sample1_nHighLevel_L	equ	42h
Sample1_nLowLevel_H		equ	43h
Sample1_nLowLevel_L		equ	44h

*/

Sample0_nHighLevel_H	equ	38h
Sample0_nHighLevel_L	equ	39h
Sample0_nLowLevel_H		equ	3Ah
Sample0_nLowLevel_L		equ	3Bh

Sample1_nHighLevel_H	equ	3Ch
Sample1_nHighLevel_L	equ	3Dh
Sample1_nLowLevel_H		equ	3Eh
Sample1_nLowLevel_L		equ	3Fh

LEARN_RESULT			EQU 40H
LEARN_TYPE				EQU 41H

Index_Bit				equ	45h

total_index				equ	46h

_LENGTH_h_learn			equ	48h
_LENGTH_l_learn			equ	49h
_LEARN_FLAG				EQU 4AH
	L_OUT		EQU 0
	L_TIMEOUT	EQU 1
	L_DATA		EQU 2
	L_LEN		EQU 3
	L_SAMPLE	EQU 4
n_IndexCount_h		equ			42h
n_IndexCount_l		equ			43h	
n_Crc				equ	4bh
n_PartIndexCount	equ	4ch
n_Sample			equ	4dh
n_Index				equ	4eh	
FreqH				equ 4eh	
FreqL				equ	4fh

FreqH1				equ 60h	
FreqL1				equ	61h
FreqH2				equ 62h	
FreqL2				equ	63h
FreqH3				equ 64h	
FreqL3				equ	65h
FreqH4				equ 66h	
FreqL4				equ	67h

;===========================================
; send ram
;===========================================
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
;_crc_temp		equ		36h
;_LENGTH_h_temp	equ		37h
;_LENGTH_l_temp	equ		38h
_nPairs_h_temp	equ		39h
_nPairs_l_temp	equ		3ah

tianjiacode_1		equ		23h
tianjiacode_2		equ		24h

tianjia_temp1		equ   2Ch
tianjia_temp2		equ   2Dh

tianjia_temp5		equ   31h

