#!/bin/bash
export LC_NUMERIC=C
set -euo pipefail

if [ $# -lt 3 ]; then
  echo "Uso: $0 <num_filas> <num_columnas> <num_abolladuras>"
  echo "               [x_min=0] [x_max=100] [y_min=0] [y_max=100]"
  echo "               [z_min=-5] [z_max=5]  [r_min=3] [r_max=20]"
  echo "               [i_min=0.1] [i_max=1.0]"
  exit 1
fi

ROWS=$1; COLS=$2; ND=$3
XM=${4:-0};   XX=${5:-100}
YM=${6:-0};   YX=${7:-100}
ZM=${8:--5};  ZX=${9:-5}
RM=${10:-3};  RX=${11:-20}
IM=${12:-0.1}; IX=${13:-1.0}

ID=$(date +%Y%m%d_%H%M%S)
OBJ="${ID}.obj"
DENTS="${ID}_dents.txt"

awk -v rows="$ROWS" -v cols="$COLS" -v seed=$RANDOM \
    -v xm="$XM" -v xx="$XX" -v ym="$YM" -v yx="$YX" \
    -v zm="$ZM" -v zx="$ZX" 'BEGIN{
  srand(seed);
  dx=(xx-xm)/(cols-1); dy=(yx-ym)/(rows-1);
  for(r=0;r<rows;r++)
    for(c=0;c<cols;c++)
      printf "v %.6f %.6f %.6f\n",
        xm+c*dx, ym+r*dy, zm+rand()*(zx-zm)
}' > "$OBJ"

awk -v n="$ND" -v seed=$RANDOM \
    -v xm="$XM" -v xx="$XX" -v ym="$YM" -v yx="$YX" \
    -v rm="$RM" -v rx="$RX" -v im="$IM" -v ix="$IX" 'BEGIN{
  srand(seed);
  for(i=0;i<n;i++){
    cx=xm+rand()*(xx-xm); cy=ym+rand()*(yx-ym);
    r=rm+rand()*(rx-rm);  I=im+rand()*(ix-im);
    printf "%.6f,%.6f;%.6f;%.6f\n", cx, cy, r, I
  }
}' > "$DENTS"

echo "Generados: ${OBJ} (grid ${ROWS}x${COLS}) y ${DENTS}"
