#!/bin/bash
cat $1 | \
sed -e 's/ \[#[0-9a-z]\+\]$//g' -e 's/^\*\*\*/###/g' -e 's/^\*\*/##/g' -e 's/^\*/#/g' -e 's/^---/        -/g' -e 's/^--/    -/g' -e 's/^\(\s*\)-\([^ ]\)/\1- \2/g' -e 's/&br;/<br>/g' -e 's/^#pre{*/```/g' -e 's/^}}*/```/g' -e 's/%%/~~/g' -e "s/^\#lsx/`echo -ne '\u0024'`lsx()/g" \
> `echo $1|sed s/.pukiwiki/.md/`


