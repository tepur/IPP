#!/bin/bash

if [ $# -eq 2 ]
then
	program_name=$1
	dir_name=$2
	
	if ! [ -d "$dir_name" ]
	then
		echo "Błąd: $dir_name nie jest katalogiem"
		exit 1
	fi
	
	if ! [ -x "$program_name" ]
	then
		echo "Błąd: $program_name nie jest plikiem wykonywalnym"
		exit 1
	fi
	
	for f in "$dir_name"/*
	do
		if [[ $(head -1 "$f") == "START" ]]
		then
			curr_file="$f"
			break
		fi
	done
	
	tmp_files="chain_poly_tmp_files"
	mkdir $tmp_files
	
	tail -n +2 "$curr_file" > $tmp_files/first_file_without_start.txt
	curr_file=$tmp_files/first_file_without_start.txt
	
	touch $tmp_files/last_out.out
	
	while true
	do
		cat "$curr_file" > $tmp_files/tmp.out
		head -n -1 $tmp_files/tmp.out > $tmp_files/tmp2.out
		cat $tmp_files/last_out.out $tmp_files/tmp2.out > $tmp_files/new_in.in
		./"$program_name" < $tmp_files/new_in.in > $tmp_files/last_out.out
		last_line=$(tail -1 "$curr_file")
		if [[ "$last_line" == "STOP" ]]
		then
			cat $tmp_files/last_out.out
			break
		else
			curr_file="$dir_name/${last_line:5}"
		fi
	done
	
	rm $tmp_files/last_out.out
	rm $tmp_files/new_in.in
	rm $tmp_files/tmp.out
	rm $tmp_files/tmp2.out
	rm $tmp_files/first_file_without_start.txt
	
	rmdir $tmp_files
	
else
	echo "Błąd: zła liczba argumentów"
	exit 1
fi

