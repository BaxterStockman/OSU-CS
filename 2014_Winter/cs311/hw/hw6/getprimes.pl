#!/usr/bin/env perl

use strict;
use warnings;
use utf8;

my $file = shift or die "No such file: $!\n";

open my $fh, '<', $file or die "Failed to open $file: $!\n";

my($last, $stlast);
$last = $stlast = 2;

while(<$fh>) {
    while(/[^\d,](\d+)[^\d,]/g) {
        print "$1\n";
    }
}
