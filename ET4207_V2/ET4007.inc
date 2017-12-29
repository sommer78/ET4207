;===========================================
; Def_ET4007.ASM				
;===========================================

; System Register Address 00h-7Fh

W		equ		0
F		equ		1
;-------------------------------------------
; system register
;-------------------------------------------		
IND0		equ		000H	; Uses contents of FSR0 to address data memory
IND1		equ		001H	; Uses contents of FSR1 to address data memory
FRS0		equ		002H	; Indirect data memory, address point 0	
FRS1		equ		003H	; Indirect data memory, address point 1	
STATUS		equ		004H	; {0, 0, 0, PD, TO,DC, C, Z}
WORK		equ		005H	; Work register
INTF		equ		006H	; {  0, 0, 0, TMAIF, TMBIF, 0, 0, E0IF}
INTE		equ		007H	; {GIE, 0, 0, TMAIE, TMBIE, 0, 0, E0IE}

;-------------------------------------------
; peripheral register
;-------------------------------------------	
MCK			equ		008H	; {M7_CK,M6_CK,M5_CK,M4_CK,M3_CK,M2_CK,M1_CK,M0_CK}
;------------------------------------------
;TIMER/TCC A
;------------------------------------------
TSETAL		equ		009H
TSETAH		equ		00AH
PWMCON		equ		00Bh	
TCOUTAL		equ		00CH	;TCOUTAL[7:0]
TCOUTAH		equ		00DH	;TCOUTAH[15:8]
TCCONA		equ		00EH	; {TCRSTA,0,0,0,TCENA,CCLKA_SEL[2:0]}
;------------------------------------------
;TIMERB
;------------------------------------------
TSETB		equ		00FH
TCOUTB		equ		010H
TCCONB		equ		011H	; {TCRSTB,0,0,0,TCENB,CCLKB_SEL[2:0]}
;-----------------------------------------------
;KEY_VALUE
;-----------------------------------------------
KEY_VALUE	equ		012H


PT1			equ		013H	; PT1[7:3]
PT1EN		equ		014H	; PT1EN[7:3]
PT1PU		equ		015H	; PT1PU[7:3]

PT2			equ		016H	; PT2[7:0]
PT2EN		equ		017H	; PT2EN[7:0]
PT2PU		equ		018H	; PT2PU[7:0]

PT1SEL  equ   019H  ; PT1SEL[7:3]
PT2SEL  equ   01AH  ; PT2SEL[7:0]

PT1MR   equ   01BH  ; PT1MR[7:3]
PT2MR   equ   01CH  ; PT2MR[7:0]

WDTCTR		equ		01DH	;{0,0,0,scan_clk_en,WDTEN,WTS[2:0]}
WDTEN		equ		3
RMTCTR		equ		01EH	;{sens_curt[7:6],pwm_curt[2],rx_en,tx_en,pwm_curt[1:0],RMT_IN}
;-----------------------------------------------
;I2C
;-----------------------------------------------
I2CCON		equ		01FH	;{i2cen, i2c_ready, i2c_sto, i2c_rw, i2c_adr_sel, pt1od[5:3]}
I2C_EN		EQU		7
I2C_READY	EQU		6
I2C_STO		EQU		5
I2C_RW		EQU		4
I2C_ADDR	EQU		3
I2C_PT15	EQU		2
I2C_PT14	EQU		1
I2C_PT13	EQU		0


OSC_TEST	equ		01FH	;{OSC_TEST[7:0]}
;OSC_OTP		equ		01FH	;{OSC_OTP[7:0]}

;80H~0FFH is Gereral Data Memory

;-------------------------------------------
; Status register bit map
;-------------------------------------------
Z		equ		0
C		equ		1
DC		equ		2
TO		equ		3	; Watch Dog Time Out Flag: 
					; Clear by write 0 or power on reset.
					; Set by Watch Dog Time Out.
PD		equ		4	; Power Down Flag. 
					; Clear by write 0 or power on reset.
					; Set by Sleep or Halt instruction.
LVD_F   equ     5   ; LVD voltage flag, 2.0/2.2V flag(select by code option)
RST_FLAG_0  equ 6
RST_FLAG_1  equ 7
;{RST_FLAG_1, RST_FLAG_0}
;00---00-power/lvd reset
;10--wdt overflow reset
;11--external pin reset
;-------------------------------------------
; Interrupt flag register bit map
;-------------------------------------------
E0IF		equ		0
I2CIF		EQU		1 ;I2C INTERRUPT FLAG
CAPIF       equ     2 ;RMT Capture interrupt flag select
TMBIF		equ		3
TMAIF		equ		4

