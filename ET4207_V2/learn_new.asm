;LED_ON:
     ; BSF PT2,LED_Z
    ;  BCF PT1,LED_F
   ;   RETURN
;LED_OFF:
    ;  BCF PT2,LED_Z
    ;  BSF PT1,LED_F
    ;  RETURN









;========================
LEARN_START:
       
    
LEARN_CSH:
       CLRF INTE
	   BCF     RMTCTR,TX_EN			 ;RX_EN=0,TX_EN=0 ��ֹ���䣬��������
       BSF     RMTCTR,RX_EN				
;	   BSF INTE,GIE
  ;     BSF F_LEARN,F_CARRY 
       BSF F_LEARN,F_HEAD 
       CLRF DAT0
	   CLRF DAT1
        MOVLW 00H
		MOVWF CARRY_COUNT
	  	CLRF  PWMCON

 		CLRF TSETAH	
		CLRF TSETAL		;Instruction Cycle 110-MCK/4 010-mck/12.5
;		MOVLW		00100010B	;step=0.8us	���˲�       ;timerʱ��Դ
		MOVLW		00100111B	;step=0.8us	���˲�       ;timerʱ��Դ
		MOVWF		TCCONA							        ;000:tcc(PT1.1)
        BSF TCCONA,TCENA
		BCF TCCONA,TCRSTA    ;2 �ڶ�������tima=0 ��ʼ���ز�  0=��0

  		  MOVLW 00 ;206
          MOVWF TSETB
          MOVLW 00000000B ;111B  ;��ʱ��Bѡ1��Ƶ
		  MOVWF TCCONB
;	      BSF TCCONB,TCENB
          BCF F_LEARN,F_TIMB
        BSF INTE,CAPIE

;********************************
;���ز����ں͵�һ���ز����峤��
;********************************
LEARN_SAA: 
       CLRF DAT0
       BCF INTF,CAPIF
LEARN_S: 
	  BCF INTF,TMBIF  ;     BCF INTE,CAPIE
	  BTFSC	INTF,I2CIE	;ѧϰģʽ�˳�
      GOTO	LEARN_ABOUT

  


  
      MOVLW 1
	  ADDWF LED_TIML,F
      MOVLW 0
	 ADDWFC LED_TIMM,F

      MOVLW 120 ;60 ;RET_10SH
	 SUBWFC LED_TIMH,W
      BTFSC STATUS,C
	  GOTO LEARN_ABOUT 

   ;    BSF INTE,CAPIE

      BTFSC INTF,CAPIF
      GOTO LEARN_S1

;      BSF PT1,LED_F  ;<<<<<<<<<<<<<<<<<<<<<<<<


;      movlw 042
;	  subwf sys1,w
;	  btfss status,c
;	  goto lled
;	  clrf sys0
;	  clrf sys1
;	  incf ledjs,f
;	  btfsc ledjs,0 

      MOVLW 051 ;45
      SUBWF LED_TIMM,W
      BTFSS STATUS,C
	  GOTO LED_SS
;	  CLRF LED_TIML
	  CLRF LED_TIMM
      INCF LED_TIMH,F
LED_SS:
	  BTFSC LED_TIMH,0
	  GOTO LED_CLR
LED_SET:
 ;     MOVLW 120 ;105 ;98
 ;     SUBWF LED_TIML,W
;	  BTFSC STATUS,C
;	  GOTO LED_CLR
	  _LED_SET  ;<<<<<<<<<<<<<<<<<<<<<<<<
      GOTO LEARN_S

LED_CLR:
	  _LED_CLR  ;<<<<<<<<<<<<<<<<<<<<<<<<
 ;     BTFSS INTF,CAPIF
 lled:
      GOTO LEARN_S

LEARN_S1: 
 	 _LED_SET  ;<<<<<<<<<<<<<<<<<<<<<<<<
     
      BCF INTF,CAPIF
	  INCF DAT0,F ;0
      BCF TCCONA,TCRSTA    ; ��1������tima=0 ��ʼ���ز�  0=��0 

