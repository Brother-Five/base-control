#ifndef _Kalman_H_
#define _Kalman_H_
#ifdef __cplusplus
 extern "C" {
#endif
//1维的卡尔曼滤波
typedef struct {
    float x;  // 系统的状态量
    float A;  // x(n)=A*x(n-1)+u(n),u(n)~N(0,q)
    float H;  // z(n)=H*x(n)+w(n),w(n)~N(0,r)
    float q;  // 预测过程噪声协方差
    float r;  // 测量过程噪声协方差
    float p;  // 估计误差协方差
    float gain;//卡尔曼增益
}kalman_struct;
void kalman_init(kalman_struct *kalman_lcw, float init_x, float init_p,float predict_q,float measure_q);
float kalman_filter(kalman_struct *kalman_lcw, float measure);

#ifdef __cplusplus
}
#endif

#endif
