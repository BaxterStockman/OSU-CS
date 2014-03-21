#!/usr/bin/env perl

# Matt Schreiber
# CS325 - Analysis of Algorithms
# Winter 2014
# Project 4 - The Traveling Salesman Problem

use lib qw( ./inc/lib/perl5 );

use strict;
use warnings;
# Stop perl from notifying about deep recursion
no warnings qw( recursion );
use utf8;

use Getopt::Long;
use Graph;
use Graph::Matching qw( max_weight_matching edges_from_Graph );
use List::Util qw( first min reduce sum );
use List::MoreUtils qw( pairwise );
use Math::Round;
use Parallel::ForkManager;

sub usage {
    my( $message, @bad ) = @_;
    print "$0: $message\n";
    print   "USAGE:   tsp.pl <options> <file>\n" .
            "OPTIONS:\n" .
            "             --nearest-neighbor | --nn         Use nearest-neighbor algorithm\n" .
            "             --minimum-spanning-tree | --mst   Use minimum-spanning-tree algorithm\n" .
            "             --christofides                    Use Christofides' Algorithm\n" .
            "             --ant-colony                      Use ant colony algorithm\n" .
            "             --processses PROCESSES            Number of processes\n" .
            "             --ants ANTS                       Number of ants\n" .
            "             --input FILE                      Specify an input file [--input flag is optional]\n" .
            "             --output FILE                     Specify an output file\n" .
            "             --help | --usage                  Print this message\n";
    die;
}

sub build_graph {
    my( $file, $graph, $cities, @bad ) = @_;

    open my $fh, '<', $file or die("$0: cannot open $file: $!\n");

    while( <$fh> ) {
        # Chomp trailing newline
        chomp;
        # Remove leading whitespace
        s/\A\s+//;
        # Split on remaining whitespace
        my($city1, $x1, $y1) = split /\s+/;

        $graph->add_vertex( $city1 );

        # Add edge from $city1 to each of the
        # cities in $cities
        while( my( $city2, $coords ) = each %$cities ) {
            my($x2, $y2) = ($coords->{'x'}, $coords->{'y'});
            my $distance = &distance( $x1, $y1, $x2, $y2 );
            $graph->add_weighted_edge( $city1, $city2, $distance );
        }

        # Add the new city to $cities
        $cities->{$city1} = {   'x' => $x1,
                                'y' => $y1  };
    }

    return $graph;
}

sub distance {
    my( $x1, $y1, $x2, $y2, @bad ) = @_;
    return round( sqrt( ($x1 - $x2) ** 2 + ($y1 - $y2) ** 2 ) );
}

sub map_over_edges {
    my( $graph, $callback, @callback_args ) = @_;
    my @returned = map { $callback->( @callback_args ) } $graph->edges;
    return \@returned;
}

