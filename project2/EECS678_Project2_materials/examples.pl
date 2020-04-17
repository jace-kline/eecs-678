#!/usr/bin/perl

# EECS678
# Adopted from CS 241 @ The University of Illinois

for $file (<examples/1/*>){
	if( $file =~ /proc(\d+)-c(\d+)-(\w+)\.out/){
	#	print "Proc $1 CORE $2 Proc $3\n";
		`./src/simulator -c $2 -s $3 examples/proc$1.csv | tail -7 > output1`;
		`tail -7 $file > output2`;
		`echo $file >> c1_diff`;
		$diff = `diff output1 output2 >> c1_diff`;
		if($diff){
			print "Test file $file differs\n$diff";
		}
	}
}

#cleanup
`rm output1 output2`;

for $file (<examples/2/*>){
	if( $file =~ /proc(\d+)-c(\d+)-(\w+)\.out/){
	#	print "Proc $1 CORE $2 Proc $3\n";
		`./src/simulator -c $2 -s $3 examples/proc$1.csv | tail -7 > output1`;
		`tail -7 $file > output2`;
		`echo $file >> c2_diff`;
		$diff = `diff output1 output2 >> c2_diff`;
		if($diff){
			print "Test file $file differs\n$diff";
		}
	}
}

#cleanup
`rm output1 output2`;

for $file (<examples/4/*>){
	if( $file =~ /proc(\d+)-c(\d+)-(\w+)\.out/){
	#	print "Proc $1 CORE $2 Proc $3\n";
		`./src/simulator -c $2 -s $3 examples/proc$1.csv | tail -9 > output1`;
		`tail -9 $file > output2`;
		`echo $file >> c3_diff`;
		$diff = `diff output1 output2 >> c3_diff`;
		if($diff){
			print "Test file $file differs\n$diff";
		}
	}
}

#cleanup
`rm output1 output2`;

