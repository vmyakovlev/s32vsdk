# ----------------------------------------------------------------------------
#
# GNU Make Standard Library (GMSL)
#
# A library of functions to be used with GNU Make's $(call) that
# provides functionality not available in standard GNU Make.
#
# Copyright (c) 2005-2014 John Graham-Cumming
#
# This file is part of GMSL
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# 
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# Neither the name of the John Graham-Cumming nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# ----------------------------------------------------------------------------

# This is the GNU Make Standard Library version number as a list with
# three items: major, minor, revision
#
# gmsl_version := 1 1 7
#
#
# Addendum: The GMSL was updated for our scope.
#

ifndef _GMSL_MK
_GMSL_MK = true

# ----------------------------------------------------------------------------
# Function:  uniq
# Arguments: 1: A list from which to remove repeated elements
# Returns:   The list with duplicate elements removed without reordering
# ---------------------------------------------------------------------------- 
uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

# ----------------------------------------------------------------------------
# Function:  reverse
# Arguments: 1: A list to reverse
# Returns:   The list with its elements in reverse order
# ----------------------------------------------------------------------------
reverse = $(if $1,$(call reverse,$(wordlist 2,$(words $1),$1))) $(firstword $1)

# ----------------------------------------------------------------------------
# Function:  tr
# Arguments: 1: The list of characters to translate from 
#            2: The list of characters to translate to
#            3: The text to translate
# Returns:   Returns the text after translating characters
# ----------------------------------------------------------------------------
tr = $(strip $(eval __gmsl_t := $3)                                       \
     $(foreach c,                                                         \
         $(join $(addsuffix :,$1),$2),                                    \
         $(eval __gmsl_t :=                                               \
             $(subst $(word 1,$(subst :, ,$c)),$(word 2,$(subst :, ,$c)), \
                 $(__gmsl_t))))$(__gmsl_t))

# Common character classes for use with the tr function.  Each of
# these is actually a variable declaration and must be wrapped with
# $() or ${} to be used.

[A-Z] := A B C D E F G H I J K L M N O P Q R S T U V W X Y Z #
[a-z] := a b c d e f g h i j k l m n o p q r s t u v w x y z #
[0-9] := 0 1 2 3 4 5 6 7 8 9 #
[A-F] := A B C D E F #

# ----------------------------------------------------------------------------
# Function:  uc
# Arguments: 1: Text to upper case
# Returns:   Returns the text in upper case
# ----------------------------------------------------------------------------
uc = $(call tr,$([a-z]),$([A-Z]),$1)

endif