;	  BTFSC RMTCTR,RMT_IN
;      GOTO $-1
;	  MOVFW TCOUTB       ;��һ������Ŀ�ȣ����ز���
;	  MOVWF FIRST_PLUS

     ; BSF   F_LEARN,F_HEAD


  	  MOVLW 156  ;206
      MOVWF TSETB ;40us�жϣ�����ز�������־
	    BCF TCCONB,TCRSTB
 	      BSF TCCONB,TCENB
       BSF INTE,TMBIE
      
	  BTFSC INTF,TMBIF     
	   GOTO LEARN_SAA   ;LOW_VOLTAGE
      BTFSS INTF,CAPIF
       GOTO $-3
	    BCF TCCONB,TCRSTB
      MOVFW TCOUTAL
	  MOVWF CARRY_SL  ;��¼��ʼֵ���������
      MOVFW TCOUTAH
	  MOVWF CARRY_SH  ;
      BCF INTF,CAPIF
	  INCF DAT0,F  ;1

	  BTFSC INTF,TMBIF     
	   GOTO LOW_VOLTAGE
      BTFSS INTF,CAPIF
       GOTO $-3
	    BCF TCCONB,TCRSTB
      MOVFW TCOUTAL
	  MOVWF CARRY_L
      MOVFW TCOUTAH
	  MOVWF CARRY_H
      BCF INTF,CAPIF
	  INCF DAT0,F   ;2
      INCF CARRY_COUNT,F ;1


	  BTFSC INTF,TMBIF     
	  GOTO LOW_VOLTAGE
      BTFSS INTF,CAPIF
      GOTO $-3
	  BCF TCCONB,TCRSTB
      MOVFW TCOUTAL
	  MOVWF CARRY_L
      MOVFW TCOUTAH
	  MOVWF CARRY_H
      BCF INTF,CAPIF
	  INCF DAT0,F    ;3
      INCF CARRY_COUNT,F ;2

	  BTFSC INTF,TMBIF     
	  GOTO LOW_VOLTAGE
      BTFSS INTF,CAPIF
      GOTO $-3
      BCF INTF,CAPIF
	  INCF DAT0,F   ;4
       BCF TCCONB,TCRSTB


	  BTFSC INTF,TMBIF     
	  GOTO LOW_VOLTAGE
      BTFSS INTF,CAPIF
      GOTO $-3
	  BCF TCCONB,TCRSTB
      MOVFW TCOUTAL
	  MOVWF CARRY_L
      MOVFW TCOUTAH
	  MOVWF CARRY_H
      BCF INTF,CAPIF
	  INCF DAT0,F  ;5
      INCF CARRY_COUNT,F ;3

	  BTFSC INTF,TMBIF     
	  GOTO LOW_VOLTAGE
      BTFSS INTF,CAPIF
      GOTO $-3
      BCF INTF,CAPIF
	  INCF DAT0,F ;6
       BCF TCCONB,TCRSTB

	  BTFSC INTF,TMBIF     
	  GOTO LOW_VOLTAGE
      BTFSS INTF,CAPIF
      GOTO $-3
      BCF INTF,CAPIF
	  INCF DAT0,F ;7
	   BCF TCCONB,TCRSTB


	  BTFSC INTF,TMBIF     
	  GOTO LOW_VOLTAGE
      BTFSS INTF,CAPIF
      GOTO $-3
      BCF INTF,CAPIF
	  INCF DAT0,F ;8
	   BCF TCCONB,TCRSTB


	  BTFSC INTF,TMBIF     
	  GOTO LOW_VOLTAGE
      BTFSS INTF,CAPIF
      GOTO $-3
	  BCF TCCONB,TCRSTB
      MOVFW TCOUTAL
	  MOVWF CARRY_L
      MOVFW TCOUTAH
	  MOVWF CARRY_H
      BCF INTF,CAPIF
	  INCF DAT0,F ;9
      INCF CARRY_COUNT,F ;4
      
	;   BSF INTE,GIE
    ;  BTFSS F_LEARN,F_TIMB  ;���˲������һ���ߵ�ƽʱ��
    ;  GOTO $-1              
HEAD0_LOOP:
	  BTFSC INTF,TMBIF     
	  GOTO LOW_VOLTAGE
      BTFSS INTF,CAPIF
      GOTO $-3
	  BCF TCCONB,TCRSTB
      BCF INTF,CAPIF
	  MOVLW 1
	  ADDWF DAT0,F
	  MOVLW 0
	  ADDWFC DAT1,F
      GOTO HEAD0_LOOP
