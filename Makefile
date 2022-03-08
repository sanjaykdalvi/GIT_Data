E=$(RB)
I=$(MN)/inc
F=$(IN)/fonctions
G=$(IF)/gen
M=$(MN)/utl
N=$(TK)/src
gen=gen
J=inc
OBJ_DIR=$(IN)/onus_BFL/objfiles
ORA_INCL=-I$(ORACLE_HOME)/precomp/public -L$(ORACLE_HOME)/lib32 -lclntsh -lsql10
CCDEFINES=-D$(V_UNIX) -D"TRACE" -D"Qset" -D"NEW_FPRINTF" -D"FREE_MOD" -D"BAL_COM" 
CCINCLUDES= -I$I -I$J -I$(UNIKITDIR)/inc
CC=cc -D STP_P -bloadmap:bloadmap   -c $(CCDEFINES) $(CCINCLUDES) -g

LIBISAM=/usr/lib/libisam.a
LIB_HSM=$(TK)/gen_ang/lib_hsm.a

#all : $E/on_us
all : on_us

on_us :  \
	$(OBJ_DIR)/dump_rthauth.o \
	$(OBJ_DIR)/dump_athtrc.o \
	$(OBJ_DIR)/rthauth.o \
	$(OBJ_DIR)/write_seqfile.o      \
	$(OBJ_DIR)/onus.o               \
	$(OBJ_DIR)/dump_iso_auto.o      \
	$(OBJ_DIR)/fill_auto.o           \
	$(OBJ_DIR)/utils_visa.o         \
	$(OBJ_DIR)/dump_iso.o           \
	$(OBJ_DIR)/onus_01.o            \
	$(OBJ_DIR)/onus_02.o            \
	$(OBJ_DIR)/onus_04.o            \
	$(OBJ_DIR)/onus_fnc.o           \
	$(OBJ_DIR)/onus_utl.o           \
	$(OBJ_DIR)/onus_stip.o           \
	$(OBJ_DIR)/onus_ini.o           \
	$(OBJ_DIR)/onus_cardauth.o          \
	$(OBJ_DIR)/onus_account.o          \
	$(OBJ_DIR)/onus_log.o           \
	$(OBJ_DIR)/update_refresh.o    \
	$(OBJ_DIR)/onus_load.o			 \
	$(OBJ_DIR)/visa_eci.o        \
	$(OBJ_DIR)/visa_rpi.o        \
	$(OBJ_DIR)/currency_conversion.o            \
	$(OBJ_DIR)/date_cmp.o        \
	$(OBJ_DIR)/century.o         \
	$(OBJ_DIR)/acs9310.o         \
	$(OBJ_DIR)/date_check.o      \
	$(OBJ_DIR)/String_lib.o      \
	$(OBJ_DIR)/FPRINTF.o	     \
	$(LIB_HSM)
	#stoprout
		cc -bloadmap:$(HOME)/tmp/onus $(ORA_INCL) \
	$(OBJ_DIR)/dump_rthauth.o \
	$(OBJ_DIR)/dump_athtrc.o \
	$(G)/use_of_card.o \
	$(G)/acq_bin.o \
	$(OBJ_DIR)/rthauth.o\
	$(OBJ_DIR)/write_seqfile.o  \
	$(OBJ_DIR)/onus.o \
	$(G)/LibQueues.o     \
	$(OBJ_DIR)/dump_iso_auto.o \
	$(OBJ_DIR)/fill_auto.o      \
	$(G)/MalFunction.o   \
	$(G)/exist_coau.o    \
	$(OBJ_DIR)/utils_visa.o    \
	$(OBJ_DIR)/dump_iso.o      \
$(G)/BlockCodeMap.o	\
	$(OBJ_DIR)/FPRINTF.o	\
