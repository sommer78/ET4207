
NegTemp			equ		25h

flag				equ	26h		
	isCarrierIn		equ	0		
	repeat			equ	1		
	trig			equ	2	
	isCarrierEnd	equ	3
	spc				equ	4
	fail			equ	5
	kerpressed		equ	6
	bLearnErr		equ	7
flag1				equ	2fh
	isHighLow		equ	0
	bLearnEnd		equ	1
	isLastPack		equ 2
	isFirstByte		equ	3


;-------------------------------------------
; Value define for rmtout
;-------------------------------------------
nCustom		 equ		30h
nCustomB	 equ		31h
nData		 equ		32h
nDataB	     equ		33h

RmtDataCount equ		34h
;===========================================
; I/O define 
;===========================================

;SCL			equ		bPT17
;SDA			equ		bPT16

;-------------------------------------------
; Value define for programing
;-------------------------------------------

DELAYCT0		equ	29h
DELAYCT1		equ	2ah
DELAYCT2		equ	2bh

;===========================================
; Memory define 
;===========================================

;tempH			equ  7ch
;tempL			equ	 7dh
FreqH			equ  7eh
FreqL			equ	 7fh

Count_total			equ	3ah
nLowLevel_H			equ	3ch
nLowLevel_L			equ	3dh
nHighLevel_H		equ	3eh
nHighLevel_L		equ	3fh
nTemp_H				equ	3eh
nTemp_L				equ	3fh

TEMP			equ	2dh

;RAM_address		equ	80h				;采集数据存放连续字节起始地址
RAM_address		equ	100h				;采集数据存放连续字节起始地址
;============================================================
;
ROM_ADDR				equ	80h		;EEPROM连续读出128字节存放首地址


	                        
;=============================================================
RMT_NUM					equ	200



nPackageToutal			equ	35h
nPackageCount			equ	36h
;nPackageLength			equ	37h
pPackPointer			equ	37h
Count					equ	38h