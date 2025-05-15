* Add MYSQL Connector/C++
* Add Encryption
* Add Login/Register Dialog
# Windows
* Clone
```bash
git clone --recursive https://github.com/exsycore/open.mp-gm-cpp.git
```
* Build
```bash
mkdir build
cd build
cmake .. -A Win32 -T ClangCL
```
* Debug / Release
```bash
cd build
cmake --build . --config Debug หรือ Release
```
