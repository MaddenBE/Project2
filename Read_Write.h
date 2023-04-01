#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <cmath>

using namespace std;

struct Header{
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};
void read(string const filename, Header& header, unsigned char*& buffer) {
    fstream input(filename, ios_base::in | ios_base::binary );
    input.read((char*)(&header.idLength),1);
    input.read((char*)(&header.colorMapType),1);
    input.read((char*)(&header.dataTypeCode),1);
    input.read((char*)(&header.colorMapOrigin),2);
    input.read((char*)(&header.colorMapLength),2);
    input.read((char*)(&header.colorMapDepth),1);
    input.read((char*)(&header.xOrigin),2);
    input.read((char*)(&header.yOrigin),2);
    input.read((char*)(&header.width),2);
    input.read((char*)(&header.height),2);
    input.read((char*)(&header.bitsPerPixel),1);
    input.read((char*)(&header.bitsPerPixel),1);
    buffer = new unsigned char[header.width * header.height *3];
    for(int i = 0; i < (header.width * header.height * 3); ++i){
        input.read((char*)(&buffer[i]),1);
    }

}

void write(string filename, Header& header, unsigned char *buffer) {
    ofstream outfile (filename, ios::binary);
    outfile.write((char*)&header.idLength, 1);
    outfile.write((char*)&header.colorMapType, 1);
    outfile.write((char*)&header.dataTypeCode, 1);
    outfile.write((char*)&header.colorMapOrigin, 2);
    outfile.write((char*)&header.colorMapLength, 2);
    outfile.write((char*)&header.colorMapDepth, 1);
    outfile.write((char*)&header.xOrigin, 2);
    outfile.write((char*)&header.yOrigin, 2);
    outfile.write((char*)&header.width, 2);
    outfile.write((char*)&header.height, 2);
    outfile.write((char*)&header.bitsPerPixel, 1);
    outfile.write((char*)&header.imageDescriptor, 1);
    for (int i = 0; i < header.width * header.height * 3; ++i) {
        outfile.write((char *) (&buffer[i]), 1);
    }
    outfile.close();
    delete[] buffer;
}
//fc.exe /b .\output\task1.tga .\examples\example_part1.tga
void multiply(char* firstimg, char* secondimg, Header& header){
    for (int i = 0; i < (header.width * header.height * header.bitsPerPixel)/8; i++ ){
        float pixelVal = ((float)(unsigned char)firstimg[i]/255.0f * (float)(unsigned char)secondimg[i]/255.0f)*255.0f + 0.5f;
        firstimg[i] = (char) pixelVal;
    }
}

void subtract(char* firstimg, char* secondimg, Header& header){
    for (int i = 0; i < (header.width * header.height * header.bitsPerPixel) / 8; i++) {
        int pixelValue = (unsigned char) firstimg[i] - (unsigned char) secondimg[i];
        if (pixelValue >= 0){
            firstimg[i] = (char)pixelValue;
        }
        else{
            firstimg[i] = 0;
        }
    }
}

void overlay(unsigned char *firstimg, unsigned char *secondimg, Header& header){
    for(int i = 0; i < ((header.width * header.height * header.bitsPerPixel) / 8); i++){
        /*unsigned char outB;
        unsigned char outG;
        unsigned char outR;
        unsigned char fB = firstimg[3 * i];
        unsigned char fG = firstimg[3 * i + 1];
        unsigned char fR = firstimg[3 * i + 2];
        unsigned char sB = secondimg[3 * i];
        unsigned char sG = secondimg[3 * i + 1];
        unsigned char sR = secondimg[3 * i + 2];
        if (sB < 128){outB =(2 * sB * fB /255);}
        else{outB = (255-2*(255-sB)*(255-fB)/255);}
        if (sG < 128){outG = (2 * sG * fG /255);}
        else{outG = (255-2*(255-sG)*(255-fG)/255);}
        if (sR < 128){outR = (2 * sR * fR /255);}
        else{outR = (255-2*(255-sR)*(255-fR)/255);}
        firstimg[3*i] = outB;
        firstimg[3*i+1] = outG;
        firstimg[3*i+2] = outR;*/
        float secpix = (float)secondimg[i]/255.0f;
        float firstpix = (float)firstimg[i]/255.f;
        if (secpix <= 0.5f){
            float pixelval = (2.0f * firstpix * secpix) * 255.0f +0.5f;
            firstimg[i] = (unsigned char)pixelval;
        }
        else{
            float pixelval = 1.0f - (2.0f * (1.0f - firstpix) * (1.0f - secpix));
            pixelval = pixelval * 255.0f + 0.5f;
            firstimg[i] = (unsigned char) pixelval;
        }

    }

}

