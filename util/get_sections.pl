#!/usr/bin/perl

use strict;
use warnings;

my $sections = `objdump -s \`find kernel/obj -type f\` | grep "Contents of section"`;
my @lines = split('\n', $sections);
my %distinct = ();

while (scalar @lines > 0) {
	my $line = shift @lines;
	if (!defined $distinct{$line}) {
		$distinct{$line} = 1;
	}
}

foreach my $key (keys %distinct) {
	print "$key\n";
}
