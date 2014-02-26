/^%/ { print; }

/^[^%]/ { 
    max = 0;
    for(i=3; i <= NF; i++) {
	if($i > max) max = $i;
    }
    printf("%s\t%s", $1, $2);
    for(i=3; i <= NF; i++) {
	printf("\t%s", $i / max);
    }
    printf("\n");
}