void screen(unsigned char* firstimg, unsigned char* secondimg, Header& header){
    for (int i = 0; i < (header.width * header.height * header.bitsPerPixel) / 8; i++){
        float pix_val = 255 - ((255 - (unsigned char)firstimg[i]) * (255 - (unsigned char)secondimg[i]))/255.0f;
        firstimg[i] = static_cast<unsigned char>(pix_val + 0.5f);
    }
}

void combine(unsigned char*& firstimg, unsigned char*& secimg, unsigned char*& thirdimg, Header& header){
    for (int i = 0; i < (header.width * header.height * header.bitsPerPixel)/8; i += 3){
        firstimg[i + 1] = (char)secimg[i + 1];
        firstimg[i] = (char)thirdimg[i];
    }
}

void flip(unsigned char* img, Header& header) {
    for(int i = 0; i < header.height/2; i++) {
        for(int j = 0; j < header.width * (header.bitsPerPixel/8);j+= header.bitsPerPixel/8){
            int top = i * header.width * (header.bitsPerPixel/8) + j;
            int bottom = (header.height - i - 1) * header.width *(header.bitsPerPixel/8) + j;
            swap(img[top],img[bottom]);
            swap(img[top + 1], img[bottom + 1]);
            swap(img[top+2],img[bottom+2]);
        }
    }
    for(int i = 0; i < header.height;i++){
        for(int j = 0; j < (header.width/2) * (header.bitsPerPixel/8); j+= header.bitsPerPixel/8){
            int left = i * header.width *(header.bitsPerPixel/8) + j;
            int right = (i+1) * header.width * (header.bitsPerPixel/8) - (j + header.bitsPerPixel/8);
            swap(img[left], img[right]);
            swap(img[left + 1], img[right + 1]);
            swap(img[left + 2], img[right + 2]);
        }
    }
}
void onlyred(char* img, Header& header){
    int numPix = header.height * header.width;
    for (int i = 0; i < numPix; i++){
        int Index = i * (header.bitsPerPixel/8);
        img[Index] = img[Index + 2];
        img[Index + 1] = img[Index + 2];
    }
}

void onlygreen(char* img, Header& header){
    int numPix = header.height * header.width;
    for (int i = 0; i < numPix; i++){
        int Index = i * (header.bitsPerPixel/8);
        img[Index] = img[Index + 1];
        img[Index + 2] = img[Index + 1];
    }
}

void onlyblue(char* img, Header& header){
    int numPix = header.height * header.width;
    for (int i = 0; i < numPix; i++){
        int Index = i * (header.bitsPerPixel/8);
        img[Index + 1] = img[Index];
        img[Index + 2] = img[Index];
    }
}

void addred(char* img, int val, Header& header){
    int numPix = header.height * header.width;
    for (int i = 0; i < numPix; i++){
        int Index = i * (header.bitsPerPixel/8);
        int inc = (unsigned char)img[Index + 2];
        inc += val;
        if (inc > 255){
            img[Index+2] = (char)255;
        }
        else{
            img[Index+2] = (char)inc;
        }
    }
}

void addgreen(char* img, int val, Header& header){
    int numPix = header.height * header.width;
    for (int i = 0; i < numPix; i++){
        int Index = i * (header.bitsPerPixel/8);
        int inc = (unsigned char)img[Index + 1];
        inc += val;
        if (inc > 255){
            img[Index+1] = (char)255;
        }
        else{
            img[Index+1] = (char)inc;
        }
    }
}

void addblue(char* img, int val, Header& header){
    int numPix = header.height * header.width;
    for (int i = 0; i < numPix; i++){
        int Index = i * (header.bitsPerPixel/8);
        int inc = (unsigned char)img[Index];
        inc += val;
        if (inc > 255){
            img[Index] = (char)255;
        }
        else{
            img[Index] = (char)inc;
        }
    }
}

void scalered(char* img, Header& header, float factor){
    int pix = header.bitsPerPixel/8;
    for(int i =0; i < (header.height * header.width); i++){
        int val = (unsigned char)img[i * pix + 2];
        val *= factor;
        if (val > 255){
            val = (unsigned char)255;
        }
        else if (val < 0){
            val = (unsigned char)0;
        }
        img[i * pix + 2] = (char)val;
    }
}

void scalegreen(char* img, Header& header, float factor){
    int pix = header.bitsPerPixel/8;
    for(int i =0; i < (header.height * header.width); i++){
        int val = (unsigned char)img[i * pix + 1];
        val *= factor;
        if (val > 255){
            val = (unsigned char)255;
        }
        else if (val < 0){
            val = (unsigned char)0;
        }
        img[i * pix + 1] = (char)val;
    }
}

void scaleblue(char* img, Header& header, float factor){
    int pix = header.bitsPerPixel/8;
    for(int i =0; i < (header.height * header.width); i++){
        int val = (unsigned char)img[i * pix];
        val *= factor;
        if (val > 255){
            val = (unsigned char)255;
        }
        else if (val < 0){
            val = (unsigned char)0;
        }
        img[i * pix] = (char)val;
    }
}