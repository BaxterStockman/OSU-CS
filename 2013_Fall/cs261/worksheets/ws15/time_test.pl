#!/usr/bin/env perl

use strict;
use warnings;
use utf8;

#die("Usage: time_test [program] [initial size] [maximum number]") unless (@ARGV <= 3);
die("Usage...") if(not @ARGV);
my $cmd = shift @ARGV // "./time_test";
my $init_size = shift @ARGV // 5;
my $max = shift @ARGV // 5;

open my $my_fh, ">>", "time_results.txt" or die $!;

say $my_fh `$cmd $init_size $max` until(($max *=2) >= 1000);

