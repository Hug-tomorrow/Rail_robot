#include "user_lib.h"

/**
  * @brief          ����һ�����ľ���ֵ
  * @param[in]      value: ���������ֵ
  * @retval         ���ؾ���ֵ
  */
int abs(int value) {
    if (value > 0) {
        return value;
    } else {
        return -value;
    }
}

/**
  * @brief          ��һ��ֵ�����뷶Χӳ�䵽�����Χ
  * @param[in]      value: ����ֵ
  * @param[in]      in_min: ���뷶Χ����Сֵ
  * @param[in]      in_max: ���뷶Χ�����ֵ
  * @param[in]      out_min: �����Χ����Сֵ
  * @param[in]      out_max: �����Χ�����ֵ
  * @retval         ӳ����ֵ
  */
// ӳ�亯��
int map(int value, int in_min, int in_max, int out_min, int out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * �޷�����
 * @param value ����ֵ
 * @param min   ��Сֵ
 * @param max   ���ֵ
 * @return      ���ƺ��ֵ
 */
int16_t saturate(int16_t value, int16_t min, int16_t max) {
    if (value < min) {
        return min;  // ���С����Сֵ��������Сֵ
    } else if (value > max) {
        return max;  // ����������ֵ���������ֵ
    } else {
        return value;  // ���򷵻�ԭʼֵ
    }
}