;********************
;����ң���ź���ֵ����
;********************
LOW_VOLTAGE:
		MOVLW		10101010B	;step=0.8us	���˲�       ;timerʱ��Դ
;		MOVLW		10001010B	;step=0.8us	���˲�       ;timerʱ��Դ
		MOVWF		TCCONA							        ;000:tcc(PT1.1)
       BCF TCCONA,TCRSTA  ;TMA���㿪ʼ�� �͵�ƽ


      BCF   F_LEARN,F_HEAD
	  BCF INTF,TMBIF  ;     BCF INTE,CAPIE

	  MOVFW DAT1        ;��һ���ߵ�ƽ����
	  MOVWF HEAD0_TIMH   ;Ĭ���ڵ�0���뱣��
;	  MOVWF TMPH
	  MOVFW DAT0
	  MOVWF HEAD0_TIML
;	  MOVWF TMPL

	   BSF INTE,GIE
    ;   BCF F_LEARN,F_CARRY ;GGGGGGGGGGGGGGG
      
	  MOVLW BUFER_START  ;�����������ݴ�Ż�������ַ
	  MOVWF FRS1
	  MOVLW BUFER_START_A  ;�����������ݴ�Ż�����ʵ����ַ
	  MOVWF FRS0
	  CLRF  REC_CYCLE_COUNT ;���յ������ڱ�ż���
      CLRF  CYCLE_REC_NUM   ;���յ�������������

	   BCF F_LEARN,F_M_LOW ;�����͵�ƽ��־

;	  MOVFW DAT1        ;��һ���ߵ�ƽ����
;	  MOVWF HEAD0_TIMH   ;Ĭ���ڵ�0���뱣��
;	  MOVWF TMPH
;	  MOVFW DAT0
;	  MOVWF HEAD0_TIML
;	  MOVWF TMPL

;   MOVLW 2  ;������ƽ�޵�2������
;   SUBWF TMPL,F
;   MOVLW 0
;   SUBWFC TMPH,F
;   BTFSS STATUS,C
;   GOTO CARRY_M
;   MOVFW TMPL
;   MOVWF HEAD0_TIML
;   MOVFW TMPH
;   MOVWF HEAD0_TIMH

CARRY_M:
      MOVFW CARRY_SL    ;�ز���ֵֹ-��ʼֵ
	  SUBWF CARRY_L,F
	  MOVFW CARRY_SH
	  SUBWFC CARRY_H,F
	  BSF F_LEARN,F_PLUS
	  MOVFW CARRY_COUNT
	  ADDPCW
      GOTO CARRY_NONE
      GOTO CARRY_P1
      GOTO CARRY_P2 ;/2
      GOTO CARRY_P3 ;/4
CARRY_P4: ;/8
      BCF STATUS,C
	  RRF CARRY_H,F
	  RRF CARRY_L,F
CARRY_P3:
      BCF STATUS,C
	  RRF CARRY_H,F
	  RRF CARRY_L,F
CARRY_P2:
      BCF STATUS,C
	  RRF CARRY_H,F
	  RRF CARRY_L,F
CARRY_P1:
       MOVFW CARRY_L
	   MOVWF CARRY_TIM      ;����ز�����

	   BCF F_LEARN,F_C56
	   SUBLW 110 ;25 45K�ز�Ƶ����Ϊ�ֽ��
	   BTFSC STATUS,C
	   BSF F_LEARN,F_C56

	   BCF F_LEARN,F_PLUS
	   GOTO CARRY_END
CARRY_NONE:
	 ;  GOTO LEARN_START
CARRY_END:
     ;  BCF F_LEARN,F_CARRY ;GGGGGGGGGGGGGGG
;LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
CYCLE_LOOP:

FRAMEND_CHK: ;һ֡���������
;       BTFSS F_LEARN,F_TF
;	   GOTO CYCLE_CHK
       MOVFW REC_FRAME_COUNT
	   XORLW 4
	   BTFSC STATUS,Z
	   GOTO DATA_MATCH

       BTFSS TCCONB,0
	   GOTO CYCLE_CHK
	   
       MOVFW TCOUTB
	   MOVWF TMPL 
       MOVLW 155
	   SUBWF TMPL,W
	   BTFSS STATUS,C
       GOTO F_C56K
	   MOVWF TSETB

        BCF TCCONB,TCRSTB ;TIMEB���¼���
