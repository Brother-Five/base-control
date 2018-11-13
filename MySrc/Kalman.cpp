#include "Kalman.h"

/**
 *kalman_init - 卡尔曼滤波器初始化
 *@kalman_lcw：卡尔曼滤波器结构体
 *@init_x：待测量的初始值
 *@init_p：后验状态估计值误差的方差的初始值
 */
void kalman_init(kalman_struct *kalman_lcw, float init_x, float init_p,float predict_q,float measure_q)
{
    kalman_lcw->x = init_x;//待测量的初始值，如有中值一般设成中值
    kalman_lcw->p = init_p;//后验状态估计值误差的方差的初始值
    kalman_lcw->A = 1;
    kalman_lcw->H = 1;
    kalman_lcw->q = predict_q;//预测（过程）噪声方差 实验发现修改这个值会影响收敛速率
    kalman_lcw->r = measure_q;//测量（观测）噪声方差
    //这里两个参数是最关键的
}

/**
 *kalman_filter - 卡尔曼滤波器
 *@kalman_lcw:卡尔曼结构体
 *@measure；测量值
 *返回滤波后的值
 */
float kalman_filter(kalman_struct *kalman_lcw, float measure)
{
    /* Predict */
    kalman_lcw->x = kalman_lcw->A * kalman_lcw->x;//%x的先验估计由上一个时间点的后验估计值和输入信息给出
    kalman_lcw->p = kalman_lcw->A * kalman_lcw->A * kalman_lcw->p + kalman_lcw->q;  /*计算先验均方差 p(n|n-1)=A^2*p(n-1|n-1)+q */

    /* Measurement */
    kalman_lcw->gain = kalman_lcw->p * kalman_lcw->H / (kalman_lcw->p * kalman_lcw->H * kalman_lcw->H + kalman_lcw->r);
    kalman_lcw->x = kalman_lcw->x + kalman_lcw->gain * (measure - kalman_lcw->H * kalman_lcw->x);//利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出
    kalman_lcw->p = (1 - kalman_lcw->gain * kalman_lcw->H) * kalman_lcw->p;//%计算后验均方差

    return kalman_lcw->x;
}
