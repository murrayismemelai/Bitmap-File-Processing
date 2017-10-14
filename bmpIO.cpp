#include "bmpIO.h"

void readimage(ifstream &fin, FILEHEADER &FH, INFOHEADER &IH, unsigned char **palette, unsigned char **raw_data){
    if(!fin)
        return;
    fin.seekg(0, ios::beg);
    fin.read((char*) &FH, sizeof(FH));
    fin.read((char*) &IH, sizeof(IH));
    unsigned image_size = IH.im_size;
    unsigned palette_num = FH.offset - 54;
    //cout << palette_num << endl;
    
    if(palette_num){
        *palette  = new unsigned char[palette_num];
        fin.read((char*)*palette, palette_num);
    }
    int pad = (4-(IH.width*IH.bits/8)%4);
    
    if((IH.width*IH.bits/8)%4 != 0)
        *raw_data = new unsigned char[image_size - pad * IH.height];
    else
        *raw_data = new unsigned char[image_size];
    
    if((IH.width*IH.bits/8)%4 != 0){
        for(int j = 0; j < IH.height; j++){
            for(int i =0; i < IH.width * IH.bits/8; i++){
                fin.read((char*)(*raw_data + IH.width * IH.bits/8 * j + i), 1);
            }
            fin.seekg(pad, ios::cur);
        }
    }
    else
        fin.read((char*)*raw_data, image_size);
    fin.close();
}

void printHeader(FILEHEADER FH, INFOHEADER IH){
    cout << "== BMP FILEHEADER =="  <<  endl;
    cout << "+ ID             :"    <<  FH.id[0]      << FH.id[1]   << endl;
    cout << "+ File Size      :"    <<  FH.fsize      << " byte(s)" << endl;
    cout << "+ Reserved1      :"    <<  FH.reserve_1                << endl;
    cout << "+ Reserved2      :"    <<  FH.reserve_2                << endl;
    cout << "+ Data Offset    :"    <<  FH.offset     << " byte(s)" << endl;
    cout << "+ FILEHEADER size:"    <<  sizeof(FH)                  << endl;
    cout << "== BMP INFOHEADER =="  <<  endl;
    cout << "+ Info Size      :"    <<  IH.info_size  << " byte(s)" << endl;
    cout << "+ width          :"    <<  IH.width                    << endl;
    cout << "+ height         :"    <<  IH.height                   << endl;
    cout << "+ channel bits:  :"    <<  IH.bits       << " bit(s)"  << endl;
    cout << "+ INFOHEADER size:"    <<  sizeof(IH)                  << endl;
}

void writeimage(const char *outfile_name, FILEHEADER &FH, INFOHEADER &IH, unsigned char **palette, unsigned char **raw_data){
    ofstream fout(outfile_name, ios::binary); 
    fout.seekp(0, ios::beg);
    unsigned palette_num = FH.offset - 54;
    
    int pad = (4-(IH.width*IH.bits/8)%4);
    if((IH.width*IH.bits/8)%4 != 0){
        IH.im_size = IH.width * IH.height * (int)IH.bits / 8 + pad * IH.height;
        FH.fsize = IH.im_size + FH.offset;
    }
    else{
        IH.im_size = IH.width * IH.height * (int)IH.bits / 8;
        FH.fsize = IH.im_size + FH.offset;
    }
    
    fout.write((char*) &FH, sizeof(FH));
    fout.write((char*) &IH, sizeof(IH));
    if(palette_num){
        fout.write((char*)*palette, palette_num);
    }

    unsigned image_size = IH.im_size;
    unsigned char zero[1] = {0};
    if((IH.width*IH.bits/8)%4 != 0){
        for(int j = 0; j < IH.height; j++){
            for(int i =0; i < IH.width * IH.bits/8; i++){
                fout.write((char*)(*raw_data + IH.width * IH.bits/8 * j + i), 1);
            }
            for(int k =0; k < pad ; k++)
                fout.write((char*)zero, 1);
        }
    }
    else
        fout.write((char*)*raw_data, image_size);

    fout.close();
}