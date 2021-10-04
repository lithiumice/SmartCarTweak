#ifndef __IMAGE_PROCESS
#define __IMAGE_PROCESS
#include "math.h"
#include <vector>
#include "common.h"

#define IMG_W 320
#define IMG_H 240
#define IMG_120_160 img_proc_c.bin_120_160_image
#define IMG_240_320 img_proc_c.bin_240_320_image

class imgproc_ctrl
{
public:
    uint8_t HistoGram[256];

    uint8_t img_threshold = 70;
    uint8_t fix_threshold = 70;

    uint8_t bin_240_320_image[240][320];
    uint8_t gray_120_160_image[120][160];
    uint8_t bin_120_160_image[120][160];
    uint8_t f_120_160_Image[120][160];

    void gray_to_binary(uint8_t src[240][320],
                        uint8_t dst[240][320],
                        uint8_t mode);
    void gray_to_binary(uint8_t src[120][160],
                        uint8_t dst[120][160],
                        uint8_t mode);
    void img_resize(uint8_t src[240][320],
                    uint8_t dst[120][160]);
    void copy_to_final(uint8_t src[120][160]);

    int mean_threshold(const uint8_t img[240][320]);
    int mean_threshold(const uint8_t img[120][160]);
    int ostu_threshold(const uint8_t img[240][320]);
    int ostu_threshold(const uint8_t img[120][160]);
};
extern imgproc_ctrl img_proc_c;

#endif