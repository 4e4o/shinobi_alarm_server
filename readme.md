git clone --recurse-submodules git@github.com:4e4o/shinobi_alarm_server.git  
mkdir shinobi_alarm_server_build && cd shinobi_alarm_server_build  
cmake -DCMAKE_BUILD_TYPE=Release ../shinobi_alarm_server .  
cmake --build .  
sudo cmake --install .  

sudo systemctl daemon-reload  
sudo systemctl enable shinobi_alarm_server.service  
sudo systemctl start shinobi_alarm_server.service  
