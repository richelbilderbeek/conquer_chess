#!/bin/bash
./scripts/start_mashing_button.R

exit 0

# Use the R package instead :-)

echo "You have 3 seconds to focus to Conquer Chess. Do it now :-)"
sleep 3

sleep_time=1 # seconds

xdotool key d
#sleep ${sleep_time}
xdotool key s
#sleep ${sleep_time}
xdotool key a
#sleep ${sleep_time}
xdotool key w
#sleep ${sleep_time}


#xdotool keydown a
#sleep ${sleep_time}
#xdotool keyup a
#sleep ${sleep_time}

#xdotool keydown Right
#sleep ${sleep_time}
#xdotool keyup Down
#sleep ${sleep_time}
