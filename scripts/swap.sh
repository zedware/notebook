#! /bin/bash
#
# swap.sh: Shows the swap usage of each process
# Author: Robert Love

swap_total=0

for i in /proc/[0-9]*; do
  pid=$(echo $i | sed -e 's/\/proc\///g')
  swap_pid=$(cat /proc/$pid/smaps |
  awk 'BEGIN{total=0}/^Swap:/{total+=$2}END{print total}')
  if [ "$swap_pid" -gt 0 ]; then
    name=$(cat /proc/$pid/status | grep ^Name: |
    awk '{print $2}')
    echo "${name} (${pid}) ${swap_pid} kB"
    let swap_total+=$swap_pid
  fi
done
echo

echo "Total: ${swap_total} kB"

