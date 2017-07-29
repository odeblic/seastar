#!/bin/sh

readonly SERVER=./bin/seastar
readonly CLIENT=netcat

readonly PORT=1101

# Number of simultaneous clients
CONN_NUMBER=10

TERMINAL=gnome-terminal

# Replace gnome-terminal by xterm if necessary
which $TERMINAL 2>&1 > /dev/null
if [ $? -ne 0 ]
then
  TERMINAL=xterm
fi

echo "The terminal ($TERMINAL) will be used"

echo "Test for $(basename $SERVER)..."

$TERMINAL -e "$SERVER -c" &
echo "Server process ($(basename $SERVER)) created with PID $!"

while [ $CONN_NUMBER -gt 0 ]
do
  $TERMINAL -e "$CLIENT localhost $PORT" &
  CONN_NUMBER=$(($CONN_NUMBER - 1))
  echo "Client process ($CLIENT) created with PID $!"
  sleep 0.1
done

for SECONDS in $(seq 10 -1 0)
do
  echo "Will stop in $SECONDS seconds"
  sleep 1
done

killall -s INT $(basename $SERVER) 2>&1 > /dev/null

if [ $? -eq 0 ]
then
  echo "The INT signal was sent"
else
  sleep 1
  echo "Error when sending the INT signal"
  echo "Try to send the KILL signal"
  killall -s KILL $(basename $SERVER) 2>&1 > /dev/null
  killall -s KILL $CLIENT 2>&1 > /dev/null
fi

echo "Test completed"

