TARGET=main
#	---------------------------------
#	Standard makefile template
#	just specify the target name above
#	and the make file should sort it
#	all out (hopefully)
#	---------------------------------

INCLUDEFILE = include.mk

CC=g++

# this should appear above the 'include' directive below
# calls for the target and then the cleanup
# =========================================
all: ${TARGET} cleanup 


-include ${INCLUDEFILE}

# ${INCLUDEFILE} should not exist when it's included.
# As it does not exist it is treated as a target.
# Therefore it is created anew each time
# by the appropriate rule, below
# Which finds .c and .h files and calls makedepend for them
# =========================================================

${INCLUDEFILE}:
	@echo "### creating ${INCLUDEFILE}"
	@echo "### creating ${INCLUDEFILE}" > ${INCLUDEFILE}
	-@ echo  *.cpp|xargs echo SRCFILES = >> ${INCLUDEFILE}
	-@ echo  *.h|xargs echo HFILES = >> ${INCLUDEFILE}
	@  makedepend  -f${INCLUDEFILE} *.cpp


# (SRCFILES defined in INCLUDEFILE)
# =================================
OBJFILES = ${SRCFILES:.cpp=.o}


# this is the working target
# ==========================

${TARGET}: ${OBJFILES} 
	${CC} -o $@ ${OBJFILES}

help: print_help cleanup

print_help:
	echo  OBJFILES = ${OBJFILES}
	echo  SRCFILES = ${SRCFILES}
	cat ${INCLUDEFILE}
	

clean:cleanup
	-@ rm -f  *.o 

# we need to delete the INCLUDEFILE otherwise
# new source and header files won't be picked up
# next time
# =========
cleanup:
	@echo "### deleting ${INCLUDEFILE}"
	-@ rm -f  ${INCLUDEFILE}