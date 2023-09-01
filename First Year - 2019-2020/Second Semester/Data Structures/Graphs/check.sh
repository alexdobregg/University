#!/bin/bash
#//////////////////////////////////////////////////////
#	Created by Nan Mihai on 09.05.2020
#	Copyright (c) 2020 Nan Mihai. All rights reserved.
#	Checker Tema 3 - Structuri de date
#	Grafuri neorientate
#	Facultatea de Automatica si Calculatoare
#	Anul Universitar 2019-2020, Seria CD
#/////////////////////////////////////////////////////
print_header()
{
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 71 ]; then
		padding=$(((71 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

check_bonus()
{
	filesRef=$(find tests/problema1/ref/bonus/ -type f -name "bonus$1*")
	fileIn="tests/problema1/bonus"$1".in"
	fileOut="tests/out/test"$1".out"
	timeout 60 ./movies -b $fileIn $fileOut > /dev/null 2>&1
	return=1
	for fileRef in $filesRef
	do
		diff $fileOut $fileRef > /dev/null
		EXIT_CODE=$?
		if [ $EXIT_CODE -eq $ZERO ] 
		then
			return=$ZERO
		fi
	done
}


result1=0
result2=0
result3=0
result=0
depunctari=0
ZERO=0
VALGRIND="valgrind --leak-check=full --track-origins=yes -q --log-file=rezultat_valgrind.txt"
make build > /dev/null 2>&1
var=10
BEST=100
print_header "Testare - Tema 3 (SD-CD)"
#Cerinta 1 - Productia maximă
echo "Cerinta 1 - Productia maximă"
for i in {0..14}
do
	fileIn="tests/task1/in/test"$i".in"
	fileOut="tests/task1/out/test"$i".out"
	fileRef="tests/task1/ref/test"$i".ref"
	timeout 20 ./movies -c1 $fileIn $fileOut > /dev/null 2>&1
	diff $fileOut $fileRef > /dev/null
	EXIT_CODE=$?
	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+0.0: Cerinta 1 Test $idx ................................................. PASS"
		result=$(awk "BEGIN {print $result+2; exit}")
		result1=$(awk "BEGIN {print $result1+2; exit}")
	else
		echo "-1.0: Cerinta 1 Test $idx ................................................. FAIL"
	fi
done

#Cerinta 2 - Gradul de inrudire
echo "Cerinta 2 - Gradul de inrudire"
for i in {0..14}
do
	fileIn="tests/task2/in/test"$i".in"
	fileOut="tests/task2/out/test"$i".out"
	fileRef="tests/task2/ref/test"$i".ref"
	timeout 20 ./movies -c2 $fileIn $fileOut > /dev/null 2>&1
	diff $fileOut $fileRef > /dev/null
	EXIT_CODE=$?
	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+0.0: Cerinta 2 Test $idx ................................................. PASS"
		result=$(awk "BEGIN {print $result+2; exit}")
		result2=$(awk "BEGIN {print $result2+2; exit}")
	else
		echo "-1.0: Cerinta 2 Test $idx ................................................. FAIL"
	fi
done

#Cerinta 3 - Muchiile critice
echo "Cerinta 3 - Muchiile critice"
for i in {1..10}
do
	fileIn="tests/task3/in/test"$i".in"
	fileOut="tests/task3/out/test"$i".out"
	fileRef="tests/task3/ref/test"$i".ref"
	timeout 30 ./movies -c3 $fileIn $fileOut > /dev/null 2>&1
	diff $fileOut $fileRef > /dev/null
	EXIT_CODE=$?
	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+0.0: Cerinta 3 Test $idx ................................................. PASS"
		result=$(awk "BEGIN {print $result+4; exit}")
		result3=$(awk "BEGIN {print $result3+4; exit}")
	else
		echo "-1.0: Cerinta 3 Test $idx ................................................. FAIL"
	fi
done

echo "Verificarea dealocării corecte a memoriei"
fileIn="tests/task1/in/test10.in"
fileOut="tests/task1/out/test10.out"
$VALGRIND ./movies -c1 $fileIn $fileOut > /dev/null 2>&1
if [[ -z $(cat rezultat_valgrind.txt) ]]; then
	printf "+0.0: VALGRIND Cerinta 1 ................................................ PASS\n"
else
	depunctare=$(echo "scale=1; -($result1/10)" | bc -l)
	printf -- "$depunctare: VALGRIND Cerinta 1 ................................................ FAIL\n"
	result=$(awk "BEGIN {print $result-($result1/10); exit}")
	depunctari=$(awk "BEGIN {print $depunctari+($result1/10); exit}")
fi
fileIn="tests/task2/in/test11.in"
fileOut="tests/task2/out/test11.out"
$VALGRIND ./movies -c2 $fileIn $fileOut > /dev/null 2>&1
if [[ -z $(cat rezultat_valgrind.txt) ]]; then
	printf "+0.0: VALGRIND Cerinta 2 ................................................ PASS\n"
else
	depunctare=$(echo "scale=1; -($result2/10)" | bc -l)
	printf -- "$depunctare: VALGRIND Cerinta 2 ................................................ FAIL\n"
	result=$(awk "BEGIN {print $result-($result2/10); exit}")
	depunctari=$(awk "BEGIN {print $depunctari+($result2/10); exit}")
fi
fileIn="tests/task3/in/test10.in"
fileOut="tests/task3/out/test10.out"
$VALGRIND ./movies -c3 $fileIn $fileOut > /dev/null 2>&1
if [[ -z $(cat rezultat_valgrind.txt) ]]; then
	printf "+0.0: VALGRIND Cerinta 3 ................................................ PASS\n"
else
	depunctare=$(echo "scale=1; -($result3/3)" | bc -l)
	printf -- "$depunctare: VALGRIND Cerinta 3 ................................................ FAIL\n"
	result=$(awk "BEGIN {print $result-($result3/10); exit}")
	depunctari=$(awk "BEGIN {print $depunctari+($result3/10); exit}")
fi
echo "Rezultate"
printf "Cerinta 1 : "$result1"\n"
printf "Cerinta 2 : "$result2"\n"
printf "Cerinta 3 : "$result3"\n"
printf "Depunctari: "$depunctari"\n"

if (( total == BEST )); then
	echo "Felicitari! Ai punctajul maxim: "$BEST"p! :)"
else
	echo "Ai acumulat "$result"p din maxim 100p! :("
fi
make clean > /dev/null 2>&1
