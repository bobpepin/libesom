BEGIN { FS="\t"; OFS="\t"; SKIP1=1; SKIP2=10; }

{ if(SKIPLAST) NF--; }

NR == 1 { 
  "wc -l " FILENAME | getline lines; 
  printf("%% %d\n%% %d\n%% 9", lines-1, NF - (SKIP2-SKIP1) + 1);
  printf("%% %d\n", lines-1) > "esom.names";
  printf("%% %d\n", lines-1) > "esom.hits";
  for(i=1; i <= NF - (SKIP2-SKIP1); i++) { printf "\t1"; } 
  printf "\n% Id\t";
}

NR > 1 { 
  printf("%d\t", NR-1); 
  printf("%d\t%s\n", NR-1, $1) >> "esom.names";
  printf("%d\t%s\n", NR-1, $9) >> "esom.hits";
}

{ 
  gsub(" ", "_");
  for(i=1; i <= NF; i++) {
      if(i < SKIP1 || i >= SKIP2) {
	  printf "%s%s", $i, (i == NF) ? "\n" : "\t";
      }
  }
}