$(G)/DPRINTF.o	\
$(G)/PRINTAUTH.o	\
$(G)/dump.o	\
	$(G)/bal_utils.o       \
	$(G)/bal_disp.o       \
	$(OBJ_DIR)/onus_01.o       \
	$(OBJ_DIR)/onus_02.o       \
	$(OBJ_DIR)/onus_04.o       \
	$(OBJ_DIR)/onus_fnc.o      \
	$(OBJ_DIR)/onus_utl.o      \
	$(OBJ_DIR)/onus_stip.o      \
	$(OBJ_DIR)/onus_ini.o      \
	$(OBJ_DIR)/onus_cardauth.o     \
	$(OBJ_DIR)/onus_account.o     \
	$(OBJ_DIR)/onus_log.o      \
	$(OBJ_DIR)/update_refresh.o  \
	$(OBJ_DIR)/onus_load.o		\
	$(G)/trace.o          \
	$(G)/utils_iso_mci.o \
	$(G)/utils_iso_visa.o \
	$(G)/utils_iso_euro.o \
	$(G)/utils_iso_aegn.o \
	$(G)/utils_commun.o   \
	$(G)/Aut_Ref.o        \
	$(OBJ_DIR)/visa_eci.o       \
	$(OBJ_DIR)/visa_rpi.o       \
	$(OBJ_DIR)/currency_conversion.o           \
	$(OBJ_DIR)/century.o        \
	$(OBJ_DIR)/acs9310.o        \
	$(OBJ_DIR)/date_cmp.o       \
	$(OBJ_DIR)/date_check.o     \
	$(IN)/qset/hashit.o    \
	$(IN)/qset/get_date.o  \
	$(IN)/qset/dbgprint.o  \
	$(OBJ_DIR)/String_lib.o     \
	$(MN)/$(gen)_ang/libDate.a \
	$(IN)/map/libMapVISA.a \
	$(IN)/map/libMapNations_1.a \
	$(IN)/map/libMapPLUS.a \
	$(IN)/map/libMapEURO.a \
	$(IN)/map/libMapAMEX.a \
	$(IN)/map/libMapAEGN.a \
	$(IN)/map/libMapMCI.a  \
	$(IN)/map/authorization.a  \
	$(IN)/map/reload_transactions.a  \
	$(IN)/map/wrong_pin_card_block.a  \
	$(UNIKITDIR)/lib/libscr.a \
	$(UNIKITDIR)/lib/libfnc.a \
	$(UNIKITDIR)/lib/libecr.a \
	$(UNIKITDIR)/lib/libdec.a \
	$(IN)/onus_BFL/gen/apply_conn.a \
	$(IN)/onus_BFL/gen/card_control.a \
	$(IN)/onus_BFL/gen/kotak_api.a \
	$(IN)/onus_BFL/gen/cibil_call.a \
	$(IN)/onus_BFL/gen/void_reversal.a \
	$(LIBISAM) \
	$(LIB_HSM) \
	$(CCRS6000) \
	-lcrypto -lssl -o on_us
#	mv $E/on_us $E/ON_US_$(BAL_REF)
#	mv on_us $E/ON_US_NEW_$(BAL_REF)
	mv on_us $E/LOAN_ONUS_$(BAL_REF)

MalFunction.o : $(IF)/MalFunction.c 
	$(CC) -o $(OBJ_DIR)/MalFunction.o $(IF)/MalFunction.c

LibQueues.o : $F/LibQueues.c 
	$(CC) -o $(OBJ_DIR)/LibQueues.o $F/LibQueues.c

exist_coau.o : $(IF)/exist_coau.c 
	$(CC) -o $(OBJ_DIR)/exist_coau.o $(IF)/exist_coau.c

$(OBJ_DIR)/utils_visa.o : $(IN)/visa/utils_visa.c 
	$(CC) -o $(OBJ_DIR)/utils_visa.o $(IN)/visa/utils_visa.c

$(OBJ_DIR)/utils_plus.o : $(IN)/PLUS/utils_plus.c 
	$(CC) -o $(OBJ_DIR)/utils_plus.o $(IN)/PLUS/utils_plus.c

BlockCodeMap.o : $F/BlockCodeMap.c
	$(CC) -o $(OBJ_DIR)/BlockCodeMap.o $F/BlockCodeMap.c

$(OBJ_DIR)/FPRINTF.o : $F/FPRINTF.c
	$(CC) -o $(OBJ_DIR)/FPRINTF.o $F/FPRINTF.c

DPRINTF.o : $F/DPRINTF.c
	$(CC) -o $(OBJ_DIR)/DPRINTF.o $F/DPRINTF.c

PRINTAUTH.o : $F/PRINTAUTH.c
	$(CC) -o $(OBJ_DIR)/PRINTAUTH.o $F/PRINTAUTH.c

use_of_card.o : $F/use_of_card.c
	$(CC) -o $(OBJ_DIR)/use_of_card.o $F/use_of_card.c 

acq_bin.o : $F/acq_bin.c    
	$(CC) -o $(OBJ_DIR)/acq_bin.o $F/acq_bin.c    

dump.o : $F/dump.c
	$(CC) -o $(OBJ_DIR)/dump.o $F/dump.c

bal_disp.o : $F/bal_disp.c
	$(CC) -o $(OBJ_DIR)/bal_disp.o $F/bal_disp.c

bal_utils.o : $F/bal_utils.c
	$(CC) -o $(OBJ_DIR)/bal_utils.o $F/bal_utils.c

$(OBJ_DIR)/onus.o : onus.c 
	$(CC) -o $(OBJ_DIR)/onus.o onus.c

$(OBJ_DIR)/write_seqfile.o : write_seqfile.c 
	$(CC) -o $(OBJ_DIR)/write_seqfile.o write_seqfile.c

$(OBJ_DIR)/dump_iso_auto.o : dump_iso_auto.c
	$(CC) -o $(OBJ_DIR)/dump_iso_auto.o dump_iso_auto.c 

$(OBJ_DIR)/dump_rthauth.o : dump_rthauth.c
	$(CC) -o $(OBJ_DIR)/dump_rthauth.o dump_rthauth.c 

$(OBJ_DIR)/dump_athtrc.o : dump_athtrc.c
	$(CC) -o $(OBJ_DIR)/dump_athtrc.o dump_athtrc.c 

$(OBJ_DIR)/rthauth.o : rthauth.c       
	$(CC) -o $(OBJ_DIR)/rthauth.o rthauth.c 

