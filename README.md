* Add MYSQL Connector/C++
* Add Encryption
* Add Login/Register Dialog
# Windows
* Build
```bash
git clone --recursive https://github.com/exsycore/open.mp-gm-cpp.git
mkdir build
cd build
cmake .. -A Win32 -T ClangCL
```
* Debug / Release
```bash
cd build
cmake --build . --config Debug หรือ Release
```
