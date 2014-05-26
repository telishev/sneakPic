#!/bin/sh

QRC_NAME="sneakPic.qrc"

n_icons_before=`wc -l $QRC_NAME | sed -nre 's/^ *([0-9]+).*$/\1/p'`
n_icons_before=$(( $n_icons_before - 4 ))  # cut off header and footer

mv $QRC_NAME $QRC_NAME.bkp
echo '<RCC>' >> $QRC_NAME
echo '    <qresource prefix="/">' >> $QRC_NAME
find . -regex '\(.*\.\(png\|svg\|xpm\)\)' -printf '        <file>%p</file>\n' | sed '/\/source\//d' | sed 's/<file>.\//<file>/' | sort >> $QRC_NAME
echo '    </qresource>' >> $QRC_NAME
echo '</RCC>' >> $QRC_NAME

n_icons_after=`wc -l $QRC_NAME | sed -nre 's/^ *([0-9]+).*$/\1/p'`
n_icons_after=$(( $n_icons_after - 4 ))  # cut off header and footer

sort $QRC_NAME > $QRC_NAME.sorted
sort $QRC_NAME.bkp > $QRC_NAME.bkp.sorted

diff --ignore-space-change $QRC_NAME.sorted $QRC_NAME.bkp.sorted | sed '/^[^<>]/d' | sed 's/^< */add     /' | sed 's/^> */remove  /' | sed -r 's/<\/?file>//g'
rm $QRC_NAME.bkp
rm $QRC_NAME.sorted
rm $QRC_NAME.bkp.sorted

echo ""
echo "Icons before: $n_icons_before"
echo "Icons after:  $n_icons_after"
