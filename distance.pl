#!/usr/local/bin/perl

$victim_file = 'mem_trace.out';
$attacker_file = 'dcache_trace.out';
open(FILE, $file) or die("Could not open file!");
while(<FILE>){
	chomp;
	my $data = $_;
	my @values = split(';', $data);
	print "$values[0]\n";	

}
