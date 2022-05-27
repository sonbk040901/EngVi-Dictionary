#
# (GNU) Makefile for btree library and test harness
#
# MODIFICATION HISTORY
# Mnemonic	Date	Rel	Who
# BTMAKE	01Feb24	1.0	mpw
#	Created.
# BTMAKE	02Jan29	1.1	mpw
#	Heavily simplified to use implicit rules
# BTMAKE    03Jan14 1.2 mpw
#   Add -Wno-long-long to CFLAGS for use under FreeBSD
# BTMAKE	03Dec12	1.3	mpw
#	Revised dependency handling - use 'make depend' to recreate if necessary
# BTMAKE    04Oct02 1.4 mpw
#   Added compile for big file tester
# BTMAKE	05Dec27 1.6 mpw
#	Added release target
# BTMAKE    070413  1.8 mpw
#   Revised linking 
# BTMAKE    080509  mpw
#	Make dependency file handling cleaner: ${DEP} is dependent on ${SRC}
# BTMAKE    100525
#   Added support for large files (> 2GB), by setting LFS=1
# BTMAKE    200629  mpw
#   ar deterministic mode broke btlib dependencies, requiring a change in the
#   way the btlib.a archive is updated.  depend target no longer required.
#
# $Id: Makefile,v 1.35 2020/07/04 10:52:20 mark Exp $

# Uncomment the following line for a debug version of the library
# DEBUG=-g

# Headers in INC_DIR
INC_DIR=./inc
# BT library source in $SRC_DIR
SRC_DIR=./src-lib
# Main programs (test harness, copy utility) in SRC_MAIN
SRC_MAIN=./src-main
# BT archive in LIB_DIR
LIB_DIR=./lib
LIB_NAME=bt
LIB_FILE=${LIB_DIR}/lib${LIB_NAME}.a
# Testcases in TESTCASES
TESTCASES=./Testcases

ifeq (${LFS},1)
LFSFLAG=-D_FILE_OFFSET_BITS=64
endif

# is readline available?
RDLINE := ${wildcard /usr/include/readline/*}
ifdef RDLINE
RDLINEFLAG=-DREADLINE
RDLINELIB=-lreadline
endif

CFLAGS=-pedantic-errors -Wall -Wno-long-long ${DEBUG} -I${INC_DIR} \
	${LFSFLAG} ${RDLINEFLAG}

LIBS=-L${LIB_DIR} -l${LIB_NAME} ${RDLINELIB}

ifeq (${shell uname -s},OpenBSD)
LIBS += -lcurses
endif

# location of release tarfile (see release target)
RELTMP=/tmp

# activate this macro to ignore ld errors
#LDFLAGS=-Xlinker -noinhibit-exec

SRC := ${wildcard ${SRC_DIR}/*.c}

OBJ := ${patsubst %.c,%.o,${SRC}}

HDR := ${wildcard ${INC_DIR}/*.h}

.PHONY: all clean test_init test_run release

.INTERMEDIATE: ${OBJ}

all:	bt kcp bigt bigtdel btr

${SRC}:	 ${INC_DIR}/bc.h ${INC_DIR}/bt.h ${INC_DIR}/btree_int.h \
		${INC_DIR}/btree.h
	touch $@

${LIB_FILE}: ${SRC} 
	${CC} -c ${CFLAGS} $?
	${AR} ${ARFLAGS} $@ *.o
	rm -f *.o

bt:	${SRC_MAIN}/bt.c ${INC_DIR}/btcmd.h ${SRC_MAIN}/btcmd.c ${LIB_FILE} 
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ ${SRC_MAIN}/bt.c ${SRC_MAIN}/btcmd.c \
	${LIBS} 

kcp:	${SRC_MAIN}/kcp.c ${LIB_FILE}
	${CC} ${CFLAGS} -o $@ ${SRC_MAIN}/kcp.c ${LIBS}

btr:  ${SRC_MAIN}/btr.c ${LIB_FILE} ${INC_DIR}/btr.h
	${CC} ${CFLAGS} -o $@ ${SRC_MAIN}/btr.c ${LIBS}

clean:
	rm -f bt bigt bigtdel ${LIB_FILE} kcp \
	btr ${OBJ} ${TESTCASES}/corrupt

TAGS:	${SRC} ${HDR} ${wildcard ${SRC_MAIN}/*.c}
	@etags $^

test_run:
	cd ${TESTCASES};sh test_control.sh
	find ${TESTCASES} -type f -name "test_db" -exec rm  {} \;

test_init:
	cd ${TESTCASES};sh ./create_output_masters.sh
	find ${TESTCASES} -type f -name "test_db" -exec rm  {} \;

# Build testing binaries
bigt: ${SRC_MAIN}/bigt.c ${LIB_FILE}
	${CC} ${CFLAGS} -o $@ ${SRC_MAIN}/bigt.c ${LIBS}

bigtdel:  ${SRC_MAIN}/bigtdel.c ${LIB_FILE}
	${CC} ${CFLAGS} -o $@ ${SRC_MAIN}/bigtdel.c ${LIBS}

corrupt:  ${TESTCASES}/corrupt.c
	${CC} ${CFLAGS} -o ${TESTCASES}/$@ ${TESTCASES}/corrupt.c


release:
ifndef REL
	${error Target release must be defined e.g. REL=release-2-0-2}
endif
	reldir=`echo $$REL|sed -e 's/release-//;s/-/./g;s/^/bt-/'` ;\
	mkdir -p ${RELTMP}/$$reldir ; \
	cvs export -d ${RELTMP}/$$reldir -r $$REL bt ; \
	cd ${RELTMP} ; \
	tar czf $$reldir.tar.gz $$reldir ; \
	rm -rf $$reldir
