#!/bin/bash

for study in base low medium high; do

  file="results_${study}/metrics_${study}.txt"

  sort -g -k11 "$file" > "results_${study}/sortedA.txt"
  sort -g -k13 "$file" > "results_${study}/sortedF.txt"
  sort -g -k15 "$file" > "results_${study}/sortedRatio.txt"
done