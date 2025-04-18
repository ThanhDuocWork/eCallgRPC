# GUIDE
## 1. How to gen code gRPC
- In "~/eCallgRPC/Services/ServiceManager", use command:
    ` protoc --grpc_out=gencode --cpp_out=gencode --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` service.proto `

## 2. Build source
   - In "~/eCallgRPC" folder, use command:
     `./build.sh `

   - If all libs already exist, the source is ready to build
    - Install libs:
    - check GLIBC SSH ver: ` ldd --version `
    - Check kernel: ` cat /proc/version `
    - Check OS: ` lsb_release -a `
    - Install lib MQTT Mosquitto: ` sudo apt install libmosquitto-dev `
    - Install lib Audio:` sudo apt install libasound2-dev `  

## 3. How to run source code 

   - After build source we run code using command:
        - For ServiceManager:
            `cd ~/eCallgRPC/output/Services/ServiceManager `
            ` ./ServiceManager`
        - For EcallApp:
            ` cd ~/eCallgRPC/output/Apps `
            ` ./MainApp`
        - For ServiceManager:
            `cd ~/eCallgRPC/output/Services/ServiceManager `
            ` ./ServiceManager`
        - For MQTTSrv:
            `cd ~/eCallgRPC/output/Services/MQTTSrv `
            ` ./MQTTSrv `
        - For AudioSrv:
            `cd ~/eCallgRPC/output/Services/AudioSrv  `
            ` ./AudioSrv `    
        - For HMIsrv:
            `cd ~/eCallgRPC/output/Services/HMISrv  `
            ` ./HMISrv `     

Link refer: 
- [gRPC example](https://github.com/chungphb/grpc-cpp)
- [gRPC-cpp](https://grpc.io/docs/languages/cpp/) 

## 4. Prepare IPC gRPC
### 4.1. Clone repo gRPC, ussing command:
- Clone source for linux:
    ` git clone --recurse-submodules -b v1.37.1 https://github.com/grpc/grpc `
    ` cd grpc `
### 4.2. Create dir build and install gRPC
- Using command:
        ` mkdir -p cmake/build `
        ` pushd cmake/build `
        ` cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR ../.. `
        ` make -j$(nproc) `
        ` make install `
        ` popd `
### 4.3. Check
- Using command:
        ` protoc --version `
        ` ls /usr/local/lib | grep grpc `
        ` ls /usr/local/include/grpc `
        ` ls ~/grpc/cmake/build `
        ` ls ~/grpc/cmake/build/lib `
### 4.4. Export lib:
- Using command:
        ` export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH `
        ` sudo ldconfig `
    - Check export: 	
        ` pkg-config --modversion grpc `
        ` export PKG_CONFIG_PATH=/usr/local/lib/libs/opt/pkgconfig:$PKG_CONFIG_PATH `
        ` echo 'export PKG_CONFIG_PATH=/usr/local/lib/libs/opt/pkgconfig:$PKG_CONFIG_PATH' >> ~/.bashrc `
        ` source ~/.bashrc `
### 4.5. Copy lib to lib system
- Using command:
    ` sudo cp -r ~/grpc/cmake/build/lib* /usr/local/lib/ `
    ` sudo cp -r ~/grpc/include /usr/local/include/grpc `
    ` sudo ldconfig `
    ` echo "/usr/local/lib" | sudo tee -a /etc/ld.so.conf.d/grpc.conf `
    ` sudo ldconfig `

## 5. Tool tilix
*Note: install tilix, used to divide the terminal screen for easy*
`sudo apt update`
`sudo apt installl tilix`
[Link refer](https://www.geeksforgeeks.org/how-to-install-tilix-terminal-emulator-in-ubuntu-fedora-arch-linux/)     
                    
