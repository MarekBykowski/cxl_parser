#!/usr/intel/bin/perl
#-----------------------------------------------------------------------
# Find sim to wall clock time
#-----------------------------------------------------------------------
use strict;
use warnings;
use List::Util qw(min max);

# process command line arguments
my $num_args = $#ARGV + 1;
if ($num_args < 2) {
    print "\nUsage: %s rt_time_log wc_time_log_file <optional skip_lines> ", $ARGV[0];
    exit;
}

my $rt_log  = $ARGV[0];         # wall-clock log
my $wc_log  = $ARGV[1];         # real-time (simulation) log

# Open file
open(my $wc_data, '<:encoding(UTF-8)', $wc_log)
    or die "Could not open file '$wc_log' $!";
my @wc_lines = <$wc_data>;
close $wc_data;

open(my $rt_data, '<:encoding(UTF-8)', $rt_log)
    or die "Could not open file '$rt_log' $!";
my @rt_lines = <$rt_data>;
close $rt_data;


my $wc_line;
my $rt_line;
my $wc_index = 0;
my $size_wc  = @wc_lines;
my $size_rt  = @rt_lines;
my $size;
my @wc_words;
my @rt_words;
my $wc_time;
my $rt_time;
my $clock_period = 1000;        # 1000 ps = 1 ns = 1 GHz
my $count = 1;
my $i;

$size = min($size_wc, $size_rt);

printf "Assumes 1 GHz with clock period of 1 ns or 1000 ps\n";

for ($i = 0; $i < $size;  $i++ ) {

    $wc_line     =  $wc_lines[$i];
    $rt_line     =  $rt_lines[$i];

    if ($wc_line =~ /^\s*$/)   { next; } # skip blank lines

#count=          1 r0w1= 0 req_ts=          0 rsp_ts=        409 delta_ts=        409
#count=          1 r0w1= 0 req_ts=  201616500 rsp_ts=  202087500 delta_ts=     471000

    @wc_words = split(' ', $wc_line);
    @rt_words = split(' ', $rt_line);    
    
    $wc_time = $wc_words[9];
    $rt_time = $rt_words[9];    
    
    printf ("count= %10d wc_time= %10d seconds, rt_time= %10d ps, cycles/sec= %f\n", $count++, $wc_time, $rt_time, $rt_time/$wc_time/1000 );

}