;	   MOVFW TMPL
       MOVLW 155
	   ADDWF TF_COUNTL,F
	   MOVLW 0
	   ADDWFC TF_COUNTH,F
	   
F_C56K:
	   BTFSS F_LEARN,F_C56
        GOTO F_C38K
	   MOVLW TIMA_5MSL 
       SUBWF TF_COUNTL,W
	   MOVLW TIMA_5MSH
	  SUBWFC TF_COUNTH,W
	   BTFSS STATUS,C  
        GOTO CYCLE_CHK 
        GOTO TF_CHECK

F_C38K:
;	   MOVFW CYCLE1_LH    ;++++
;       XORLW 0FFH         ;++++
;	   BTFSC STATUS,Z     ;++++
;        GOTO CYCLE_CHK    ;++++
       MOVLW LV_4MSL      ;++++
	   SUBWF CYCLE1_LL,W  ;++++
       MOVLW LV_4MSH      ;++++
	   SUBWFC CYCLE1_LH,W ;++++

       BTFSC STATUS,C
       GOTO MEASURE_15MS
MEASURE_6MS:
	   MOVLW TIMA_6MSL 
       SUBWF TF_COUNTL,W
	   MOVLW TIMA_6MSH
	  SUBWFC TF_COUNTH,W
	   BTFSS STATUS,C  
        GOTO CYCLE_CHK 
        GOTO TF_CHECK

MEASURE_15MS:
	   MOVLW TIMA_15MSL 
       SUBWF TF_COUNTL,W
	   MOVLW TIMA_15MSH
	  SUBWFC TF_COUNTH,W
	   BTFSS STATUS,C  
        GOTO CYCLE_CHK 
       ; GOTO TF_CHECK


;****************************
;׼��������һ֡���ݳ�ʼ��   
;****************************     
TF_CHECK: ;��������
       MOVFW TCOUTB
	   MOVWF TMPL 
       MOVLW 155
	   SUBWF TMPL,W
       MOVWF TMPL
	   BTFSS STATUS,C
       GOTO TF_CHECK0
	   MOVWF TSETB
       BCF TCCONB,TCRSTB ;TIMEB���¼���
;	   MOVFW TMPL
       MOVLW 155
	   ADDWF TF_COUNTL,F
	   MOVLW 0
	   ADDWFC TF_COUNTH,F
TF_CHECK0:
        MOVLW TIMA_200MSL 
        SUBWF TF_COUNTL,W
	    MOVLW TIMA_200MSH
	   SUBWFC TF_COUNTH,W
	    BTFSC STATUS,C  
		 GOTO DATA_MATCH ;�������200ms ��Ϊ�û�ֹͣ�������������ݼ����������
       
TF_CHECK_NEXT:
        BTFSC F_LEARN,F_M_LOW 
        GOTO TF_CHECK
		 
        MOVFW TF_COUNTL
		MOVWF TF_TMPL
        MOVFW TF_COUNTH
		MOVWF TF_TMPH

        

	   MOVFW TMPL
;       MOVLW 155
	   ADDWF TF_TMPL,F
	   MOVLW 0
	   ADDWFC TF_TMPH,F

        MOVFW TF_TMPL
		MOVWF TF_L 
        MOVFW TF_TMPH
		MOVWF TF_H 
        
;		GOTO TF_CHECK_END
		MOVLW 01CH
		SUBWF TF_TMPH,W
		BTFSC STATUS,C
		GOTO TF_CHECK_END
        MOVFW FRS0
		MOVWF TMPL
        
        SETDP 0

		MOVFW IND0
		MOVWF TF_TMPL
		INCF FRS0,F
		MOVFW IND0
		MOVWF TF_TMPH
        MOVFW TMPL
		MOVWF FRS0

        SETDP 1

		BCF STATUS,C
		RRF TF_TMPH,F
		RRF TF_TMPL,F
		BCF STATUS,C
		RRF TF_TMPH,F
		RRF TF_TMPL,F
		BCF STATUS,C
		RRF TF_TMPH,F
		RRF TF_TMPL,F
        MOVFW TF_TMPL
		MOVWF TF_L 
        MOVFW TF_TMPH
		MOVWF TF_H 

        
