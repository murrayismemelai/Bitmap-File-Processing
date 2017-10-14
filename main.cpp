#include "bmpIO.h"
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <string.h>
using namespace std;

void nearest_scaling(int width, int height, unsigned short bits, int width_resize, int height_resize, unsigned char **raw_data, unsigned char** resize_img){
    int x, y, index;
    unsigned char A, B, C, D;
    float x_diff, y_diff;
    int bytes = (int) bits / 8;
    //int width_resize = width * scale_factor;
    //int height_resize = height * scale_factor;
    float x_ratio = (float)(width-1)/(width_resize);
    float y_ratio = (float)(height-1)/(height_resize);
    *resize_img = new unsigned char[width_resize*height_resize*bytes];
    
    for(int row = 0; row < height_resize; row++)
        for(int col = 0; col < width_resize; col++){
            x = (int) (x_ratio * col);
            y = (int) (y_ratio * row);
            x_diff = x_ratio * col - x;
            y_diff = y_ratio * row - y;
            for(int depth = 0; depth < bytes; depth++){
                index = y*width*bytes + x*bytes + depth;
                A = *(*raw_data + index);
                B = *(*raw_data + index + bytes);
                C = *(*raw_data + index + width*bytes);
                D = *(*raw_data + index + width*bytes + bytes);
                
                if (x_diff <= (1-x_diff) && y_diff <= (1-y_diff))
                    *(*resize_img + row*width_resize*bytes + col*bytes + depth) = A;
                else if(x_diff > (1-x_diff) && y_diff <= (1-y_diff))
                    *(*resize_img + row*width_resize*bytes + col*bytes + depth) = B;
                else if(x_diff <= (1-x_diff) && y_diff > (1-y_diff))
                    *(*resize_img + row*width_resize*bytes + col*bytes + depth) = C;
                else if(x_diff > (1-x_diff) && y_diff > (1-y_diff))
                    *(*resize_img + row*width_resize*bytes + col*bytes + depth) = D;
                else
                    *(*resize_img + row*width_resize*bytes + col*bytes + depth) = A;
            }
        }
}

