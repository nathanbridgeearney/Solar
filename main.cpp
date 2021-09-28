#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "image_l4.hpp"


ImagePPM doKernal(double kernel[3][3], ImagePPM inputImage) {
    ImagePPM outImage;
    init_image(outImage, inputImage.width, inputImage.height);
    for (int row = 1; row < inputImage.height - 1; row++) {
        for (int col = 1; col < inputImage.width - 1; col++) {
            double pixelValue = 0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    pixelValue += get_pixel(inputImage, row + i, col + j, 3) * kernel[i + 1][j + 1];

                }
            }
            set_pixel(outImage, row, col, pixelValue, pixelValue, pixelValue);
        }
    }
    return outImage;
}

double signaltonoise(ImagePPM inputImage) {
    float signal = 0;
    for (int col = 10; col < 20; ++col) {
        for (int row = 10; row < 20; ++row) {
            signal += get_pixel(inputImage, row, col, 3);

        }
    }
    signal /= 100;
    float signalPow = signal * signal;
    float noise = 0;
    for (int col = 10; col < 20; ++col) {
        for (int row = 10; row < 20; ++row) {
            noise += (get_pixel(inputImage, row, col, 3) - signal) * (get_pixel(inputImage, row, col, 3) - signal);
        }
    }
    noise /= 100;
    float noisePow = noise * noise;
    return signalPow / noisePow;
}


int main() {
    double blurKer[3][3] = {{1.0 / 9, 1.0 / 9, 1.0 / 9},
                            {1.0 / 9, 1.0 / 9, 1.0 / 9},
                            {1.0 / 9, 1.0 / 9, 1.0 / 9}};
    double gxKer[3][3] = {{-1.0, 0.0, 1.0},
                          {-2.0, 0.0, 2.0},
                          {-1.0, 0.0, 1.0}};
    double gyKer[3][3] = {{-1.0, 2.0, -1.0},
                          {0.0,  0.0, 0.0},
                          {1.0,  2.0, 1.0}};
    ImagePPM image;
    ImagePPM blurred;
    ImagePPM gx;
    ImagePPM gy;
    ImagePPM gFinal;

    OpenPPMFile("line.ppm", image);
    std::cout << signaltonoise(image) << std::endl;
    blurred = doKernal(blurKer, image);
    gx = doKernal(gxKer, image);
    gy = doKernal(gyKer, image);

    for (int row = 1; row < image.height - 1; row++) {
        for (int col = 1; col < image.width - 1; col++) {
            float gxNum = get_pixel(gx, col, row, 3);
            float gyNum = get_pixel(gy, col, row, 3);
            float gNum = sqrt((gxNum * gxNum) + (gyNum * gyNum));
            set_pixel(gFinal, col, row, gNum, gNum, gNum);

        }
    }


    SavePPMFile("blurout.ppm", blurred);
    SavePPMFile("gxout.ppm", gx);
    SavePPMFile("gyout.ppm", gy);
    SavePPMFile("gFinalout", gFinal);
}