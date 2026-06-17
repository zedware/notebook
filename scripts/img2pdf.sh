#!/usr/bin/env bash

# If we are using Windows box, simply sort the files in File Explorer, and select
# the files, then right click on the first file, choose Print.

processed_files=()

# Safely find images sorted by modification time (mtime) in ascending order.
# %T@ outputs the modification time as a Unix epoch decimal (e.g., 1765961795.149)
# We filter for files matching the extensions case-insensitively via regex.
while IFS= read -r -d '' line; do
    # Strip the leading epoch timestamp and space to extract the raw filename
    x="${line#* }"
    
    [[ -f "$x" ]] || continue
    [[ "$x" == x-* ]] && continue

    echo "Processing: $x"
    
    # Define the temporary filename
    tmp_file="x-$(basename "$x")"
    
    # Flatten the image layers/transparency using convert
    convert "$x" -background white -alpha remove -alpha off "$tmp_file"
    
    # Append the temporary filename to the Bash array safely
    processed_files+=("$tmp_file")

done < <(find . -maxdepth 1 -type f \( -name "*.png" -o -name "*.jpg" -o -name "*.jpeg" \) -printf "%T@ %p\0" | sort -z -n)

if (( ${#processed_files[@]} == 0 )); then
    echo "No matching image files found."
    exit 1
fi

output="${PWD##*/}"
echo "Output: $output.pdf"

img2pdf --output "$output.pdf" "${processed_files[@]}"

# Clean up temporary files safely
rm -f x-*.png x-*.jpg x-*.jpeg
