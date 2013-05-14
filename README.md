libsvm-perl
===========

Thread safe swig implementation of [libsvm](http://www.csie.ntu.edu.tw/~cjlin/libsvm) perl bindings. 
Perl bindings in CPAN for libsvm break on compile (can be fixed easily), but are not thread safe.
Memory isn't force cleared, and the binding C file has a hard defined svm/param variables defined which means
if you use pthreads they will have memory colisions.

## Dependencies

* Perl - This was built/tested with 5.14.2 (you should be ok with 5.8.8 or better)

* Swig 2.0 (didn't try building with anything newer than this).

* g++ (tested with version 4.7.2)

## Bugs
For bug reports and issues use the [github issues page](https://github.com/rosasaul/libsvm-perl/issues), try to make sure I'll be able to reproduce the issue.
* If you have compile erros from 'unknown type' try adding the following to proto.h 
  (perl binding files, find the path with ```echo `perl -e 'use Config; print \$Config{archlib};'`/CORE/proto.h```)
```C
#include <stdint.h>
typedef __off64_t off64_t;
```

## Installation
To install follow standard perl module install

    $ perl Makefile.PL
    $ make
    $ make test
    $ make install

## Usage
See the test.pl file for use cases.

Loading the library
```perl
use libsvm;
```

Defined a param object (stores things like what type of kernel you have
```perl
my $param = libsvm::new_param();
```

Load a model File (include path if needed)
```perl
my $model = libsvm::load_model('sample.model');
```

## Copyright and Licence
libsvm-perl is a libsvm swig perl binding that is thread safe.
Copyright (C) 2013 Saul Rosa

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Author Information
This module was written by Saul Rosa <rosa.saul@megaframe.org>.

I'm an electrical engineer who uses a lot of code to get my work done. The 
modules and wrappers were written to address the shortcommings of libsvm 
(it's non-threaded and I need to be able to pause the job and resume later on 
another machine) while making it perl native so I can integrate it into other 
data analysis tools.

## Acknowledgements
Thanks to the authors of [libsvm](http://www.csie.ntu.edu.tw/~cjlin/libsvm).