# Implements a min-max ant colony
# Sources:
# http://en.wikipedia.org/wiki/Ant_colony_optimization_algorithms
# http://www.math.ucla.edu/~wittman/10c.1.11s/Lectures/Raids/ACO.pdf
# http://www.zemris.fer.hr/~golub/clanci/ea2011.pdf
# http://www.liacs.nl/assets/2012-08SjoerdvanEgmond.pdf
# http://www.agent.ai/doc/upload/200302/dori02.pdf
sub ant_colony_tour {
    my( $graph, $ants, $global_best_distance, $global_best_trail, $iterations, $new, @bad ) = @_;

    # Bail out if we've gone through all the iterations
    return $global_best_trail if $iterations <= 0;

    # alpha >= 0.  Pheromone weight - the lower this gets, the more
    # likely the ants are to visit the closest city.
    my $alpha = 1;
    # beta >= 0.  Heuristic weight - the lower this gets, the more
    # likely the ant is to choose the path with highest pheromone strength
    my $beta = 6;
    # Strengths for $alpha and $beta chosen based on experimental results
    # described at http://www.zemris.fer.hr/~golub/clanci/ea2011.pdf

    # Minimum and maximum strengths of pheromones
    my $min_strength = 1.0;
    my $max_strength = 4;

    # Maximum number of ants -- set to 30 unless
    # specified in call
    my $max_ants = 30;
    $ants = $max_ants unless $ants > 0;

    # Coefficient of pheromone evaporation
    my $evaporation = 0.2;

    # Set initial pheromone strength
    if( $new ) {
        map { $graph->set_edge_attribute( @$_, "pheromone", $max_strength ) } $graph->edges;
    }

    # variable to hold paths returned by worker processes
    my @paths;
    # Spawn ants
    my $pm = Parallel::ForkManager->new( $ants );
    $pm->set_max_procs( $max_ants );
    # Set up data structure retrieval from children
    # Adapted from example at
    # http://search.cpan.org/~szabgab/Parallel-ForkManager-1.06/lib/Parallel/ForkManager.pm#RETRIEVING_DATASTRUCTURES_from_child_processes
    $pm->run_on_finish(
        sub{
            my( $pid, $exit_code, $ident, $exit_signal, $core_dump, $ds_ref) = @_;
            if( defined $ds_ref ) {
                my $path = ${$ds_ref};
                push @paths, $path;
            } else {
                print STDERR "$0: worker ant process $pid did not return data\n";
            }
        }
    );

    # Release the ants!
    for ( 1..$ants ) {
        my $pid = $pm->start and next;

        # Select random root
        my $root = ($graph->vertices)[int( rand $graph->vertices )];

        my %visited;
        my @vertices;
        # Traverse the graph
        &traverse_ant( $graph, $root, \%visited, \@vertices, $alpha, $beta );

        # Generate distances on individual legs
        my $distances = &path_distances( $graph, \@vertices );
        # Get distance of whole path
        my $distance = sum( @$distances );

        # Create hashref with $distance as key
        # and the path traversed as value
        my $trail = { $distance => \@vertices };

        # Send $path to parent and terminate
        $pm->finish( 0, \$trail );
    }

    # Wait for worked processes to return
    $pm->wait_all_children;

    # Update pheromone strength
    #
    # For the ant with the shortest trail,
    # add delta(t) pheromone to each arc.
    #
    # delta(t): 1/L(t), where L(t) is length of tour,
    # 0 otherwise.
    my %trails = map {
        my $key = (keys %$_)[0];
        $key, $_->{$key}
    } @paths;

    my $best_distance = min keys %trails;
    my $best_trail = $trails{$best_distance};

    # If the most recent trail is the best, or we're on
    # the first run, update global_best variables
    if( $new || $best_distance < $global_best_distance ) {
        $global_best_distance = $best_distance;
        $global_best_trail = $best_trail;
    }

    # Simpl(e|istic) means of increasing the likelihood that
    # the global_best path is chosen as iterations
    # proceed.  Increase $threshold to encourage more
    # exploration.
    my $threshold = 0.6;
    if( ((rand 1) + (1 / $iterations)) > $threshold ) {
        &update_pheromones( $graph, $global_best_distance, $global_best_trail, $evaporation, $min_strength, $max_strength );
    } else {
        &update_pheromones( $graph, $best_distance, $best_trail, $evaporation, $min_strength, $max_strength );
    }

    # Make recursive call with updated graph
    $iterations--;
    &ant_colony_tour( $graph, $ants, $global_best_distance, $global_best_trail, $iterations, 0 );
}

sub update_pheromones {
    my( $graph, $distance, $trail, $evaporation, $min_strength, $max_strength ) = @_;

    # Evaporate pheromones
    #
    # For every arc in the graph, multiply
    # current pheromone strength by (1 - p),
    # where 1 < p <= 1 is the evaporation coefficient
    foreach my $edge ( $graph->edges ) {
        my $strength = $graph->get_edge_attribute( @$edge, "pheromone" );
        my $weaken = $strength * (1 - $evaporation);
        $weaken = $min_strength if $weaken < $min_strength;
        $graph->set_edge_attribute( @$edge, "pheromone", $weaken );
    }

    # Copy list of in-order vertices
    my @shadow = @$trail;
    # Move first vertex in tour to back of @shadow
    my $first = shift @shadow;
    push @shadow, $first;

    # Augment the strength of the pheromones
    # on edges that the ant visited
    pairwise {
        my $strength = $graph->get_edge_attribute( $a, $b, "pheromone" );
        my $augment = $strength + (1 / $distance);
        $augment  = $max_strength if $augment > $max_strength;
        $graph->set_edge_attribute( $a, $b, "pheromone", $augment )
    } @$trail, @shadow;
}

sub traverse_ant {
    my( $graph, $root, $visited, $vertices, $alpha, $beta, @bad ) = @_;

    # Store root node in list representing path
    push @$vertices, $root;
    $visited->{$root} = 1;

    # Create list of unvisited vertices
    my @unvisited = grep { ! $visited->{$_} } $graph->neighbors( $root );
    return unless @unvisited;

    my %probabilities;
    foreach my $vertex ( @unvisited ) {
        my $distance = $graph->get_edge_weight( $root, $vertex );
        my $pheromone = $graph->get_edge_attribute( $root, $vertex, "pheromone" );
        $probabilities{$vertex} = (($pheromone ** $alpha) * ((1 / $distance) ** $beta));
    }

    my $new_root = &get_rand_by_weight( \%probabilities );

    &traverse_ant( $graph, $new_root, $visited, $vertices, $alpha, $beta );
}

