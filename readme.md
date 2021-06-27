mkdir shinobi_alarm_build
cd shinobi_alarm_build
cmake -DCMAKE_INSTALL_PREFIX="../shinobi_alarm_install" -DCMAKE_BUILD_TYPE=MinSizeRel ../shinobi_alarm .
cmake --build .
cmake --install .