TF_CHECK_END:

	  MOVLW BUFER_START  ;�����������ݴ�Ż�����
	  MOVLW 22H  ;�����������ݴ�Ż�����
	  MOVWF FRS1
;	  MOVLW BUFER_START_A  ;�����������ݴ�Ż�����
      MOVLW 24H
	  MOVWF FRS0
      CLRF  REC_CYCLE_COUNT

SEC_HEAD:;�ڶ���������ƽ����
      BSF   F_LEARN,F_HEAD
      BTFSS F_LEARN,F_TIMB  ;������ƽ
      GOTO $-1
	             
	  			    
      MOVFW REC_FRAME_COUNT
      ADDPCW
	  GOTO FRAME_TF0 
	  GOTO FRAME_TF1 
	  GOTO FRAME_TF2 
	  GOTO FRAME_TF3 
;	  GOTO FRAME_TF4 
FRAME_TF4:
      MOVFW TF_L
	  MOVWF TF4_TL
      MOVFW TF_H
	  MOVWF TF4_TH
	  GOTO  FRAME_TF
FRAME_TF3:
      MOVFW TF_L
	  MOVWF TF3_TL
      MOVFW TF_H
	  MOVWF TF3_TH
	  GOTO  FRAME_TF
FRAME_TF2:
      MOVFW TF_L
	  MOVWF TF2_TL
      MOVFW TF_H
	  MOVWF TF2_TH
	  GOTO  FRAME_TF

FRAME_TF1:
      MOVFW TF_L
	  MOVWF TF1_TL
      MOVFW TF_H
	  MOVWF TF1_TH
	  GOTO  FRAME_TF
FRAME_TF0:
      MOVFW TF_L
	  MOVWF TF0_TL
      MOVFW TF_H
	  MOVWF TF0_TH
FRAME_TF:
        INCF REC_FRAME_COUNT,F  ;����֡����+1
        MOVFW REC_FRAME_COUNT
		SUBLW 4
		BTFSC STATUS,C
		GOTO FRAME_HEAD_SAVE
		MOVLW 4
		MOVWF REC_FRAME_COUNT ;ֻ����0123֡�����һ֡���� 

FRAME_HEAD_SAVE:
;      MOVFW DAT0
;	  MOVWF HEAD4_TIML
;      MOVFW DAT1
;	  MOVWF HEAD4_TIMH

      MOVFW DAT0
	  MOVWF TMPL
      MOVFW DAT1
	  MOVWF TMPH

   BTFSS F_LEARN,F_C56
   GOTO FRAME_HEAD_SAVE_NOR

   MOVLW 1  ;�ڶ���������ƽ 1������
   ADDWF TMPL,F
   MOVLW 0
  ADDWFC TMPH,F
;   BTFSC STATUS,Z
;    GOTO FRAME_HEAD_SAVE1
   MOVFW TMPL
   MOVWF DAT0
   MOVFW TMPH
   MOVWF DAT1
    GOTO FRAME_HEAD_SAVE1


FRAME_HEAD_SAVE_NOR:
;   MOVLW 0 ;1  ;�ڶ���������ƽ�޵�1������
;   SUBWF TMPL,F
;   MOVLW 0
;  SUBWFC TMPH,F
;   BTFSS STATUS,C
;    GOTO FRAME_HEAD_SAVE1
;   MOVFW TMPL
;   MOVWF DAT0
;   MOVFW TMPH
;   MOVWF DAT1



FRAME_HEAD_SAVE1:
      MOVFW REC_FRAME_COUNT
      ADDPCW
	  GOTO FRAME_HEAD0 
	  GOTO FRAME_HEAD1 
	  GOTO FRAME_HEAD2 
	  GOTO FRAME_HEAD3 
;	  GOTO FRAME_TF4 
FRAME_HEAD4:
      MOVFW DAT0
	  MOVWF HEAD4_TIML
      MOVFW DAT1
	  MOVWF HEAD4_TIMH
	  GOTO  FRAME_HEAD
