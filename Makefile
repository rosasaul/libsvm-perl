# libsvm-perl swig bindings
#   Using libsvm-3.17, no midifications done to svm.cpp or svm.h so they should
#   be upgradable assuming no major interface changes.
#
# Author: Saul Rosa <rosa.saul@megaframe.org>
# Date: 05/09/2013
#
# If you have compile erros from unknown type add this
#   #include <stdint.h>
#   typedef __off64_t off64_t;
# to the file `perl -e 'use Config; print \$Config{archlib};'`/CORE/proto.h
# at the very begining of the file


CC=g++
CFLAGS_LIB=-Wall -Wconversion -03 -fPIC -c -I`perl -e 'use Config; print \$Config{archlib};'`/CORE
CFLAGS_SHARED=-shared

libsvm-perl.so:swig-binding.i swig-binding.h svm-binding.h svm-binding.c svm.h svm.cpp
	swig -perl swig-binding.i
	$(CC) $(CFLAGS_LIB) svm-binding.c swig-binding_wrap.c svm.cpp
	$(CC) $(CFLAGS_SHARED) svm-binding.o swig-binding_wrap.o svm.o -o libsvm-perl.so

clean:
	rm -rf *.o *.so swig-binding_wrap.c

test:
	perl -I`pwd` test.pl

.PHONY: clean test