sub get_rand_by_weight {
    my( $possibilities, @bad ) = @_;
    my( $weight_sum, $running_weight );

    my @key_order = sort{ $possibilities->{$a} <=> $possibilities->{$b} } keys %$possibilities;

    foreach my $key ( @key_order ) {
        $weight_sum += $possibilities->{$key};
    }

    # Choose random value in range of $weight_sum
    my $rand = rand( $weight_sum );

    # Return the $key if the random value chosen
    # is within the range covered by the $weight
    # corresponding to the $key
    while( my( $key, $weight ) = ( each %$possibilities ) ) {
        return $key if ($running_weight += $weight) >= $rand;
    }
}

# Adapted from description, pseudocode, and visuals presented at
# http://en.wikipedia.org/wiki/Christofides_algorithm
sub christofides_tour {
    my( $graph, @bad ) = @_;

    # Get minimum spanning tree of $graph
    my $tree = $graph->minimum_spanning_tree;

    # Find all nodes of even degree
    my @evens = grep { $tree->degree($_) % 2 == 0 } $tree->vertices;

    # Create a graph with all nodes of even degree removed
    my $odd_graph = $graph->deep_copy_graph;
    map { $odd_graph->delete_vertex($_) } @evens;

    # Create array ref of array refs of the form [[v1, v2, weight * -1] ... ],
    # where v1 and v2 are vertices in $odd_graph, and 'weight' is the
    # weight of the edge between them. This create a data structure of the
    # appropriate form to pass to max_weight_matching().
    #
    # We negate the edge weights so that calling max_weight_matching()
    # produces a minimum weight perfect matching  -- since a maximum weight
    # matching in a graph with given weights for each edge is a minimum
    # weight matching in a graph which is identical except that all edge
    # weights are negated.
    my @neg_odd_graph;
    foreach my $edge ( $odd_graph->edges ) {
        my ($v1, $v2) = @$edge;
        my $weight = $odd_graph->get_edge_weight($v1, $v2);
        my $array_ref = [$v1, $v2, $weight];
        push @neg_odd_graph, [$v1, $v2, -1 * $weight];
    }

    # Create maximum matching
    my %matching = max_weight_matching(\@neg_odd_graph, 1);

    # Add the edges from the matching into the
    # minimum spanning tree
    while( my( $v1, $v2 ) = each %matching ) {
        my $weight = $odd_graph->get_edge_weight($v1, $v2);
        $tree->add_weighted_edge($v1, $v2, $weight)
    }

    # Traverse the tree
    my %visited;
    my @vertices;
    my $root = ($tree->vertices)[0];
    &traverse_preorder( $tree, $root, \%visited, \@vertices );
    return \@vertices;
}

# Adapted from description and pseudocode at
# http://en.wikipedia.org/wiki/Nearest_neighbour_algorithm
sub nearest_neighbor_tour {
    my( $graph, @bad ) = @_;

    my %visited;
    my @vertices;
    # Select random vertex
    my $root = ($graph->vertices)[int( rand $graph->vertices )];

    &traverse_greedy( $graph, $root, \%visited, \@vertices );
    return \@vertices;
}

sub traverse_greedy {
    my( $graph, $root, $visited, $vertices, @bad ) = @_;

    # Store root in list representing the path
    push @$vertices, $root;
    $visited->{$root} = 1;

    # Create list of unvisited vertices
    my @unvisited = grep { ! $visited->{$_} } $graph->neighbors($root);
    return unless @unvisited;

    # Find the edge with the smallest weight
    my $new_root = reduce { $graph->get_edge_weight( $root, $a ) <
                            $graph->get_edge_weight( $root, $b ) ?
                            $a : $b } @unvisited;

    # Make recursive call with $new_root as
    # $root argument
    &traverse_greedy( $graph, $new_root, $visited, $vertices );
}

# I discovered this algorithm in the materials for
# Walt Mankowski's presentation to the Pittsburgh
# Perl Workshop entitled "Approximation Algorithms
# in Perl".  The materials are available at
# http://www.mawode.com/~waltman/talks/approx_ppw.pdf
#
# Because I didn't come up with this on my own
# (although I did spend much time studying the
# algorithm to make sure I understand what's
# going on), I have set this program's default
# algorithm to something else.
sub mst_tour { my( $graph, @bad ) = @_;
    my $tree= $graph->minimum_spanning_tree;

    my %visited;
    my @vertices;
    my $root = ($tree->vertices)[0];
    &traverse_preorder( $tree, $root, \%visited, \@vertices );
    return \@vertices;
}