void bilinear_scaling(int width, int height, unsigned short bits, int width_resize, int height_resize, unsigned char **raw_data, unsigned char** resize_img){
    int x, y, index;
    unsigned char A, B, C, D;
    float x_diff, y_diff;
    int bytes = (int) bits / 8;
    //int width_resize = width * scale_factor;
    //int height_resize = height * scale_factor;
    float x_ratio = (float)(width-1)/(width_resize);
    float y_ratio = (float)(height-1)/(height_resize);
    *resize_img = new unsigned char[width_resize*height_resize*bytes];
    
    for(int row = 0; row < height_resize; row++)
        for(int col = 0; col < width_resize; col++){
            x = (int) (x_ratio * col);
            y = (int) (y_ratio * row);
            x_diff = x_ratio * col - x;
            y_diff = y_ratio * row - y;
            for(int depth = 0; depth < bytes; depth++){
                index = y*width*bytes + x*bytes + depth;
                A = *(*raw_data + index);
                B = *(*raw_data + index + bytes);
                C = *(*raw_data + index + width*bytes);
                D = *(*raw_data + index + width*bytes + bytes);
                
                *(*resize_img + row*width_resize*bytes + col*bytes + depth) = 
                    A*(1-x_diff)*(1-y_diff) + B*(x_diff)*(1-y_diff) + C*(1-x_diff)*(y_diff) + D*(x_diff)*(y_diff); 
            }
        }
}
/*
void bicubic_scaling(int width, int height, unsigned short bits, int width_resize, int height_resize, unsigned char **raw_data, unsigned char** resize_img){
    int x, y, index;
    unsigned char A, B, C, D;
    float x_diff, y_diff;
    int bytes = (int) bits / 8;
    //int width_resize = width * scale_factor;
    //int height_resize = height * scale_factor;
    float x_ratio = (float)(width-1)/(width_resize);
    float y_ratio = (float)(height-1)/(height_resize);
    *resize_img = new unsigned char[width_resize*height_resize*bytes];
    
    for(int row = 0; row < height_resize; row++)
        for(int col = 0; col < width_resize; col++){
            x = (int) (x_ratio * col);
            y = (int) (y_ratio * row);
            x_diff = x_ratio * col - x;
            y_diff = y_ratio * row - y;
            for(int depth = 0; depth < bytes; depth++){
                index = y*width*bytes + x*bytes + depth;
                A = *(*raw_data + index);
                B = *(*raw_data + index + bytes);
                C = *(*raw_data + index + width*bytes);
                D = *(*raw_data + index + width*bytes + bytes);
                
                //to do
            }
        }
}
*/
int main(int argc, char **argv)
{
    int mode = atoi(argv[1]);
    char *in_file  = argv[2];
    //char *out_file = argv[3];
    
    stringstream ss;
    
    if(argc!=3){
        cerr << "error command" << endl;
        exit(1);
    }
    ifstream fin(in_file, ios::binary);
    //ofstream fout(out_file, ios::binary); 
    FILEHEADER FH;
    INFOHEADER IH;
    unsigned char *palette;
    unsigned char *raw_data;
    unsigned char *resize_img;
    
    
    switch (mode){
        case 0 :
            readimage(fin, FH, IH, &palette, &raw_data);
            printHeader(FH, IH);
            if (strcmp(in_file,"input1.bmp")==0)
                ss << "output1.bmp";
            else if(strcmp(in_file,"input2.bmp")==0)
                ss << "output2.bmp";
            else{
                ss << "output3.bmp";
                //cerr << "invalid input" << endl;
                //exit(1);
            }
            //writeimage(fout, FH, IH, &palette, &raw_data);
            writeimage(ss.str().c_str(), FH, IH, &palette, &raw_data);
            break;
        case 1 :
            readimage(fin, FH, IH, &palette, &raw_data);
            for (int quantize = 1; quantize < 4; quantize++){
                ss.str("");
                for(int j = 0; j < IH.height; j++){
                    for(int i = 0; i < IH.width; i++){
                        *(raw_data + IH.bits/8*(IH.width*j+i)+2) = *(raw_data + IH.bits/8*(IH.width*j+i)+2) / (16*pow(2,quantize-1));
                        *(raw_data + IH.bits/8*(IH.width*j+i)+1) = *(raw_data + IH.bits/8*(IH.width*j+i)+1) / (16*pow(2,quantize-1));
                        *(raw_data + IH.bits/8*(IH.width*j+i)+0) = *(raw_data + IH.bits/8*(IH.width*j+i)+0) / (16*pow(2,quantize-1));
                                                                                                                                   
                        *(raw_data + IH.bits/8*(IH.width*j+i)+2) = *(raw_data + IH.bits/8*(IH.width*j+i)+2) * (16*pow(2,quantize-1));
                        *(raw_data + IH.bits/8*(IH.width*j+i)+1) = *(raw_data + IH.bits/8*(IH.width*j+i)+1) * (16*pow(2,quantize-1));
                        *(raw_data + IH.bits/8*(IH.width*j+i)+0) = *(raw_data + IH.bits/8*(IH.width*j+i)+0) * (16*pow(2,quantize-1));
                    }
                }
                
                if (strcmp(in_file,"input1.bmp")==0)
                    ss << "output1_" << quantize << ".bmp";
                else if(strcmp(in_file,"input2.bmp")==0)
                    ss << "output2_" << quantize << ".bmp";
                else{
                    ss << "output3_" << quantize << ".bmp";
                    //cerr << "invalid input" << endl;
                    //exit(1);
                }
                writeimage(ss.str().c_str(), FH, IH, &palette, &raw_data);
            }
            
            break;
        case 2 :
            readimage(fin, FH, IH, &palette, &raw_data);
            int raw_width = IH.width;
            int raw_height = IH.height;
            unsigned raw_im_size = IH.im_size;
            unsigned raw_fsize = FH.fsize;
            printHeader(FH, IH);
            
            IH.width  *= 1.5;
            IH.height *= 1.5;
            //IH.im_size = IH.width * IH.height * (int)IH.bits / 8;
            //FH.fsize = IH.im_size + FH.offset;
            
            bilinear_scaling(raw_width, raw_height, IH.bits, IH.width, IH.height, &raw_data, &resize_img);
            cout << endl << "up_scaling" << endl;
            printHeader(FH, IH);
            
            if (strcmp(in_file,"input1.bmp")==0)
                ss << "output1_up.bmp";
            else if(strcmp(in_file,"input2.bmp")==0)
                ss << "output2_up.bmp";
            else{
                ss << "output3_up.bmp";
                //cerr << "invalid input" << endl;
                //exit(1);
            }
            
            //writeimage(fout, FH, IH, &palette, &raw_data);
            writeimage(ss.str().c_str(), FH, IH, &palette, &resize_img);
            
            delete [] resize_img;
            
            IH.width  = raw_width;
            IH.height = raw_height;
            IH.im_size = raw_im_size;
            FH.fsize = raw_fsize;
            
            IH.width  /= 1.5;
            IH.height /= 1.5;
            //IH.im_size = IH.width * IH.height * (int)IH.bits / 8;
            //FH.fsize = IH.im_size + FH.offset;
            bilinear_scaling(raw_width, raw_height, IH.bits, IH.width, IH.height, &raw_data, &resize_img);
            
            cout << endl << "down_scaling" << endl;
            printHeader(FH, IH);
            ss.str("");
            
            if (strcmp(in_file,"input1.bmp")==0)
                ss << "output1_down.bmp";
            else if(strcmp(in_file,"input2.bmp")==0)
                ss << "output2_down.bmp";
            else{
                ss << "output3_down.bmp";
                //cerr << "invalid input" << endl;
                //exit(1);
            }
            
            writeimage(ss.str().c_str(), FH, IH, &palette, &resize_img);
            break;
        
    }
    
    return 0;

}
