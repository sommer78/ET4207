
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

ADDRESS_PartIndexCount		equ		50h			;��� ĳ2��������index��Ŀ
ADDRESS_Sample				equ		100h			;�������
ADDRESS_Index				equ		0c0h			;�������ֵ

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


FLAG					equ		22h
	bTIMERB_CALCULATE	equ	0
	bRMTOUT_LEARN		equ	1
	isHighLow			equ	2		;Ϊ1˵�������ڸߵ�ƽ,Ϊ0���ڵ͵�ƽ
	bLearnEnd			equ	3
	isCmdByte		equ 4
	isCmdEnd		equ	5
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
_CRC_COUNT		equ	47h
;-------------------------------------------
; version rom  
;-------------------------------------------

nVersion1		equ	1f8h
nVersion2		equ	1f9h
nVersion3		equ	1fah
nVersion4		equ	1fbh

ncount			equ	29h
nWriteByteCount_h equ	2ah
nWriteByteCount_L equ	2bh

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


LEARN_RESULT			EQU 40H
LEARN_TYPE				EQU 41H

Index_Bit				equ	45h

_crc_learn				equ	46h

_LENGTH_h_learn			equ	48h
_LENGTH_l_learn			equ	49h
_LEARN_FLAG			EQU 4AH
	L_OUT		EQU 0
	L_TIMEOUT	EQU 1
	L_DATA		EQU 2
	
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
_crc_temp		equ		36h
_LENGTH_h_temp	equ		37h
_LENGTH_l_temp	equ		38h
_nPairs_h_temp	equ		39h
_nPairs_l_temp	equ		3ah

tianjiacode_1		equ		23h
tianjiacode_2		equ		24h

tianjia_temp1		equ   2Ch
tianjia_temp2		equ   2Dh

tianjia_temp5		equ   31h

;===========================================
; other ram
;===========================================
ledjs equ 1cfh
FIRST_PLUS EQU 1D0H ;��¼��һ������ʱ�䣬����Ϊ���ز���
W_BAK      EQU 1D1H
STATUS_BAK EQU 1D2H
CARRY_COUNT      EQU  1D3H
CARRY_L          EQU  1D4H
CARRY_H          EQU  1D5H
CARRY_SL          EQU 1D6H
CARRY_SH          EQU 1D7H

SPEC_CODE_NUM EQU 1D3H  ;3010 RC6 RCT100 3004 ����
SPEC_CODE_SYS EQU 1D4H  ;����ϵͳ
SPEC_CODE_DAT EQU 1D5H  ;��������
;TF_50MS        EQU 1D5H ;��һ֡��͵ڶ�֡��֮��ļ�� ���λ��ÿ��λ50ms
TF_H           EQU 1D6H ;��һ֡��͵ڶ�֡��֮��ļ�� ��λ��С��50ms��ʱ��ֵ
TF_L           EQU 1D7H ;��һ֡��͵ڶ�֡��֮��ļ�� ��λ��С��50ms��ʱ��ֵ


REC_FRAME_COUNT EQU 1DAH ;���յ���ÿ֡�����
REC_CYCLE_COUNT EQU 1DBH ;���յ����������ڼ���


CYCLE_LL       EQU 1DCH ;���ڵĵ͵�ƽ��λ �͵�ƽ��¼����ʱ��
CYCLE_LH       EQU 1DDH ;���ڵĵ͵�ƽ��λ
CYCLE_HL       EQU 1DEH ;���ڵĸߵ�ƽ��λ �ߵ�ƽ��¼�ز�����
CYCLE_HH       EQU 1DFH ;���ڵĸߵ�ƽ��λ




LED_TIML       EQU 1DFH
LED_TIMM       EQU 1E0H
LED_TIMH       EQU 1E1H

TMPL	    EQU	  01E2H ;E9H
TMPH	    EQU   01E3H ;EAH
DAT0        EQU   01E4H ;EBH
DAT1        EQU   01E5H ;ECH

KEYCOUNT		EQU	 01E6H ;EDH
GNDBUFFL		EQU	 01E7H ;EEH
GNDBUFFH		EQU	 01E8H ;EFH
KEYBUFFL		EQU	 01E9H ;F0H
KEYBUFFH		EQU  01EAH ;F1H

KEYNUM2	    EQU   01ceH
KEYNUMO	    EQU   01F9H
KEYNUM	    EQU   01FAH

