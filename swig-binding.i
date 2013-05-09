/* svm_bindings.i */
%module libsvm
%{
#include "svm.h"
#include "svm-binding.h"
#include "swig-binding.h"
%}

%include "swig-binding.h"

