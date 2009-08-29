#!/bin/sh

COUNT=`expr $#`
MODE=$1
VERSION=$2
TARGET_DIR=""
TARGET_TAR=""


function print_usage_message {
	echo "Useage:"
	echo $0" {mode} {version}"
	echo "    {mode}   : 'src' or 'pkg'"
	echo "    {version}: ex) 0.0.1"
}

function delete_backup_file {
	
}


if [ ${COUNT} != '2' ]
then
	print_usage_message
	exit

elif [ ${MODE} = 'src' ]
then
	TARGET_DIR="cria-src-"${VERSION}
	TARGET_TAR=${TARGET_DIR}.tar.gz

elif [ ${MODE} = 'pkg' ]
then
	TARGET_DIR="cria-"${VERSION}
	TARGET_TAR=${TARGET_DIR}.tar.gz

else
	print_usage_message()
	exit
fi



rm -rf ${TARGET_DIR}
rm -rf ${TARGET_TAR}


echo "Create target directory. ["${TARGET_DIR}"]"
mkdir ${TARGET_DIR}

echo "Copy files and directories."

echo "... 'COPYLIGHT' file."
cp ../COPYRIGHT ${TARGET_DIR}

echo "... 'README' file."
cp ../README ${TARGET_DIR}

echo "... 'pkg' directory"
cp -r ../pkg ${TARGET_DIR}

echo "... 'sample' directory"
cp -r ../sample ${TARGET_DIR}

echo "... 'src' directory."
cp -r ../src ${TARGET_DIR}





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

tar zcvf ${TARGET_TAR} ${TARGET_DIR} 
