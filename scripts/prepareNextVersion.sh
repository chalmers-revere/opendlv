#!/bin/bash

echo "Script to prepare the next version."
echo "Version: "
read OD_VERSION
#OD_VERSION=2.2.2

echo "Date: "
read OD_DATE_IN
#OD_DATE_IN="31 December 2015"
OD_DATE=$(echo $OD_DATE_IN | sed s/\ /%%%/g)

echo "One line description: "
read OD_ONELINER
#OD_ONELINER="ABC DEF"

echo "Date: $OD_DATE_IN"
echo "Version: $OD_VERSION"
echo "One liner: $OD_ONELINER"

echo "Updating ChangeLog."
for i in $(find . -name "ChangeLog" | grep -v "3rdParty"); do
    cat $i > $i.new
    echo "$OD_VERSION - $OD_ONELINER" > $i
    cat $i.new >> $i
    rm $i.new
done

echo "Updating man pages."
for i in $(find code -name "*.1" | grep -v "build" | grep -v "git" | grep -v "3rdParty" | grep -v "LICENSE"); do
    cat $i | sed -e 's/\".*\"\ \".*\"\ /\"%ODDATE%\"\ \"%ODVERSION%\"\ /' | sed s/%ODVERSION%/$OD_VERSION/ | sed s/%ODDATE%/$OD_DATE/ | tr -s "%" " "  > $i.new && mv $i.new $i
done

echo "Updating VERSION"
echo $OD_VERSION > VERSION

