# !/bin/bash
# script to start the server, which also starts the TA-AI client :P
# TODO: Please make a utils folder and there may be libraries in common, I am high right now and maybe writing random comments, so please delete this comment afterwards... 

echo "extra options: sh startServer <optinal_port_no>"
port=$1
if [ -z "$1" ];	then
	port=$(grep port: server/config.txt | sed 's/port: //g')
fi

# this seemd weird, but is not :p
cd client
python client.py $port localhost TA-AI '../server/ta_run.sh' 0.2 &

cd ../server
python server.py $port
