#ifndef _Findline_H_
#define _Findline_H_

#include "main.h"
#ifndef CLION_DEBUG
#include "common.h"
#include <rtthread.h>
using namespace rtthread;
#include "main.h"
#include "img_proc.h"
#include "ctrllor_class.h"
#include "common_macro.h"
#else
#include "main.h"
#include "common.h"
#include "img_proc.h"
#include "common_macro.h"
#endif

#define ENABLE_CROSS
#define ENABLE_CURVE
#define ENABLE_LOOP
#define ENABLE_FORK
#define ENABLE_QRCODE

#define ENABLE_START
#define ENABLE_GARAGE
#define ENABLE_RAMP

#define USE_GYRO_LOOP 0
#define USE_GYRO_RAMP 0
#define LONG_STRAIGHT_ENDROW 30

typedef struct
{
  uint8 en_element_list = 0;
  uint8 en_chassis_debug = 1;
  uint8 en_chassis_update = 1;
  uint8 en_stop_car = 1;
  uint8 en_ano_upload = 0;

  uint8 en_ramp_det = 0;
  uint8 en_fork_regco = 1;
  uint8 en_anifriut_regco = 1;

  uint8 ano_type = 0;
  uint8 disp_pic_type = 0;
  uint8 en_test_pos_pid = 0;
} ctrl_struct;
extern ctrl_struct f0;

typedef struct
{
  //1: loop
  //2: fork
  //3: qrcode
  //4: garage
  //5: start
  int element_list[5] = {2, 2, 1, 1, 4};
  int current_element_pos = 0;
  uint8 reset_ele_signal = 0;

  uint8 garage_enable = 1;
  uint8 loop_enable = 1;
  uint8 cross_enable = 1;
  uint8 fork_enable = 1;
  uint8 curve_enable = 1;
  uint8 tag_enable = 1;
  uint8 ramp_enable = 1;
  uint8 start_enable = 1;

  int16 midline[120];
  int16 leftline[120];
  int16 rightline[120];
  int16 leftlineflag[120];
  int16 rightlineflag[120];

  int16 tag_L_start_row;
  int16 tag_R_start_row;
  int16 road_real_width[120];
  int16 tag_black_duty[120];
  int16 tag_leftline[120];
  int16 tag_leftlineflag[120];
  int16 tag_rightline[120];
  int16 tag_rightlineflag[120];
  int16 tag_down_line[160];
  int16 tag_down_line_flag[160];

  // int16 road_real_width[240];
  // int16 tag_black_duty[240];
  // int16 tag_leftline[240];
  // int16 tag_leftlineflag[240];
  // int16 tag_rightline[240];
  // int16 tag_rightlineflag[240];
  // int16 tag_down_line[320];
  // int16 tag_down_line_flag[320];

  int16 leftstartrow;
  int16 rightstartrow;
} Findline1_TypeDef;
extern Findline1_TypeDef f1;

