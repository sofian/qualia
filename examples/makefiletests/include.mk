### creating include.mk
SRCFILES = dummy.cpp foo.cpp main.cpp
HFILES = dummy.h foo.h
# DO NOT DELETE

foo.o: foo.h /usr/include/stdio.h /usr/include/_types.h
foo.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
foo.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
main.o: /usr/include/stdio.h /usr/include/_types.h /usr/include/sys/_types.h
main.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
main.o: /usr/include/i386/_types.h foo.h lib/goo.h
