#!/bin/bash

for matrix in test1 test2 test3 test4 test5 torso1 TSOPF_RS_b678_c2; do
  result_filepath="results/$matrix.csv"
  matrix_filepath="matrix/${matrix}.mtx"
  b_filepath="matrix/b_for_${matrix}.mtx"
  echo "original,parallel,level_partition" >"$result_filepath"
  echo "./main $matrix_filepath $b_filepath 1>> $result_filepath"
  # 10 trials
  for i in {1..10}; do ./main "$matrix_filepath" "$b_filepath" 1>>"$result_filepath"; done
done
