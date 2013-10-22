#!/usr/bin/env perl

use strict;
use warnings;
use utf8;


my $cmd = shift @ARGV // "./prog";
my $max = shift @ARGV // 1000;

open my $fh, ">", "$cmd" . ".out" or die $!;

while($max <=256000) {
    say $fh `$cmd $max`;
    $max *= 2;
}
