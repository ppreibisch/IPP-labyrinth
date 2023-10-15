#!/bin/bash

prog=$1
dir=$2

for f in "$dir"/*.in
do
	valgrind --log-file="${f%in}mem" --error-exitcode=123 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --quiet ./"$prog" < "$f" 1>"${f%in}outx" 2>"${f%in}errx"
	numer_testu="${f%.in}"
	numer_testu="${numer_testu#$dir/}"
	echo -n "test $numer_testu: "
	if ! diff -b "${f%in}outx" "${f%in}out" >/dev/null
	then
		echo -n "błędny wynik, "
	else
		echo -n "poprawny wynik, "
	fi

	if ! diff -b "${f%in}errx" "${f%in}err" >/dev/null
	then
		echo -n "błędny error, "
	else
		echo -n "poprawny error, "
	fi

	if [ -s "${f%in}mem" ];
	then
		echo -n "błąd w pamięci."
	else
		echo "poprawna pamięć."
	fi

	rm "${f%in}outx"
	rm "${f%in}errx"
	rm "${f%in}mem"
done
