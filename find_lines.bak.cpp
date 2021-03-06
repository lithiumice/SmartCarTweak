#include "find_lines.h"

Findline1_TypeDef f1;
Findline2_TypeDef f2;

int16 half_width[120] =
    {0, 0, 1, 1, 2, 2, 3, 3, 3, 4,
     4, 5, 5, 5, 6, 6, 7, 7, 8, 8,
     9, 9, 9, 10, 10, 11, 11, 12, 12, 12,
     12, 13, 13, 13, 13, 14, 14, 14, 14, 14,
     15, 15, 16, 16, 17, 18, 19, 20, 21, 21,
     21, 22, 23, 23, 24, 25, 25, 26, 27, 27,
     28, 29, 30, 31, 32, 33, 34, 35, 36, 36,
     37, 38, 39, 39, 40, 41, 41, 42, 43, 44,
     44, 45, 46, 47, 47, 48, 48, 49, 50, 50,
     51, 52, 53, 54, 54, 55, 56, 57, 58, 58,
     59, 60, 60, 61, 62, 63, 63, 64, 65, 65,
     66, 67, 67, 68, 69, 70, 70, 71, 72, 73};

int16 loop_half_width[120] =
    {22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
     22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
     22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
     22, 22, 22, 22, 22, 22, 22, 22, 22, 25,
     28, 31, 33, 35, 36, 37, 39, 41, 42, 44,
     45, 44, 44, 43, 43, 43, 43, 43, 43, 43,
     43, 43, 43, 43, 46, 46, 47, 48, 48, 48,
     49, 50, 50, 50, 50, 51, 51, 52, 52, 53,
     53, 53, 54, 54, 55, 55, 55, 56, 57, 57,
     58, 58, 59, 60, 60, 61, 61, 62, 63, 64,
     64, 64, 65, 67, 67, 67, 68, 68, 68, 68,
     68, 68, 68, 68, 69, 69, 69, 70, 70, 71};

void reset_all_elements()
{
  f2 = {0};
  f2.column_start = 80;
}

void enable_all_elements()
{
  f1.garage_enable = 1;
  f1.fork_enable = 1;
  f1.curve_enable = 1;
  f1.loop_enable = 1;
  f1.cross_enable = 1;
  f1.tag_enable = 1;
  f1.ramp_enable = 1;
  f1.start_enable = 1;
}

void disable_all_elements()
{
  f1.garage_enable = 0;
  f1.fork_enable = 0;
  f1.curve_enable = 0;
  f1.loop_enable = 0;
  f1.cross_enable = 0;
  f1.tag_enable = 0;
  f1.ramp_enable = 0;
  f1.start_enable = 0;
}

void en_one_element(int type)
{
  disable_all_elements();
  switch (type)
  {
  case 1:
    f1.loop_enable = 1;
    break;
  case 2:
    f1.fork_enable = 1;
    break;
  case 3:
    f1.tag_enable = 1;
    break;
  case 4:
    f1.garage_enable = 1;
    break;
    // default:
    //   enable_all_elements();
  }
}

void goto_next_element()
{
  f1.current_element_pos++;
  int list_max_size = sizeof(f1.element_list) / sizeof(int);
  if (f1.current_element_pos > list_max_size - 1)
  {
    f1.current_element_pos = 0;
  }
}

void find_L_loop_exit()
{
  f2.L_loop_exit_flag = 0;
  f2.L_loop_exit_row = 0;
  f2.L_loop_exit_col = 0;

  for (int i = 115; i > 60; i--)
  {
    if (
        i > 90 &&
        f1.rightline[i] > 80 &&
        f1.rightline[i] < f1.rightline[i + 3] &&
        f1.rightline[i + 3] < f1.rightline[i + 6] &&

        f1.rightlineflag[i + 3] == 1 &&
        f1.rightlineflag[i + 6] == 1 &&
        f1.rightlineflag[i] == 1 &&

        (f1.rightlineflag[i - 25] == 0 ||
         f1.rightlineflag[i - 20] == 0 ||
         f1.rightlineflag[i - 15] == 0 ||
         f1.rightlineflag[i - 10] == 0 ||
         f1.rightlineflag[i - 5] == 0)

        // abs(f1.rightline[i - 3] - f1.rightline[i + 3]) < 12 &&
        // f1.rightline[i + 2] + f1.rightline[i - 2] - 2 * f1.rightline[i] > 4
    )
    {
      f2.L_loop_exit_flag = 1;
      f2.L_loop_exit_row = i;
      f2.L_loop_exit_col = f1.rightline[i];
      break;
    }
  }
}

void find_L_loop_circle()
{
  f2.L_loop_circle_flag = 0;
  f2.L_loop_circle_row = 0;
  f2.L_loop_circle_col = 0;

  for (int i = f2.L_loop_down_row; i > f2.toppoint; i--)
  {
    if (
        f1.leftline[i] > f1.leftline[i - 4] &&
        f1.leftline[i] > f1.leftline[i + 4] &&
        f1.leftline[i] > f1.leftline[i + 6] &&

        f1.leftlineflag[i - 4] &&
        f1.leftlineflag[i] &&
        f1.leftlineflag[i + 4] &&
        f1.leftlineflag[i + 6]

        // (f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i - 3] - f1.leftline[i - 3]) &&
        //(f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i + 3] - f1.leftline[i + 3]) &&
        //(f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i + 5] - f1.leftline[i + 5])
        && is_region_one_color(0, i - 1, f1.leftline[i], i + 1, 0, 85))
    {
      f2.L_loop_circle_flag = 1;
      f2.L_loop_circle_row = i;
      f2.L_loop_circle_col = f1.leftline[i];
      break;
    }
  }
}

void find_R_loop_exit()
{
  f2.R_loop_exit_flag = 0;
  f2.R_loop_exit_row = 0;
  f2.R_loop_exit_col = 0;

  for (int i = 115; i > 60; i--)
  {
    if (
        i > 90 &&
        f1.leftline[i] < 80 &&
        f1.leftline[i] > f1.leftline[i + 3] &&
        f1.leftline[i + 3] > f1.leftline[i + 6] &&

        f1.leftlineflag[i + 3] == 1 &&
        f1.leftlineflag[i + 6] == 1 &&
        f1.leftlineflag[i] == 1 &&

        (f1.leftlineflag[i - 25] == 0 ||
         f1.leftlineflag[i - 20] == 0 ||
         f1.leftlineflag[i - 15] == 0 ||
         f1.leftlineflag[i - 10] == 0 ||
         f1.leftlineflag[i - 5] == 0))
    {
      f2.R_loop_exit_flag = 1;
      f2.R_loop_exit_row = i;
      f2.R_loop_exit_col = f1.leftline[i];
      break;
    }
  }
}

void find_R_loop_circle()
{
  f2.R_loop_circle_flag = 0;
  f2.R_loop_circle_row = 0;
  f2.R_loop_circle_col = 0;

  for (int i = f2.R_loop_down_row; i > f2.toppoint; i--)
  {
    if (
        f1.rightline[i] < f1.rightline[i - 4] &&
        f1.rightline[i] < f1.rightline[i + 4] &&
        f1.rightline[i] < f1.rightline[i + 6] &&

        f1.rightlineflag[i - 4] &&
        f1.rightlineflag[i] &&
        f1.rightlineflag[i + 4] &&
        f1.rightlineflag[i + 6]

        && is_region_one_color(f1.rightline[i], i - 1, 159, i + 1, 0, 70))
    {
      f2.R_loop_circle_flag = 1;
      f2.R_loop_circle_row = i;
      f2.R_loop_circle_col = f1.rightline[i];
      break;
    }
  }
}

void find_post_proc()
{
  find_toppoint();
  long_judge();
  short_judge();
  stop_judge();

  f2.column_start = f1.midline[118];
  __Limit_AB(f2.column_start, 10, 150);
}

void find_qrcode_pos()
{
  f2.qrcode_row_reg = f2.tag_row_down;
  f2.tag_row_down = 0;
  for (int j = 118; j > 60; j--)
  {
    if (
        IMG_120_160[j - 1][80] == 0 &&
        IMG_120_160[j][80] == 0 &&
        IMG_120_160[j + 1][80] != 0 &&
        IMG_120_160[j + 2][80] != 0)
    {
      f2.tag_row_down = j;
      break;
    }
  }

  if (abs(f2.tag_row_down - f2.qrcode_row_reg) > 15)
  {
    f2.tag_row_down = f2.qrcode_row_reg;
  }
}

void find_qrcode_row()
{
  f2.qrcode_row_reg = f2.tag_row_down;
  qrcode_judge();
  if (!f2.tag_flag)
  {
    f2.tag_row_down = f2.qrcode_row_reg;
  }
  if (is_region_one_color(50, 100, 110, 118, 0, 40))
  {
    f2.tag_row_down = 118;
  }
}

void find_L_R_infec()
{
  f2.fork_L_flag = 0;
  f2.fork_R_flag = 0;

  f2.fork_L_col = 0;
  f2.fork_R_col = 159;

  f2.fork_R_row = 119;
  f2.fork_L_row = 119;

  for (int i = 115; i >= 50; i--)
  {
    //??????????????????????
    if (
        f1.leftline[i] < 80 &&
        f1.leftline[i] - f1.leftline[i - 4] >= 1 &&
        f1.leftline[i] - f1.leftline[i + 4] >= 1 &&
        is_L_R_nonlose(i - 3, i + 3, 0, 90))
    {
      f2.fork_L_flag = 1;
      f2.fork_L_row = i;
      f2.fork_L_col = f1.leftline[i];
    }

    //?????????????????????
    if (
        f1.rightline[i] > 80 &&
        f1.rightline[i - 4] - f1.rightline[i] >= 1 &&
        f1.rightline[i + 4] - f1.rightline[i] >= 1 &&
        is_L_R_nonlose(i - 3, i + 3, 1, 90))
    {
      f2.fork_R_flag = 1;
      f2.fork_R_row = i;
      f2.fork_R_col = f1.rightline[i];
    }
  }
}

void find_up_infec()
{
  find_L_R_infec();
  f2.fork_up_flag = 0;
  f2.fork_up_col = 80;
  f2.fork_up_row = 119;

  memset(f2.fork_up_line, 0, sizeof(f2.fork_up_line));
  memset(f2.fork_up_line_flag, 0, sizeof(f2.fork_up_line_flag));

  for (int i = f2.fork_L_col; i < f2.fork_R_col; i++)
  {
    for (int j = 110; j > 5; j--)
    {
      if (
          IMG_120_160[j + 1][i] == 0 &&
          IMG_120_160[j][i] != 0)
      {
        break;
      }

      if (
          IMG_120_160[j + 1][i] != 0 &&
          IMG_120_160[j][i] == 0)
      {
        f2.fork_up_line[i] = j - 1;
        f2.fork_up_line_flag[i] = 1;
        break;
      }
    }
  }

  f2.fork_up_flag = 1;
  f2.fork_up_col = 80;
  f2.fork_up_row = f2.fork_up_line[80];
  for (int i = 80; i < f2.fork_R_col; i++)
  {
    if (abs(f2.fork_up_line[i] - f2.fork_up_line[i - 1]) > 10)
    {
      break;
    }
    if (f2.fork_up_row < f2.fork_up_line[i])
    {
      f2.fork_up_row = f2.fork_up_line[i];
      f2.fork_up_col = i;
    }
  }
  for (int i = 80; i > f2.fork_L_col; i--)
  {
    if (abs(f2.fork_up_line[i] - f2.fork_up_line[i + 1]) > 10)
    {
      break;
    }
    if (f2.fork_up_row < f2.fork_up_line[i])
    {
      f2.fork_up_row = f2.fork_up_line[i];
      f2.fork_up_col = i;
    }
  }

  if (!f2.fork_up_flag)
  {
    f2.fork_up_row = f2.toppoint_reg;
    f2.fork_up_col = 80;
  }

  if (is_region_one_color(50, 100, 110, 118, 0, 50))
  {
    f2.fork_up_col = 80;
    f2.fork_up_row = 119;
  }
}

void find_toppoint()
{
  for (int i = 115; i > 5; i--)
  {
    if (
        ((IMG_120_160[i][f1.midline[i]] != 0) &&
         (IMG_120_160[i - 1][f1.midline[i]] == 0 ||
          IMG_120_160[i - 2][f1.midline[i]] == 0)) ||
        (f1.midline[i] >= 159 || f1.midline[i] <= 0))
    {
      f2.toppoint = i;
      f2.toppoint_flag = 1;
      break;
    }
  }
  f2.toppoint_flag = 1;
  __Limit_AB(f2.toppoint, 10, 110);
}

