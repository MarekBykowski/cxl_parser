#!/usr/intel/bin/perl
#-----------------------------------------------------------------------
# Converts HIOP tracker log into address data for each transaction
#-----------------------------------------------------------------------
use strict;
use warnings;

# (1) quit unless we have the correct number of command-line args
my $num_args = $#ARGV + 1;
if ($num_args < 1) {
    print "\nUsage: %s hiop_log_file <optional skip_lines> ", $ARGV[0];
    exit;
}

# (2) we got two command line args
my $hiop_log   = $ARGV[0];
my $skip_lines = 0;            # default header lines

# Open file
open(my $hiop_data, '<:encoding(UTF-8)', $hiop_log)
    or die "Could not open file '$hiop_log' $!";
my @hiop_lines = <$hiop_data>;
close $hiop_data;

if (defined($ARGV[1])) {
    $skip_lines = $ARGV[1];
}

for (my $i = 0; $i < $skip_lines; $i++) {
    shift @hiop_lines;
}

my $hiop_line;
my $hiop_index = 0;
my $hiop_timestamp;
my $size_data  = @hiop_lines;
my @words;

my $req;
my $adrs;
my $r0w1;
my $size;
my @data;

#REQ: [11-10 20:37:50 RT: 000000 sec] pkt#=          1 packet_type=  1 sim_type=  1 (  SIMICS) ADRS= 0000000000000000 r0w1= 0 BYTES=    4 
#RSP: [11-10 20:41:22 RT: 000212 sec] pkt#=          1 packet_type=  1 sim_type=  1 (  SIMICS) ADRS= 0000000000000000 r0w1= 0 BYTES=    4  data[0:3]: CD 17 00 01 
#REQ: [11-11 00:45:20 RT: 014850 sec] pkt#=        301 packet_type=  1 sim_type=  1 (  SIMICS) ADRS= 0000000000000010 r0w1= 1 BYTES=    4  data[0:3]: FF FF FF FF 
#RSP: [11-11 00:46:11 RT: 014901 sec] pkt#=        301 packet_type=  1 sim_type=  1 (  SIMICS) ADRS= 0000000000000010 r0w1= 1 BYTES=    4 


for ($hiop_index = 0; $hiop_index < $size_data;  $hiop_index++ ) {

    $hiop_line     =  $hiop_lines[$hiop_index];
    if ($hiop_line =~ /^\s*$/)   { next; } # skip blank lines

    $hiop_line =~ m/ADRS= (\w+)/;
    $adrs = hex($1);
    printf "ADRS= %8x", $adrs;

    $hiop_line =~ m/r0w1= (\d)/;
    $r0w1 = $1;
    printf "r0w1= %1d", $r0w1;

    $hiop_line =~ m/BYTES=\s+(\d)/;
    $size = $1;
    printf "size= %2d", $size;

    if ($hiop_line =~ m/REQ:/)
        $req = 1;
    else
        $req = 0;               # rsp

    if ($r0w1)
        
        
    
}
