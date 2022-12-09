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
my $hiop_timestamp;
my $size_data  = @hiop_lines;
my @words;
my @address;
my $data;
my $i;
my $value;

#|      161648000|D|CfgWr1  |0|01:00:0 01000010|ff00_142|   1|0/0|1111_0000|         F1000000|h|2088/0100|000000|0076|00000|0|    xx| x   x|00000000|1|11|00000000   |  0|0699c|e254592fa63a892156930207|

for ($hiop_index = 0; $hiop_index < $size_data;  $hiop_index++ ) {

    $hiop_line     =  $hiop_lines[$hiop_index];
    if ($hiop_line =~ /^\s*$/)   { next; } # skip blank lines
    @words = split(/\|/, $hiop_line);
    # for ($i = 0; $i < $#words; $i++) {
    #     printf "%d  %s\n", $i, $words[$i];
    # }

    # 01:00:0 01000010
    @address = split ' ', $words[5];
    if ( ($words[3] =~ m/CfgWr1/) || ($words[3] =~ m/CfgRd1/) ) {
        $value = hex($address[1]);
        printf "address=%4x", ($value & 0x0000FFFF);
    }

    if ($words[3] =~ m/CfgWr1/) {
        #"          F1000000"
        $words[10] =~ m/\s*([0-9A-Fa-f]+)/;
        printf " WR-data= %x\n", hex($1);
        $hiop_index++;          # skip write completion
    } else {
        $hiop_index++;
        $hiop_line     =  $hiop_lines[$hiop_index];
        if (defined($hiop_line)) { # can be end of file
            @words = split(/\|/, $hiop_line);
            $words[10] =~ m/\s*([0-9A-Fa-f]+)/;        
            printf " RD-data= %08x\n", hex($1);
        }
    }
        
}
