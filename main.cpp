#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "image_l4.hpp"


ImagePPM doKernal(double kernel[3][3], ImagePPM inImage){
    ImagePPM outImage;
    init_image(outImage, inImage.width - 2, inImage.height - 2);

    for(int row = 1; row < inImage.width - 1; row++){
        for(int col = 1; col < inImage.height - 1; col++){
            double pixelValue = 0;
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    pixelValue += get_pixel(inImage, row, col, 3) * kernel[i][j];
                    std::cout<<pixelValue<<std::endl;
                }
            }
        }
    }
    return outImage;
}

int gx() {


}

int gy() {

}

double signaltonoise(){
    double signal = 0;
    double powerNoise=0;

}

int main() {
    double kernel[3][3] = {{1.0 / 9, 1.0 / 9, 1.0 / 9},
                           {1.0 / 9, 1.0 / 9, 1.0 / 9},
                           {1.0 / 9, 1.0 / 9, 1.0 / 9}};



    ImagePPM image;
    OpenPPMFile("line.ppm", image);

    doKernal(kernel,image);

    SavePPMFile("blurout.ppm", image);


}