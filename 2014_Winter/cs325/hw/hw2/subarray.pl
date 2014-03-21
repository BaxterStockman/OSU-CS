#!/usr/bin/env perl

use strict;
use warnings;
use utf8;
use List::Util qw( sum max );
use Benchmark qw( timethis timethese );
use Data::Dumper qw( Dumper );

my(%args, @positionals) = &parse_args(\@ARGV, { "-f" => undef,
                                                "-e" => 0 });

# Set to 1 to enable error checking
our $err_check = $args{'-e'};
# Optional filename
my $fname = $args{'-f'};

# Number of random elements to create
my @extents =   ( 100, 200, 300, 400, 500, 600, 700, 800, 900,
                  1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000 );
# Upper limit on size of random numbers generated
my $lim = 1000000;
# Number of times to run benchmark tests
my $count = 100;

# If a filename was given, parse the
# file for number lists and output the
# maximum subarray sums as computed
# by the four algorithms.
if( defined($fname) ) {
    local $Data::Dumper::Indent = 0;
    local $Data::Dumper::Terse = 1;
    my %arrays = &parse_file_lists($fname);
    print "-" x 79 . "\n";
    foreach my $key (keys %arrays) {
        my $array = $arrays{$key};
        print Dumper($array) . "\n\n";
        print "Correct sum: $key\n";
        print "  algo1: " . &algo1($array) . "\n";
        print "  algo2: " . &algo2($array) . "\n";
        print "  algo3: " . &algo3($array) . "\n";
        print "  kadane: " . &kadane($array) . "\n";
        print "-" x 79 . "\n";
    }
} else {
# Create hash with number of elements
# as keys and references to lists of
# those sizes as values.
    my %arrays = map { $_ => &gen_rand_ints($lim, $_) } @extents;
    my %results;
    my $mark;
    foreach my $key (sort{$a <=> $b} keys %arrays) {
        my $array = $arrays{$key};
        print "Checking execution speed for an array of $key random elements:\n";
        $mark = timethese($count, {
                    'algo1' => sub { &algo1($array) if $key <= 900 },
                    'algo2' => sub { &algo2($array) },
                    'algo3' => sub { &algo3($array) },
                    'kadane' => sub { &kadane($array) }
                });
        $results{$key} = $mark;
    }
}


sub algo1 {
    my $array_ref;
    if( $err_check ) {
        ($array_ref, my @bad) = @_;
        die "$0: too many arguments to algo1\n" if @bad;
    } else {
        $array_ref = shift;
    }

    my $len = @$array_ref;
    my $max = 0;

    foreach my $i ( 0..$len-1 ) {
        foreach my $j ( $i..$len-1 ) {
            my $sum = sum(@$array_ref[$i..$j]);
            $max = $sum > $max ? $sum : $max;
        }
    }

    return $max;
}

sub algo2 {
    my $array_ref;
    if( $err_check ) {
        ($array_ref, my @bad) = @_;
        die "$0: too many arguments to algo2\n" if @bad;
    } else {
        $array_ref = shift;
    }

    my $len = @$array_ref;
    my $max = 0;

    foreach my $i ( 0..$len-1 ) {
        my $so_far = $array_ref->[$i];
        foreach my $j ( ($i+1)..($len-1) ) {
            $so_far += $array_ref->[$j];
            $max = $so_far > $max ? $so_far : $max;
        }
    }

    return $max;
}

sub algo3 {
    my $array_ref;
    if( $err_check ) {
        ($array_ref, my @bad) = @_;
        die "$0: too many arguments to algo3\n" if @bad;
    } else {
        $array_ref = shift;
    }

    my $len = @$array_ref;

    # If the array has only one element,
    # return that element if it is greater
    # than 0; otherwise return 0.
    if($len == 1) {
        return $array_ref->[0] > 0 ? $array_ref->[0] : 0;
    }

    # Split array in half
    my $half = $len / 2;
    my @left = @$array_ref[0..($half - 1)];
    my @right = @$array_ref[$half..($len - 1)];
    my $max_left = my $max_right = 0;

    # Find largest subarray that includes the
    # last element in the first half of the
    # original array
    my $half_sum;
    foreach my $left_elem (reverse @left) {
        $half_sum += $left_elem;
        $max_left = $half_sum if $half_sum > $max_left;
    }

    # Find the largest subarray that includes
    # the first element in the right half of
    # the original array
    $half_sum = 0;
    foreach my $right_elem (@right) {
        $half_sum += $right_elem;
        $max_right = $half_sum if $half_sum > $max_right;
    }

    # Return the max of the sums of (1) the largest
    # subarray that exists solely in the left subhalf,
    # (2) the largest subarray that exists solely in the
    # right subhalf, and (3) the largest subarray that
    # overlaps the halves.
    return max( max( &algo3( \@left ), &algo3( \@right ) ), $max_left + $max_right );
}

# Kadane's algorithm.
# Adapted from example in Python given at
# http://en.wikipedia.org/wiki/Maximum_subarray_problem
sub kadane {
    my $array_ref;
    if( $err_check ) {
        ($array_ref, my @bad) = @_;
        die "$0: too many arguments to kadane\n" if @bad;
    } else {
        $array_ref = shift;
    }

    my $max_ending_here = my $max = 0;
    foreach my $elem ( @$array_ref ) {
        $max_ending_here = max(0, $max_ending_here + $elem);
        $max = max($max, $max_ending_here);
    }

    return $max;
}

# To be used with the file 'mstest.txt', or any
# file in the same format.
sub parse_file_lists {
    my($fname, @bad) = @_;
    die "$0: too many arguments to parse_file_lists()\n" if @bad and $err_check;

    open my $fh, '<', $fname or die "$0: failed opening $fname: $!\n";

    # Create a hash whose keys are the largest
    # subarray sum as given in the file, and whose
    # values are the lists from the file.
    my %arrays;
    eval {
        while( <$fh> ) {
            /\A     # line-start anchor
            \[      # opening bracket of number list
            (.*)    # comma-separated list of numbers (capture group $1)
            \]      # closing bracket of number list
            ,\s+    # comma and whitespace separating list from sum
            (\d+)   # maximum subarray sum (capture group $2)
            \Z      # line-end anchor
            /x;
            $arrays{$2} = [split(', ', $1)];
        }
    };

    return %arrays;
}

sub gen_rand_ints {
    my($lim, $num, @bad) = @_;
    my $subr = (caller(0))[3];
    die "$0: too many arguments to $subr\n" if @bad and $err_check;

    # rand() invoked without an argument returns a number
    # from 0 to 1.  This is used with the ternary operation
    # to randomly produce either a positive or negative number.
    my @array = map{ (rand > 0.5 ? 1 : -1) * int( rand($lim) ) } (1..$num);
    return \@array;
}

sub parse_args {
    my($argv_ref, $arg_hash_ref, @bad) = @_;
    my @args = my @positionals = @$argv_ref;
    foreach my $i ( 0..(@args - 1) ) {
        if( exists $arg_hash_ref->{$args[$i]} ) {
            eval {  if(! defined $arg_hash_ref->{$args[$i]}) {
                        $arg_hash_ref->{$args[$i]} = $args[$i + 1];
                        shift @positionals;
                        shift @positionals;
                        $i++;
                    } else {
                        $arg_hash_ref->{$args[$i]} = 1;
                        shift @positionals;
                    }
            };
        }
    }
    return %$arg_hash_ref, @positionals;
}
