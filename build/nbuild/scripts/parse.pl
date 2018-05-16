#!/usr/bin/perl
# The purpose of this perl module is to create a kernel database (.db) file.
# The input to parse() is the output of the gnu preprocessor, used on the .c kernel modules
# The output is a .db file, in the same format that was used on APEX 1.

# Notes:
# - currently accepts spatial_dep even if the parameter index does not match a valid parameter.
# - once the parameter variable types allowed by the Target compiler are known, their bit sizes
#   need to be reviewed and implemented below.
# - only handles native C parameter types.  #defines are resolved by the preprocessor, but #typedefs probably aren't.
# - assumes parameter typs is of the format __keyword.  Since the preprocessor will remove this if defined via #define,
#   we may need to invoke and maintain a separate header file to pass the type, via the .i file, to this module. 

# todo:  install pp to package the final product as a windows exe

use strict;
use warnings;

# The purpose of this module is to process the following file:
# ACF2_Offline input FILE to extract 2 pieces of information:

# INSTANCE_ID:  in FILE_ID_apu_process.hpp, the string used in the Initialize() call.
# CLASS_ID: the name of the class derived from ACF_Process_Desc_APU in FILE_ID_apu_process.hpp 

# These values are then output in text fashion.  

# pass along command line parameters to parse. todo: path to ACF2_Offline.mak
# Ideally, the usage should probably be:
#  parse - use assumed makefile, extrace FILE= from there
#  parse MAKEFILE - use provided makefile, extract FILE= from there
#  parse MAKEFILE FILE - invoke provided makefile, but extract FILE from command line
#
# Alternately, it would be nice if we could invoke this utility from the makefile (passing FILE),
# and somehow return CLASS_ID and INSTANCE_ID to the makefile.  I don't think that can
# be done, though.
parse(@ARGV);


sub parse {
	my $in;
	
	my $FILE = shift;
	my $PAR2EXTRACT = shift;
	if (!defined($PAR2EXTRACT)) {
	  $PAR2EXTRACT = "DEADBEEF";
	}
	# print the help if the first parameter
	if (!defined($FILE)) {
		print "\nMissing FILE parameter.\n";
		printUsage();
		exit(-1);
	} elsif ($FILE =~ /-/) {
		printUsage();
		exit;
	}
	
	# extract CLASS_ID and INSTANCE_ID from FILE
	my $CLASS_ID;
	my $INSTANCE_ID;
	my $numClassLines = 0;
	my $numInstanceLines = 0;
	my @classLines;
	my @instanceLines;
	#print "Reading following file to extract CLASS and INSTANCE name\n";
	#print "  $FILE\n";
	open($in, "<", $FILE) or die "Can't open $FILE: $!";
	 
	while (<$in>) { # assigns each line in turn to $_
		chomp;
		my $line = $_;
		next if (/^\s*\/\//);	# skip commented out lines
		# class CLASS_ID : public ACF_Process_Desc_APU
#		if (/class.*:\s(public|private)?\s*(.+)[\r\n]*$/) {
		if (/class\s*(.*)\s*:\s*(public|private)?\s*ACF_Process_Desc_APU[\r\n]*$/) {
			$CLASS_ID = $1;
			$numClassLines++;
			push @classLines, "$line\n";

		# Initialize(mADD_FLT_A_graph, "INSTANCE_ID");
		} elsif (/^\s*Initialize\s*\(.*,(.*)\)/) {
			if (/^\s*Initialize\s*\(.*"(.*)"\)/) {
				$INSTANCE_ID = $1;
				$numInstanceLines++;
				push @instanceLines, "$line\n";
			} else {
				$INSTANCE_ID = $1;
				$numInstanceLines++;
				push @instanceLines, "$line\n";
			}
		}
	}
	
	# Handle errors in extracting CLASS_ID, INSTANCE_ID
	# NB: Output may be visible to external customers, so
	#     be as clear and helpful as possible
	if ($numClassLines == 0) {
		print "\nERROR!\n";
		print " Could not find CLASS_ID in file:\n";
		print "  $FILE\n";
		print " Expected to encounter a line of the format:\n";
		print "-->class CLASS_ID : public ACF_Process_Desc_APU\n";
	}
	
	if ($numInstanceLines == 0) {
		print "\nERROR!\n";
		print " Could not find INSTANCE_ID in file:\n";
		print "  $FILE\n";
		print " Expected to encounter a line of the format:\n";
		print "-->Initialize(mGraph, \"INSTANCE_ID\")\n";
	}
	
	# Handle case where more than 1 result was found
	if ($numClassLines > 1) {
		print "\nERROR!\n";
		print " Found multiple CLASS_ID candidates in file:\n";
		print "  $FILE\n";
		print " Please ensure that only one class derived from ACF_Process_Desc_APU is present.\n";
		print " Expected to encounter a single line of the format:\n";
		print "-->class CLASS_ID : public ACF_Process_Desc_APU\n";
		print "\n Found the following candidates:\n"; 
		print "-----\n";
		print "@classLines\n";
		print "-----\n";
	}
	if ($numInstanceLines > 1) {
		print "\nERROR!\n";
		print " Found multiple INSTANCE_ID candidates in file:\n";
		print "  $FILE\n";
		print " Please ensure that only one call to Initialize() is present.\n";
		print " Expected to encounter a single line of the format:\n";
		print "-->Initialize(mGraph, \"INSTANCE_ID\")\n";
		print "\n Found the following candidates:\n"; 
		print "-----\n";
		print "@instanceLines\n";
		print "-----\n";
	}

	if (($numClassLines != 1) || ($numInstanceLines != 1)) {
		print "Aborting build\n";
		exit (-1);
	}	

	# Display parsed values 	
	#print " CLASS Identifier is $CLASS_ID\n";
	#print " INSTANCE Identifier is $INSTANCE_ID\n";

	# build file	
#	my $buildFile = "./build_ACF2.bat";
#	open(my $out, ">", $buildFile) or die "Can't create $buildFile: $!";
#	print $out "make -C Applications\\Cognivue\\ICP_SDK -f ACF2_Offline.mak build INSTANCE_ID=$INSTANCE_ID CLASS_ID=$CLASS_ID";
#	close $out;
#	print "\n$buildFile generated\n";
	
	# clean file
#	my $cleanFile = "./clean_ACF2.bat";
#	open($out, ">", $cleanFile) or die "Can't create $cleanFile: $!";
#	print $out "make -C Applications\\Cognivue\\ICP_SDK -f ACF2_Offline.mak clean INSTANCE_ID=$INSTANCE_ID CLASS_ID=$CLASS_ID";
#	close $out;
#	print "$cleanFile generated\n";

	# build_args.txt
	my $textFile = "build_args.txt";
	open(my $out, ">", $textFile) or die "Can't create $textFile: $!";
	if ($PAR2EXTRACT eq "INST") {
	  print "$INSTANCE_ID";
	}
	elsif ($PAR2EXTRACT eq "CLASS") {
	  print "$CLASS_ID";
	}
	else {
	  print $out "INSTANCE_ID=$INSTANCE_ID CLASS_ID=$CLASS_ID";
	  close $out;
	  print "$textFile generated\n";
	}
}

sub printUsage()
{
	print "Usage:\n";
	print "  build_ACF2 inputfile:\n";
	print "   - extracts the CLASS and INSTANCE names from the given file.\n";
	
}