FRAME_HEAD3:
      MOVFW DAT0
	  MOVWF HEAD3_TIML
      MOVFW DAT1
	  MOVWF HEAD3_TIMH
	  GOTO  FRAME_HEAD
FRAME_HEAD2:
      MOVFW DAT0
	  MOVWF HEAD2_TIML
      MOVFW DAT1
	  MOVWF HEAD2_TIMH
	  GOTO  FRAME_HEAD
FRAME_HEAD1:
      MOVFW DAT0
	  MOVWF HEAD1_TIML
      MOVFW DAT1
	  MOVWF HEAD1_TIMH
	  GOTO  FRAME_HEAD
FRAME_HEAD0:
      MOVFW DAT0
	  MOVWF HEAD0_TIML
      MOVFW DAT1
	  MOVWF HEAD0_TIMH
FRAME_HEAD:
      CLRF DAT0
	  CLRF DAT1 
       BCF F_LEARN,F_HEAD

;***********************
;�������ڼ���¼
;***********************
CYCLE_CHK:
        ;MOVFW FRS1
		;MOVWF TMPL
        ;MOVLW BUFER_OFF
		;SUBWF TMPL,W
        MOVLW BUFER_OFF
		SUBWF FRS1,W
		BTFSC STATUS,C
		GOTO DSP_ERR1 ;�������ڸ�������48������
;		GOTO CYCLE_LOOP ;CHK
;        MOVLW 5 ;0
;		CALL DLY
;		GOTO CYCLE_LOOP ;CHK

        MOVLW 24H
		SUBWF FRS1,W
		BTFSS STATUS,C
		GOTO CYCLE_CHK_END  ;������㷵��      

 ;	GOTO CYCLE_LOOP   ;CYCLE_CHK

		MOVFW FRS0
		SUBWF FRS1,W
		BTFSS STATUS,C
		GOTO DSP_ERR2
		SUBLW 3 ;��ָ�����3�����м���
		BTFSC STATUS,C
		GOTO CYCLE_CHK_END  ;������㷵��      
         
        SETDP 0
		MOVFW IND0   ;ȡ���Ƚϵ�ȡ������
		MOVWF CYCLE_LL
;		MOVWF CYCLE_LL_TMP
        MOVWF TMPL
		INCF FRS0,F
		MOVFW IND0
		MOVWF CYCLE_LH
;		MOVWF CYCLE_LH_TMP
        MOVWF TMPH

		INCF FRS0,F
		MOVFW IND0
		MOVWF CYCLE_HL
		MOVWF CYCLE_HL_TMP
		INCF FRS0,F
		MOVFW IND0
		MOVWF CYCLE_HH
		MOVWF CYCLE_HH_TMP
		INCF FRS0,F

        MOVFW FRS0
		MOVWF FRS0_BAK ;��ǰȡ��������ָ�� ����
DAT_BC: ;���ݲ���  
        MOVFW CYCLE_REC_NUM
        XORLW 0
		BTFSS STATUS,Z
		GOTO DAT1_7_BC
DAT0_BC:
   MOVLW 10  ;������ƽ�ĵ͵�ƽ����8us
   ADDWF TMPL,F
   MOVLW 0
   ADDWFC TMPH,F
   MOVFW TMPL
   MOVWF CYCLE_LL
   MOVFW TMPH
   MOVWF CYCLE_LH
   GOTO DAT_BC_END
DAT1_7_BC:        
   MOVLW 25  
   SUBWF TMPL,F
   MOVLW 0
   SUBWFC TMPH,F
   BTFSS STATUS,C
   GOTO DAT_BC_END
   MOVFW TMPL
   MOVWF CYCLE_LL
   MOVFW TMPH
   MOVWF CYCLE_LH
DAT_BC_END:
		MOVFW CYCLE_LL
		MOVWF CYCLE_LL_TMP
		MOVFW CYCLE_LH
		MOVWF CYCLE_LH_TMP
        
        MOVLW 4
		SUBWF CYCLE_HH,W
		BTFSC STATUS,C
		GOTO  DSP_ERR
DAT_BC_END1:
	;    BTFSS F_LEARN,F_C56 ;����45k�ز������϶�ʧ��һ���ز�
	;	GOTO DAT_SEARCH
		MOVLW 1
		ADDWF CYCLE_HL,F
		MOVLW 0
		ADDWFC CYCLE_HH,F


