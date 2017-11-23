#!/bin/bash


mv base/*.gcda .
mv building/*.gcda .
mv tools/*.gcda .
mv economy/*.gcda . 
mv event/*.gcda .
mv fields/*.gcda .
mv fields/crops/*.gcda .
mv fields/fieldOperation/*.gcda .
mv fields/staticCrops/*.gcda .

mv livestock/*.gcda .
mv manager/*.gcda .
mv products/*.gcda .
mv soil/*.gcda .
mv technics/*.gcda .

cp base/*.gcno .
cp building/*.gcno .
cp tools/*.gcno .
cp economy/*.gcno . 
cp event/*.gcno .
cp fields/*.gcno .
cp fields/crops/*.gcno .
cp fields/fieldOperation/*.gcno .
cp fields/staticCrops/*.gcno .

cp livestock/*.gcno .
cp manager/*.gcno .
cp products/*.gcno .
cp soil/*.gcno .
cp technics/*.gcno .
