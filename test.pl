#!/usr/bin/perl

use libsvm;

my $param = libsvm::new_param();

my $model = libsvm::load_model('sample.model');

my @set = (0.0424107142857143, 0.0915178571428571, 0.0401785714285714,
  0.0156250000000000, 0.0156250000000000, 0.0223214285714286,
  0.0223214285714286, 0.0825892857142857, 0.1205357142857140,
  0.0736607142857143, 0.0535714285714286, 0.0535714285714286,
  0.0178571428571429, 0.0357142857142857, 0.1116071428571430,
  0.0334821428571429, 0.0223214285714286, 0.0602678571428571,
  0.0200892857142857, 0.0647321428571429);

my $dataset = libsvm::new_dataset(1);

for(my $i = 0; $i < @set; $i++){
  print "$dataset,$i,$set[$i]\n";
  libsvm::set_attribute($dataset,$i,$set[$i]);
}

my $y = libsvm::predict($model,$dataset);

print "Predict $y\n";

libsvm::destroy_dataset($dataset);
libsvm::destroy_model($model);

if($y eq -10){
  print "Everything is OK!\nPassed testing and destroy calls\n";
}
else{
  print "Something is NOT ok!\nY should have returned -10, returned $y\n";
}
exit;

