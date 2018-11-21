#!/bin/bash

#Run from an edge server

filename=$1
owner=$2
group=$3

en_input_path="/var/lib/csc2228/Term-Project/files/${filename}"
en_output_path="/var/lib/csc2228/Term-Project/files/${filename}.en"
sp_input_path=$en_output_path
sp_output_path="/var/lib/csc2228/Term-Project/tmp/${filename}"

psql_command="psql -d csc2228 -U postgres -w -c"
rpsql_command="psql -d csc2228 -U postgres -w -h 192.168.0.34 -c"
encrypt_file_command="/var/lib/csc2228/Term-Project/scripts/encrypt"
split_file_command="/var/lib/csc2228/Term-Project/scripts/split_file"
chunk_size=4096

file_size=$(stat -c %s $en_input_path)

#integrity_check() {
#	#Check for user permissions to make sure user has rights to get file
#	#Check if user has enough on their quota
#	return 1
#}


transfer_file() {
	tf_filename=$1
	final_filename=$2
	tf_hostname=$3
	rsync -avq --remove-source-files "${tf_filename}" ${tf_hostname}:/var/lib/csc2228/files/${final_filename}
	return 0
}

#Find IP Addresses of storage servers to push to
server_ids=$($psql_command "select server_id from servers where type = 'storage'" | tail -n +3 | head -n -2 | sed "s/ //g")
server_hosts=$($psql_command "select ip_address from servers where type = 'storage'" | tail -n +3 | head -n -2 | sed "s/ //g")
mapfile -t server_ids <<< "$server_ids"
mapfile -t server_hosts <<< "$server_hosts"
num_servers=${#server_hosts[@]}
s_index=0

#Encrypt file, then split it into chunks
$encrypt_file_command "${en_input_path}" "${en_output_path}"
num_chunks=$($split_file_command "${sp_input_path}" "${sp_output_path}" "$chunk_size")

#Insert file information into database
$rpsql_command "insert into files(filename, fowner, fgroup, permissions, size, num_chunks) values ('$filename', '$owner', '$group', 750, $file_size, $num_chunks)"
file_id=$($psql_command "select max(file_id) from files" | tail -n +3 | head -n -2 | sed "s/ //g")

for chunk in $(seq 1 $num_chunks)
do
#	echo "${servers[$s_index]}"

	#Transfer file to storage server
	transfer_file "${sp_output_path}.${chunk}" "${file_id}.${chunk}" "${server_hosts[$s_index]}"
	#Add file chunk location to database
	$rpsql_command "insert into file_chunks values ($file_id, $chunk, '${server_ids[$s_index]}')"
	s_index=$(($((s_index+1)) % $num_servers))
done

#Delete encrypted file
rm $en_output_path

#Delete original file
#rm $en_input_path
