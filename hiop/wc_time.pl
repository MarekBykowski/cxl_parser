#!/usr/intel/bin/perl
#-----------------------------------------------------------------------
# Process WallClock (wc) tracker log into time taken by each transaction
#-----------------------------------------------------------------------
use strict;
use warnings;

# process command line arguments
my $num_args = $#ARGV + 1;
if ($num_args < 1) {
    print "\nUsage: %s wc_log_file <optional skip_lines> ", $ARGV[0];
    exit;
}

my $wc_log   = $ARGV[0];
my $skip_lines = 13;            # default header lines

# Open file
open(my $wc_data, '<:encoding(UTF-8)', $wc_log)
    or die "Could not open file '$wc_log' $!";
my @wc_lines = <$wc_data>;
close $wc_data;


my $wc_line;
my $wc_index = 0;
my $size_data  = @wc_lines;
my @words;
my $req_timestamp;
my $rsp_timestamp;
my $r0w1;
my $count = 1;

for ($wc_index = 0; $wc_index < $size_data;  $wc_index++ ) {

    $wc_line     =  $wc_lines[$wc_index];
    if ($wc_line =~ /^\s*$/)   { next; } # skip blank lines
    @words = split(' ', $wc_line);

#REQ: [09-14 00:21:19 RT: 000000 sec] pkt#=          1 packet_type=  1 sim_type=  1 (  SIMICS) ADRS= 0000000000000000 r0w1= 0 BYTES=    4 
    if ($words[0] =~ m/REQ/) {
        $req_timestamp = $words[4];
        $r0w1 = $words[17];

        next;
    }

#RSP: [09-14 00:28:08 RT: 000409 sec] pkt#=          1 packet_type=  1 sim_type=  1 (  SIMICS) ADRS= 0000000000000000 r0w1= 0 BYTES=    4  data: 67 C6 69 73 
    if ($words[0] =~ m/RSP/) {
        $rsp_timestamp = $words[4];
        printf ("count= %10d r0w1= %d req_ts= %10d rsp_ts= %10d delta_ts= %10d\n", $count++, $r0w1, $req_timestamp, $rsp_timestamp, $rsp_timestamp-$req_timestamp);
    }
}