KEY_COUNT       EQU 01E6H
SCAN_ROW		EQU	01E2H
SCAN_TEMP		EQU 01E3H
SCAN_COUNT		EQU 01cDH
SCAN_BUFL		EQU 01cCH
SCAN_BUFH		EQU 01cBH

SCAN_COLUMN		EQU 01F9H
KEY_VAL1		EQU	01FAH
KEY_VAL2			EQU 01ceH
KEY_VAL3			EQU 01cFH
KEY_FUN	    EQU   01CFH

CYCLE_LL_TMP    EQU  01E7H ;EEH
CYCLE_LH_TMP    EQU  01E8H ;EFH
CYCLE_HL_TMP    EQU  01E9H ;F0H
CYCLE_HH_TMP    EQU  01EAH ;F1H



SMJS		EQU  01EBH
SMJS_BAK    EQU  01ECH
 
SM_BUFH		EQU  01ECH
SM_BUFL		EQU  01EDH

;KEY_OLD EQU    2EH
SYS0    EQU    01EEH
SYS1    EQU    01EFH
SYS2    EQU    01F0H
SYS3    EQU    01F1H

TF_COUNTL       EQU 01F0H
TF_COUNTH       EQU 01F1H
FRS0_BAK        EQU 01F2H
CYCLE_REC_NUM   EQU 01F3H ;�������ڴ��ŵ��Ѽ�¼��

E_BUF           EQU 01F5H

KEY_OLD         EQU 01F6H
CTR_JS           EQU 01F7H
TMP    EQU 1F8H

TF_TMPL EQU 01F7H
TF_TMPH EQU 01F8H


DB2	        EQU   01D0H
DB0	        EQU   01FBH
DB1         EQU   01FCH

F_LEARN     EQU   01FDH
  F_C56        EQU 0 ;=1��56K�ز�
  F_PLUS       EQU 1 ;=1 ���ز����䷽ʽ =0�ز���ʽ
  F_TIMB       EQU 2 
;  F_CARRY      EQU 3 ;=1���ز� =0������ز�
  F_TF         EQU 3
  F_M_SEL      EQU 4 ;=0ѡ��������ز� =1ѡ���֡���ʱ�� 
  F_M_LOW      EQU 5 ;=1�����͵�ƽ�н�� 
  F_HEAD       EQU 6 ;
  F_CMP_R      EQU 7 ; �������ݱȽϽ����=0���Ȼ��ڷ�Χ 
                                         ;=1��Ȼ��ڷ�Χ
  
 ; F_RC5L   EQU 5 ;0=3010�͵�ƽ844 1=3010�͵�ƽ1688 
 ; F_RC5H   EQU 6 ;0=3010�ߵ�ƽ844 1=3010�ߵ�ƽ1688

LEARN_KEY_NUM   EQU 01FEH ;��ѧϰ����
LEARN_FLAG            EQU 01FFH
FIR   EQU  0         
FFSN   EQU  1
FSEN   EQU  2
FSET   EQU  3
FTIM   EQU  4 
FSCH   EQU	5	;=1 ȫ��power������=0Ʒ������
FNEW   EQU  7 ;=1 �¼� =0�ɼ�

;RAM_ADDR 100
FRAME_STRU       EQU 100H ;��ṹ
CARRY_TIM        EQU 101H ;�ز�ʱ�� =ffʱΪ���ز���ʽ

