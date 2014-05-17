#!/bin/bash
#PBS -N dedekind_8
#PBS -l nodes=2:ppn=12
#PBS -l walltime=12:00:00
#PBS -j oe
#PBS -o out-file.txt
#PBS -e err-file.txt
#PBS -m abe
#PBS -M email@address.com
#PBS -q nodes

cd bachelor-project
mpirun -np 24 ./project -d 8