void find_little_curve()
{
  for (int i = 115; i > f2.toppoint + 3; i--)
  {
    //????????????
    if (f1.leftlineflag[i] == 0 &&
        f2.leftturnflag == 0 &&
        f1.leftlineflag[i + 1] == 1 &&
        f1.leftlineflag[i + 2] == 1 &&
        f1.rightlineflag[i] == 1 &&
        f1.rightlineflag[i - 5] == 1 &&
        i > 30)
    {
      f2.leftturn_row = i;
      f2.leftturnflag = 1;
      if (f2.leftturn_row < 60 &&
          f1.rightline[f2.leftturn_row - 5] > 100 &&
          f1.rightline[f2.leftturn_row - 5] > f1.rightline[f2.leftturn_row] &&
          f1.rightline[f2.leftturn_row - 5] - f1.rightline[f2.leftturn_row] > 10)
      {
        f2.rightturn_row = 0;
        f2.rightturnflag = 0;
      }

      //????????????????????
      for (int k = 110; k > f2.toppoint; k--)
      {
        if (
            !f1.leftlineflag[k + 6] &&
            !f1.leftlineflag[k + 4] &&
            !f1.leftlineflag[k + 2] &&
            f1.leftlineflag[k] &&
            f1.leftlineflag[k - 2] &&
            f1.leftlineflag[k - 4] &&
            f1.leftlineflag[k - 6] &&

            f1.rightlineflag[k + 6] &&
            f1.rightlineflag[k + 4] &&
            f1.rightlineflag[k + 2] &&
            f1.rightlineflag[k] &&
            f1.rightlineflag[k - 2] &&
            f1.rightlineflag[k - 4] &&
            f1.rightlineflag[k - 6]

            && f1.leftline[k - 6] - f1.leftline[k] > 6)
        {
          f2.leftturn_row = 0;
          f2.leftturnflag = 0;
          break;
        }
      }

      //????????????????
      int lose_cnt = 0;
      if (f2.cross_flag == 1) //???????????????????????????????????????????????????????????
      {
        for (int j = f2.leftturn_row; j > f2.toppoint; j--)
          if (f1.rightlineflag[j] == 0)
            lose_cnt++;
        if (lose_cnt > 5)
        {
          f2.leftturn_row = 0;
          f2.leftturnflag = 0;
        }
      }
      break;
    }

    //????????????
    if (f1.rightlineflag[i] == 0 &&
        f2.rightturnflag == 0 &&
        f1.rightlineflag[i + 1] == 1 &&
        f1.rightlineflag[i + 2] == 1 &&
        f1.leftlineflag[i] == 1 &&
        f1.leftlineflag[i - 5] == 1 &&
        i > 30)
    {
      f2.rightturn_row = i;
      f2.rightturnflag = 1;
      if (f2.rightturn_row <= 60 &&
          f1.leftline[f2.rightturn_row - 5] < 60 &&
          f1.leftline[f2.rightturn_row - 5] < f1.leftline[f2.rightturn_row] &&
          f1.leftline[f2.rightturn_row] - f1.leftline[f2.rightturn_row - 5] > 10)
      {
        f2.rightturn_row = 0;
        f2.rightturnflag = 0;
      }

      //????????????????????
      for (int k = 110; k > f2.toppoint; k--)
      {
        if (
            !f1.rightlineflag[k + 6] &&
            !f1.rightlineflag[k + 4] &&
            !f1.rightlineflag[k + 2] &&
            f1.rightlineflag[k] &&
            f1.rightlineflag[k - 2] &&
            f1.rightlineflag[k - 4] &&
            f1.rightlineflag[k - 6] &&

            f1.leftlineflag[k + 6] &&
            f1.leftlineflag[k + 4] &&
            f1.leftlineflag[k + 2] &&
            f1.leftlineflag[k] &&
            f1.leftlineflag[k - 2] &&
            f1.leftlineflag[k - 4] &&
            f1.leftlineflag[k - 6]

            && f1.rightline[k] - f1.rightline[k - 6] > 6)
        {
          f2.rightturn_row = 0;
          f2.rightturnflag = 0;
          break;
        }
      }

      //????????????????
      int lose_cnt = 0;
      if (f2.cross_flag == 1)
      {
        for (int j = f2.rightturn_row; j > f2.toppoint; j--)
          if (f1.leftlineflag[j] == 0)
            lose_cnt++;
        if (lose_cnt > 5)
        {
          f2.rightturn_row = 0;
          f2.rightturnflag = 0;
        }
      }
      break;
    }
  }
}

void find_large_curve()
{
  int flag_L = 0, flag_R = 0;

  if (f2.leftturnflag == 0 && f2.rightturnflag == 0)
  {
    for (int i = 118; i > 112; i--) //????????????????????????????????
    {
      if (f1.leftlineflag[i] == 0 && flag_L == 0) //????????????118-112????????????????????
      {
        flag_L = 1;
      }
      if (f1.rightlineflag[i] == 0 && flag_R == 0) //????????????118-112????????????????????
      {
        flag_R = 1;
      }
    }
    if (flag_L == 1)
    {
      for (int i = 100; i > f2.toppoint + 10; i--) //100????????????90????????????????????????????????????????????????????
      {
        if (f1.leftlineflag[i] == 1 || f1.rightlineflag[i] == 0)
        {
          flag_L = 0;
        }
      }
    }
    if (flag_R == 1)
    {
      for (int i = 100; i > f2.toppoint + 10; i--) //100????????????90????????????????????????????????????????????????????
      {
        if (f1.leftlineflag[i] == 0 || f1.rightlineflag[i] == 1)
        {
          flag_R = 0;
        }
      }
    }
    if (flag_L == 1 && flag_R == 0)
    {
      f2.leftturnflag = 3;
    }
    else if (flag_L == 0 && flag_R == 1)
    {
      f2.rightturnflag = 3;
    }
  }
}

void find_cross_infec()
{
  f2.L_infec_flag = 0;
  f2.L_infec_row = 0;
  f2.L_infec_col = 0;

  f2.R_infec_flag = 0;
  f2.R_infec_row = 0;
  f2.R_infec_col = 0;

  f2.L_up_infec_flag = 0;
  f2.L_up_infec_row = 0;
  f2.L_up_infec_col = 0;

  f2.R_up_infec_flag = 0;
  f2.R_up_infec_row = 0;
  f2.R_up_infec_col = 0;

  if (f2.toppoint > 110)
    return;

  //???????????????????????????
  for (int i = 110; i > f2.toppoint; i--)
  {
    if (
        f1.leftlineflag[i + 3] == 1 &&
        f1.leftlineflag[i + 5] == 1 &&
        f1.leftlineflag[i] == 1 &&

        abs(f1.leftline[i] - f1.leftline[i + 4]) < 5 &&

        (f1.leftlineflag[i - 5] == 0 ||
         f1.leftlineflag[i - 10] == 0 ||
         f1.leftlineflag[i - 15] == 0))
    {
      f2.L_infec_flag = 1;
      f2.L_infec_row = i;
      f2.L_infec_col = f1.leftline[i];
      break;
    }
  }

  for (int i = 110; i > f2.toppoint; i--)
  {
    if (
        f1.rightlineflag[i + 3] == 1 &&
        f1.rightlineflag[i + 5] == 1 &&
        f1.rightlineflag[i] == 1 &&

        abs(f1.leftline[i] - f1.leftline[i + 4]) < 5 &&

        (f1.rightlineflag[i - 5] == 0 ||
         f1.rightlineflag[i - 10] == 0 ||
         f1.rightlineflag[i - 15] == 0))
    {
      f2.R_infec_flag = 1;
      f2.R_infec_row = i;
      f2.R_infec_col = f1.rightline[i];
      break;
    }
  }

  int16 start_row = 0;
  if (f2.R_infec_flag == 1 && f2.L_infec_flag == 0)
    start_row = f2.R_infec_row;
  else if (f2.R_infec_flag == 0 && f2.L_infec_flag == 1)
    start_row = f2.L_infec_row;
  else if (f2.R_infec_flag == 1 && f2.L_infec_flag == 1)
    start_row = MAX(f2.R_infec_row, f2.L_infec_row);
  else
    return;

  //???????????????????????????
  for (int i = start_row; i > f2.toppoint; i--)
  {
    if (
        f1.leftlineflag[i - 3] == 1 &&
        f1.leftlineflag[i - 5] == 1 &&
        f1.leftlineflag[i] == 1 &&

        abs(f1.leftline[i] - f1.leftline[i - 3]) < 5 &&

        (f1.leftlineflag[i + 5] == 0 ||
         f1.leftlineflag[i + 10] == 0 ||
         f1.leftlineflag[i + 15] == 0))
    {
      f2.L_up_infec_flag = 1;
      f2.L_up_infec_row = i;
      f2.L_up_infec_col = f1.leftline[i];
      break;
    }
  }

  for (int i = f2.toppoint; i < 90; i++)
  {
    if (
        f1.rightlineflag[i - 3] == 1 &&
        f1.rightlineflag[i - 5] == 1 &&
        f1.rightlineflag[i] == 1 &&

        abs(f1.leftline[i] - f1.leftline[i - 3]) < 5 &&

        (f1.rightlineflag[i + 5] == 0 ||
         f1.rightlineflag[i + 10] == 0 ||
         f1.rightlineflag[i + 15] == 0))
    {
      f2.R_up_infec_flag = 1;
      f2.R_up_infec_row = i;
      f2.R_up_infec_col = f1.rightline[i];
      break;
    }
  }
}

#if 1

