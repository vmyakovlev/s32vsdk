# Copyright (c) 2016 NXP Semiconductors
# All Rights Reserved.
# 
# This file contains sample code only.
# 
# THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.

#!/bin/bash
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: generate.sh
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

if [ "2" != $# ]
then
    echo "USAGE: $0 InputFile.h OutputFile.cpp"
    exit 1
fi

INPUT_FILE=$1
OUTPUT_FILE=$2

echo -n "Generating $INPUT_FILE ... "

echo "/* DO NOT EDIT, THIS FILE IS GENERATED BASED ON ${INPUT_FILE##*/} */" >$OUTPUT_FILE  &&
echo >>$OUTPUT_FILE  &&
echo "/*" >>$OUTPUT_FILE  &&
echo "  Copyright (c) 2016 NXP Semiconductors" >>$OUTPUT_FILE  &&
echo "  All Rights Reserved." >>$OUTPUT_FILE  &&
echo "  " >>$OUTPUT_FILE  &&
echo "  This file contains sample code only." >>$OUTPUT_FILE  &&
echo "  " >>$OUTPUT_FILE  &&
echo "  THIS SOFTWARE IS PROVIDED BY NXP \"AS IS\" AND ANY EXPRESSED OR" >>$OUTPUT_FILE  &&
echo "  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES" >>$OUTPUT_FILE  &&
echo "  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED." >>$OUTPUT_FILE  &&
echo "  IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT," >>$OUTPUT_FILE  &&
echo "  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES" >>$OUTPUT_FILE  &&
echo "  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR" >>$OUTPUT_FILE  &&
echo "  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)" >>$OUTPUT_FILE  &&
echo "  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT," >>$OUTPUT_FILE  &&
echo "  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING" >>$OUTPUT_FILE  &&
echo "  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF" >>$OUTPUT_FILE  &&
echo "  THE POSSIBILITY OF SUCH DAMAGE." >>$OUTPUT_FILE  &&
echo "*/" >>$OUTPUT_FILE  &&
echo >>$OUTPUT_FILE  &&
echo "#include <stdint.h>" >>$OUTPUT_FILE  &&
echo "#include \"cfg_file.h\"" >>$OUTPUT_FILE  &&
echo >>$OUTPUT_FILE  &&

echo "trCfgFileKeys CFGFILE_arKeys[] = {" >>$OUTPUT_FILE  &&

cat $INPUT_FILE |
    grep -E '^\s*\#define\s+APP_KEY_([A-Za-z0-9_]+)\s+([0-9-][0-9xbXB]{0,1}[0-9a-zA-Z]*[ulUL]{0,3})\s+\/\*.*CFG_FILE.*\*\/.*' |
    sed -re 's@^\s*\#define\s+APP_KEY_([A-Za-z0-9_]+)\s+([0-9-][0-9xbXB]{0,1}[0-9a-zA-Z]*[ulUL]{0,3})\s+\/\*.*CFG_FILE.*\*\/.*@    {"\1", \2},@' >>$OUTPUT_FILE  &&

echo "};" >>$OUTPUT_FILE  &&
echo >>$OUTPUT_FILE  &&

echo "const uint32_t CFGFILE_cou32KeyCount = sizeof(CFGFILE_arKeys) / sizeof(trCfgFileKeys);" >>$OUTPUT_FILE  &&

{ echo "[ DONE ]"; exit 0; } || { echo "[FAILED]"; exit 1; }
