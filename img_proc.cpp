#include "img_proc.h"

// void imgproc_ctrl::bin_image_filter(uint8_t imageIn[120][160], uint8_t imageOut[120][160])
// {
//     int16 i = 0, j = 0;

//     for (i = 0; i < IMG_H; i++)
//     {
//         for (j = 0; j < IMG_W; j++)
//         {
//             imageOut[i][j] = imageIn[i][j];
//         }
//     }

//     for (i = 118; i > 0; i--)
//     {
//         for (j = 5; j <= 154; j++)
//         {
//             if (imageOut[i][j] != 0 &&
//                 ((imageOut[i][j - 1] == 0 &&
//                   imageOut[i][j - 2] == 0) ||
//                  (imageOut[i][j - 1] == 0 &&
//                   imageOut[i][j - 3] == 0) ||
//                  (imageOut[i][j - 2] == 0 &&
//                   imageOut[i][j - 3] == 0)) &&
//                 ((imageOut[i][j + 1] == 0 &&
//                   imageOut[i][j + 2] == 0) ||
//                  (imageOut[i][j + 1] == 0 &&
//                   imageOut[i][j + 3] == 0) ||
//                  (imageOut[i][j + 2] == 0 &&
//                   imageOut[i][j + 3] == 0)))
//                 imageOut[i][j] = 0;
//             if (imageOut[i][j] == 0 &&
//                 ((imageOut[i][j - 1] != 0 &&
//                   imageOut[i][j - 2] != 0) ||
//                  (imageOut[i][j - 1] != 0 &&
//                   imageOut[i][j - 3] != 0) ||
//                  (imageOut[i][j - 2] != 0 &&
//                   imageOut[i][j - 3] != 0)) &&
//                 ((imageOut[i][j + 1] != 0 &&
//                   imageOut[i][j + 2] != 0) ||
//                  (imageOut[i][j + 1] != 0 &&
//                   imageOut[i][j + 3] != 0) ||
//                  (imageOut[i][j + 2] != 0 &&
//                   imageOut[i][j + 3] != 0)))
//                 imageOut[i][j] = 255;
//         }
//     }

//     for (i = 118; i > 0; i--)
//     {
//         for (j = 5; j <= 154; j++)
//         {
//             if (imageOut[i][j] != 0 &&
//                 ((imageOut[i][j - 1] == 0 &&
//                   imageOut[i][j - 2] != 0 &&
//                   imageOut[i][j - 3] != 0 &&
//                   imageOut[i][j - 4] != 0 &&
//                   imageOut[i][j + 1] == 0 &&
//                   imageOut[i][j + 2] == 0 &&
//                   imageOut[i][j + 3] == 0) ||
//                  (imageOut[i][j - 1] == 0 &&
//                   imageOut[i][j - 2] == 0 &&
//                   imageOut[i][j - 3] == 0 &&
//                   imageOut[i][j + 1] == 0 &&
//                   imageOut[i][j + 2] != 0 &&
//                   imageOut[i][j + 3] != 0 &&
//                   imageOut[i][j + 4] != 0)))
//                 imageOut[i][j] = 0;
//         }
//     }
// }

//mode: 1:ostu; 0:mean; 2:fix
void imgproc_ctrl::gray_to_binary(uint8_t src[240][320], uint8_t dst[240][320], uint8_t mode)
{
    // uint8(*src)[320];
    // mt9v03x_csi_image
    if (0 == mode)
    {
        img_threshold = mean_threshold(src);
    }
    else if (1 == mode)
    {
        img_threshold = ostu_threshold(src);
        //大津法阈值Threshold = (uint8_t)(Threshold * 0.5) + 70;
    }
    else if (2 == mode)
    {
        img_threshold = fix_threshold;
    }

    for (int i = 0; i < 240; i++)
    {
        for (int j = 0; j < 320; j++)
        {
            if (src[i][j] > img_threshold) //数值越大，显示的内容越多，较浅的图像也能显示出来
                dst[i][j] = 255;
            else
                dst[i][j] = 0;
        }
    }
}