void fork_road_judge()
{
  find_L_R_infec();
  f2.fork_up_flag = 0;
  f2.fork_up_row = 0;
  f2.fork_up_col = 80;

  //???????????????????????????????????????????
  memset(f2.fork_up_line, 0, sizeof(f2.fork_up_line));
  memset(f2.fork_up_line_flag, 0, sizeof(f2.fork_up_line_flag));
  int row_start = MIN(f2.fork_R_row, f2.fork_L_row);
  for (int i = f2.fork_L_col; i < f2.fork_R_col; i++)
  {
    for (int j = row_start; j > 5; j--)
    {
      if (
          IMG_120_160[j + 1][i] == 0 &&
          IMG_120_160[j][i] != 0)
      {
        break;
      }

      if (
          // Image[j + 2][i] != 0 &&
          IMG_120_160[j + 1][i] != 0 &&
          IMG_120_160[j][i] == 0)
      {
        f2.fork_up_line[i] = j - 1;
        f2.fork_up_line_flag[i] = 1;
        break;
      }
    }
  }

  for (int i = f2.fork_L_col; i < f2.fork_R_col; i++)
  {
    if (
        //??????
        (f2.fork_up_line[i] >= f2.fork_up_line[i + 2] &&
         f2.fork_up_line[i] >= f2.fork_up_line[i - 2] &&

         f2.fork_up_line[i] > f2.fork_up_line[i + 10] &&
         f2.fork_up_line[i] > f2.fork_up_line[i + 20] &&
         f2.fork_up_line[i] > f2.fork_up_line[i - 10] &&
         f2.fork_up_line[i] > f2.fork_up_line[i - 20] &&

         f2.fork_up_line_flag[i] &&
         f2.fork_up_line_flag[i + 10] &&
         f2.fork_up_line_flag[i + 20] &&
         f2.fork_up_line_flag[i - 10] &&
         f2.fork_up_line_flag[i - 20] &&

         abs(f2.fork_up_line[i - 10] - f2.fork_up_line[i + 10]) < 4 &&
         abs(f2.fork_up_line[i - 20] - f2.fork_up_line[i + 20]) < 4) ||

        //??????
        (f2.fork_up_line[i] >= f2.fork_up_line[i + 2] &&
         f2.fork_up_line[i] >= f2.fork_up_line[i - 2] &&

         f2.fork_up_line[i] > f2.fork_up_line[i + 8] &&
         f2.fork_up_line[i] > f2.fork_up_line[i + 15] &&
         f2.fork_up_line[i] > f2.fork_up_line[i - 8] &&
         f2.fork_up_line[i] > f2.fork_up_line[i - 15] &&

         f2.fork_up_line_flag[i] &&
         f2.fork_up_line_flag[i + 8] &&
         f2.fork_up_line_flag[i + 15] &&
         f2.fork_up_line_flag[i - 8] &&
         f2.fork_up_line_flag[i - 15] &&

         abs(f2.fork_up_line[i - 8] - f2.fork_up_line[i + 8]) < 4 &&
         abs(f2.fork_up_line[i - 15] - f2.fork_up_line[i + 15]) < 4))
    {
      f2.fork_up_flag = 1;
      f2.fork_up_col = i;
      f2.fork_up_row = f2.fork_up_line[i];
    }
  }

  if (
      (abs(f2.fork_L_row - f2.fork_R_row) <= 10 &&
       abs(((f2.fork_R_col + f2.fork_L_col) / 2) - f2.fork_up_col) < 6 &&

       f2.fork_up_flag == 1 &&
       f2.fork_L_flag == 1 &&
       f2.fork_R_flag == 1 &&
       f2.fork_R_col - f2.fork_L_col > 10 &&

       f2.fork_up_row < f2.fork_L_row - 10 &&
       f2.fork_up_row < f2.fork_R_row - 10) ||

      //????????????
      (f2.fork_L_flag == 0 &&
       f2.fork_R_flag == 1 &&
       f2.fork_up_flag == 1 &&
       f2.fork_up_col < 75 &&

       f2.fork_up_line[2] > 35 &&
       f2.fork_up_line[2] < 65 &&
       f2.fork_R_col - f2.fork_up_col > 20 &&
       f2.fork_R_row - f2.fork_up_row > 20) ||

      //????????????
      (f2.fork_L_flag == 1 &&
       f2.fork_R_flag == 0 &&
       f2.fork_up_flag == 1 &&
       f2.fork_up_col > 85 &&

       f2.fork_up_line[157] > 35 &&
       f2.fork_up_line[157] < 65 &&
       f2.fork_up_col - f2.fork_L_col > 20 &&
       f2.fork_L_row - f2.fork_up_row > 20))
  {
    f2.fork_flag = 1;
    f2.fork_frames++;
  }
  else
  {
    f2.fork_flag = 0;
    f2.fork_frames = 0;
  }
}
#else
void fork_road_judge()
{
  find_L_R_infec();
  f2.fork_up_flag = 0;
  f2.fork_up_row = 0;
  f2.fork_up_col = 80;

  //???????????????????????????????????????????
  memset(f2.fork_up_line, 0, sizeof(f2.fork_up_line));
  memset(f2.fork_up_line_flag, 0, sizeof(f2.fork_up_line_flag));
  int row_start = MIN(f2.fork_R_row, f2.fork_L_row);
  for (int i = f2.fork_L_col; i < f2.fork_R_col; i++)
  {
    for (int j = row_start; j > 5; j--)
    {
      if (
          IMG_120_160[j + 1][i] == 0 &&
          IMG_120_160[j][i] != 0)
      {
        break;
      }

      if (
          IMG_120_160[j + 1][i] != 0 &&
          IMG_120_160[j][i] == 0)
      {
        f2.fork_up_line[i] = j - 1;
        f2.fork_up_line_flag[i] = 1;
        break;
      }
    }
  }

  f2.fork_up_flag = 1;
  f2.fork_up_col = 80;
  f2.fork_up_row = f2.fork_up_line[80];
  for (int i = 80; i < f2.fork_R_col; i++)
  {
    if (abs(f2.fork_up_line[i] - f2.fork_up_line[i + 1]) > 10)
    {
      f2.fork_up_R = i;
      break;
    }
    if (f2.fork_up_row < f2.fork_up_line[i])
    {
      f2.fork_up_row = f2.fork_up_line[i];
      f2.fork_up_col = i;
    }
  }
  for (int i = 80; i > f2.fork_L_col; i--)
  {
    if (abs(f2.fork_up_line[i] - f2.fork_up_line[i - 1]) > 10)
    {
      f2.fork_up_L = i;
      break;
    }
    if (f2.fork_up_row < f2.fork_up_line[i])
    {
      f2.fork_up_row = f2.fork_up_line[i];
      f2.fork_up_col = i;
    }
  }
  int flag = 0;
  if (is_straight_line(f2.fork_up_col + 2, f2.fork_up_R - 2, f2.fork_up_line, 3) &&
      is_straight_line(f2.fork_up_L + 2, f2.fork_up_col - 2, f2.fork_up_line, 3) &&
      abs(f2.fork_up_R - f2.fork_up_col) > 10 &&
      abs(f2.fork_up_col - f2.fork_up_L) > 10)
  {
    flag = 1;
  }
  if (
      (
          // f2.fork_up_line[f2.fork_up_col - 10] < f2.fork_up_line[f2.fork_up_col] &&
          flag &&

          abs(f2.fork_up_line[f2.fork_up_col - 10] - f2.fork_up_line[f2.fork_up_col + 10]) < 4 &&
          abs(f2.fork_up_line[f2.fork_up_col - 20] - f2.fork_up_line[f2.fork_up_col + 20]) < 4) ||
      (
          // f2.fork_up_line[f2.fork_up_col - 8] < f2.fork_up_line[f2.fork_up_col] &&
          flag &&
          abs(f2.fork_up_line[f2.fork_up_col - 8] - f2.fork_up_line[f2.fork_up_col + 8]) < 4 &&
          abs(f2.fork_up_line[f2.fork_up_col - 15] - f2.fork_up_line[f2.fork_up_col + 15]) < 4))
  {
    f2.fork_up_row = f2.fork_up_row;
    f2.fork_up_col = f2.fork_up_col;
    f2.fork_up_flag = f2.fork_up_flag;
  }
  else
  {
    f2.fork_up_row = 0;
    f2.fork_up_col = 0;
    f2.fork_up_flag = 0;
  }

  if (
      (abs(f2.fork_L_row - f2.fork_R_row) <= 15 &&
       abs(((f2.fork_R_col + f2.fork_L_col) / 2) - f2.fork_up_col) < 6 &&

       f2.fork_up_flag == 1 &&
       f2.fork_L_flag == 1 &&
       f2.fork_R_flag == 1 &&
       f2.fork_R_col - f2.fork_L_col > 10 &&

       f2.fork_up_row < f2.fork_L_row + 10 &&
       f2.fork_up_row < f2.fork_R_row + 10) ||

      //????????????
      (f2.fork_L_flag == 0 &&
       f2.fork_R_flag == 1 &&
       f2.fork_up_flag == 1 &&
       f2.fork_up_col < 75 &&

       f2.fork_up_line[1] > 35 &&
       f2.fork_up_line[1] < 65 &&
       f2.fork_R_col - f2.fork_up_col > 20 &&
       f2.fork_R_row - f2.fork_up_row > 20) ||

      //????????????
      (f2.fork_L_flag == 1 &&
       f2.fork_R_flag == 0 &&
       f2.fork_up_flag == 1 &&
       f2.fork_up_col > 85 &&

       f2.fork_up_line[158] > 35 &&
       f2.fork_up_line[158] < 65 &&
       f2.fork_up_col - f2.fork_L_col > 20 &&
       f2.fork_L_row - f2.fork_up_row > 20))
  {
    f2.fork_flag = 1;
    f2.fork_frames++;
  }
  else
  {
    f2.fork_flag = 0;
    f2.fork_frames = 0;
  }
}
#endif
void find_edge();
#if 1
void qrcode_judge() {
    find_edge();
int start_row=115;
int duty_threshold=10;
    int top_row = 0;
    f2.tag_flag = 0;
    f2.tag_row_up = 0;
    f2.tag_row_down = 0;


    for (int i = start_row; i > 10; i--)
    {
        f1.road_real_width[i] = f1.rightline[i] - f1.leftline[i];
        int black_point_num = 0;
        f1.tag_black_duty[i]=0;
        for (int j = f1.leftline[i]; j < f1.rightline[i]; j++)
        {
            if (IMG_120_160[i][j] == 0)
                black_point_num++;
        }

        if( f1.road_real_width[i]!=0)
            f1.tag_black_duty[i] = black_point_num * 100 / f1.road_real_width[i];
    }

    for (int i = start_row; i > 10; i--)
    {
        if (f1.leftlineflag[i]==0||
                f1.rightlineflag[i]==0)
        {
            top_row = i;
            break;
        }
    }

    for (int i = start_row; i > top_row; i--)
    {
        if (f1.tag_black_duty[i + 0] > duty_threshold &&
            f1.tag_black_duty[i - 1] > duty_threshold &&
            f1.tag_black_duty[i - 2] > duty_threshold &&
            f1.tag_black_duty[i - 3] > duty_threshold &&

            f1.tag_black_duty[i + 0] < 70 &&
            f1.tag_black_duty[i - 1] < 70 &&
            f1.tag_black_duty[i - 2] < 70 &&
            f1.tag_black_duty[i - 3] < 70)
        {
            f2.tag_row_down = i;
            break;
        }
    }

    for (int i = f2.tag_row_down; i > top_row; i--)
    {
        if ((f1.tag_black_duty[i -1] < duty_threshold||
                f1.tag_black_duty[i -1] >70) &&

                f1.tag_black_duty[i + 0] > duty_threshold &&
            f1.tag_black_duty[i + 1] > duty_threshold &&
            f1.tag_black_duty[i + 2] > duty_threshold &&
            f1.tag_black_duty[i + 3] > duty_threshold &&

            f1.tag_black_duty[i + 0] < 70 &&
            f1.tag_black_duty[i + 1] < 70 &&
            f1.tag_black_duty[i + 2] < 70 &&
            f1.tag_black_duty[i + 3] < 70)
        {
            f2.tag_row_up = i;
            break;
        }
    }

    if (
            top_row &&
            f2.tag_row_up &&
            f2.tag_row_down &&
            is_straight_line(f2.tag_row_down+2,
                             f2.tag_row_up-2,
                             f1.leftline, 3) &&
            is_straight_line(f2.tag_row_down+2,
                             f2.tag_row_up-2,
                             f1.rightline, 3)

        // is_straight_line(f2.tag_row_down / 2 + 4,
        //                  f1.leftstartrow - 1,
        //                  f1.leftline, 4) &&
        // is_straight_line(f2.tag_row_down / 2 + 4,
        //                  f1.rightstartrow - 1,
        //                  f1.rightline, 4)

            )
    {
        f2.tag_flag = 1;
        f2.tag_frames++;
    }
    else
    {
        f2.tag_flag = 0;
        f2.tag_frames = 0;
    }
}
#elif 0
void qrcode_judge()
{
  f2.tag_flag = 0;

  f2.tag_row_up = 0;
  f2.tag_row_down = 0;

  f2.tag_left_col = 0;
  f2.tag_right_col = 0;

  for (int i = 222; i > 20; i--)
  {
    f1.tag_leftline[i] = 0;
    f1.tag_leftlineflag[i] = 0;
    f1.tag_rightline[i] = 319;
    f1.tag_rightlineflag[i] = 0;
    for (int j = 10; j < 319; j++)
    {
      if (
          IMG_240_320[i][j + 2] != 0 &&
          IMG_240_320[i][j + 1] != 0 &&
          IMG_240_320[i][j] != 0 &&
          IMG_240_320[i][j - 1] == 0 &&
          IMG_240_320[i][j - 2] == 0 &&
          IMG_240_320[i][j - 3] == 0)
      {
        f1.tag_leftline[i] = j - 1;
        f1.tag_leftlineflag[i] = 1;
        break;
      }
    }
    for (int j = 319; j > 10; j--)
    {
      if (
          IMG_240_320[i][j - 2] != 0 &&
          IMG_240_320[i][j - 1] != 0 &&
          IMG_240_320[i][j] != 0 &&
          IMG_240_320[i][j + 1] == 0 &&
          IMG_240_320[i][j + 2] == 0 &&
          IMG_240_320[i][j + 3] == 0)
      {
        f1.tag_rightline[i] = j + 1;
        f1.tag_rightlineflag[i] = 1;
        break;
      }
    }
  }

  for (int i = 222; i > 10; i--)
  {
    f1.road_real_width[i] = f1.tag_rightline[i] - f1.tag_leftline[i];
    int black_point_num = 0;
    for (int j = f1.tag_leftline[i]; j < f1.tag_rightline[i]; j++)
    {
      if (IMG_240_320[i][j] == 0)
        black_point_num++;
    }

    if( f1.road_real_width[i]!=0)
    f1.tag_black_duty[i] = black_point_num * 100 / f1.road_real_width[i];
  }

  int top_row = 0;
  for (int i = 222; i > 10; i--)
  {
    if (abs(f1.road_real_width[i] - 4 * half_width[i]) > 40 &&
        abs(f1.road_real_width[i - 3] - 4 * half_width[i - 3]) > 40 &&
        abs(f1.road_real_width[i - 6] - 4 * half_width[i - 6]) > 40)
    {
      top_row = i;
      break;
    }
  }

  for (int i = 222; i > top_row; i--)
  {
    if (f1.tag_black_duty[i + 0] > 30 &&
        f1.tag_black_duty[i - 1] > 30 &&
        f1.tag_black_duty[i - 2] > 30 &&
        f1.tag_black_duty[i - 3] > 30 &&

        f1.tag_black_duty[i + 0] < 70 &&
        f1.tag_black_duty[i - 1] < 70 &&
        f1.tag_black_duty[i - 2] < 70 &&
        f1.tag_black_duty[i - 3] < 70)
    {
      f2.tag_row_down = i;
      break;
    }
  }

  for (int i = f2.tag_row_down; i > top_row; i--)
  {
    if (f1.tag_black_duty[i + 0] > 30 &&
        f1.tag_black_duty[i + 1] > 30 &&
        f1.tag_black_duty[i + 2] > 30 &&
        f1.tag_black_duty[i + 3] > 30 &&

        f1.tag_black_duty[i + 0] < 70 &&
        f1.tag_black_duty[i + 1] < 70 &&
        f1.tag_black_duty[i + 2] < 70 &&
        f1.tag_black_duty[i + 3] < 70)
    {
      f2.tag_row_up = i;
      break;
    }
  }

  if (
      top_row &&
      f2.tag_row_up &&
      f2.tag_row_down &&
      is_straight_line(f2.tag_row_down,
                       f2.tag_row_up,
                       f1.tag_leftline, 4) &&
      is_straight_line(f2.tag_row_down,
                       f2.tag_row_up,
                       f1.tag_rightline, 4)

      // is_straight_line(f2.tag_row_down / 2 + 4,
      //                  f1.leftstartrow - 1,
      //                  f1.leftline, 4) &&
      // is_straight_line(f2.tag_row_down / 2 + 4,
      //                  f1.rightstartrow - 1,
      //                  f1.rightline, 4)

  )
  {
    f2.tag_flag = 1;
    f2.tag_frames++;
  }
  else
  {
    f2.tag_flag = 0;
    f2.tag_frames = 0;
  }
}
#elif 0
void qrcode_judge()
{
  // f2.tag_flag = 0;
  // f2.tag_row_cnt = 0;
  // f2.tag_left_col = 0;
  // f2.tag_right_col = 0;
  // f2.tag_row_down = 0;
  // memset(f1.tag_leftline, 0, sizeof(f1.tag_leftline));
  // memset(f1.tag_leftlineflag, 0, sizeof(f1.tag_leftlineflag));
  // memset(f1.tag_rightline, 159, sizeof(f1.tag_rightline));
  // memset(f1.tag_rightlineflag, 0, sizeof(f1.tag_rightlineflag));

  f2.tag_flag = 0;
  memset(f1.tag_down_line, 0, sizeof(f1.tag_down_line));
  memset(f1.tag_down_line_flag, 0, sizeof(f1.tag_down_line_flag));

  f1.L_start_col = f1.leftline[f1.leftstartrow];
  f1.R_start_col = f1.rightline[f1.rightstartrow];
  f1.L_end_col = 80;
  f1.R_end_col = 80;
  f2.tag_row_down = 0;

  for (int i = f1.L_start_col; i < f1.R_start_col; i++)
  {
    for (int j = 118; j > 5; j--)
    {
      if (
          IMG_120_160[j + 1][i] == 0 &&
          IMG_120_160[j][i] != 0)
      {
        break;
      }

      if (
          IMG_120_160[j + 1][i] != 0 &&
          IMG_120_160[j][i] == 0)
      {
        f1.tag_down_line[i] = j - 1;
        f1.tag_down_line_flag[i] = 1;
        break;
      }
    }
  }

  for (int i = f1.L_start_col + 1; i < f1.R_start_col; i++)
  {
    if (abs(f1.tag_down_line[i] - f1.tag_down_line[i + 1]) >= 3)
    {
      f1.L_end_col = i;
      break;
    }
  }

  for (int i = f1.R_start_col - 1; i > f1.L_start_col; i--)
  {
    if (abs(f1.tag_down_line[i] - f1.tag_down_line[i - 1]) >= 3)
    {
      f1.R_end_col = i;
      break;
    }
  }

  int top_row = (f1.tag_down_line[f1.L_end_col] + f1.tag_down_line[f1.R_end_col]) / 2;
  for (int i = f1.L_end_col; i < f1.R_end_col; i++)
  {
    if (abs(f1.tag_down_line[i] - f1.tag_down_line[i + 1]) < 2 &&
        abs(f1.tag_down_line[i] - f1.tag_down_line[i - 1]) < 2 &&
        f1.tag_down_line[i] - top_row > 10)
    {
      // f2.tag_row_down = i;
      for (int i = f1.L_end_col; i < f1.R_end_col; i++)
      {
        if (f2.tag_row_down < f1.tag_down_line[i])
        {
          f2.tag_row_down = f1.tag_down_line[i];
        }
      }
      break;
    }
  }

  float k1 = (f1.tag_down_line[f1.L_start_col + 2] - f1.tag_down_line[f1.L_end_col - 2]) / (f1.L_end_col - f1.L_start_col);
  float k2 = (f1.tag_down_line[f1.R_start_col + 2] - f1.tag_down_line[f1.R_end_col - 2]) / (f1.R_start_col - f1.R_end_col);

  if (
      top_row < 60 &&
      f1.L_end_col - f1.L_start_col > 10 &&
      f1.R_start_col - f1.R_end_col > 10 &&

      fabs(k1 - k2) < 1 &&

      is_straight_line(f1.L_start_col + 2,
                       f1.L_end_col - 2,
                       f1.tag_down_line, 3) &&
      is_straight_line(f1.R_end_col + 2,
                       f1.R_start_col - 2,
                       f1.tag_down_line, 3))
  {
    f2.tag_flag = 1;
    f2.tag_frames++;
  }
  else
  {
    f2.tag_flag = 0;
    f2.tag_frames = 0;
  }
}
#else
void qrcode_judge()
{
  f2.tag_flag = 0;
  f2.tag_row_cnt = 0;
  f2.tag_left_col = 0;
  f2.tag_right_col = 0;
  f2.tag_row_down = 0;

  for (int i = 118; i >= 40; i--)
  {
    f2.tag_left[0] = 0;
    f2.tag_left[1] = 0;
    f2.tag_right[0] = 0;
    f2.tag_right[1] = 0;

    f2.L_col1 = 0;
    f2.L_col2 = 0;
    f2.R_col1 = 0;
    f2.R_col2 = 0;

    //???????????
    for (int j = 5; j < 140; j++)
    {
      if (IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j] == 0)
      {
        f2.tag_left[0] = 1;
        f2.L_col1 = j;
      }

      if (IMG_120_160[i][j - 1] == 0 &&
          IMG_120_160[i][j] != 0 &&
          f2.tag_left[0] == 1)
      {
        f2.tag_left[1] = 1;
        f2.L_col2 = j;
      }

      if (f2.tag_left[0] == 1 &&
          f2.tag_left[1] == 1)
        break;
    }

    //??????????
    for (int j = 155; j > 20; j--)
    {
      if (IMG_120_160[i][j + 1] != 0 &&
          IMG_120_160[i][j] == 0)
      {
        f2.tag_right[0] = 1;
        f2.R_col1 = j; //????????
      }

      if (IMG_120_160[i][j + 1] == 0 &&
          IMG_120_160[i][j] != 0 &&

          f2.tag_right[0] == 1)
      {
        f2.tag_right[1] = 1;
        f2.R_col2 = j;
      }

      if (f2.tag_right[0] == 1 &&
          f2.tag_right[1] == 1)
        break;
    }

    //????????
    if (
        f2.tag_left[0] == 1 &&
        f2.tag_left[1] == 1 &&
        f2.L_col2 - f2.L_col1 >= 1 &&

        f2.tag_right[0] == 1 &&
        f2.tag_right[1] == 1 &&
        f2.R_col1 - f2.R_col2 >= 1 &&

        f2.R_col2 - f2.L_col2 > 5 &&
        f2.R_col2 - f2.L_col2 < 100)
    {
      f2.tag_right[0] = 0;
      f2.tag_right[1] = 0;
      f2.tag_left[0] = 0;
      f2.tag_left[1] = 0;

      f2.tag_row_cnt++;
      if (!f2.tag_row_down)
      {
        f2.tag_left_col = f2.L_col1;
        f2.tag_right_col = f2.R_col1;
        f2.tag_row_down = i;
      }
    }
  }

  //??????????
  memset(f1.tag_down_line, 0, sizeof(f1.tag_down_line));
  for (int i = f2.tag_left_col; i < f2.tag_right_col; i++)
  {
    for (int j = 118; j > f2.tag_row_down - 10; j--)
    {
      if (
          IMG_120_160[j + 1][i] != 0 &&
          IMG_120_160[j][i] == 0)
      {
        f1.tag_down_line[i] = j - 1;
        break;
      }
    }
  }

  int tag_down_flag = 1;
  for (int i = f2.tag_left_col + 1; i < f2.tag_right_col - 1; i++)
  {
    if (abs(f1.tag_down_line[i] - f2.tag_row_down) > 15)
      tag_down_flag = 0;
  }

  //????????
  if (f2.tag_row_cnt >= 5 &&
      f2.tag_row_down &&
      tag_down_flag &&
      f2.tag_right_col - f2.tag_left_col > 11 &&

      is_straight_line(f2.tag_row_down + 3,
                       f1.leftstartrow - 3,
                       f1.leftline, 3) &&

      is_straight_line(f2.tag_row_down + 3,
                       f1.rightstartrow - 3,
                       f1.rightline, 3) &&

      is_straight_line(f2.tag_left_col + 1,
                       f2.tag_right_col - 1,
                       f1.tag_down_line, 3)

  )
  {
    f2.tag_flag = 1;
    f2.tag_frames++;
  }
  else
  {
    f2.tag_flag = 0;
    f2.tag_frames = 0;
  }
}
#endif

