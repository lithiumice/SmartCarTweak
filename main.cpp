#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include <iostream>
#include "find_lines.h"
#include "img_proc.h"
#include "main.h"
#define NUM 3

chassis_ctrl chassis;
inoutdev_ctrl inoutdev;
ctrl_struct f0;
imgproc_ctrl img_proc_c;

void inoutdev_ctrl::beep(int num) { printf("beep.\n");}
void inoutdev_ctrl::long_beep(int num) {printf("long beep.\n");}

char *readPath[NUM] = {
        ".\\src_txt_240\\1.txt",
        ".\\src_txt_240\\2.txt",
        ".\\src_txt_240\\3.txt",
};

char *writePath_120[NUM] = {

        ".\\dst_txt_120\\1.txt",
        ".\\dst_txt_120\\2.txt",
        ".\\dst_txt_120\\3.txt",
};
char *writePath_240[NUM] = {

        ".\\dst_txt_240\\1.txt",
        ".\\dst_txt_240\\2.txt",
        ".\\dst_txt_240\\3.txt",
};

//用 -1 标识结束
int left_array[] = {0, 1, 2, 6, 7, 8, 11, 12, 13, 17, 18, 22, 28, 29, 30, 33, 54};
int right_array[] = {0, 1, 2, 11, 12, 13, 17, 18, 20, 21, 22, 26, 29, 31, 32, 33, 34, 35, 48, 49};
int cross_file[] = {0, 1, 2, 10, 11, 12, 13, 17, 18, 22, 26, 28, 29, 30, 31, 33, 33, 34, 54, -1};
int loop_file[] = {20, 21, 48, 49, -1};

int read_txt(char *file)
{
    FILE *fp = fopen(file, "rb");
    if (NULL == fp)
    {
        printf("open file failed!\n");
        return 0;
    }
    int temp = 0;
    int count = 0;
    while (fscanf(fp, "%3d", &temp))
    {
        int row = (int)(floor(count / 320));
        IMG_240_320[row][count % 320] = temp;
        if (temp == -1)
            break;
        temp = -1;
        count++;
    }
    fclose(fp);
    return 1;
}

void save_txt_120(char *file)
{
    FILE *fp = fopen(file, "wb");
    if (NULL == fp)
    {
        printf("open file failed!\n");
    }
    for (int i = 0; i < 120; i++)
    {
        for (int j = 0; j < 160; j++)
        {
            fprintf(fp, "%3d", IMG_120_160[i][j]);
        }
        fprintf(fp, "%s", "\r\n");
    }
    printf("save txt img %s done\n",file);
    fclose(fp);
}

void save_txt_240(char *file)
{
    FILE *fp = fopen(file, "wb");
    if (NULL == fp)
    {
        printf("open file failed!\n");
    }
    for (int i = 0; i < 240; i++)
    {
        for (int j = 0; j < 320; j++)
        {
            fprintf(fp, "%3d", IMG_240_320[i][j]);
        }
        fprintf(fp, "%s", "\r\n");
    }
    printf("save txt img %s done\n",file);
    fclose(fp);
}

void draw_midline()
{
    for(int i=0;i<120;i++)
    {
        IMG_120_160[i][f1.midline[i]]=0;
    }
}
int main(int argc, char *argv[])
{
    for (int i = 0; i < NUM; i++)
    {
        if (read_txt(readPath[i]) != 1)
            continue;

        img_proc_c.img_resize(IMG_240_320, img_proc_c.gray_120_160_image);
        img_proc_c.fix_threshold = img_proc_c.ostu_threshold(img_proc_c.gray_120_160_image);
        img_proc_c.gray_to_binary(IMG_240_320, img_proc_c.bin_240_320_image, 2);
        img_proc_c.img_resize(img_proc_c.bin_240_320_image, img_proc_c.bin_120_160_image);
        
        find_track();
        find_elements();
        draw_midline();
        save_txt_120(writePath_120[i]);
        save_txt_240(writePath_240[i]);
    }
    return 0;
}
