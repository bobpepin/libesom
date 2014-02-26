#!/usr/bin/perl

@headercols = 0..8;

@idregex = (q/^KH([CP])_(\d+)_(\d+)/);
$label = q|KH_%d_%d|;

@headers = split /\t/, <>;

$org = {};

@classes = ([935, 921, 875, 856, 968],
	    [991, 982, 981, 977, 975, 967, 966]);

my $col = -1;
foreach my $hdr (@headers) {
    $col++;
    next if grep { $_ == $col } @headercols;
    foreach my $re (@idregex) {
	($group, $sample, $replica) = ($hdr =~ /$re/) or next;
#	print "Id: $hdr, Group: $group, Sample: $sample, Replica: $replica\n";
	$org->{$group} = {} unless defined($org->{$group});
	$org->{$group}->{$sample} = {} unless defined($org->{$group}->{$sample});
	$org->{$group}->{$sample}->{$replica} = $col;
    }
}

my $idcol = shift @headercols;
my @groups = keys %{ $org };

foreach my $g (@groups) {
    my @samples = keys %{ $org->{$g} };
    foreach my $s (@samples) {
	my @replicates = keys %{ $org->{$g}->{$s} };
	foreach my $r (@replicates) {
	    foreach my $g2 (@groups) {
#		print STDERR "$g/$s/$r $g2\n";
		next if defined($org->{$g2}->{$s}->{$r});
		print STDERR "Replicate $g/$s/$r missing in group ${g2}, skipping.\n";
		delete $org->{$g}->{$s}->{$r};
	    }
	}
	if(! values %{ $org->{$g}->{$s} }) {
	    print STDERR "Removing sample $g/$s.\n";
	    delete $org->{$g}->{$s};
	}
    }
}

print $headers[$idcol];
foreach my $col (@headercols) {
    print "\t", $headers[$col];
}
my $g = $groups[0];
my $colcount = 0;
foreach my $s (sort { $a <=> $b } keys %{ $org->{$g} }) {
    foreach my $r (sort { $a <=> $b } keys %{ $org->{$g}->{$s} }) {
	print "\t";
	printf($label, $s, $r);
	$colcount++;
    }
}
print "\n";

open COLCLS, ">split.colcls";
print COLCLS "% $colcount\n";
# my $sampleid = 0;
# my $col = 0;
# foreach my $s (sort { $a <=> $b } keys %{ $org->{$g} }) {
#     foreach my $r (sort { $a <=> $b } keys %{ $org->{$g}->{$s} }) {
# 	print COLCLS "$col\t$sampleid\n";
# 	$col++;
#     }
#     $sampleid++;
# }

my $col = 0;
foreach my $s (sort { $a <=> $b } keys %{ $org->{$g} }) {
    my $cls = undef;
    for(my $i=0; $i < @classes; $i++) {
	if(grep {$_ == $s} @{ $classes[$i] }) {
	    $cls = $i;
	}
    }
    foreach my $r (sort { $a <=> $b } keys %{ $org->{$g}->{$s} }) {
	print COLCLS "$col\t$cls\n";
	$col++;
    }
}


while(<>) {
    @values = split /\t/;
    foreach my $g (@groups) {
	print $values[$idcol] . "_$g";
	foreach my $hc (@headercols) {
	    print "\t", $values[$hc];
	}
	my @samples = sort { $a <=> $b } keys %{ $org->{$g} };
	foreach my $s (@samples) {
	    my @replicates = sort { $a <=> $b } keys %{ $org->{$g}->{$s} };
	    foreach my $r (@replicates) {
		my $col = $org->{$g}->{$s}->{$r};
		print "\t", $values[$col];
	    }
	}
	print "\n";
    }
}

# use Data::Dumper;
# print Dumper($org);
