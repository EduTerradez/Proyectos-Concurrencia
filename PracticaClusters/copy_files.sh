HOSTS=$(cat host_file)

for I in $HOSTS
do
 ssh $I mkdir edu
 scp mpi_main $I:~/edu/
done