;*********************************
;�������αȽ�,�����ֵͬ��������ֵ
;*********************************
DAT_SEARCH:
		 CLRF SMJS    ;�����������ŵ���ʱֵ 
        SETDP 1
        MOVLW CYCLE0_LL
		MOVWF FRS0   
REC_CYCLE_SAVE_LOOP:
        MOVFW SMJS
		XORWF CYCLE_REC_NUM,W
		BTFSC STATUS,Z
		 GOTO REC_CYCLE_SAVE ;ѭ��ֵ���ѱ������������û�ҵ��������ݣ���Ϊ�����ݱ��� 
;**************************
        ;�������αȽ�
;**************************
CYCLE_COMPARE:

        MOVFW IND0    ;1.�Ƚ�˫�ֽڵ͵�ƽ
		MOVWF TMPL    
         INCF FRS0,F   
        MOVFW IND0
		MOVWF TMPH    
         INCF FRS0,F
CYCLE_COMPARE_L:
        MOVFW CYCLE_LL_TMP 
        SUBWF TMPL,F
        MOVFW CYCLE_LH_TMP
        SUBWFC TMPH,F
        BTFSS STATUS,C
		CALL ABS
        MOVLW PLUS_TIME_PC
        SUBWF TMPL,F
		MOVLW 0
		SUBWFC TMPH,F  ;TMPH.TMPL - �̶�ƫ��ֵ
         BTFSS STATUS,C
		 GOTO CYCLE_COMPARE_H
         INCF FRS0,F
         INCF FRS0,F
  	     GOTO CYCLE_COMPARE_N     ;������ֵ 

CYCLE_COMPARE_H:
        MOVFW IND0    ;2.�Ƚ�˫�ֽڸߵ�ƽ
		MOVWF TMPL    
         INCF FRS0,F
        MOVFW IND0
		MOVWF TMPH    
         INCF FRS0,F

        MOVFW CYCLE_HL_TMP 
        SUBWF TMPL,F
        MOVFW CYCLE_HH_TMP
       SUBWFC TMPH,F
        BTFSS STATUS,C
		CALL ABS
        MOVLW PLUS_COUNT_PC
        SUBWF TMPL,F
		MOVLW 0
		SUBWFC TMPH,F  ;TMPH.TMPL - �̶�ƫ��ֵ
        BTFSS STATUS,C
		GOTO  CYCLE_COMPARE_END     
		       
CYCLE_COMPARE_N:    ;������ֵ ȡ��һ�����ݱȽ�
        INCF SMJS,F 
		MOVLW 8
		SUBWF SMJS,W
		BTFSC STATUS,C
		GOTO DSP_ERR3 ;����8����������->����   
        GOTO REC_CYCLE_SAVE_LOOP
CYCLE_COMPARE_END: 
        MOVFW SMJS
        MOVWF SM_BUFL  ;������������
         GOTO REC_DATA_SAVE ;���Ƚ�����Ϊ�ɣ�ֱ�Ӵ洢���ڱ��
;--------------------
REC_CYCLE_SAVE: ;�����������ݴ洢 

         INCF CYCLE_REC_NUM,F
        MOVFW SMJS
        MOVWF SM_BUFL  ;������������
        MOVLW CYCLE0_LL  ;???????????��ʼ��λ��
        MOVWF FRS0
	    SETDP 1   ;100H

RECCYCLE_ADD_SEARCH: ;�������ݴ洢��ַ���� 
		MOVLW 1
		SUBWF SMJS,F
		BTFSS STATUS,C
		GOTO RECCYCLE_SAVE
        MOVLW 04
		ADDWF FRS0,F
		GOTO RECCYCLE_ADD_SEARCH ;RECDATA_ADD_SEARCH        
RECCYCLE_SAVE:
        MOVFW CYCLE_LL
        MOVWF IND0
        INCF FRS0,F
        MOVFW CYCLE_LH
        MOVWF IND0
        INCF FRS0,F
        MOVFW CYCLE_HL
        MOVWF IND0
        INCF FRS0,F
        MOVFW CYCLE_HH
        MOVWF IND0