CYCLE0_LL       EQU  102H ;��0�����ڵĵ͵�ƽ��λ �͵�ƽ��¼����ʱ��
CYCLE0_LH       EQU  103H ;��0�����ڵĵ͵�ƽ��λ
CYCLE0_HL       EQU  104H ;��0�����ڵĸߵ�ƽ��λ �ߵ�ƽ��¼�ز�����
CYCLE0_HH       EQU  105H ;��0�����ڵĸߵ�ƽ��λ
CYCLE1_LL       EQU  106H ;��1�����ڵĵ͵�ƽ��λ
CYCLE1_LH       EQU  107H ;��1�����ڵĵ͵�ƽ��λ
CYCLE1_HL       EQU  108H ;��1�����ڵĸߵ�ƽ��λ
CYCLE1_HH       EQU  109H ;��1�����ڵĸߵ�ƽ��λ
CYCLE2_LL       EQU  10AH ;��2�����ڵĵ͵�ƽ��λ
CYCLE2_LH       EQU  10BH ;��2�����ڵĵ͵�ƽ��λ
CYCLE2_HL       EQU  10CH ;��2�����ڵĸߵ�ƽ��λ
CYCLE2_HH       EQU  10DH ;��2�����ڵĸߵ�ƽ��λ
CYCLE3_LL       EQU  10EH ;��3�����ڵĵ͵�ƽ��λ
CYCLE3_LH       EQU  10FH ;��3�����ڵĵ͵�ƽ��λ
CYCLE3_HL       EQU  110H ;��3�����ڵĸߵ�ƽ��λ
CYCLE3_HH       EQU  111H ;��3�����ڵĸߵ�ƽ��λ
CYCLE4_LL       EQU  112H ;��4�����ڵĵ͵�ƽ��λ
CYCLE4_LH       EQU  113H ;��4�����ڵĵ͵�ƽ��λ
CYCLE4_HL       EQU  114H ;��4�����ڵĸߵ�ƽ��λ
CYCLE4_HH       EQU  115H ;��4�����ڵĸߵ�ƽ��λ
CYCLE5_LL       EQU  116H ;��5�����ڵĵ͵�ƽ��λ
CYCLE5_LH       EQU  117H ;��5�����ڵĵ͵�ƽ��λ
CYCLE5_HL       EQU  118H ;��5�����ڵĸߵ�ƽ��λ
CYCLE5_HH       EQU  119H ;��5�����ڵĸߵ�ƽ��λ
CYCLE6_LL       EQU  11AH ;��6�����ڵĵ͵�ƽ��λ
CYCLE6_LH       EQU  11BH ;��6�����ڵĵ͵�ƽ��λ
CYCLE6_HL       EQU  11CH ;��6�����ڵĸߵ�ƽ��λ
CYCLE6_HH       EQU  11DH ;��6�����ڵĸߵ�ƽ��λ
CYCLE7_LL       EQU  11EH ;��7�����ڵĵ͵�ƽ��λ
CYCLE7_LH       EQU  11FH ;��7�����ڵĵ͵�ƽ��λ
CYCLE7_HL       EQU  120H ;��7�����ڵĸߵ�ƽ��λ
CYCLE7_HH       EQU  121H ;��7�����ڵĸߵ�ƽ��λ 

HEAD0_TIML      EQU  122H
HEAD0_TIMH      EQU  123H
DAT0_00         EQU  124H
DAT0_01         EQU  125H
DAT0_02         EQU  126H
DAT0_03         EQU  127H
DAT0_04         EQU  128H
DAT0_05         EQU  129H
DAT0_06         EQU  12AH
DAT0_07         EQU  12BH
DAT0_08         EQU  12CH
DAT0_09         EQU  12DH
DAT0_0A         EQU  12EH
DAT0_0B         EQU  12FH
DAT0_0C         EQU  130H
DAT0_0D         EQU  131H
DAT0_0E         EQU  132H
DAT0_0F         EQU  133H
DAT0_10         EQU  134H
DAT0_11         EQU  135H
DAT0_12         EQU  136H
DAT0_13         EQU  137H
DAT0_14         EQU  138H
DAT0_15         EQU  139H
DAT0_16         EQU  13AH
DAT0_17         EQU  13BH ;�ָߵ�4λ��¼0-7��>7yiһ֡�뷢�����
TF0_TL          EQU  13DH
TF0_TH          EQU  13EH
;TF0_50MS        EQU  13EH ;����50ms�����ļ���


HEAD1_TIML      EQU  13FH
HEAD1_TIMH      EQU  140H
DAT1_00         EQU  141H
DAT1_01         EQU  142H
DAT1_02         EQU  143H
DAT1_03         EQU  144H
DAT1_04         EQU  145H
DAT1_05         EQU  146H
DAT1_06         EQU  147H
DAT1_07         EQU  148H
DAT1_08         EQU  149H
DAT1_09         EQU  14AH
DAT1_0A         EQU  14BH
DAT1_0B         EQU  14CH
DAT1_0C         EQU  14DH
DAT1_0D         EQU  14EH
DAT1_0E         EQU  14FH
DAT1_0F         EQU  150H
DAT1_10         EQU  151H
DAT1_11         EQU  152H
DAT1_12         EQU  153H
DAT1_13         EQU  154H
DAT1_14         EQU  155H
DAT1_15         EQU  156H
DAT1_16         EQU  157H
DAT1_17         EQU  158H ;�ָߵ�4λ��¼0-7��>7yiһ֡�뷢�����
TF1_TL          EQU  15AH
TF1_TH          EQU  15BH
;TF1_50MS        EQU  15BH

