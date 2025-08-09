#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <binary> [REL_OFFSET_HEX]"
  echo "Example: $0 level1 0x84"
  exit 1
fi

BIN="$1"
REL_OFF="${2:-0x84}"  # offset relatif (main + 0x84)

# Copie de travail
OUT="${BIN%.patched}.patched"
cp -f "$BIN" "$OUT"

# Besoin de objdump + nm
command -v objdump >/dev/null || { echo "objdump manquant"; exit 1; }
command -v nm >/dev/null || { echo "nm manquant"; exit 1; }

# Récupère adresse ELF de .text (virt) et son offset fichier
TEXT_ADDR_HEX=$(objdump -h "$OUT" | awk '$2==".text"{print "0x"$4}')
TEXT_OFF_HEX=$(objdump -h "$OUT" | awk '$2==".text"{print "0x"$6}')

# Adresse ELF de main (symbole non-strippé)
MAIN_ELF_HEX=$(nm -an "$OUT" | awk '/ T main$/{print "0x"$1}')

if [[ -z "$MAIN_ELF_HEX" ]]; then
  echo "Impossible de trouver le symbole main. Binaire strippé ?"
  exit 1
fi

# Calculs
JCC_ELF=$(( MAIN_ELF_HEX + REL_OFF ))
FILE_OFF=$(( JCC_ELF - TEXT_ADDR_HEX + TEXT_OFF_HEX ))

printf "Patching %s at main+%s => ELF 0x%X, file offset 0x%X (%d)\n" \
  "$OUT" "$REL_OFF" "$JCC_ELF" "$FILE_OFF" "$FILE_OFF"

# Écrit 6 NOP (0x90)
printf '\x90\x90\x90\x90\x90\x90' | dd of="$OUT" bs=1 seek="$FILE_OFF" conv=notrunc status=none

# Vérif rapide autour de main
echo "Disassembly check:"
objdump -Mintel -d "$OUT" | sed -n '/<main>/,/<.*>:/p' | head -n 120
echo "Done -> $OUT"

