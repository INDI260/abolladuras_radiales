#!/bin/bash
export LC_NUMERIC=C
set -euo pipefail

if [ $# -ne 2 ]; then
  echo "Uso: $0 <num_puntos> <num_abolladuras>"
  exit 1
fi

ID=$(date +%Y%m%d_%H%M%S)
OBJ="${ID}.obj"
DENTS="${ID}_dents.txt"

awk -v n="$1" -v seed=$RANDOM 'BEGIN{
  srand(seed);
  for(i=0;i<n;i++)
    printf "v %.6f %.6f %.6f\n", rand(), rand(), (rand()-0.5)*0.1
}' > "$OBJ"

printf "centro;radio;intensidad\n" > "$DENTS"
awk -v n="$2" -v seed=$RANDOM 'BEGIN{
  srand(seed);
  for(i=0;i<n;i++){
    cx=0.1+rand()*0.8; cy=0.1+rand()*0.8;
    r=0.03+rand()*0.2;  I=0.1+rand()*0.9;
    printf "%.6f,%.6f,0;%.6f;%.6f\n", cx, cy, r, I
  }
}' >> "$DENTS"

echo "Generados: ${OBJ} y ${DENTS}"