$(OBJ_DIR)/fill_auto.o : fill_auto.c
	$(CC) -o $(OBJ_DIR)/fill_auto.o fill_auto.c 

$(OBJ_DIR)/onus_01.o : onus_01.c
	$(CC) -o $(OBJ_DIR)/onus_01.o onus_01.c 

$(OBJ_DIR)/onus_02.o : onus_02.c
	$(CC) -o $(OBJ_DIR)/onus_02.o onus_02.c 

$(OBJ_DIR)/onus_04.o : onus_04.c
	$(CC) -o $(OBJ_DIR)/onus_04.o onus_04.c

$(OBJ_DIR)/onus_fnc.o : onus_fnc.c
	$(CC) -o $(OBJ_DIR)/onus_fnc.o onus_fnc.c

$(OBJ_DIR)/onus_utl.o : onus_utl.c
	$(CC) -o $(OBJ_DIR)/onus_utl.o onus_utl.c

$(OBJ_DIR)/onus_stip.o : onus_stip.c
	$(CC) -o $(OBJ_DIR)/onus_stip.o onus_stip.c

$(OBJ_DIR)/onus_ini.o : onus_ini.c
	$(CC) -o $(OBJ_DIR)/onus_ini.o onus_ini.c

$(OBJ_DIR)/onus_cardauth.o : onus_cardauth.c
	$(CC) -o $(OBJ_DIR)/onus_cardauth.o onus_cardauth.c

$(OBJ_DIR)/onus_account.o : onus_account.c
	$(CC) -o $(OBJ_DIR)/onus_account.o onus_account.c

$(OBJ_DIR)/onus_log.o : onus_log.c
	$(CC) -o $(OBJ_DIR)/onus_log.o onus_log.c

$(OBJ_DIR)/update_refresh.o : update_refresh.c
	$(CC) -o $(OBJ_DIR)/update_refresh.o update_refresh.c

trace.o : $F/trace.c
	$(CC) -o $(OBJ_DIR)/trace.o $F/trace.c 

$(OBJ_DIR)/dump_iso.o : $F/dump_iso.c
	$(CC) -o $(OBJ_DIR)/dump_iso.o $F/dump_iso.c

utils_iso_mci.o : $F/utils_iso_mci.c
	$(CC) -o $(OBJ_DIR)/utils_iso_mci.o $F/utils_iso_mci.c

utils_iso_visa.o : $F/utils_iso_visa.c
	$(CC) -o $(OBJ_DIR)/utils_iso_visa.o $F/utils_iso_visa.c

utils_iso_plus.o : $F/utils_iso_plus.c
	$(CC) -o $(OBJ_DIR)/utils_iso_plus.o $F/utils_iso_plus.c

utils_iso_aegn.o : $F/utils_iso_aegn.c
	$(CC) -o $(OBJ_DIR)/utils_iso_aegn.o $F/utils_iso_aegn.c

utils_iso_euro.o : $F/utils_iso_euro.c
	$(CC) -o $(OBJ_DIR)/utils_iso_euro.o $F/utils_iso_euro.c

utils_commun.o : $F/utils_commun.c
	$(CC) -o $(OBJ_DIR)/utils_commun.o $F/utils_commun.c

Aut_Ref.o : $F/Aut_Ref.c
	$(CC) -o $(OBJ_DIR)/Aut_Ref.o $F/Aut_Ref.c 

$(OBJ_DIR)/visa_eci.o : $M/visa_eci.c
	$(CC) -o $(OBJ_DIR)/visa_eci.o $M/visa_eci.c 

$(OBJ_DIR)/visa_rpi.o : $M/visa_rpi.c
	$(CC) -o $(OBJ_DIR)/visa_rpi.o $M/visa_rpi.c 

$(OBJ_DIR)/currency_conversion.o : $M/currency_conversion.c
	$(CC) -o $(OBJ_DIR)/currency_conversion.o $M/currency_conversion.c  \
	-Dtest

$(OBJ_DIR)/date_cmp.o : $M/date_cmp.c
	$(CC) -o $(OBJ_DIR)/date_cmp.o $M/date_cmp.c  -Dtest

$(OBJ_DIR)/century.o : $M/century.c
	$(CC) -o $(OBJ_DIR)/century.o $M/century.c  -Dtest

$(OBJ_DIR)/acs9310.o : $M/acs9310.c
	$(CC) -o $(OBJ_DIR)/acs9310.o $M/acs9310.c  -Dtest

$(OBJ_DIR)/date_check.o : $M/date_check.c
	$(CC) -o $(OBJ_DIR)/date_check.o $M/date_check.c  -Dtest

$(OBJ_DIR)/String_lib.o : $M/String_lib.c
	$(CC) -o $(OBJ_DIR)/String_lib.o $M/String_lib.c

$(OBJ_DIR)/onus_load.o : onus_load.c
	$(CC) -o $(OBJ_DIR)/onus_load.o onus_load.c

remake: \
	clear \
	all

clear:
	/bin/rm -f $(OBJ_DIR)/*.o

