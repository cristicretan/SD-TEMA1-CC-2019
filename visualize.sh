#!/bin/bash

for i in "$@"
do
	python visualize.py $i &
done
