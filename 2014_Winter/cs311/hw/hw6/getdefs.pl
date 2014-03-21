#!/usr/bin/env perl

#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 6

use strict;
use warnings;
use utf8;

my %defines;
my $include = shift or die "No include file specified";
my $limit = shift or die "No macro specified";
$limit = uc $limit;

&get_defs($include, \%defines);

eval {
    no warnings 'all';
    if(defined (my $out = $defines{$limit})) {
        print "$out\n";
    }
};

sub get_defs {
    my($include, $defs, @bad) = @_;
    my $file = "/usr/include/$include";
    open my $fh, '<', $file or die "$file: no such file: $!\n";
    while(<$fh>) {
        # Deep recursion problem here: too many open
        # files.  Need to find a way of closing the
        # current file, then reopening to do the
        # parsing for #defines.
        #if(/\A#\s*include\w*\s+[<"](.*)[>"]/) {
        #    &get_defs($1, $defs);
        #}
        if(/\A#\s*define\s+(\w*)\s+(.+)/) {
            $defs->{$1} = $2;
        }
    }
    foreach my $key (keys %$defs) {
        my $value = $defs->{$key};
        next if $value =~ /\A-?\d+\z/;
        $value =~ /(\w+)/;
        my $embed = $1;
        if(defined $embed) {
            my $substit = $embed;
            while(defined $defs->{$embed}) {
                $embed = $defs->{$embed};
            }
            next if $embed !~ /\A-?\d+(U*L*)\z/;
            $value =~ s/$substit/$embed/;
            $value =~ s/U*|L*//g;
            $value = eval "no warnings 'all'; $value";
        }
        $defs->{$key} = $value;
    }
}
