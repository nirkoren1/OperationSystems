#!/bin/bash
#Nir Koren 316443902

#check if the number of arguments is below 2
if [ $# -lt 2 ]
then
echo "Not enough parameters"
exit
fi

path=$1
word=$2

#find all files in the current directory with .out extension and delete them (not including subdirectories)
for file in `find $path -maxdepth 1 -type f -name "*.out"`
do
rm $file
done

#find all files with .c extension that contain the word inside the file and compile them in the the same directory with .out extension
for file in `find $path -maxdepth 1 -type f -name "*.c" -exec grep -l $word {} \;`
do
  # not include the last 2 characters of the file name (the .c extension)
gcc -o ${file:0:-2}.out $file
done

# if -r flag is on, recursively call the script on all subdirectories and so on
if [ "$3" = "-r" ]
then
  for dir in `find $path -maxdepth 1 -wholename "${path}/*" -type d`
  do
    $0 ${dir} $word "-r"
  done
fi
