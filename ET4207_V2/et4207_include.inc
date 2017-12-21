
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

state_flag			equ		21h	
	isError		equ	0		
	isLearnP14		equ	1


FLAG					equ		22h
	bTIMERB_CALCULATE	equ	0
	bRMTOUT_LEARN		equ	1
	isHighLow			equ	2		;为1说明正处于高电平,为0处于低电平
	bLearnEnd			equ	3
	isCmdByte		equ 4
	isCmdEnd		equ	5

LEARN_ERR		equ 25h
	L_TIMEOUT	EQU 1

	
;-------------------------------------------
; Delay time  
;-------------------------------------------

DELAYCT0		equ	23h
DELAYCT1		equ	24h
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
FIRST_PLUS EQU 1D0H ;记录第一个脉冲时间，可用为无载波型
W_BAK      EQU 1D1H
STATUS_BAK EQU 1D2H
CARRY_COUNT      EQU  1D3H
CARRY_L          EQU  1D4H
CARRY_H          EQU  1D5H
CARRY_SL          EQU 1D6H
CARRY_SH          EQU 1D7H

SPEC_CODE_NUM EQU 1D3H  ;3010 RC6 RCT100 3004 代码
SPEC_CODE_SYS EQU 1D4H  ;特殊系统
SPEC_CODE_DAT EQU 1D5H  ;特殊数据
;TF_50MS        EQU 1D5H ;第一帧码和第二帧码之间的间隔 最高位，每单位50ms
TF_H           EQU 1D6H ;第一帧码和第二帧码之间的间隔 高位，小于50ms的时间值
TF_L           EQU 1D7H ;第一帧码和第二帧码之间的间隔 低位，小于50ms的时间值


REC_FRAME_COUNT EQU 1DAH ;接收到的每帧码计数
REC_CYCLE_COUNT EQU 1DBH ;接收到的脉冲周期计数


CYCLE_LL       EQU 1DCH ;周期的低电平低位 低电平记录测量时间
CYCLE_LH       EQU 1DDH ;周期的低电平高位
CYCLE_HL       EQU 1DEH ;周期的高电平低位 高电平记录载波个数
CYCLE_HH       EQU 1DFH ;周期的高电平高位




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
CYCLE_REC_NUM   EQU 01F3H ;特征周期代号的已记录数

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
  F_C56        EQU 0 ;=1是56K载波
  F_PLUS       EQU 1 ;=1 无载波发射方式 =0载波方式
  F_TIMB       EQU 2 
;  F_CARRY      EQU 3 ;=1测载波 =0脱离测载波
  F_TF         EQU 3
  F_M_SEL      EQU 4 ;=0选择测脱离载波 =1选择测帧间隔时间 
  F_M_LOW      EQU 5 ;=1测量低电平有结果 
  F_HEAD       EQU 6 ;
  F_CMP_R      EQU 7 ; 两组数据比较结果，=0不等或不在范围 
                                         ;=1相等或在范围
  
 ; F_RC5L   EQU 5 ;0=3010低电平844 1=3010低电平1688 
 ; F_RC5H   EQU 6 ;0=3010高电平844 1=3010高电平1688

LEARN_KEY_NUM   EQU 01FEH ;待学习键号
LEARN_FLAG            EQU 01FFH
FIR   EQU  0         
FFSN   EQU  1
FSEN   EQU  2
FSET   EQU  3
FTIM   EQU  4 
FSCH   EQU	5	;=1 全局power搜索，=0品牌搜索
FNEW   EQU  7 ;=1 新键 =0旧键

;RAM_ADDR 100
FRAME_STRU       EQU 100H ;码结构
CARRY_TIM        EQU 101H ;载波时间 =ff时为无载波方式

CYCLE0_LL       EQU  102H ;第0种周期的低电平低位 低电平记录测量时间
CYCLE0_LH       EQU  103H ;第0种周期的低电平高位
CYCLE0_HL       EQU  104H ;第0种周期的高电平低位 高电平记录载波个数
CYCLE0_HH       EQU  105H ;第0种周期的高电平高位
CYCLE1_LL       EQU  106H ;第1种周期的低电平低位
CYCLE1_LH       EQU  107H ;第1种周期的低电平高位
CYCLE1_HL       EQU  108H ;第1种周期的高电平低位
CYCLE1_HH       EQU  109H ;第1种周期的高电平高位
CYCLE2_LL       EQU  10AH ;第2种周期的低电平低位
CYCLE2_LH       EQU  10BH ;第2种周期的低电平高位
CYCLE2_HL       EQU  10CH ;第2种周期的高电平低位
CYCLE2_HH       EQU  10DH ;第2种周期的高电平高位
CYCLE3_LL       EQU  10EH ;第3种周期的低电平低位
CYCLE3_LH       EQU  10FH ;第3种周期的低电平高位
CYCLE3_HL       EQU  110H ;第3种周期的高电平低位
CYCLE3_HH       EQU  111H ;第3种周期的高电平高位
CYCLE4_LL       EQU  112H ;第4种周期的低电平低位
CYCLE4_LH       EQU  113H ;第4种周期的低电平高位
CYCLE4_HL       EQU  114H ;第4种周期的高电平低位
CYCLE4_HH       EQU  115H ;第4种周期的高电平高位
CYCLE5_LL       EQU  116H ;第5种周期的低电平低位
CYCLE5_LH       EQU  117H ;第5种周期的低电平高位
CYCLE5_HL       EQU  118H ;第5种周期的高电平低位
CYCLE5_HH       EQU  119H ;第5种周期的高电平高位
CYCLE6_LL       EQU  11AH ;第6种周期的低电平低位
CYCLE6_LH       EQU  11BH ;第6种周期的低电平高位
CYCLE6_HL       EQU  11CH ;第6种周期的高电平低位
CYCLE6_HH       EQU  11DH ;第6种周期的高电平高位
CYCLE7_LL       EQU  11EH ;第7种周期的低电平低位
CYCLE7_LH       EQU  11FH ;第7种周期的低电平高位
CYCLE7_HL       EQU  120H ;第7种周期的高电平低位
CYCLE7_HH       EQU  121H ;第7种周期的高电平高位 

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
DAT0_17         EQU  13BH ;分高低4位记录0-7，>7yi一帧码发射结束
TF0_TL          EQU  13DH
TF0_TH          EQU  13EH
;TF0_50MS        EQU  13EH ;超过50ms次数的计数


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
DAT1_17         EQU  158H ;分高低4位记录0-7，>7yi一帧码发射结束
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
DAT2_17         EQU  175H ;分高低4位记录0-7，>7yi一帧码发射结束
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
DAT3_17         EQU  192H ;分高低4位记录0-7，>7yi一帧码发射结束
TF3_TL          EQU  194H
TF3_TH          EQU  195H 
;TF3_50MS        EQU  195H 

