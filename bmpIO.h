#include <iostream>
#include <fstream>
using namespace std;
#pragma pack(push, 1)
struct FILEHEADER{
    unsigned char   id[2];
    unsigned        fsize;//bmp_file size
    unsigned short  reserve_1;
    unsigned short  reserve_2;
    unsigned        offset;//header bias
};
#pragma pack(pop)

#pragma pack(push, 1)
struct INFOHEADER{
    unsigned        info_size;//info header size
    int             width;//image width
    int             height;//image height
    unsigned short  planes;
    unsigned short  bits;//bit per pixel
    unsigned        compress;
    unsigned        im_size;
    int             x_reso;
    int             y_reso;
    unsigned        color;
    unsigned        imp_color;   
};
#pragma pack(pop)

void readimage(ifstream &fin, FILEHEADER &FH, INFOHEADER &IH, unsigned char **palette, unsigned char **raw_data);

void printHeader(FILEHEADER FH, INFOHEADER IH);

void writeimage(const char *outfile_name, FILEHEADER &FH, INFOHEADER &IH, unsigned char **palette, unsigned char **raw_data);