;-------------------------------------------
; Interrupt enable register bit map
;-------------------------------------------
E0IE		equ		0
I2CIE		EQU		1
CAPIE       equ     2 ;RMT Capture interrupt enable select
TMBIE		equ		3
TMAIE		equ		4
GIE			equ		7

;-------------------------------------------
; PWMCON register bit map
;-------------------------------------------
PWM_PORT_SEL_0  equ     0
PWM_PORT_SEL_1  equ     1
PWM_PORT_SEL_2  equ     2
PWMEN			equ		3
TCOUTA_DIR		equ		4
DUTY_SEL0		equ		5
DUTY_SEL1		equ		6
PWM_SEL			equ		7

;-------------------------------------------
; TCCONA register bit map
;-------------------------------------------
cclkA_sel0		equ		0
cclkA_sel1		equ		1
cclkA_sel2		equ		2
TCENA			equ		3	;  1 ENABLE TIMEA 0 DISABLE TIMEA
CAPPHA			equ		4 ;//RMT phase select
CAPDEB			equ		5 ;//RMT debounce time select
CAPSEL			EQU		6	; RMT INPUT PIN 0 RMT 1 P14
TCRSTA			equ		7	; SET 1 CLEAR TIMEA

;cclkA_sel0		equ		0
;cclkA_sel1		equ		1
;cclkA_sel2		equ		2
;[2:0]=111----5MHz
;[2:0]=110----2.5MHz
;[2:0]=101----0.15625MHz
;[2:0]=100----0.3125MHz
;[2:0]=011----0.625MHz
;[2:0]=010----1.25MHz
;[2:0]=001----~tcc
;[2:0]=000----tcc
;-------------------------------------------
; TCCONB register bit map
;-------------------------------------------
cclkB_sel0		equ		0
cclkB_sel1		equ		1
cclkB_sel2		equ		2
TCENB			equ		3	;  1 ENABLE TIMEB 0 DISABLE TIMEB
TCRSTB			equ		7	; SET 1 CLEAR TIMEB
;cclkB_sel0		equ		0
;cclkB_sel1		equ		1
;cclkB_sel2		equ		2
;[2:0]=111----5MMHz
;[2:0]=110----2.5MHz
;[2:0]=101----1.25MHz/32
;[2:0]=100----1.25MHz/16
;[2:0]=011----1.25MHz/8
;[2:0]=010----1.25MHz/4
;[2:0]=001----1.25MHz/2
;[2:0]=000----1.25MHz

;-------------------------------------------
; RMTCTR register bit map
;-------------------------------------------
RMT_IN			equ		0
PWM_CURT0		EQU		1
PWM_CURT1		EQU		2
TX_EN			equ		3
RX_EN			equ		4
PWM_CURT		equ		5
SENS_CURT0		equ		6
SENS_CURT1		equ		7
;-------------------------------------------
; PT1MR register bit map
;-------------------------------------------
;E0M1		equ		1
;E0M0		equ		0
;E0M[1:0]: GPIO Port 2 bit 0 interrupt trigger mode
;11 = PT20 is triggered at state change
;10 = PT20 is triggered at state change
;01 = PT20 is triggered at positive edge
;00 = PT20 is triggered at negative edge


;-------------------------------------------
; MCK  register bit map
;-------------------------------------------
M7_CK		equ	7	;0-Internal 1.25MHz
M6_CK		equ	6	;0-External 32.768KHz/4MHz
M5_CK		equ	5	;Timer/LCD 0-MCK/2000 1-Ext/32
M4_CK		equ	4	;
M3_CK		equ	3	;
M2_CK		equ	2	;÷∏¡Ó÷‹∆⁄
M1_CK		equ	1	;xx1-Ext/8 110-MCK/4 100-MCK/8
M0_CK		equ	0	;010-MCK/12.5 000-MCK/25

;ADC sample Freq M1_CK(0-MCK/50 1-MCK/25)
;Beeper Clock M0_CK/S_BEEP(01-MCK/500 00-MCK/750 1x-Ext/8)
;Volt Doubler Op.Freq M0_CK/S_PCK(01-MCK/50 00-MCK/200 1x-Ext/32)
;-------------------------------------------
;PT1 Register bit map
;-------------------------------------------
bPT17		equ		07h
bPT16		equ		06h
bPT15		equ		05h
bPT14		equ		04h
bPT13		equ		03h
bPT12		equ		02h
bPT11		equ		01h
bPT10		equ		00h


bPT27		equ		07h
bPT26		equ		06h
bPT25		equ		05h
bPT24		equ		04h
bPT23		equ		03h
bPT22		equ		02h
bPT21		equ		01h
bPT20		equ		00h




	
