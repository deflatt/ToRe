#include <functional>
#include <iostream>
#include <Windows.h>
#include <map>
#include <tuple>
#include <set>

struct Image {
    int i;
};

struct FileImage {
    
    

};

struct DrawImage : public Image {



};

struct DrawFileImage : public DrawImage, public FileImage {};

int main() {
 
    

    return 0;
}