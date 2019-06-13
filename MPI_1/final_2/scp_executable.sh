for i in $(cat $1);do scp $2  udesc@$i:~/ ; done;