void garage_judge()
{
  f2.garage_row = 0;
  f2.garage_tmp[0] = 0;
  f2.garage_tmp[1] = 0;
  int16 garage_start_row = 0;

  for (uint8 i = 110; i >= 50; i--)
  {
    for (uint8 j = 20; j < 140; j++)
    {
      if (IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j - 2] != 0 &&
          IMG_120_160[i][j] == 0 &&
          IMG_120_160[i][j + 1] == 0)
      {
        f2.garage_tmp[0] = 1;
      }
      if (IMG_120_160[i][j - 1] == 0 &&
          IMG_120_160[i][j - 2] == 0 &&
          IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j + 1] != 0)
      {
        if (f2.garage_tmp[0] == 1)
          f2.garage_tmp[1] = 1;
      }

      if (f2.garage_tmp[0] == 1 &&
          f2.garage_tmp[1] == 1)
      {
        f2.garage_tmp[0] = 0;
        f2.garage_tmp[1] = 0;

        if (!garage_start_row)
        {
          garage_start_row = i;
        }

        f2.garage_cnt++;
      }

      if (f2.garage_cnt >= 6 &&
          f2.garage_cnt <= 11)
      //????????????,????????9??????
      {
        f2.garage_row++;
        f2.garage_cnt = 0;
        break;
      }
    }
    f2.garage_cnt = 0;
  }

  qrcode_judge();
  if (f2.garage_row >= 10 &&
      garage_start_row > 65 &&
      !f2.tag_flag)
  //??????????????
  {
    f2.garage_flag = 1;
    f2.garage_frames++;

    if (cal_region_color_duty(0, 60, 80, 118, 255) >
        cal_region_color_duty(80, 60, 159, 118, 255))
      f2.garage_dir = 0; //left
    else
      f2.garage_dir = 1;
  }
  else
  {
    f2.garage_flag = 0;
    f2.garage_frames = 0;
  }
}

void ramp_judge()
{
  //40-80: ??????????????????????, ??????????????????????, ????????????>20, ??????>70
#if USE_GYRO_RAMP
  if (fabs(inoutdev.gyro_y_angle) > 6)
  {
    f2.ramp_flag = 1;
    f2.ramp_frames++;
  }
  else
  {
    f2.ramp_flag = 0;
    f2.ramp_frames = 0;
  }
#else
  f2.ramp_top_row = 118;
  int down_start = MIN(f1.leftstartrow, f1.rightstartrow);
  for (int i = down_start; i > f2.toppoint; i--)
  {
    if (f1.leftline[i] - f1.leftline[i + 1] > 2 ||
        f1.rightline[i] - f1.rightline[i + 1] < -2)
    {
      f2.ramp_top_row = i;
    }
  }

  if (f2.ramp_top_row < 50 &&
      is_L_R_nonlose(f2.ramp_top_row + 5, f1.leftstartrow, 0, 95) &&
      is_L_R_nonlose(f2.ramp_top_row + 5, f1.rightstartrow, 1, 95) &&
      is_straight_line(f2.ramp_top_row + 5, down_start, f1.midline, 4) &&
      f1.rightline[f2.ramp_top_row + 5] - f1.leftline[f2.ramp_top_row + 5] > 70
      //&&fabs(inoutdev.gyro_y_angle) > 6
      )
  {
    f2.ramp_flag = 1;
    f2.ramp_frames++;
  }
  else
  {
    f2.ramp_flag = 0;
    f2.ramp_frames = 0;
  }
#endif
}

void cross_judge()
{
  find_cross_infec();

  f2.cross_flag = 0;
  f2.cross_type = 0;
  //????????????????????
  if (f2.L_infec_flag == 1 &&
      f2.R_infec_flag == 1 &&
      f2.L_up_infec_flag == 1 &&
      f2.R_up_infec_flag == 1 &&

      f2.L_infec_row > f2.L_up_infec_row &&
      f2.R_infec_row > f2.R_up_infec_row &&

      f2.R_infec_col - f2.L_infec_col > 20 &&
      f2.R_up_infec_col - f2.L_up_infec_col > 10 &&
      f2.R_infec_col - f2.L_infec_col > f2.R_up_infec_col - f2.L_up_infec_col
      // f2.L_infec_col < f2.L_up_infec_col &&
      // f2.R_infec_col > f2.R_up_infec_col
  )
  {
    f2.cross_flag = 1;
    f2.cross_frames++;
    f2.cross_type = 0;
  }
  //????????????????????????
  else if (f2.L_infec_flag == 1 &&
           f2.R_infec_flag == 0 &&
           f2.L_up_infec_flag == 1 &&
           f2.R_up_infec_flag == 1 &&

           //  f2.L_infec_col > 40 &&
           f2.L_infec_row > f2.L_up_infec_row &&
           f2.R_up_infec_col - f2.L_up_infec_col > 10 &&
           f2.L_infec_col < f2.L_up_infec_col)
  {
    f2.cross_flag = 1;
    f2.cross_frames++;
    f2.cross_type = 1;
  }
  //?????????????????????????
  else if (f2.R_infec_flag == 1 &&
           f2.L_infec_flag == 0 &&
           f2.L_up_infec_flag == 1 &&
           f2.R_up_infec_flag == 1 &&

           //  f2.R_infec_col < 120 &&
           f2.R_infec_row > f2.R_up_infec_row &&
           f2.R_up_infec_col - f2.L_up_infec_col > 10 &&
           f2.R_infec_col > f2.R_up_infec_col)
  {
    f2.cross_flag = 1;
    f2.cross_frames++;
    f2.cross_type = 2;
  }
  else
  {
    f2.cross_flag = 0;
    f2.cross_frames = 0;
  }
}

