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
CFLAGS_LIB=-Wall -Wconversion -fPIC -c
#`perl -e 'use Config; print \$Config{archlib};'`/CORE
# /usr/lib/perl/5.14.2/CORE
CFLAGS_SHARED=-shared
PERL_CORE=$(shell echo `perl -e 'use Config; print $$Config{archlib};'`/CORE/)

all: libsvm-perl.so

libsvm-perl.so:swig-binding.i svm-binding.c svm.cpp
	swig -perl swig-binding.i
	$(CC) $(CFLAGS_LIB) svm.cpp
	$(CC) $(CFLAGS_LIB) svm-binding.c
	$(CC) $(CFLAGS_LIB) -I$(PERL_CORE) swig-binding_wrap.c
	$(CC) $(CFLAGS_SHARED) svm.o svm-binding.o swig-binding_wrap.o -o libsvm.so

clean:
	rm -rf *.o *.so swig-binding_wrap.c

test:
	perl -I`pwd` test.pl

.PHONY: clean test