sub traverse_preorder {
    my( $tree, $root, $visited, $vertices, @bad ) = @_;

    push @$vertices, $root;
    $visited->{$root} = 1;
    foreach my $neighbor( $tree->neighbors($root) ) {
        next if $visited->{$neighbor};
        &traverse_preorder( $tree, $neighbor, $visited, $vertices );
    }
}

sub path_distances {
    my( $graph, $vertices, @bad ) = @_;

    # Copy list of in-order vertices
    my @shadow = @$vertices;
    # Move first vertex in tour to back of @shadow
    my $first = shift @shadow;
    push @shadow, $first;

    # Return an array whose members represent the weights
    # of the edges connecting vertices in corresponding
    # indices in @$vertices and @shadow
    my @distances = pairwise { $graph->get_edge_weight( $a, $b ) } @$vertices, @shadow;
    return \@distances;
}

sub has_all_vertices {
    my( $graph, $visited, @bad ) = @_;
    foreach my $vertex ( $graph->vertices ) {
        unless( $visited->{$vertex} ) {
            return 0;
        }
    }
    return scalar $graph->vertices;
}

sub has_correct_edge_count {
    my( $graph, @bad ) = @_;

    my $vertex_count = scalar $graph->vertices;

    # Formula for number of edges in complete graph
    # of 'n' vertices is e(n) = (n * (n - 1)) / 2
    my $edge_count = ($vertex_count * ($vertex_count - 1)) / 2;
    return $graph->edges == $edge_count ? $edge_count : 0;
}

# Hash of references to choice algorithm functions
my %algorithms = (
    'nearest_neighbor_tour' => \&nearest_neighbor_tour,
    'mst_tour' => \&mst_tour,
    'christofides_tour' => \&christofides_tour
);

my $algorithm_name = "nearest_neighbor_tour";
my $in_file;
my $out_file = \*STDOUT;
my $procs = 12;
my $ants = 30;
my $ant_iterations = 15;
my $max_procs = 30;

# Array to store hashrefs returned from children
my @paths;

GetOptions(
    "nn|nearest-neighbor" => sub { $algorithm_name = "nearest_neighbor_tour" },
    "mst|minimum-spanning-tree|spanning-tree" => sub { $algorithm_name = "mst_tour" },
    "christofides" => sub { $algorithm_name = "christofides_tour" },
    "ant-colony" => sub { $algorithm_name = "ant_colony_tour" },
    "input=s" => \$in_file,
    "output=s" => \$out_file,
    "processes=i" => \$procs,
    "ants=i" => \$ants,
    "help|usage" => \&usage
);

$in_file = shift or usage( "no input file specified" ) unless $in_file;
usage( "process count must be from 1 to $max_procs" ) unless $procs >= 1 and $procs <= $max_procs;

# Only use 1 process if we're using the ant colony algorithm
$procs = 1 if $algorithm_name eq "ant_colony_tour";

# Create and build graph
my $graph = Graph::Undirected->new;
my %cities;
&build_graph($in_file, $graph, \%cities);

my $pm = Parallel::ForkManager->new( $procs );
$pm->set_max_procs( $max_procs );
# Set up data structure retrieval from children
# Adapted from example at
# http://search.cpan.org/~szabgab/Parallel-ForkManager-1.06/lib/Parallel/ForkManager.pm#RETRIEVING_DATASTRUCTURES_from_child_processes
$pm->run_on_finish(
    sub{
        my( $pid, $exit_code, $ident, $exit_signal, $core_dump, $ds_ref) = @_;
        if( defined $ds_ref ) {
            my $path = ${$ds_ref};
            push @paths, $path;
        } else {
            print STDERR "$0: child process $pid did not return data\n";
        }
    }
);

for ( 1..$procs ) {
    my $pid = $pm->start and next;

    my $tsp;
    # Run chosen algorithm on graph
    unless( $algorithm_name eq "ant_colony_tour" ) {
        $tsp = ($algorithms{$algorithm_name})->( $graph );
    } else {
        $tsp = &ant_colony_tour( $graph, $ants, undef, undef, $ant_iterations, 1 );
    }

    # Generate distances on individual legs
    my $distances = &path_distances( $graph, $tsp );
    # Get distance of whole path
    my $distance = sum( @$distances );

    # Create hashref with $distance as key
    # and $tsp (i.e. the path) as value
    my $path = { $distance => $tsp };

    # Send $path to parent and terminate
    $pm->finish( 0, \$path );
}

# Wait for child processes to return
$pm->wait_all_children;

my %tours = map {
    my $key = (keys %$_)[0];
    $key, $_->{$key}
} @paths;

my $best_distance = min keys %tours;
my $best_tsp = $tours{$best_distance};

print $out_file "$best_distance\n";
print $out_file map { "$_\n" } @$best_tsp;