void L_loop_judge()
{
  f2.L_loop_flag = 0;

  //step1
  if (f2.L_loop_judge_sm == 0)
  {
    f2.L_loop_down_row = 0;

    for (int i = 110; i > f2.toppoint; i--)
    {
      if (
          !f1.leftlineflag[i + 6] &&
          !f1.leftlineflag[i + 4] &&
          !f1.leftlineflag[i + 2] &&
          f1.leftlineflag[i] &&
          f1.leftlineflag[i - 2] &&
          f1.leftlineflag[i - 4] &&
          f1.leftlineflag[i - 6]

          && f1.leftline[i - 6] - f1.leftline[i] > 6)
      {
        f2.L_loop_down_row = i;
        f2.L_loop_judge_sm = 1;
        break;
      }
    }
  }

  //step2
  if (f2.L_loop_judge_sm == 1)
  {
    f2.L_loop_judge_sm = 0;
    find_L_loop_circle();
    if (
        // f1.rightstartrow > 85 &&
        // // f1.rightline[f1.rightstartrow] > 155 &&
        (f1.rightstartrow >= 118 ||

         (f1.rightstartrow > 85 &&
          f1.rightstartrow < 118 &&
          f1.rightline[f1.rightstartrow] > 155)) &&

        f2.L_loop_circle_flag &&
        f2.L_loop_circle_row < 80 &&
        f2.L_loop_circle_row < f1.rightstartrow - 8 &&

        is_straight_line(f2.L_loop_circle_row - 5,
                         f1.rightstartrow - 5,
                         f1.rightline, 2) &&
        is_L_R_nonlose(f2.L_loop_circle_row - 6,
                       f1.rightstartrow - 5, 1, 90))
    {
      f2.L_loop_judge_sm = 2;
    }
  }

  //step3
  if (f2.L_loop_judge_sm == 2)
  {
    f2.L_loop_judge_sm = 0;
    f2.L_loop_up_row = 0;

    for (int i = f2.L_loop_circle_row; i > 18; i--)
    {
      if (
          f1.leftlineflag[i + 4] &&
          f1.leftlineflag[i + 2] &&
          f1.leftline[i + 2] - f1.leftline[i] > 20)
      {
        f2.L_loop_up_row = i;
        // f2.L_loop_flag = 1;
        // f2.L_loop_judge_sm = 0;
        f2.L_loop_judge_sm = 3;
        break;
      }
    }
  }

  //step4
  if (f2.L_loop_judge_sm == 3)
  {
    f2.L_loop_judge_sm = 0;
    f2.L_loop_top_row = 0;

    for (int i = f2.L_loop_up_row; i > 18; i--)
    {
      if (
          // f1.leftlineflag[i + 4] &&
          f1.leftlineflag[i - 2] &&
          f1.leftline[i - 2] - f1.leftline[i] > 13)
      {
        f2.L_loop_top_row = i;
        f2.L_loop_flag = 1;
        f2.L_loop_judge_sm = 0;
        break;
      }
    }
  }
}

void R_loop_judge()
{
  f2.R_loop_flag = 0;

  //step1
  if (f2.R_loop_judge_sm == 0)
  {
    f2.R_loop_down_row = 0;

    for (int i = 110; i > f2.toppoint; i--)
    {
      if (
          !f1.rightlineflag[i + 6] &&
          !f1.rightlineflag[i + 4] &&
          !f1.rightlineflag[i + 2] &&
          f1.rightlineflag[i] &&
          f1.rightlineflag[i - 2] &&
          f1.rightlineflag[i - 4] &&
          f1.rightlineflag[i - 6])
      {
        f2.R_loop_down_row = i;
        f2.R_loop_judge_sm = 1;
        break;
      }
    }
  }

  //step2
  if (f2.R_loop_judge_sm == 1)
  {
    f2.R_loop_judge_sm = 0;
    find_R_loop_circle();
    if (
        (f1.leftstartrow >= 118 ||

         (f1.leftstartrow > 85 &&
          f1.leftstartrow < 118 &&
          f1.leftline[f1.leftstartrow] < 4)) &&

        f2.R_loop_circle_flag &&
        f2.R_loop_circle_row < 80 &&
        f2.R_loop_circle_row < f1.leftstartrow - 8 &&

        is_straight_line(f2.R_loop_circle_row - 5,
                         f1.leftstartrow - 5,
                         f1.leftline, 2) &&
        is_L_R_nonlose(f2.R_loop_circle_row - 6,
                       f1.leftstartrow - 5, 0, 90))
    {
      f2.R_loop_judge_sm = 2;
    }
  }

  //step3
  if (f2.R_loop_judge_sm == 2)
  {
    f2.R_loop_judge_sm = 0;
    f2.R_loop_up_row = 0;

    for (int i = f2.R_loop_circle_row; i > 18; i--)
    {
      if (
          f1.rightlineflag[i + 4] &&
          f1.rightlineflag[i + 2] &&
          f1.rightline[i] - f1.rightline[i + 2] > 20)
      {
        f2.R_loop_up_row = i;
        // f2.R_loop_flag = 1;
        // f2.R_loop_judge_sm = 0;
        f2.R_loop_judge_sm = 3;
        break;
      }
    }
  }

  //step4
  if (f2.R_loop_judge_sm == 3)
  {
    f2.R_loop_judge_sm = 0;
    f2.R_loop_top_row = 0;

    for (int i = f2.R_loop_up_row; i > 18; i--)
    {
      if (
          // f1.rightlineflag[i + 4] &&
          f1.rightlineflag[i - 2] &&
          f1.rightline[i - 2] - f1.rightline[i] > 13)
      {
        f2.R_loop_top_row = i;
        f2.R_loop_flag = 1;
        f2.R_loop_judge_sm = 0;
        break;
      }
    }
  }
}

void long_judge()
{
  f2.long_straight_flag = 0;

  if (
      is_M_center(118, LONG_STRAIGHT_ENDROW, 8) &&
      f2.toppoint <= LONG_STRAIGHT_ENDROW &&
      is_L_R_nonlose(LONG_STRAIGHT_ENDROW, 100, 2, 92))
  {
    f2.long_straight_flag = 1;
  }
}

void short_judge()
{
  int16 stdDeviation = (int16)solve_variance(100, 40);
  if (stdDeviation <= 6)
  {
    f2.shortstraight_flag = 1;
  }
  // else if (stdDeviation <= 11)
  // {
  //   f2.shortstraight_flag = 2;
  // }
  else
  {
    f2.shortstraight_flag = 0;
  }
}

void stop_judge()
{
  f2.stop_car_flag = 0;
  f2.stop_car_reg[0] = 0;

  if (is_region_one_color(1, 117, 158, 118, 0, 95))
    f2.stop_car_reg[0] = 1;

  int stop_count = 0;
  for (int i = 19; i >= 1; i--)
  {
    f2.stop_car_reg[i] = f2.stop_car_reg[i - 1];
    if (f2.stop_car_reg[i])
      stop_count++;
  }

  if (stop_count >= 18)
    f2.stop_car_flag = 1;
}

void L_mid_endrow_proc()
{
  if (f2.L_mid_endrow == 0)
    f2.L_mid_endrow = f2.toppoint;
  for (int i = f2.L_mid_endrow; i > 0; i--)
  {
    f1.leftline[i] = 0;
    f1.rightline[i] = 0;
    f1.midline[i] = 0;
  }
}

void R_mid_endrow_proc()
{
  if (f2.R_mid_endrow == 0)
    f2.R_mid_endrow = f2.toppoint;
  for (int i = f2.R_mid_endrow; i > 0; i--)
  {
    f1.leftline[i] = 159;
    f1.rightline[i] = 159;
    f1.midline[i] = 159;
  }
}

void L_loop_patch()
{
#if 0
  if (f2.L_loop_sm_state == 1)
  {
    for (int i = 117; i > f2.toppoint; i--)
    {
      if (f1.rightlineflag[i] == 0)
        f1.midline[i] = f1.midline[i + 1];
      else
        f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]) - 2;
    }

    L_mid_endrow_proc();

    int loop_up_L_infec_row = 0;
    for (int i = 100; i > f2.toppoint; i--)
    {
      if (
          f1.leftline[i] > 20 &&

          // f1.leftlineflag[i - 9] == 1 &&
          // f1.leftlineflag[i - 5] == 1 &&
          f1.leftlineflag[i - 3] == 1 &&
          f1.leftlineflag[i] == 1 &&

          abs(f1.leftline[i] - f1.leftline[i - 3]) < 4 &&
          // abs(f1.leftline[i - 6] - f1.leftline[i - 9]) < 4 &&
          f1.leftline[i] - f1.leftline[i + 3] > 20 &&

          (f1.leftlineflag[i + 2] == 0 ||
           f1.leftlineflag[i + 4] == 0))
      {
        loop_up_L_infec_row = i;
        break;
      }
    }
    if (loop_up_L_infec_row > 23)
    {
      f2.L_loop_sm_state = 2;
      chassis.loop_distance = 0;
    }
  }

  if (f2.L_loop_sm_state == 2)
  {
    for (int i = 110; i > 35; i--)
    {
      if (i < 108 && f1.leftlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] - 2;
      }
      else if (i >= 108 && f1.leftlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] - 1;
      }
      else
      {
        f1.midline[i] = f1.leftline[i] + (int16)(loop_half_width[i]);
      }

      if (abs(f1.midline[i] - f1.midline[i + 1]) > 30)
        f1.midline[i] = f1.midline[i + 1];

      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }

    L_mid_endrow_proc();
    if (is_L_R_lose(90, 118, 1, 90))
      f2.L_loop_sm_state = 4;
  }
#else
  //??????????1
  if (f2.L_loop_sm_state == 1)
  {
    for (int i = 117; i > f2.toppoint; i--)
    {
      if (f1.rightlineflag[i] == 0)
        f1.midline[i] = f1.midline[i + 1];
      else
        f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]) - 2;
    }

    L_mid_endrow_proc();
    if (is_L_R_lose(90, 118, 0, 95))
    {
      f2.L_loop_sm_state = 2;
      //chassis.loop_distance = 0;
    }
  }

  //??????????2
  if (f2.L_loop_sm_state == 2)
  {
    for (int i = 117; i > f2.toppoint; i--)
    {
      if (f1.rightlineflag[i] == 0)
        f1.midline[i] = f1.midline[i + 1];
      else
        f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]) - 2;
    }

    L_mid_endrow_proc();
    if (
        // is_L_R_nonlose(80, 110, 0, 70) &&
        //chassis.loop_distance > 40 &&
        f1.leftstartrow > 91)
    {
      f2.L_loop_sm_state = 3;
      //chassis.loop_distance = 0;
    }
  }

  //??????????3
  if (f2.L_loop_sm_state == 3)
  {
    for (int i = 110; i > 35; i--)
    {
      if (i < 108 && f1.leftlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] - 2;
      }
      else if (i >= 108 && f1.leftlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] - 1;
      }
      else
      {
        f1.midline[i] = f1.leftline[i] + (int16)(loop_half_width[i]);
      }

      if (abs(f1.midline[i] - f1.midline[i + 1]) > 30)
        f1.midline[i] = f1.midline[i + 1];

      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }

    L_mid_endrow_proc();
    if (is_L_R_lose(90, 118, 1, 90))
      f2.L_loop_sm_state = 4;
  }
#endif

  //??????????4
  if (f2.L_loop_sm_state == 4)
  {
    for (int i = 110; i > 35; i--)
    {
      if (i < 108 && f1.leftlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] - 2;
      }
      else if (i >= 108 && f1.leftlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] - 1;
      }
      else
      {
        f1.midline[i] = f1.leftline[i] + (int16)(loop_half_width[i]);
      }

      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }

    L_mid_endrow_proc();

    if (is_L_R_nonlose(80, 110, 1, 80))
      f2.L_loop_sm_state = 5;
  }

  //????????????????5
  if (f2.L_loop_sm_state == 5)
  {
    for (int i = 110; i > 35; i--)
    {
      f1.midline[i] = f1.rightline[i] - (int16)(loop_half_width[i]);

      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }

    L_mid_endrow_proc();

#if USE_GYRO_LOOP
    if (fabs(inoutdev.gyro_z_angle) >= 260)
      f2.L_loop_sm_state = 6;
#else
    find_L_loop_exit();
    if (f2.L_loop_exit_flag)
      f2.L_loop_sm_state = 6;
#endif
  }

  //????????????????6
  if (f2.L_loop_sm_state == 6)
  {
    //????????????????????
    int tmp = 5;
    tmp = (int)(f2.toppoint * 0.15);
    __Limit_AB(tmp, 0, 60);

    int flag = 0;
    if (is_L_R_nonlose(90, 100, 1, 100) &&
        is_L_R_lose(110, 118, 0, 100))
      flag = 1;

    for (int i = 110; i < 120; i++)
    {
      if (flag)
        f1.midline[i] = f1.rightline[i] / 2 - tmp;
      else
        f1.midline[i] = 80 - tmp;
    }

    //????????????????????
    for (int i = 100; i > 35; i--)
    {
      if (f1.rightlineflag[i] == 0 || f1.rightline[i] > f1.rightline[i + 1])
      {
        f1.midline[i] = f1.midline[i + 1] - 2;
      }
      else
      {
        f1.midline[i] = f1.midline[i + 1] - (f1.rightline[i + 1] - f1.rightline[i]);
      }

      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }

    L_mid_endrow_proc();

#if USE_GYRO_LOOP
    if (fabs(inoutdev.gyro_z_angle) >= 300)
    {
      f2.L_loop_sm_state = 7;
      inoutdev.beep(1);
    }
#else
    find_L_loop_exit();
    for (int i = f2.toppoint; i < 95; i++)
    {
      if (!f1.rightlineflag[i])
        break;

      if (!f2.L_loop_exit_flag &&
          f1.rightline[i] >= f1.rightline[i + 3] - 3 &&
          f1.rightlineflag[i] == 1 &&
          f1.rightlineflag[i + 3] == 1)
      {
        f2.L_loop_sm_state = 7;
        break;
      }
    }
#endif
  }

  //????????????????7
  if (f2.L_loop_sm_state == 7)
  {
    //???????????????????????
    for (int i = 110; i > f2.toppoint; i--)
    {
      if (i < 85 && f1.rightlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] - 2;
      }
      else if (i >= 85 && f1.rightlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] - 1;
      }
      else if (f1.rightline[i] > f1.rightline[i + 1] && f1.rightlineflag[i] != 0)
      {
        //?????????????????????????
        f1.midline[i] = f1.midline[i + 1] + 1;
      }
      else
      {
        f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]);
      }

      // if (f1.midline[i] - f1.midline[i + 1] >= 3)
      //   f1.midline[i] = f1.midline[i + 1] + 3;
      // if (f1.midline[i] - f1.midline[i + 1] <= -3)
      //   f1.midline[i] = f1.midline[i + 1] - 3;

      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }

    L_mid_endrow_proc();
    if (is_L_R_nonlose(55, 100, 2, 80))
    {
      //inoutdev.beep(2);
      f2.L_loop_sm_state = 0;
      enable_all_elements();
      goto_next_element();
    }
  }
}

