#!/usr/local/bin/perl

$victim_file = 'mem_trace.out';
$attacker_file = 'dcache_trace.out';
open(V_FILE, $victim_file) or die("Could not open file!");
open(A_FILE, $attacker_file) or die("Could not open file!");


while(<A_FILE>){
	chomp;
	my $a_data = $_;
	my $v_data = <V_FILE>;
	
	my @values = split(';', $data);
	print "$values[0]\n";	

}