HEAD2_TIML      EQU  15CH
HEAD2_TIMH      EQU  15DH
DAT2_00         EQU  15EH
DAT2_01         EQU  15FH
DAT2_02         EQU  160H
DAT2_03         EQU  161H
DAT2_04         EQU  162H
DAT2_05         EQU  163H
DAT2_06         EQU  164H
DAT2_07         EQU  165H
DAT2_08         EQU  166H
DAT2_09         EQU  167H
DAT2_0A         EQU  168H
DAT2_0B         EQU  169H
DAT2_0C         EQU  16AH
DAT2_0D         EQU  16BH
DAT2_0E         EQU  16CH
DAT2_0F         EQU  16DH
DAT2_10         EQU  16EH
DAT2_11         EQU  16FH
DAT2_12         EQU  170H
DAT2_13         EQU  171H
DAT2_14         EQU  172H
DAT2_15         EQU  173H
DAT2_16         EQU  174H
DAT2_17         EQU  175H ;�ָߵ�4λ��¼0-7��>7yiһ֡�뷢�����
TF2_TL          EQU  177H
TF2_TH          EQU  178H
;TF2_50MS        EQU  178H

HEAD3_TIML      EQU  179H
HEAD3_TIMH      EQU  17AH
DAT3_00         EQU  17BH
DAT3_01         EQU  17CH
DAT3_02         EQU  17DH
DAT3_03         EQU  17EH
DAT3_04         EQU  17FH
DAT3_05         EQU  180H
DAT3_06         EQU  181H
DAT3_07         EQU  182H
DAT3_08         EQU  183H
DAT3_09         EQU  184H
DAT3_0A         EQU  185H
DAT3_0B         EQU  186H
DAT3_0C         EQU  187H
DAT3_0D         EQU  188H
DAT3_0E         EQU  189H
DAT3_0F         EQU  18AH
DAT3_10         EQU  18BH
DAT3_11         EQU  18CH
DAT3_12         EQU  18DH
DAT3_13         EQU  18EH
DAT3_14         EQU  18FH
DAT3_15         EQU  190H
DAT3_16         EQU  191H
DAT3_17         EQU  192H ;�ָߵ�4λ��¼0-7��>7yiһ֡�뷢�����
TF3_TL          EQU  194H
TF3_TH          EQU  195H 
;TF3_50MS        EQU  195H 

HEAD4_TIML      EQU  196H ;��������4֡���ݣ�����֡��Ϊ����������
HEAD4_TIMH      EQU  197H
DAT4_00         EQU  198H
DAT4_01         EQU  199H
DAT4_02         EQU  19AH
DAT4_03         EQU  19BH
DAT4_04         EQU  19CH
DAT4_05         EQU  19DH
DAT4_06         EQU  19EH
DAT4_07         EQU  19FH
DAT4_08         EQU  1A0H
DAT4_09         EQU  1A1H
DAT4_0A         EQU  1A2H
DAT4_0B         EQU  1A3H
DAT4_0C         EQU  1A4H
DAT4_0D         EQU  1A5H
DAT4_0E         EQU  1A6H
DAT4_0F         EQU  1A7H
DAT4_10         EQU  1A8H
DAT4_11         EQU  1A9H
DAT4_12         EQU  1AAH
DAT4_13         EQU  1ABH
DAT4_14         EQU  1ACH
DAT4_15         EQU  1ADH
DAT4_16         EQU  1AEH
DAT4_17         EQU  1AFH ;�ָߵ�4λ��¼0-7��>7yiһ֡�뷢�����
TF4_TL          EQU  1B1H
TF4_TH          EQU  1B2H
;TF4_50MS        EQU  1B2H

; 1b3h 1ceh


;=======��������======
TF_4700USL EQU 06AH ;����������
TF_4700USH EQU 003H

TIMA_200MSH  EQU  07AH  ;200000/6.4= 31250
TIMA_200MSL  EQU  012H 

