#!/bin/sh
# This script will run mkmemuse, run the forkmem executable, and then output
# the runtests executable to the output file.  The script will conduct
# 100 tests, each interspersed by a single second.

output_file="$1"
index=1

# Clear the output file of any old data
echo '' > "$output_file"

# Compile all necessary files
./mkmemuse

# Conduct 100 tests
for SZ in 1 3 6 8 12 16 24 32 40; do
    # Fork off some memory in the background
    ./forkmem "$SZ+0+$((11-$index))" &
    
    # Run another test
    ./runtests "$index" >> "$output_file"
    index=$((index+1))
    sleep 1
done

echo "Done."