void imgproc_ctrl::gray_to_binary(uint8_t src[120][160], uint8_t dst[120][160], uint8_t mode)
{
    if (0 == mode)
    {
        img_threshold = mean_threshold(src);
    }
    else if (1 == mode)
    {
        img_threshold = ostu_threshold(src);
    }
    else if (2 == mode)
    {
        img_threshold = fix_threshold;
    }

    for (int i = 0; i < 240; i++)
    {
        for (int j = 0; j < 320; j++)
        {
            if (src[i][j] > img_threshold)
                dst[i][j] = 255;
            else
                dst[i][j] = 0;
        }
    }
}

int imgproc_ctrl::mean_threshold(const uint8_t img[240][320])
{
    uint32 tv = 0;
    int threshold = 0;

    for (int i = 0; i < 320; i++)
    {
        for (int j = 0; j < 240; j++)
        {
            tv += img[j][i];
        }
    }
    threshold = tv / 320 / 240;          //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100
    threshold = threshold * 7 / 10 + 10; //此处阈值设置，根据环境的光线来设定
    return threshold;
}

int imgproc_ctrl::mean_threshold(const uint8_t img[120][160])
{
    uint32 tv = 0;
    int threshold = 0;

    for (int i = 0; i < 160; i++)
    {
        for (int j = 0; j < 120; j++)
        {
            tv += img[j][i];
        }
    }
    threshold = tv / 160 / 120;          //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100
    threshold = threshold * 7 / 10 + 10; //此处阈值设置，根据环境的光线来设定
    return threshold;
}

int imgproc_ctrl::ostu_threshold(const uint8_t img[240][320])
{
    int16_t i, j;
    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32_t PixelIntegralFore = 0;
    int32_t PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    int16_t MinValue, MaxValue;
    uint8_t Threshold = 0;

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < 320; j++)
    {
        for (i = 0; i < 240; i++)
        {
            HistoGram[img[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ; //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--)
        ; //获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue; // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue; // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j]; //  像素总数

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j; //灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];                                              //前景像素点数
        PixelFore = Amount - PixelBack;                                                    //背景像素点数
        OmegaBack = (float)PixelBack / Amount;                                             //前景像素百分比
        OmegaFore = (float)PixelFore / Amount;                                             //背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;                                             //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;                             //背景灰度值
        MicroBack = (float)PixelIntegralBack / PixelBack;                                  //前景灰度百分比
        MicroFore = (float)PixelIntegralFore / PixelFore;                                  //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore); //计算类间方差
        if (Sigma > SigmaB)                                                                //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold; //返回最佳阈值;
}

int imgproc_ctrl::ostu_threshold(const uint8_t img[120][160])
{
    int16_t i, j;
    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32_t PixelIntegralFore = 0;
    int32_t PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    int16_t MinValue, MaxValue;
    uint8_t Threshold = 0;

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < 160; j++)
    {
        for (i = 0; i < 120; i++)
        {
            HistoGram[img[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ; //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--)
        ; //获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue; // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue; // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j]; //  像素总数

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j; //灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];                                              //前景像素点数
        PixelFore = Amount - PixelBack;                                                    //背景像素点数
        OmegaBack = (float)PixelBack / Amount;                                             //前景像素百分比
        OmegaFore = (float)PixelFore / Amount;                                             //背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;                                             //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;                             //背景灰度值
        MicroBack = (float)PixelIntegralBack / PixelBack;                                  //前景灰度百分比
        MicroFore = (float)PixelIntegralFore / PixelFore;                                  //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore); //计算类间方差
        if (Sigma > SigmaB)                                                                //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold; //返回最佳阈值;
}

void imgproc_ctrl::copy_to_final(uint8_t src[120][160])
{
    for (int i = 0; i < 120; i++)
    {
        for (int j = 0; j < 160; j++)
        {
            f_120_160_Image[i][j] = src[i][j];
        }
    }
}

void imgproc_ctrl::img_resize(uint8_t src[240][320],
                              uint8_t dst[120][160])
{
    for (int i = 0; i < 120; i++)
    {
        for (int j = 0; j < 160; j++)
        {
            dst[i][j] = src[2 * i][2 * j];
        }
    }
}