typedef struct
{
  //状态机
  int16 tag_num = 0;
  int16 tag_sm_state = 0;
  int16 tag_proc_sm = 0;

  int16 L_loop_judge_sm = 0;
  int16 R_loop_judge_sm = 0;
  int L_loop_sm_state;
  int R_loop_sm_state;

  int fork_proc_sm = 0;
  int sch_sm_state = 0;
  int16 fork_sm_state = 0;

  int16 fork_dir = 0;
  //0: left, 1: right
  int16 fork_frames = 0;
  int fork_num = 0;

  //坡道
  uint8 ramp_sm_state = 0;
  uint8 ramp_flag = 0;
  uint8 ramp_frames = 0;
  int16 ramp_top_row = 0;

  //出库
  uint8 en_start = 0;
  uint8 start_sm_state = 0;
  uint8 first_start_dir = 1;

  //停车
  int16 stop_car_reg[20];
  int16 stop_car_flag;

  //弯道
  int16 leftturnflag;
  int16 rightturnflag;
  int16 leftturn_row;
  int16 rightturn_row;

  //二维码
  int16 tag_frames;
  int16 tag_flag;

  int16 tag_top_row;
  int16 tag_row_cnt;
  int16 tag_row_up;
  int16 tag_row_down;
  int qrcode_row_reg;

  int16 tag_left_col;
  int16 tag_right_col;

  int16 tag_left[2];
  int16 tag_right[2];

  int16 L_col1 = 0;
  int16 L_col2 = 0;
  int16 R_col1 = 0;
  int16 R_col2 = 0;

  //入库
  uint8 garage_tmp[2] = {0};
  uint8 garage_cnt = 0;
  uint8 garage_row = 0;
  uint8 garage_frames = 0;

  uint8 garage_dir = 0;
  uint8 garage_flag = 0;
  int16 garage_sm_state = 0;

  //拐点
  int16 L_infec_col;
  int16 L_infec_row;
  int16 L_infec_flag;

  int16 R_infec_flag;
  int16 R_infec_col;
  int16 R_infec_row;

  int16 L_up_infec_col;
  int16 L_up_infec_row;
  int16 L_up_infec_flag;

  int16 R_up_infec_flag;
  int16 R_up_infec_col;
  int16 R_up_infec_row;

  //基础巡线
  int16 toppoint;
  int16 toppoint_reg;
  int16 toppoint_flag;
  int16 column_start = 80;

  //十字
  int16 L_mid_endrow = 0;
  int16 R_mid_endrow = 0;
  int16 cross_sm_state = 0;
  int16 cross_flag;
  int16 cross_frames;
  int16 cross_type;
  float cross_K_L;
  float cross_K_R;

  //左环岛
  int L_loop_flag;

  int L_loop_circle_row;
  int L_loop_circle_col;
  int L_loop_circle_flag;

  int L_loop_top_row;
  int L_loop_up_row;
  int L_loop_down_row;

  int16 L_loop_exit_row;
  int16 L_loop_exit_col;
  int16 L_loop_exit_flag;

  //右环岛
  int16 R_loop_flag;

  int16 R_loop_circle_row;
  int16 R_loop_circle_col;
  int16 R_loop_circle_flag;

  int16 R_loop_top_row;
  int16 R_loop_up_row;
  int16 R_loop_down_row;

  int16 R_loop_exit_row;
  int16 R_loop_exit_col;
  int16 R_loop_exit_flag;

  //三岔路
  int16 fork_flag;
  int16 fork_L_flag;
  int16 fork_L_col;
  int16 fork_L_row;
  int16 fork_R_flag;
  int16 fork_R_col;
  int16 fork_R_row;
  int16 fork_up_flag;
  int16 fork_up_col;
  int16 fork_up_row;

  int16 fork_up_L_col;
  int16 fork_up_R_col;

  int16 fork_up_line[160];
  int16 fork_up_line_flag[160];

  int16 R_fork_mid_endrow = 0;
  int16 L_fork_mid_endrow = 0;

  //长直道
  int16 shortstraight_flag;
  int16 crossstraight_flag;
  int16 long_straight_flag;

} Findline2_TypeDef;
extern Findline2_TypeDef f2;

void find_toppoint(void);
void find_little_curve(void);
void find_large_curve(void);
void find_cross_infec(void);
void find_L_loop_exit();
void qrcode_judge();
void cross_judge(void);
void find_track(void);
void find_elements(void);
void find_L_loop_circle();
void long_judge();
void short_judge();
void stop_judge();
void find_up_infec();
void find_qrcode_pos();
void find_qrcode_row();
void enable_all_elements();
void find_edge();
void find_L_edge();
void find_R_edge();

float solve_variance(int begin_row, int end_row);
float solve_slope(int begin, int end, int16 *p);
int solve_region_duty(int x1, int y1, int x2, int y2, int color);

int is_L_R_lose(int start, int end, int mode, int threshold);
int is_L_R_nonlose(int start, int end, int mode, int threshold);
int is_region_one_color(int x1, int y1, int x2, int y2, int color, int threshold);
int is_straight_line(int start_index, int end_index, int16 line_[IMG_H], int scale);
int is_M_center(int start, int end, int half_err);

#endif