;**************************
;�������������������
;**************************
REC_DATA_SAVE:
        MOVFW REC_FRAME_COUNT ;ȡ��0-4֡����׵�ַ
		                      ;һ֡������ж� if REC_FRAME_COUNT>3 THEN REC_FRAME_COUNT=4 
        ADDPCW
        GOTO FRAME0
        GOTO FRAME1
        GOTO FRAME2
        GOTO FRAME3
FRAME4:
       MOVLW DAT4_00
       GOTO  FRAME_SET
FRAME3:
       MOVLW DAT3_00
       GOTO  FRAME_SET
FRAME2:
       MOVLW DAT2_00
       GOTO  FRAME_SET
FRAME1:
       MOVLW DAT1_00
       GOTO  FRAME_SET
FRAME0:
       MOVLW DAT0_00
FRAME_SET:
       MOVWF FRS0
 ;       MOVWF SM_BUFL  ;������������
/*
   �����������ڱ�Ŵ洢��ʽ
   HL HL HL HL HL
   01 23 45 67 89 .......
   0-���յĵ�һ������ 1-�ڶ������� ��������
*/      
RECDATA_ADD_SEARCH:
       MOVFW REC_CYCLE_COUNT ;����->���յ����������ڱ�� ���洢����ʼ��ַ
       MOVWF SMJS
REC_DATA_SAVE_LOOP:
       MOVLW 1
	   SUBWF SMJS,F
	   BTFSS STATUS,C
	   GOTO SAVE_RECDAT_H ;�洢�����ݸ�λ
       MOVLW 1
	   SUBWF SMJS,F
	   BTFSS STATUS,C
	   GOTO SAVE_RECDAT_L ;�洢�����ݵ�λ
       INCF FRS0,F ;ָ����һ��ַ
	   GOTO REC_DATA_SAVE_LOOP

SAVE_RECDAT_H: ;�洢�����ݸ�λ
	   RLF SM_BUFL,F
	   RLF SM_BUFL,F
	   RLF SM_BUFL,F
	   RLF SM_BUFL,F
       MOVLW 0F0H
	   ANDWF SM_BUFL,F
       MOVFW IND0
	   ANDLW 0FH
       XORWF SM_BUFL,W
	   MOVWF IND0
	   GOTO SAVE_REDAT_N

SAVE_RECDAT_L: ;�洢�����ݵ�λ
       MOVLW 0FH
	   ANDWF SM_BUFL,F
       MOVFW IND0
	   ANDLW 0F0H
       XORWF SM_BUFL,W
	   MOVWF IND0
SAVE_REDAT_N:
       INCF REC_CYCLE_COUNT,F
       MOVFW REC_CYCLE_COUNT
	   SUBLW 49 ;48
	   BTFSS STATUS,C
	   GOTO DSP_ERR4 ;��¼��>48����
	   MOVFW FRS0_BAK ;�������������ָ��ָ�
       MOVWF FRS0
CYCLE_CHK_END:  
       GOTO CYCLE_LOOP


DATA_MATCH:
   BCF INTE,GIE ;CAPIE
       NOP
       CALL MATCH ;������������������
	   NOP
   		MOVLW		11111000B;PT1_SEL ;��Ӳ������ɨ��								;PT1.7--PT1.3 IO
		MOVWF		PT1SEL
;	   CALL SAVE_TO_EEPROM
   		MOVLW		PT1_SEL ;��Ӳ������ɨ��								;PT1.7--PT1.3 IO
		MOVWF		PT1SEL

	   NOP
	   NOP

       BCF state_flag,FTIM



		GOTO	LEARN_ABOUT
;	   GOTO POTINI 
	  ; GOTO LEARN_START


 DSP_ERR1: ;�������ڸ�������48������
    NOP
	NOP
	NOP
 DSP_ERR2:
    NOP ;��ָ��
	NOP                                                    
	NOP
 DSP_ERR3: ;����8����������->����  
    NOP
	NOP
	NOP
 DSP_ERR4: ;��¼��>48����
    NOP
    NOP
	NOP

 DSP_ERR:
 ;  BCF INTE,GIE ;CAPIE
    NOP
	NOP
	
  GOTO LEARN_START



LEARN_ABOUT:
	MOVLW	01h
	MOVWF	_LEARN_FLAG
	SETDP	00h
	GOTO	SleepMode
;	   GOTO $-1
