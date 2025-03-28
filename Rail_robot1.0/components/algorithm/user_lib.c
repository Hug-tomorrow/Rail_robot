#include "user_lib.h"

/**
  * @brief          计算一个数的绝对值
  * @param[in]      value: 输入的整数值
  * @retval         返回绝对值
  */
int abs(int value) {
    if (value > 0) {
        return value;
    } else {
        return -value;
    }
}

/**
  * @brief          将一个值从输入范围映射到输出范围
  * @param[in]      value: 输入值
  * @param[in]      in_min: 输入范围的最小值
  * @param[in]      in_max: 输入范围的最大值
  * @param[in]      out_min: 输出范围的最小值
  * @param[in]      out_max: 输出范围的最大值
  * @retval         映射后的值
  */
// 映射函数
int map(int value, int in_min, int in_max, int out_min, int out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * 限幅函数
 * @param value 输入值
 * @param min   最小值
 * @param max   最大值
 * @return      限制后的值
 */
int16_t saturate(int16_t value, int16_t min, int16_t max) {
    if (value < min) {
        return min;  // 如果小于最小值，返回最小值
    } else if (value > max) {
        return max;  // 如果大于最大值，返回最大值
    } else {
        return value;  // 否则返回原始值
    }
}