void R_loop_patch()
{
  //??????????1
  if (f2.R_loop_sm_state == 1)
  {
    for (int i = 117; i > f2.toppoint; i--)
    {
      if (f1.leftlineflag[i] == 0)
        f1.midline[i] = f1.midline[i + 1];
      else
        f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]) + 2;
    }

    R_mid_endrow_proc();
    if (is_L_R_lose(90, 118, 1, 95))
    {
      f2.R_loop_sm_state = 2;
      //chassis.loop_distance = 0;
    }
  }

  //??????????2
  if (f2.R_loop_sm_state == 2)
  {
    for (int i = 117; i > f2.toppoint; i--)
    {
      if (f1.leftlineflag[i] == 0)
        f1.midline[i] = f1.midline[i + 1];
      else
        f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]) + 2;
    }

    R_mid_endrow_proc();
    if (
        // is_L_R_nonlose(80, 110, 1, 70) &&
//        chassis.loop_distance > 40 &&
        f1.rightstartrow > 91)
    {
      f2.R_loop_sm_state = 3;
      //chassis.loop_distance = 0;
    }
  }

  //??????????3
  if (f2.R_loop_sm_state == 3)
  {
    for (int i = 110; i > 35; i--)
    {
      if (i < 108 && f1.rightlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] + 2;
      }
      else if (i >= 108 && f1.rightlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] + 1;
      }
      else
      {
        f1.midline[i] = f1.rightline[i] - (int16)(loop_half_width[i]);
      }

      if (abs(f1.midline[i] - f1.midline[i + 1]) > 30)
        f1.midline[i] = f1.midline[i + 1];

      if (f1.midline[i] >= 159)
      {
        f1.midline[i] = 159;
        f2.R_mid_endrow = i;
        break;
      }
    }

    R_mid_endrow_proc();
    if (is_L_R_lose(90, 118, 0, 90))
      f2.R_loop_sm_state = 4;
  }

  //??????????4
  if (f2.R_loop_sm_state == 4)
  {
    for (int i = 110; i > 35; i--)
    {
      if (i < 108 && f1.rightlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] + 2;
      }
      else if (i >= 108 && f1.rightlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] + 1;
      }
      else
      {
        f1.midline[i] = f1.rightline[i] - (int16)(loop_half_width[i]);
      }

      if (f1.midline[i] >= 159)
      {
        f1.midline[i] = 159;
        f2.R_mid_endrow = i;
        break;
      }
    }

    R_mid_endrow_proc();

    if (is_L_R_nonlose(80, 110, 0, 80))
      f2.R_loop_sm_state = 5;
  }

  //????????????????5
  if (f2.R_loop_sm_state == 5)
  {
    for (int i = 110; i > 35; i--)
    {
      f1.midline[i] = f1.leftline[i] + (int16)(loop_half_width[i]);

      if (f1.midline[i] >= 159)
      {
        f1.midline[i] = 159;
        f2.R_mid_endrow = i;
        break;
      }
    }

    R_mid_endrow_proc();

#if USE_GYRO_LOOP
    if (fabs(inoutdev.gyro_z_angle) >= 260)
      f2.R_loop_sm_state = 6;
#else
    find_R_loop_exit();
    if (f2.R_loop_exit_flag)
      f2.R_loop_sm_state = 6;
#endif
  }

  //????????????????6
  if (f2.R_loop_sm_state == 6)
  {
    //????????????????????
    int start_offset = 5;
    start_offset = (int)(f2.toppoint * 0.15);
    __Limit_AB(start_offset, 0, 60);

    int flag = 0;
    if (is_L_R_nonlose(90, 100, 0, 100) &&
        is_L_R_lose(110, 118, 1, 100))
      flag = 1;

    for (int i = 110; i < 120; i++)
    {
      if (flag)
        f1.midline[i] = f1.leftline[i] / 2 + start_offset;
      else
        f1.midline[i] = 80 + start_offset;
    }

    //????????????????????
    for (int i = 100; i > 35; i--)
    {
      if (f1.leftlineflag[i] == 0 || f1.leftline[i] < f1.leftline[i + 1])
      {
        f1.midline[i] = f1.midline[i + 1] + 2;
      }
      else
      {
        f1.midline[i] = f1.midline[i + 1] + (f1.leftline[i + 1] - f1.leftline[i]); //????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
      }

      if (f1.midline[i] >= 159)
      {
        f1.midline[i] = 159;
        f2.R_mid_endrow = i;
        break;
      }
    }

    R_mid_endrow_proc();

#if USE_GYRO_LOOP
    if (fabs(inoutdev.gyro_z_angle) >= 300)
    {
      f2.R_loop_sm_state = 7;
      inoutdev.beep(1);
    }
#else
    find_R_loop_exit();
    for (int i = f2.toppoint; i < 95; i--)
    {
      if (!f1.leftlineflag[i])
        break;

      if (!f2.R_loop_exit_flag &&
          f1.leftline[i] <= f1.leftline[i + 3] + 3 &&
          f1.leftlineflag[i] == 1 &&
          f1.leftlineflag[i + 3] == 1)
      {
        f2.R_loop_sm_state = 7;
        break;
      }
    }

#endif
  }

  //????????????????7
  if (f2.R_loop_sm_state == 7)
  {
    //???????????????????????
    for (int i = 110; i > f2.toppoint; i--)
    {
      if (i < 85 && f1.leftlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] + 2;
      }
      else if (i >= 85 && f1.leftlineflag[i] == 0)
      {
        f1.midline[i] = f1.midline[i + 1] + 1;
      }
      else if (f1.leftline[i] < f1.leftline[i + 1] && f1.leftlineflag[i] != 0)
      {
        //?????????????????????????
        f1.midline[i] = f1.midline[i + 1] - 1;
      }
      else
      {
        f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]);
      }

      if (f1.midline[i] - f1.midline[i + 1] > 10)
        f1.midline[i] = f1.midline[i + 1] + 3;
      if (f1.midline[i] - f1.midline[i + 1] < -10)
        f1.midline[i] = f1.midline[i + 1] - 3;

      if (f1.midline[i] >= 159)
      {
        f1.midline[i] = 159;
        f2.R_mid_endrow = i;
        break;
      }
    }

    R_mid_endrow_proc();
    if (is_L_R_nonlose(45, 95, 2, 80))
    {
      //inoutdev.beep(2);
      f2.R_loop_sm_state = 0;
      enable_all_elements();
      goto_next_element();
    }
  }
}

void L_fork_patch()
{
  f2.L_fork_mid_endrow = f2.toppoint;

  for (int i = 115; i > 10; i--)
  {
    f1.midline[i] = 80;

    f2.fork_leftline[i] = 0;
    f2.fork_leftline_flag[i] = 0;

    f2.fork_rightline_flag[i] = 0;
    f2.fork_rightline[i] = 159;

    for (int j = 0; j < 120; j++)
    {

      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j - 2] == 0)
      {
        f2.fork_leftline[i] = j - 1;
        f2.fork_leftline_flag[i] = 1;
        break;
      }

      if (
          IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j + 1] != 0 &&
          IMG_120_160[i][j + 2] == 0)
      {
        f2.fork_rightline[i] = j + 1;
        f2.fork_rightline_flag[i] = 1;
        break;
      }
    }

    if (f2.fork_leftline_flag[i] == 0 && f2.fork_rightline_flag[i] == 1)
    {
      f1.midline[i] = f2.fork_rightline[i] - half_width[i];
    }
    else if (f2.fork_leftline_flag[i] == 1 && f2.fork_rightline_flag[i] == 0)
    {
      f1.midline[i] = f2.fork_leftline[i] + half_width[i];
    }
    else
    {
      f1.midline[i] = f1.midline[i + 1] - 1;
    }

    if (abs(f1.midline[i] - f1.midline[i + 1]) > 30)
      f1.midline[i] = f1.midline[i + 1] - 1;

    if (f1.midline[i] <= 0)
    {
      f1.midline[i] = 0;
      f2.L_fork_mid_endrow = i;
      break;
    }
  }

  for (int i = f2.L_fork_mid_endrow; i > 0; i--)
  {
    f1.leftline[i] = 0;
    f1.rightline[i] = 0;
    f1.midline[i] = 0;
  }
}

void R_fork_patch()
{
  f2.R_fork_mid_endrow = f2.toppoint;

  for (int i = 115; i > 10; i--)
  {
    f1.midline[i] = 80;

    f2.fork_leftline[i] = 0;
    f2.fork_leftline_flag[i] = 0;

    f2.fork_rightline_flag[i] = 0;
    f2.fork_rightline[i] = 159;

    //?????????????????????????????
    for (int j = 159; j > 40; j--)
    {
      if (
          IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j + 1] != 0 &&
          IMG_120_160[i][j + 2] == 0)
      {
        f2.fork_rightline[i] = j + 1;
        f2.fork_rightline_flag[i] = 1;
        break;
      }

      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j - 2] == 0)
      {
        f2.fork_leftline[i] = j - 1;
        f2.fork_leftline_flag[i] = 1;
        break;
      }
    }

    if (f2.fork_leftline_flag[i] == 1 && f2.fork_rightline_flag[i] == 0)
    {
      f1.midline[i] = f2.fork_leftline[i] + half_width[i];
    }
    else if (f2.fork_leftline_flag[i] == 0 && f2.fork_rightline_flag[i] == 1)
    {
      f1.midline[i] = f2.fork_rightline[i] - half_width[i];
    }
    else
    {
      // f1.midline[i] = (int)(f1.midline[i + 1] + 1.5);
      f1.midline[i] = f1.midline[i + 1] + 1;
    }

    if (abs(f1.midline[i] - f1.midline[i + 1]) > 30)
      f1.midline[i] = f1.midline[i + 1] + 1;
    // f1.midline[i] = (int)(f1.midline[i + 1] + 1.5);

    if (f1.midline[i] >= 159)
    {
      f1.midline[i] = 159;
      f2.R_fork_mid_endrow = i;
      break;
    }
  }

  for (int i = f2.R_fork_mid_endrow; i > 0; i--)
  {
    f1.leftline[i] = 159;
    f1.rightline[i] = 159;
    f1.midline[i] = 159;
  }
}

void cross_patch()
{
  //????????????????????
  if (f2.cross_type == 0)
  {
    f2.cross_K_L = ((float)(f2.L_up_infec_col - f2.L_infec_col)) / (f2.L_infec_row - f2.L_up_infec_row);
    for (int i = f2.L_infec_row; i >= f2.L_up_infec_row; i--)
    {
      f1.leftline[i] = (int)(f2.L_infec_col + f2.cross_K_L * (f2.L_infec_row - i));
      __Limit_AB(f1.leftline[i], 0, 159);
    }

    f2.cross_K_R = ((float)(f2.R_infec_col - f2.R_up_infec_col)) / (f2.R_infec_row - f2.R_up_infec_row);
    for (int i = f2.R_infec_row; i >= f2.R_up_infec_row; i--)
    {
      f1.rightline[i] = (int)(f2.R_infec_col - f2.cross_K_R * (f2.R_infec_row - i));
      __Limit_AB(f1.rightline[i], 0, 159);
    }

    for (int i = MAX(f2.L_infec_row, f2.R_infec_row); i > MIN(f2.L_up_infec_row, f2.R_up_infec_row); i--)
    {
      f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
      __Limit_AB(f1.midline[i], 0, 159);
    }
  }
  //??????????????????????
  else if (f2.cross_type == 1)
  {
    f2.cross_K_L = ((float)(f2.L_up_infec_col - f2.L_infec_col)) / (f2.L_infec_row - f2.L_up_infec_row);
    for (int i = f2.L_infec_row; i >= f2.L_up_infec_row; i--)
    {
      f1.leftline[i] = (int)(f2.L_infec_col + f2.cross_K_L * (f2.L_infec_row - i));
      __Limit_AB(f1.leftline[i], 0, 159);
    }
    for (int i = f2.L_infec_row; i > MIN(f2.L_up_infec_row, f2.R_up_infec_row); i--)
    {
      f1.midline[i] = f1.leftline[i] + half_width[i];
      __Limit_AB(f1.midline[i], 0, 159);
    }
  }
  //???????????????????????
  else if (f2.cross_type == 2)
  {
    f2.cross_K_R = ((float)(f2.R_infec_col - f2.R_up_infec_col)) / (f2.R_infec_row - f2.R_up_infec_row);
    for (int i = f2.R_infec_row; i >= f2.R_up_infec_row; i--)
    {
      f1.rightline[i] = (int)(f2.R_infec_col - f2.cross_K_R * (f2.R_infec_row - i));
      __Limit_AB(f1.rightline[i], 0, 159);
    }
    for (int i = f2.R_infec_row; i > MIN(f2.L_up_infec_row, f2.R_up_infec_row); i--)
    {
      f1.midline[i] = f1.rightline[i] - half_width[i];
      __Limit_AB(f1.midline[i], 0, 159);
    }
  }
}

