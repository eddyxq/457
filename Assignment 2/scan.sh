find -type f -name "*$1" -printf "%p %s\n" | sort -k 2 -n -r | head -$2 | awk '{print $1" "$2; x = x + $2} END {print "Total size: " x}'
