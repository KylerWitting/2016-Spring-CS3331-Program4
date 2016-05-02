CC       = c++
FLAGS    = 
CFLAGS   = -g -O2 -c
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 
-DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I ThreadMentor-Linux64/include
TMLIB    =    ThreadMentor-Linux64/Visual/libthreadclass.a
TMLIB_NV =    ThreadMentor-Linux64/NoVisual/libthreadclass.a

OBJ_FILE = thread.o thread-main.o thread-support.o
EXE_FILE = prog4 

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread

thread.o: thread.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} thread.cpp

thread-main.o: thread-main.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} thread-main.cpp

thread-support.o: thread-support.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} thread-support.cpp

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
