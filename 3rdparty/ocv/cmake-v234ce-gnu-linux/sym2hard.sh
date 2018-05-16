##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2014, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Looks for *.so symlinks in $1 directory and converts them to hard links.
#
#  USAGE
#    $1 specifies directory to work in (not recursive).
#    If $2 == "d" the original files and intermediate links are deleted.
#
#  AUTHOR
#    Tomas Babinec
#
#  DATE
#    2014-03-16
#
##############################################################################

#﻿!/bin/bash
# Looks for *.so links in $1 directory and converts them to hard links.
# If $2 == "d" the original files and intermediate links are deleted
# If $2 == "r" the linking order is changed. *.so are hardlinks and version numbered files link to them.

if [ -d $1 ] ; then
#    echo "About to convert all symbolic links in '$1' . Continue? [y/N]"
#    read confirm
#    if [ $confirm == "y" ]; then
        cd "$1" &&
        FILES="*.so"	#only for *.so libraries
        for file in $FILES
        do
	    # only for links
            if [ -L "$file" ]; then
                target=$(readlink "$file")
		while [ -L "$target" ]
		do
		  echo "$target is a symbolic link again => going deeper"
		  rem=$target
		  target=$(readlink "$target")
		  if [ "$2" == "d" ]
		  then
		    rm "$rem"
		  fi
		done
                echo "Converting symbolic link ($file) to hardlink"
                mv "$file" "$file.old" &&
                ln "$target" "$file" &&
                rm "$file.old"
		if [ "$2" == "d" ]
		then
		  echo "Removing original file ($file)"
		  rm "$target"
		fi
            else
                echo "$file not a symbolic link, skipping"
            fi
        done
#    else
#        echo "Exiting. No changes made."
#    fi

else
    echo "$1 is not a directory, exiting."
fi
exit
