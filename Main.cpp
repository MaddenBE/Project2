#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include "Read_Write.h"
using namespace std; //5

int main() {
    Header header;
    unsigned char *buffer1 = new unsigned char[0];
    unsigned char *buffer2 = new unsigned char[0];
    //unsigned char* buffer3 = new unsigned char[0];
    read(R"(C:/Users/suzan/Desktop/ImageProcessing Spring 2023/input/layer1.tga)", header, buffer1);
    read(R"(C:/Users/suzan/Desktop/ImageProcessing Spring 2023/input/pattern1.tga)", header, buffer2);
    //read(R"(C:/Users/suzan/Desktop/ImageProcessing Spring 2023/input/pattern2.tga)", header, buffer3);
    overlay(buffer1,buffer2,header);
    //subtract(buffer1,buffer3,header);
    write(R"(C:/Users/suzan/Desktop/ImageProcessing Spring 2023/output/Task5.tga)", header, buffer1);
    return 0;
}