#!/usr/bin/env perl

#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 6

use strict;
use warnings;
use utf8;

my $file = shift or die "No such file: $!\n";

open my $fh, '<', $file or die "Failed to open $file: $!\n";

my($last, $stlast);
$last = $stlast = 2;

while(<$fh>) {
    while(/[^\d,](\d+)[^\d,]/g) {
        my $prime = $1;
        if($last == $prime - 2) {
            print "$last $prime\n";
        } elsif($stlast == $prime - 2) {
            print "$stlast $prime\n";
        }
        $stlast = $last;
        $last = $prime;
    }
}
