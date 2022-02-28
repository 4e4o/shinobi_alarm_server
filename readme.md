mkdir shinobi_alarm_build && cd shinobi_alarm_build
cmake -DCMAKE_BUILD_TYPE=Release ../shinobi_alarm .
cmake --build .
sudo cmake --install .
