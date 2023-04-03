#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

 int bmp_s;
 int width;
 int height;
 int frame_s;

 #pragma pack(push, 1)
struct BMPHeader
{
    char signature[2] = {'B', 'M'};
    uint32_t file_size=0;
    uint32_t reserved = 0;
    uint32_t offset = 54;
};
struct BMPInfoHeader
{
    uint32_t headerSize = 40;
    int32_t width = 0;
    int32_t height = 0;
    uint16_t planes = 1;
    uint16_t bitCount = 24;
    uint32_t compression = 0;
    uint32_t imageSize = 0;
    int32_t xPixelsPerMeter = 0;
    int32_t yPixelsPerMeter = 0;
    uint32_t colorsUsed = 0;
    uint32_t colorsImportant = 0;
};
#pragma pack(pop)

int main()
{

cout<<"Enter width of the video:";
cin>>width;

cout<<"Enter height of the video:";
cin>>height;

 frame_s = width * height * 3 / 2;
 bmp_s = width * height * 3;



string vF;
cout<<"Enter the name of the video:";
cin>>vF;

ifstream videoFile(vF, ios::in | ios::binary);

    if (!videoFile)
    {
        cout << "Failed to open video file!" << endl;
        return 1;
    }

    int frameNumber;

    cout<<"Enter Number of frame in the video:";
    cin>>frameNumber;

    system("mkdir output");

    for (int frame = 0; frame < frameNumber; frame++)
    {
        
        unsigned char *yuvBuffer = new unsigned char[frame_s];
        unsigned char *rgbBuffer = new unsigned char[bmp_s];

        
     videoFile.read(reinterpret_cast<char *>(yuvBuffer), frame_s);

        
        int i = 0;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int Y = yuvBuffer[y * width + x];
                int U = yuvBuffer[width * height + (y / 2) * (width / 2) + (x / 2)];
                int V = yuvBuffer[width * height * 5 / 4 + (y / 2) * (width / 2) + (x / 2)];

                int R = Y + 1.13983 * (V - 128);
                int G = Y - 0.39465 * (U - 128) - 0.58060 * (V - 128);
                int B = Y + 2.03211 * (U - 128);

                R = min(max(R, 0), 255);
                G = min(max(G, 0), 255);
                B = min(max(B, 0), 255);

                rgbBuffer[i++] = static_cast<unsigned char>(B);
                rgbBuffer[i++] = static_cast<unsigned char>(G);
                rgbBuffer[i++] = static_cast<unsigned char>(R);
            }
        }


        char output_filename[256];
        sprintf(output_filename, "output/frame%04d.bmp", frame);
        ofstream outputFile(output_filename, ios::binary);

        BMPHeader header;
        BMPInfoHeader infoHeader;
        header.file_size = sizeof(header) + sizeof(infoHeader) + bmp_s;
        infoHeader.width = width;
        infoHeader.height =-height;

       
        outputFile.write(reinterpret_cast<char *>(&header), sizeof(header));
        outputFile.write(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));
        outputFile.write(reinterpret_cast<char*>(rgbBuffer), bmp_s);

        outputFile.close();
        
        delete[] yuvBuffer;
        delete[] rgbBuffer;

        cout << frame << endl;

    
    }

    videoFile.close();

    return 0;
}