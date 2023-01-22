#! /bin/sh

cat "" > /var/log/messages
bash $1
cat /var/log/messages > $1.txt