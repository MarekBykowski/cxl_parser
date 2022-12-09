#!/usr/intel/bin/perl
#-----------------------------------------------------------------------
# Converts HIOP tracker log into address data for each transaction
#-----------------------------------------------------------------------
use strict;
use warnings;

# process command line arguments
my $num_args = $#ARGV + 1;
if ($num_args < 1) {
    print "\nUsage: %s hiop_log_file <optional skip_lines> ", $ARGV[0];
    exit;
}

my $hiop_log   = $ARGV[0];
my $skip_lines = 13;            # default header lines

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
my $size_data  = @hiop_lines;
my @words;
my $req_timestamp;
my $rsp_timestamp;
my $r0w1;
my $count = 1;

for ($hiop_index = 0; $hiop_index < $size_data;  $hiop_index++ ) {

    $hiop_line     =  $hiop_lines[$hiop_index];
    if ($hiop_line =~ /^\s*$/)   { next; } # skip blank lines
    @words = split(/\|/, $hiop_line);

#|      201616500|D|CfgRd1  |0|01:00:0 01000000|ff00_06c|   1|0/0|1111_0000|                 |h|2088/0100|000000|002f|00000|0|    xx| x   x|00000000|0|xx|00000000   |  0|017bb|000000000000000000000000|

    # $words[2] = 201616500 
    if ( ($words[3] =~ m/CfgWr1/) || ($words[3] =~ m/CfgRd1/) ) {
        $req_timestamp = $words[1];

        if ($words[3] =~ m/CfgWr1/ ) {
            $r0w1 = 1;
        } else {
            $r0w1 = 0;
        }

        next;
    }

#|      202087500|U|CplD    |0|SUC_CPL 00000000|ff00_06c|   1|0/0|0000_0100|         7369C667|-|0000/0000|000000|0000|00000|0|      |      |00000000|1|00|00000000   |  0|00000|000000000000000000000000|
    if ( ($words[3] =~ m/Cpl/) || ($words[3] =~ m/CplD/)) {
        $rsp_timestamp = $words[1];
        printf ("count= %10d r0w1= %d req_ts= %10d rsp_ts= %10d delta_ts= %10d\n", $count++, $r0w1, $req_timestamp, $rsp_timestamp, $rsp_timestamp-$req_timestamp);
    }
}
