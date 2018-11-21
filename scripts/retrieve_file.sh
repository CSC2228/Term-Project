#!/bin/bash

#Run from the master

file_id=$1
owner=$2
edge_id=$3


tmp_path="/var/lib/csc2228/Term-Project/tmp/"
files_path="/var/lib/csc2228/Term-Project/files/"

psql_command="psql -d csc2228 -U postgres -w -c"
decrypt_file_command="/var/lib/csc2228/Term-Project/scripts/decrypt"
join_file_command="/var/lib/csc2228/Term-Project/scripts/join_file"
edge_server=$($psql_command "select ip_address from servers where type = 'edge' and server_id = $edge_id" | tail -n +3 | head -n -2 | sed "s/ //g")


#integrity_check() {
#	#Check for user permissions to make sure user has rights to get file
#	#Check if user has enough on their quota
#	return 1
#}


transfer_file() {
	tf_filename=$1
	tf_hostname=$2
	ssh -A $tf_hostname rsync -avq "/var/lib/csc2228/files/${tf_filename}" ${edge_server}:/var/lib/csc2228/Term-Project/tmp/
	return 0
}


#Find IP Addresses of storage servers to push to
server_ips=$($psql_command "select ip_address from servers s1, file_chunks s2 where type = 'storage' and s1.server_id = s2.server_id and file_id = '$file_id'" | tail -n +3 | head -n -2 | sed "s/ //g")
server_chunks=$($psql_command "select chunk_num from servers s1, file_chunks s2 where type = 'storage' and s1.server_id = s2.server_id and file_id = '$file_id'" | tail -n +3 | head -n -2 | sed "s/ //g")
mapfile -t server_ips <<< "$server_ips"
mapfile -t server_chunks <<< "$server_chunks"
num_servers=${#server_ips[@]}
s_index=0

filename=$($psql_command "select filename from files where file_id = $file_id" | tail -n +3 | head -n -2 | sed "s/ //g")

for chunk in $(seq 1 $num_servers)
do
	#Transfer file to edge server
	transfer_file "${file_id}.${server_chunks[$s_index]}" "${server_ips[$s_index]}"
	s_index=$(($((s_index+1)) % $num_servers))
done

ssh $edge_server $join_file_command "${file_id}" "${tmp_path}" "${files_path}"
ssh $edge_server $decrypt_file_command "${files_path}${file_id}" "${files_path}${filename}"
ssh $edge_server rm "${files_path}${file_id}" "${tmp_path}${file_id}.*"
