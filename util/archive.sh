#!/bin/sh

COUNT=`expr $#`
MODE=$1
VERSION=$2
TARGET_DIR=""
TARGET_TAR=""


print_usage_message() {
	echo "Useage:"
	echo $0" {mode} {version}"
	echo "    {mode}   : 'src' or 'pkg' or 'del'"
	echo "    {version}: ex) 0.0.1"
}



if [ ${COUNT} != '2' ]
then
	print_usage_message
	exit
elif [ ${MODE} = 'src' ]
then
	TARGET_DIR="cria-"${VERSION}"-src"
	TARGET_TAR=${TARGET_DIR}.tar.gz
elif [ ${MODE} = 'pkg' ]
then
	TARGET_DIR="cria-"${VERSION}
	TARGET_TAR=${TARGET_DIR}.tar.gz
elif [ ${MODE} = 'del' ]
then
	TARGET_DIR="cria-"${VERSION}
	rm -rf ${TARGET_DIR}*
	exit
else
	print_usage_message()
	exit
fi



rm -rf ${TARGET_DIR}
rm -rf ${TARGET_TAR}


svn export http://svn.sourceforge.jp/svnroot/cria ${TARGET_DIR}


if [ ${MODE} = 'pkg' ]
then
	CURRENT_DIR=`pwd`
	cd ${CURRENT_DIR}/${TARGET_DIR}/src/Main/
	make
	cd ${CURRENT_DIR}/${TARGET_DIR}/src/Cria.Tk/source/
	make
	cd ${CURRENT_DIR}
	rm -rf ${TARGET_DIR}/src
fi
rm -rf ${TARGET_DIR}/util

tar zcvf ${TARGET_TAR} ${TARGET_DIR} 