HEAD4_TIML      EQU  196H ;保存上面4帧数据，第五帧作为检测结束码用
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
DAT4_17         EQU  1AFH ;分高低4位记录0-7，>7yi一帧码发射结束
TF4_TL          EQU  1B1H
TF4_TH          EQU  1B2H
;TF4_50MS        EQU  1B2H

; 1b3h 1ceh


;=======常数定义======
TF_4700USL EQU 06AH ;被剪的码间隔
TF_4700USH EQU 003H

TIMA_200MSH  EQU  07AH  ;200000/6.4= 31250
TIMA_200MSL  EQU  012H 

LV_4MSL      EQU  088H ;4MS/0.8= 5000
LV_4MSH      EQU  013H

TIMA_15MSH  EQU  009H  ;定时器A 15ms时间 起始值0
                       ;TIMA>15ms 认为结束一帧码
TIMA_15MSL  EQU  027H ;15 ;77H 

TIMA_6MSH   EQU  003H ; 6000/6.4=
TIMA_6MSL   EQU  0A9H ; 
TIMA_5MSH   EQU  003H ;5000/6.4 = 781 
TIMA_5MSL   EQU  00DH ; 


BUFER_START    EQU 020H
BUFER_START_A  EQU 022H ;20H21H=引导电平

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

;特殊数据的判别基准
M3004_T0L   EQU 06H ;3004格式周期判别基准
M3004_T0H   EQU 18H
M3004_T1L   EQU 60H
M3004_T1H   EQU 24H

SPEC_3004_PC   EQU 250 ;100 ;特殊数据的低电平允许误差
SPEC_3004_PC_H EQU 10 ;8   ;特殊数据的高电平允许误差


RCT100_T0L   EQU 0AH ;RCT100格式周期判别基准
RCT100_T0H   EQU 0AH
RCT100_T1L   EQU 58H
RCT100_T1H   EQU 16H

RC5_TL_L0   EQU 1FH ;RC5格式周期判别基准 低电平两种 高电平两种
RC5_TH_L0   EQU 04H
RC5_TL_L1   EQU 3EH
RC5_TH_L1   EQU 08H
RC5_TL_H0   EQU 1EH
RC5_TH_H0   EQU 00H
RC5_TL_H1   EQU 3DH
RC5_TH_H1   EQU 00H
;---
RC6_HEAD_HL EQU 5FH ;引导高电平=2640
RC6_HEAD_HH EQU 00H
RC6_HEAD_LL EQU 4CH ;引导低电平 =880
RC6_HEAD_LH EQU 04H

RC6_JYL_L0   EQU 4CH  ;880 校验位
RC6_JYH_L0   EQU 04H
RC6_JYL_L1   EQU 72H  ;880+440
RC6_JYH_L1   EQU 06H
RC6_JYL_H0   EQU 1EH  ;880
RC6_JYH_H0   EQU 00H
RC6_JYL_H1   EQU 2DH  ;880+440
RC6_JYH_H1   EQU 00H


RC6_TL_L0   EQU 26H ;440  RC6格式周期判别基准 低电平两种 高电平两种
RC6_TH_L0   EQU 02H
RC6_TL_L1   EQU 4CH ;880
RC6_TH_L1   EQU 04H
RC6_TL_H0   EQU 0FH ;440
RC6_TH_H0   EQU 00H
RC6_TL_H1   EQU 1EH  ;880
RC6_TH_H1   EQU 00H
;---



SPEC_FRAME_PC   EQU 150 ;100 ;特殊数据的低电平允许误差
SPEC_FRAME_PC_H EQU 10 ;8   ;特殊数据的高电平允许误差

PLUS_TIME_PC   EQU 100 ;50 ;25 ;25*0.8=20US 定时数据偏差
PLUS_COUNT_PC  EQU 5  ;5个载波周期 载波计数偏差




FAST_LSET_KEY EQU 90 