void curve_patch()
{
  //??????????????
  if (f2.leftturnflag == 1)
  {
    for (int i = f2.leftturn_row; i > f2.toppoint; i--)
    {
      if (f1.rightline[i] == 159)
      {
        f1.midline[i] = f1.midline[i + 1] - 1;
      }
      else
      {
        f1.midline[i] = f1.midline[i + 1] - (f1.rightline[i + 1] - f1.rightline[i] + 1);
      }
      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }
    L_mid_endrow_proc();
  }

  //??????????????????
  if (f2.leftturnflag == 3)
  {
    for (int i = 118; i > 0; i--)
    {
      //TODO
      // f1.midline[i] = f1.rightline[i] - loop_half_width[i] - 1;
      f1.midline[i] = f1.rightline[i] - 75;
      // __Limit_AB(f1.midline[i], 0, 159);
      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }
    L_mid_endrow_proc();
  }

  //??????????????
  if (f2.rightturnflag == 1)
  {
    for (int i = f2.rightturn_row; i > 0; i--)
    {
      if (f1.leftline[i] == 0)
      {
        f1.midline[i] = f1.midline[i] + 1;
      }
      else
      {
        f1.midline[i] = f1.midline[i + 1] + (f1.leftline[i] - f1.leftline[i + 1] + 1);
      }

      if (f1.midline[i] >= 159)
      {
        f1.midline[i] = 159;
        f2.R_mid_endrow = i;
        break;
      }
    }
    R_mid_endrow_proc();
  }

  //??????????????????
  if (f2.rightturnflag == 3)
  {
    for (int i = 118; i > 0; i--)
    {
      //TODO
      // f1.midline[i] = f1.leftline[i] + loop_half_width[i] + 1;
      f1.midline[i] = f1.leftline[i] + 75;
      // __Limit_AB(f1.midline[i], 0, 159);
      if (f1.midline[i] >= 159)
      {
        f1.midline[i] = 159;
        f2.R_mid_endrow = i;
        break;
      }
    }
    R_mid_endrow_proc();
  }
}

void start_patch()
{
  f1.midline[119] = 80;

  //??????????????
  if (f2.first_start_dir == 1)
  {
    for (int i = 118; i > 10; i--)
    {
      f1.leftline[i] = 0;
      f1.leftlineflag[i] = 0;
      f1.midline[i] = 80;
      f1.rightline[i] = 159;
      f1.rightlineflag[i] = 0;
      for (int j = 159; j > 0; j--)
      {
        if (IMG_120_160[i][j] != 0 &&
            IMG_120_160[i][j - 1] == 0)
        {
          break;
        }

        if (IMG_120_160[i][j] == 0 &&
            IMG_120_160[i][j - 1] != 0)
        {
          f1.rightline[i] = j - 1;
          f1.rightlineflag[i] = 1;
          break;
        }
      }

      if (i >= 110)
        f1.midline[i] = f1.midline[i + 1];
      else if (!f1.rightlineflag[i])
        f1.midline[i] = f1.midline[i + 1] + 2;
      else
        f1.midline[i] = (f1.rightline[i] - half_width[i]);

      if (abs(f1.midline[i] - f1.midline[i + 1]) > 10)
        f1.midline[i] = f1.midline[i + 1] + 3;

      if (f1.midline[i] >= 159)
      {
        f1.midline[i] = 159;
        f2.R_mid_endrow = i;
        break;
      }
    }

    if (f2.R_mid_endrow == 0)
      f2.toppoint = 20;
    else
      f2.toppoint = f2.R_mid_endrow;

    R_mid_endrow_proc();
  }

  //???????????????
  if (f2.first_start_dir == 0)
  {
    for (int i = 118; i > 10; i--)
    {
      f1.leftline[i] = 0;
      f1.leftlineflag[i] = 0;
      f1.midline[i] = 80;
      f1.rightline[i] = 159;
      f1.rightlineflag[i] = 0;
      for (int j = 0; j < 159; j++)
      {
        if (IMG_120_160[i][j] == 0 &&
            IMG_120_160[i][j - 1] != 0)
        {
          break;
        }

        if (IMG_120_160[i][j] != 0 &&
            IMG_120_160[i][j - 1] == 0)
        {
          f1.leftline[i] = j - 1;
          f1.leftlineflag[i] = 1;
          break;
        }
      }

      if (i >= 110)
        f1.midline[i] = f1.midline[i + 1];
      else if (!f1.leftlineflag[i])
        f1.midline[i] = f1.midline[i + 1] - 2;
      else
        f1.midline[i] = (f1.leftline[i] + half_width[i]);

      if (abs(f1.midline[i] - f1.midline[i + 1]) > 10)
        f1.midline[i] = f1.midline[i + 1] - 3;

      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }

    if (f2.L_mid_endrow == 0)
      f2.toppoint = 20;
    else
      f2.toppoint = f2.L_mid_endrow;

    L_mid_endrow_proc();
  }
}

void garage_patch()
{
  f1.midline[119] = 80;

  //?????????????????
  if (f2.garage_dir == 1)
  {
    for (int i = 118; i > 10; i--)
    {
      f1.leftline[i] = 0;
      f1.leftlineflag[i] = 0;
      f1.midline[i] = 80;
      f1.rightline[i] = 159;
      f1.rightlineflag[i] = 0;
      for (int j = 159; j > 0; j--)
      {
        if (IMG_120_160[i][j] != 0 &&
            IMG_120_160[i][j - 1] == 0)
        {
          break;
        }

        if (IMG_120_160[i][j] == 0 &&
            IMG_120_160[i][j - 1] != 0)
        {
          f1.rightline[i] = j - 1;
          f1.rightlineflag[i] = 1;
          break;
        }
      }

      if (!f1.rightlineflag[i])
        f1.midline[i] = f1.midline[i + 1] + 3;
      else
        f1.midline[i] = (f1.rightline[i] - half_width[i]);

      if (abs(f1.midline[i] - f1.midline[i + 1]) > 10)
        f1.midline[i] = f1.midline[i + 1] + 3;

      if (f1.midline[i] >= 159)
      {
        f1.midline[i] = 159;
        f2.R_mid_endrow = i;
        break;
      }
    }

    if (f2.R_mid_endrow == 0)
      f2.toppoint = 20;
    else
      f2.toppoint = f2.R_mid_endrow;

    R_mid_endrow_proc();
  }

  //??????????????????
  if (f2.garage_dir == 0)
  {
    for (int i = 118; i > 10; i--)
    {
      f1.leftline[i] = 0;
      f1.leftlineflag[i] = 0;
      f1.midline[i] = 80;
      f1.rightline[i] = 159;
      f1.rightlineflag[i] = 0;
      for (int j = 0; j < 159; j++)
      {
        if (IMG_120_160[i][j] == 0 &&
            IMG_120_160[i][j - 1] != 0)
        {
          break;
        }

        if (IMG_120_160[i][j] != 0 &&
            IMG_120_160[i][j - 1] == 0)
        {
          f1.leftline[i] = j - 1;
          f1.leftlineflag[i] = 1;
          break;
        }
      }

      if (!f1.leftlineflag[i])
        f1.midline[i] = f1.midline[i + 1] - 3;
      else
        f1.midline[i] = (f1.leftline[i] + half_width[i]);

      if (abs(f1.midline[i] - f1.midline[i + 1]) > 10)
        f1.midline[i] = f1.midline[i + 1] - 3;

      if (f1.midline[i] <= 0)
      {
        f1.midline[i] = 0;
        f2.L_mid_endrow = i;
        break;
      }
    }

    if (f2.L_mid_endrow == 0)
      f2.toppoint = 20;
    else
      f2.toppoint = f2.L_mid_endrow;

    L_mid_endrow_proc();
  }
}

void qrcode_patch()
{
  for (int i = 118; i > 10; i--)
  {
    f1.leftline[i] = 0;
    f1.leftlineflag[i] = 0;
    f1.midline[i] = 80;
    f1.rightline[i] = 159;
    f1.rightlineflag[i] = 0;
    for (int j = 0; j < 159; j++)
    {
      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j - 2] == 0)
      {
        f1.leftline[i] = j - 1;
        f1.leftlineflag[i] = 1;
        break;
      }
    }
    for (int j = 159; j > 0; j--)
    {
      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j + 1] != 0 &&
          IMG_120_160[i][j + 2] == 0)
      {
        f1.rightline[i] = j + 1;
        f1.rightlineflag[i] = 1;
        break;
      }
    }

    f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
  }
  f2.toppoint = 40;
}

void find_edge()
{
    int start_row=115;
    f1.leftline[start_row] = 0;
    f1.leftlineflag[start_row] = 0;
    f1.midline[start_row] = 80;
    f1.rightline[start_row] = 159;
    f1.rightlineflag[start_row] = 0;
    for (int j = 80; j > 0; j--)
    {
        if (IMG_120_160[start_row][j] != 0 &&
            IMG_120_160[start_row][j - 1] == 0 )
        {
            f1.leftline[start_row] = j - 1;
            f1.leftlineflag[start_row] = 1;
            break;
        }
    }
    for (int j = 80; j < 159; j++)
    {
        if (IMG_120_160[start_row][j] != 0 &&
            IMG_120_160[start_row][j + 1] == 0)
        {
            f1.rightline[start_row] = j + 1;
            f1.rightlineflag[start_row] = 1;
            break;
        }
    }

    for (int i = start_row-1; i > 10; i--)
    {
        int k1=f1.leftline[i+1]+4;
        int k2=f1.leftline[i+1]-4;
        __Limit_AB(k1,0,159);
        __Limit_AB(k2,0,159);
        f1.leftline[i] = 0;
        f1.leftlineflag[i] = 0;
        for (int j = k1; j >k2; j--)
        {
            if (IMG_120_160[i][j] != 0 &&
                IMG_120_160[i][j - 1] == 0)
            {
                f1.leftline[i] = j - 1;
                f1.leftlineflag[i] = 1;
                break;
            }
        }

        int k3=f1.rightline[i+1]+4;
        int k4=f1.rightline[i+1]-4;
        __Limit_AB(k3,0,159);
        __Limit_AB(k4,0,159);
        f1.rightline[i] = 159;
        f1.rightlineflag[i] = 0;
        for (int j = k4; j <k3; j++)
        {
            if (IMG_120_160[i][j] != 0 &&
                IMG_120_160[i][j + 1] == 0)
            {
                f1.rightline[i] = j + 1;
                f1.rightlineflag[i] = 1;
                break;
            }
        }

        f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;

    }
}

void find_track()
{
  int16 i = 0, j = 0, k = 0;
  f1.leftstartrow = 0;
  f1.rightstartrow = 0;

  for (i = 118; i > 10; i--)
  {
    k = f2.column_start;
    f1.leftline[i] = 0;
    f1.leftlineflag[i] = 0;
    f1.midline[i] = 80;
    f1.rightline[i] = 159;
    f1.rightlineflag[i] = 0;

    for (j = k; j > 2; j--)
    {
      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j - 1] != 0 &&
          IMG_120_160[i][j - 2] == 0)
      { //?????????????????????????????
        if (f1.leftstartrow == 0)
          f1.leftstartrow = i;
        f1.leftline[i] = j - 1;
        f1.leftlineflag[i] = 1;
        break;
      }
    }
    for (j = k; j < 157; j++)
    {
      if (IMG_120_160[i][j] != 0 &&
          IMG_120_160[i][j + 1] != 0 &&
          IMG_120_160[i][j + 2] == 0)
      { //?????????????????????????????
        if (f1.rightstartrow == 0)
          f1.rightstartrow = i;
        f1.rightline[i] = j + 1;
        f1.rightlineflag[i] = 1;
        break;
      }
    }

    if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
    {
      f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
    }
    else if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 1)
    {
      f1.leftline[i] = 0;
      // f1.midline[i] = f1.rightline[i] - half_width[i];
      f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
      if (i < 99 &&
          f1.rightline[i] <= f1.rightline[i + 1] &&
          f1.leftlineflag[i + 15] == 0 &&
          f1.rightlineflag[i + 15] == 0)
      {
        f1.midline[i] = f1.midline[i + 1];
      }
    }
    else if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 0)
    {
      f1.rightline[i] = 159;
      // f1.midline[i] = f1.leftline[i] + half_width[i];
      f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
      if (i < 99 &&
          f1.leftline[i] >= f1.leftline[i + 1] &&
          f1.leftlineflag[i + 15] == 0 &&
          f1.rightlineflag[i + 15] == 0)
      {
        f1.midline[i] = f1.midline[i + 1];
      }
    }
    else if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 0)
    {
      f1.leftline[i] = 0;
      f1.rightline[i] = 159;
      if (i == 118)
        f1.midline[i] = 80;
      else
        f1.midline[i] = f1.midline[i + 1];
    }
    __Limit_AB(f1.midline[i], 0, 159);
    f2.column_start = f1.midline[i];
  }

  f2.column_start = f1.midline[118];
  find_toppoint();
  f2.toppoint_reg = f2.toppoint;
}

