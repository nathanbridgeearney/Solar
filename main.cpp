#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "image_l4.hpp"


ImagePPM doKernal(double kernel[3][3], ImagePPM inputImage) {
    ImagePPM outImage;
    init_image(outImage, inputImage.width - 2, inputImage.height - 2);
    for (int row = 1; row < inputImage.height - 1; row++) {
        for (int col = 1; col < inputImage.width - 1; col++) {
            double pixelValue = 0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    pixelValue += get_pixel(inputImage, row + i, col + j, 3) * kernel[i + 1][j + 1];
                }
            }
            if (pixelValue > 255) {
                pixelValue = 255;
            } else if (pixelValue < 0) {
                pixelValue = 0;
            }
            set_pixel(outImage, row - 1, col - 1, pixelValue, pixelValue, pixelValue);
        }
    }
    return outImage;
}

double signaltonoise(ImagePPM inputImage) {
    float signal = 0;
    int count = 0;
    for (int row = 10; row < 20; ++row) {
        for (int col = 10; col < 20; ++col) {
            signal += get_pixel(inputImage, row, col, 3);
            count++;
        }
    }
    signal /= count;
    float signalPow = signal * signal;
    float noise = 0;
    for (int col = 10; col < 20; ++col) {
        for (int row = 10; row < 20; ++row) {
            noise += (get_pixel(inputImage, row, col, 3) - signal) * (get_pixel(inputImage, row, col, 3) - signal);
        }
    }
    noise /= count;
    return signalPow / noise;
}

double linePara(ImagePPM image) {
    int count;
    double yCord;
    double xCord;
    double x2 = 0;
    double xy = 0;

    for (int row = 0; row < image.height; row++) {
        for (int col = 0; col < image.width; col++) {
            if (get_pixel(image, row, col, 3) > 100) {

                yCord = yCord + row;
                xCord = xCord + col;
                x2 = x2 + col * col;
                xy = xy + row * col;
                count++;

            }
        }
    }
    double yDash = yCord / count;
     double xDash = xCord / count;




    double b = ((xy-(yDash * xCord))/ (x2 - (xDash * xCord)));
    double a = yDash - (b * xDash);



    std::cout << "Slope: " << b << std::endl;
    std::cout << "Line Intersection: " << a << std::endl;
}


ImagePPM grad(ImagePPM image, ImagePPM gx, ImagePPM gy) {
    ImagePPM gFinal;
    double threshold = 89.819;
    init_image(gFinal, image.width, image.height);

    for (int row = 0; row < image.height; row++) {
        for (int col = 0; col < image.width; col++) {
            set_pixel(gFinal, row, col, 0, 0, 0);
        }
    }
    for (int row = 1; row < image.height - 2; row++) {
        for (int col = 1; col < image.width - 2; col++) {
            float gxNum = get_pixel(gx, row, col, 3);
            float gyNum = get_pixel(gy, row, col, 3);
            float gNum = sqrt((gxNum * gxNum) + (gyNum * gyNum));

            if (gNum < 0) {
                gNum = 0;
            } else if (gNum > threshold) {
                gNum = 255;
            }
            if (gNum > threshold) {
                set_pixel(gFinal, row, col, 255, 255, 255);
            }
        }
    }
    return gFinal;
}


int main() {
    double blurKer[3][3] = {{1.0 / 9, 1.0 / 9, 1.0 / 9},
                            {1.0 / 9, 1.0 / 9, 1.0 / 9},
                            {1.0 / 9, 1.0 / 9, 1.0 / 9}};
    double gxKer[3][3] = {{-1.0, 0.0, 1.0},
                          {-2.0, 0.0, 2.0},
                          {-1.0, 0.0, 1.0}};
    double gyKer[3][3] = {{-1.0, -2.0, -1.0},
                          {0.0,  0.0,  0.0},
                          {1.0,  2.0,  1.0}};

    ImagePPM image;
    ImagePPM blurred;
    ImagePPM gx;
    ImagePPM gy;
    ImagePPM gFinal;

    OpenPPMFile("line.ppm", image);


    blurred = doKernal(blurKer, image);
    gx = doKernal(gxKer, blurred);
    gy = doKernal(gyKer, blurred);
    gFinal = grad(blurred, gx, gy);

    linePara(gFinal);

    SavePPMFile("blurout.ppm", blurred);
    SavePPMFile("gxout.ppm", gx);
    SavePPMFile("gyout.ppm", gy);
    SavePPMFile("gFinalout.ppm", gFinal);


    std::cout << "Orignal S/N Ratio: " << signaltonoise(image) << std::endl;
    std::cout << "Blurred S/N Ratio: " << signaltonoise(blurred) << std::endl;

}