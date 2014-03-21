#!/usr/bin/env perl

use strict;
use warnings;
use utf8;


my %count;
my $sum;

while(<>) {
    chomp;
    foreach my $str (split /[^'\w]/) {
        $count{$str}++;
    }
}

foreach my $key (sort keys %count) {
    print "$key -> $count{$key}\n";
    $sum++;
}

print "There are $sum unique words in the file.\n";