void find_elements()
{
  if (f1.reset_ele_signal)
  {
    f1.reset_ele_signal = 0;
    reset_all_elements();
    enable_all_elements();
  }

  f2.L_mid_endrow = 0;
  f2.R_mid_endrow = 0;

  //?????????????????????????????????????????????????


#ifdef ENABLE_RAMP
  if (f1.ramp_enable )
  {
    //????0,????????????????
    if (f2.ramp_sm_state == 0)
    {
      ramp_judge();

      if (f2.ramp_flag)
      {
//        inoutdev.long_beep(1);
        disable_all_elements();
        f1.ramp_enable = 1;
        f2.ramp_sm_state = 1;
//        chassis.ramp_distance = 0;
//        inoutdev.gyro_y_angle = 0;
      }
    }

    //????1,????????????????????????
    if (f2.ramp_sm_state == 1)
    {
      if (1
          )
      {
        f2.ramp_sm_state = 2;
        //chassis.ramp_distance = 0;
      }
    }

    //????2,??????????????
    if (f2.ramp_sm_state == 2)
    {
      if (1)
      {
        f2.ramp_sm_state = 3;
//        chassis.ramp_distance = 0;
      }
    }

    //????3,????????????????????????
    if (f2.ramp_sm_state == 3)
    {
      if (1)
      {
//        inoutdev.long_beep(1);
        f2.ramp_sm_state = 0;
//        chassis.ramp_distance = 0;
//        inoutdev.gyro_y_angle = 0;
        enable_all_elements();
        goto_next_element();
      }
    }
  }
#endif

#ifdef ENABLE_START
  if (f1.start_enable && f2.en_start)
  {
    //????0,??????????????
    if (f2.start_sm_state == 0)
    {
//      inoutdev.beep(1);
//      chassis.start_distance = 0;
      f2.start_sm_state = 1;
      disable_all_elements();
//      inoutdev.start_angle = 0;
    }

    //????1,????????????
    if (f2.start_sm_state == 1)
    {
      if (1)
      {
        f2.start_sm_state = 2;
      }
      start_patch();
      return;
    }

    //????2,????????
    if (f2.start_sm_state == 2)
    {
//      inoutdev.beep(1);
//      chassis.start_distance = 0;
      f2.start_sm_state = 0;
      f2.en_start = 0;
      enable_all_elements();
//      inoutdev.start_angle = 0;
      goto_next_element();
    }
  }
#endif

#ifdef ENABLE_QRCODE
  if (f1.tag_enable)
  {
    //????0,??????????????????
    if (f2.tag_sm_state == 0)
    {
      qrcode_judge();
      if (f2.tag_flag)
      {
//        inoutdev.long_beep(1);
        f2.tag_sm_state = 1;
        disable_all_elements();
        f1.tag_enable = 1;
      }
    }

    //????1,????????????????????
    if (f2.tag_sm_state == 1)
    {
      qrcode_patch();
      return;
    }

    //????2,????????????????????????????
    if (f2.tag_sm_state == 2)
    {
      f2.tag_sm_state = 0;

//      inoutdev.long_beep(1);
      enable_all_elements();
      goto_next_element();
    }
  }
#endif

#ifdef ENABLE_GARAGE
  if (f1.garage_enable)
  {
    //????0,???????????????????????????
    if (f2.garage_sm_state == 0)
    {
      garage_judge();

      if (f2.garage_flag &&
          f2.garage_frames >= 1)
      {
//        inoutdev.beep(1);
        f2.garage_sm_state = 1;
//        chassis.start_distance = 0;
        disable_all_elements();
        f1.garage_enable = 1;
//        inoutdev.start_angle = 0;
      }
    }

    //????1,??????????????????
    if (f2.garage_sm_state == 1)
    {
      if (1
//              inoutdev.start_angle > 90
//      ||
//          chassis.start_distance > 80
          )
      {
//        inoutdev.beep(1);
//        chassis.start_distance = 0;
        f2.garage_sm_state = 2;
//        inoutdev.start_angle = 0;
      }
      garage_patch();
      return;
    }

    //????2,????????????????????
    if (f2.garage_sm_state == 2)
    {
      // loop here
    }
  }
#endif

#ifdef ENABLE_CROSS
  if (f1.cross_enable)
  {
    //????????????????
    if (f2.cross_sm_state == 0)
    {
      cross_judge();
      f2.cross_K_L = 0;
      f2.cross_K_R = 0;

      if (f2.cross_flag == 1 &&
          f2.cross_frames >= 1)
      {
        f2.cross_sm_state = 1;
        f2.cross_flag = 1;
        disable_all_elements();
        f1.cross_enable = 1;
      }
    }

    //????????????????
    if (f2.cross_sm_state == 1)
    {
      cross_patch();
      f2.cross_sm_state = 0;
      f2.cross_flag = 1;
      enable_all_elements();
      find_post_proc();
      return;
    }
  }
#endif

#ifdef ENABLE_FORK
  if (f1.fork_enable)
  {
    //????0,?????????????????????????????
    if (f2.fork_sm_state == 0)
    {
      fork_road_judge();
      if (f2.fork_flag &&
          f2.fork_frames >= 1)
      {
//        inoutdev.beep(1);
        f2.fork_sm_state = 1;
        disable_all_elements();
        f1.fork_enable = 1;

        if (f2.sch_sm_state != 1)
          f2.fork_num++;
      }
    }

    //????1,????????????????????????????
    if (f2.fork_sm_state == 1 ||
        f2.fork_sm_state == 2)
    {
      //????????
      if (f2.fork_dir == 1)
      {
        R_fork_patch();

        //????????????
        if (f2.fork_sm_state == 1)
          if (is_L_R_lose(90, 115, 0, 90))
            f2.fork_sm_state = 2;

        //????????????
        if (f2.fork_sm_state == 2)
          if (is_L_R_nonlose(55, 95, 2, 60))
          {
//            inoutdev.beep(1);
            f2.fork_sm_state = 0;
            enable_all_elements();

            if (f2.sch_sm_state != 1)
              goto_next_element();
          }
      }

      //????????
      if (f2.fork_dir == 0)
      {
        L_fork_patch();

        //????????????
        if (f2.fork_sm_state == 1)
          if (is_L_R_lose(90, 118, 1, 90))
            f2.fork_sm_state = 2;

        //????????????
        if (f2.fork_sm_state == 2)
          if (is_L_R_nonlose(55, 95, 2, 60))
          {
//            inoutdev.beep(1);
            f2.fork_sm_state = 0;
            enable_all_elements();

            if (f2.sch_sm_state != 1)
              goto_next_element();
          }
      }
    }
  }
#endif

#ifdef ENABLE_LOOP
  if (f1.loop_enable)
  {
    //????????????????????
    if (f2.L_loop_sm_state == 0 && f2.R_loop_sm_state == 0)
    {
      L_loop_judge();
      if (f2.L_loop_flag == 1)
      {
//        inoutdev.beep(2);
        f2.L_loop_sm_state = 1;
        disable_all_elements();
        f1.loop_enable = 1;
//        inoutdev.gyro_z_angle = 0;
//        chassis.loop_distance = 0;
      }
    }

    // if (f2.L_loop_sm_state == 0 && f2.R_loop_sm_state == 0)
    // {
    //   R_loop_judge();
    //   if (f2.R_loop_flag == 1)
    //   {
    //     inoutdev.beep(2);
    //     f2.R_loop_sm_state = 1;
    //     disable_all_elements();
    //     f1.loop_enable = 1;
    //     inoutdev.gyro_z_angle = 0;
    //     chassis.loop_distance = 0;
    //   }
    // }

    if (f2.L_loop_sm_state != 0)
    {
      L_loop_patch();
      find_post_proc();
      return;
    }

    if (f2.R_loop_sm_state != 0)
    {
      R_loop_patch();
      find_post_proc();
      return;
    }
  }
#endif

#ifdef ENABLE_CURVE
  if (f1.curve_enable)
  {
    f2.leftturnflag = 0;
    f2.leftturn_row = 0;
    f2.rightturnflag = 0;
    f2.rightturn_row = 0;

    find_little_curve();
    find_large_curve();

    if (f2.leftturnflag != 0 ||
        f2.rightturnflag != 0)
    {
      curve_patch();
      find_post_proc();
      return;
    }
  }
#endif
}

//?????????????????????????????????????err<scale????????????????????????1
//start_index<end_index
//scale: 1~4
int is_straight_line(int start_index, int end_index, int16 line_[IMG_H], int scale)
{
  float k;
  float error;

  if (start_index == end_index)
  {
    return 1;
  }

  k = (float)(line_[start_index] - line_[end_index]) / (start_index - end_index); //????????
  for (int i = (start_index < end_index ? start_index : end_index);
       i <= (start_index < end_index ? end_index : start_index); i++)
  {
    error = line_[i] - (k * (i - start_index) + line_[start_index]);
    if (fabs(error) >= scale)
      return 0;
  }

  return 1;
}

//??????????????????????????????????????????????????????????????????????????
//color 0:black;255:white
//(x1,y1) to (x2,y2)
//return: 0-100
int cal_region_color_duty(int x1, int y1, int x2, int y2, int color)
{
  int total_cnt = 0;
  int white_cnt = 0;
  for (int i = x1; i <= x2; i++)
  {
    for (int j = y1; j <= y2; j++)
    {
      total_cnt++;
      if (IMG_120_160[j][i] == color)
        white_cnt++;
    }
  }
  int per = white_cnt * 100 / total_cnt;
  return per;
}

//??????????????????????????????????????????????????????????????????????????????threshold??????????1
//color 0:black;255:white
//(x1,y1) to (x2,y2)
//threshold: 0-100
int is_region_one_color(int x1, int y1, int x2, int y2, int color, int threshold)
{
  int total_cnt = 0;
  int white_cnt = 0;
  for (int i = x1; i <= x2; i++)
  {
    for (int j = y1; j <= y2; j++)
    {
      total_cnt++;
      if (IMG_120_160[j][i] == color)
        white_cnt++;
    }
  }
  int per = white_cnt * 100 / total_cnt;
  if (per > threshold)
    return 1;
  else
    return 0;
}

//??????????????????????????????
//mode 0:left,1:right,2:both
//threshold: 0-100
//is_L_R_lose(40,120,2);
//i=40,i=120-1
int is_L_R_lose(int start, int end, int mode, int threshold)
{
  int lost_cnt = 0;
  int total_cnt = 0;

  for (int i = start; i < end; i++)
  {
    total_cnt++;

    if (mode == 0)
    {
      if (f1.leftlineflag[i] == 0)
      {
        lost_cnt++;
      }
    }
    else if (mode == 1)
    {
      if (f1.rightlineflag[i] == 0)
      {
        lost_cnt++;
      }
    }
    else
    {
      if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 0)
      {
        lost_cnt++;
      }
    }
  }
  if (lost_cnt * 100 / total_cnt >= threshold)
    return 1;
  else
    return 0;
}

//??????????????????????????????
//mode 0:left,1:right,2:both
//threshold: 0-100
//is_L_R_nonlose(40,120,2);
//i=40,i=120-1
int is_L_R_nonlose(int start, int end, int mode, int threshold)
{
  int nonlost_cnt = 0;
  int total_cnt = 0;

  for (int i = start; i < end; i++)
  {
    total_cnt++;

    if (mode == 0)
    {
      if (f1.leftlineflag[i] == 1)
      {
        nonlost_cnt++;
      }
    }
    else if (mode == 1)
    {
      if (f1.rightlineflag[i] == 1)
      {
        nonlost_cnt++;
      }
    }
    else
    {
      if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
      {
        nonlost_cnt++;
      }
    }
  }
  if (nonlost_cnt * 100 / total_cnt >= threshold)
    return 1;
  else
    return 0;
}

int is_M_center(int start, int end, int half_err)
{
  for (int i = start; i > end; i--)
  {
    if (f1.midline[i] > 80 + half_err ||
        f1.midline[i] < 80 - half_err)
    {
      return 0;
    }
  }
  return 1;
}

float solve_variance(int begin_row, int end_row)
{
  if (begin_row == end_row)
    return 0;

  if (begin_row < end_row)
  {
    int tmp1 = begin_row;
    begin_row = end_row;
    end_row = tmp1;
  }

  int center_sum = 0;
  int count_center = 0;
  int center_ave = 0;
  int quadratic_sum = 0;

  for (int i = begin_row; i > end_row; i--)
  {
    center_sum += f1.midline[i];
    count_center++;
  }

  center_ave = (int)((float)center_sum / (float)count_center); //?????????????????????????

  for (int i = begin_row; i > end_row; i--)
  {
    quadratic_sum += (f1.midline[i] - center_ave) * (f1.midline[i] - center_ave); //??????????????????????????????????????????????
  }

  float variance = (float)quadratic_sum / (float)count_center;
  return (variance);
}

float solve_slope(int begin, int end, int16 *p)
{
  if (begin == end)
    return 0;

  if (begin > end)
  {
    int tmp1 = end;
    end = begin;
    begin = tmp1;
  }

  float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
  uint8 i = 0;
  float result = 0;
  static float resultlast;
  p = p + begin;
  for (i = begin; i < end; i++)
  {
    xsum += i;
    ysum += ((float)(*p));
    xysum += i * ((float)(*p));
    x2sum += i * i;
    p = p + 1;
  }
  if ((end - begin) * x2sum - xsum * xsum)
  {
    result = (float)((end - begin) * xysum - xsum * ysum) / (float)((end - begin) * x2sum - xsum * xsum);
    resultlast = result;
  }
  else
  {
    result = resultlast;
  }
  return result;
}
