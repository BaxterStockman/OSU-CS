#!/usr/bin/env perl

# Name: Matt Schreiber
# Email: schreibm@onid.oregonstate.edu
# Class: CS311-400
# Assignment: 5

use strict;
use warnings;
use utf8;
use Benchmark qw( timethese );
use Cwd qw( cwd );
use IPC::Exe qw( exe bg );

my(%args, @positionals) = &parse_args(\@ARGV, { "-f" => undef,
                                                "-c" => undef,
                                                "-p" => undef
                                              },
                                          0);

# Filename
my $fname = $args{'-f'};
# Number of times to run benchmark tests
my $count = $args{'-c'} // 1;
# Number of child processes
my $proc = $args{'-p'};

my @files = glob("test_files/*.txt");
my @procs = ( 1, 2, 3, 4, 5, 10, 15, 20 );

@files = ( $fname ) if defined $fname;
@procs = ( $proc ) if defined $proc;

my $dir = cwd();

for my $fname ( @files ) {
    for my $proc ( @procs ) {
        print "Running $count iterations of uniqify on $fname with $proc sorters...\n";
        timethese($count, {
            'uniqify.c' => sub { &{ exe "$dir/uniqify", "-n", "$proc", ([ '<', "$fname" ], [ ">#" ]) } },
            'uniqify.bash' => sub { &{ exe "$dir/uniqify.bash", ([ '<', "$fname" ], [ ">#" ]) } }
        });
        print "\n";
    }
}

sub parse_args {
    my($argv_ref, $arg_hash_ref, $num_positionals, @bad) = @_;
    my @args = my @positionals = @$argv_ref;
    foreach my $i ( 0..(@args - 1) ) {
        my $arg = $args[$i];
        if( exists $arg_hash_ref->{$arg} ) {
            eval {  if(! defined $arg_hash_ref->{$arg}) {
                        $arg_hash_ref->{$arg} = $args[$i + 1];
                        shift @positionals;
                        shift @positionals;
                        $i++;
                    } else {
                        $arg_hash_ref->{$arg} = 1;
                        shift @positionals;
                    }
            };
        }
    }
    &usage("unrecognized option") if @positionals > $num_positionals;
    return %$arg_hash_ref, @positionals;
}

sub usage {
    my($error, @bad) = @_;
    die( "$0: $error\n"
        . "Usage: time_uniqify.pl [-f FILE] [-c COUNT] [-p PROCESSES]\n"
        . "      -f  FILE        File to parse\n"
        . "      -c  COUNT       Number of iterations to run\n"
        . "      -p  PROCESSES   Number of sorter processses\n" );
}