LV_4MSL      EQU  088H ;4MS/0.8= 5000
LV_4MSH      EQU  013H

TIMA_15MSH  EQU  009H  ;��ʱ��A 15msʱ�� ��ʼֵ0
                       ;TIMA>15ms ��Ϊ����һ֡��
TIMA_15MSL  EQU  027H ;15 ;77H 

TIMA_6MSH   EQU  003H ; 6000/6.4=
TIMA_6MSL   EQU  0A9H ; 
TIMA_5MSH   EQU  003H ;5000/6.4 = 781 
TIMA_5MSL   EQU  00DH ; 


BUFER_START    EQU 020H
BUFER_START_A  EQU 022H ;20H21H=������ƽ

BUFER_OFF      EQU 0F0H  ;E8H


PT1_SEL EQU		00000000B								;PT1.7--PT1.3 IO
PT2_SEL EQU		10110000b							    ;PT2.7--PT2.0 IO

RET_10SL   EQU 00 ;05ah ;02DH
RET_10SM   EQU 0ach ;0E6H
RET_10SH   EQU 12h ;00AH


;---------------------------------------------------

STRU_ABBB   EQU 0
STRU_ABBA   EQU 1
STRU_ABAB   EQU 2
STRU_AAAE   EQU 3
STRU_AAAA   EQU 4
STRU_AS     EQU 5

FRAME_M3004  EQU 0F0H
FRAME_RCT100 EQU 0F1H
FRAME_RC5    EQU 0F2H
FRAME_RC6    EQU 0F3H

;�������ݵ��б��׼
M3004_T0L   EQU 06H ;3004��ʽ�����б��׼
M3004_T0H   EQU 18H
M3004_T1L   EQU 60H
M3004_T1H   EQU 24H

SPEC_3004_PC   EQU 250 ;100 ;�������ݵĵ͵�ƽ�������
SPEC_3004_PC_H EQU 10 ;8   ;�������ݵĸߵ�ƽ�������


RCT100_T0L   EQU 0AH ;RCT100��ʽ�����б��׼
RCT100_T0H   EQU 0AH
RCT100_T1L   EQU 58H
RCT100_T1H   EQU 16H

RC5_TL_L0   EQU 1FH ;RC5��ʽ�����б��׼ �͵�ƽ���� �ߵ�ƽ����
RC5_TH_L0   EQU 04H
RC5_TL_L1   EQU 3EH
RC5_TH_L1   EQU 08H
RC5_TL_H0   EQU 1EH
RC5_TH_H0   EQU 00H
RC5_TL_H1   EQU 3DH
RC5_TH_H1   EQU 00H
;---
RC6_HEAD_HL EQU 5FH ;�����ߵ�ƽ=2640
RC6_HEAD_HH EQU 00H
RC6_HEAD_LL EQU 4CH ;�����͵�ƽ =880
RC6_HEAD_LH EQU 04H

RC6_JYL_L0   EQU 4CH  ;880 У��λ
RC6_JYH_L0   EQU 04H
RC6_JYL_L1   EQU 72H  ;880+440
RC6_JYH_L1   EQU 06H
RC6_JYL_H0   EQU 1EH  ;880
RC6_JYH_H0   EQU 00H
RC6_JYL_H1   EQU 2DH  ;880+440
RC6_JYH_H1   EQU 00H


RC6_TL_L0   EQU 26H ;440  RC6��ʽ�����б��׼ �͵�ƽ���� �ߵ�ƽ����
RC6_TH_L0   EQU 02H
RC6_TL_L1   EQU 4CH ;880
RC6_TH_L1   EQU 04H
RC6_TL_H0   EQU 0FH ;440
RC6_TH_H0   EQU 00H
RC6_TL_H1   EQU 1EH  ;880
RC6_TH_H1   EQU 00H
;---



SPEC_FRAME_PC   EQU 150 ;100 ;�������ݵĵ͵�ƽ�������
SPEC_FRAME_PC_H EQU 10 ;8   ;�������ݵĸߵ�ƽ�������

PLUS_TIME_PC   EQU 100 ;50 ;25 ;25*0.8=20US ��ʱ����ƫ��
PLUS_COUNT_PC  EQU 5  ;5���ز����� �ز�����ƫ��




FAST_LSET_KEY EQU 90 