#!/bin/bash
for i in $(seq 0 1 30) 
do
	cp testProgram scenario$i
	cd scenario$i
	./testProgram
        cp output.txt ../output$i.txt
	cd ..
done

