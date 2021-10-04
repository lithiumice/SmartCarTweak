//
// Created by lithiumice on 2021/5/30.
//

#ifndef CAR_MAIN_H
#define CAR_MAIN_H
#define CLION_DEBUG


class inoutdev_ctrl
{
public:
    float gyro_z_angle = 0;
    float gyro_y_angle = 0;
    float start_angle = 0;
    void beep(int num);
    void long_beep(int num);
};

class chassis_ctrl
{
public:
    float all_distance = 0;
    float loop_distance = 0;
    float fork_distance = 0;
    float tag_distance = 0;
    float start_distance = 0;
    float ramp_distance = 0;
};

extern chassis_ctrl chassis;
extern inoutdev_ctrl inoutdev;

#endif //CAR_MAIN_H
