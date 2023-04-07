# README.md

### Notice

support motor: A1 motor、 B1 motor 

not support motor: GO-M8010-6 motor (Check GO-M8010-6 branch for support)

gcc >= 5.4.0 (for libunitreeMotorSDK_Linux64.so)

gcc >= 8.3.0 (for libunitreeMotorSDK_Arm64.so)

run gcc -v  command to check your gcc version

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Run
Run examples with 'sudo',e.g.
```bash
sudo ./motorctrl
```