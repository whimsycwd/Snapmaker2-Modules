#ifndef  _Quaternion_H_
#define _Quaternion_H_

#define PI  3.14159265359f
#define PIOVER180  0.0174532925f//常量
#define Rad2Deg  (360.0f / (3.14159265359f * 2))
typedef struct Quaternion
{
	float w;
	float x;
	float y;
	float z;
}Quaternion;


//创建四元数，调用处要用变量接收一下
Quaternion QuaterCreat(float w, float x, float y, float z);
//功能：将欧拉角转成四元数
//Roll-x      Pitch - y    Yaw - z
Quaternion QuaterFromEuler(float x_roll, float y_pitch, float z_yaw);
//加法
Quaternion QuaterAdd(Quaternion q1, Quaternion q2);
//减法
Quaternion QuaterSub(Quaternion q1, Quaternion q2);
//乘法，q1*q2
Quaternion QuaterMul(Quaternion q1,Quaternion q2);
/* 四元数的点乘
* 点乘结果是标量，两个单位四元数点乘的结果区间为[-1,1]。
* 点乘结果的绝对值越大，相乘的两个四元数代表的角位移越相似。
*/
float QuaterDot(Quaternion q1, Quaternion q2);
float QuaterAngle(Quaternion q1, Quaternion q2);
//四元数除以一个浮点数
Quaternion QuaterDivNum(Quaternion q, float num);

//范数的平方
float QuaterNormPow(Quaternion q);
//范数 
float QuaterNorm(Quaternion q);
//规范化
Quaternion QuaterNormalize(Quaternion q);
/*共轭
* 四元数的共轭就是让四元数的向量部分取负
* 四元数和它的共轭代表相反的角位移，因为相当于旋转轴反向。*/
Quaternion QuaterConjugate(Quaternion q);
/* 四元数的逆
* 四元数的逆定义为四元数的共轭除以它的模
* 一般使用单位四元数，此时它的逆和共轭其实是相等的。*/
Quaternion QuaterInverse(Quaternion q);

/**求两个四元数的差
* *利用四元数的乘法和逆，可以计算两个四元数的“差”，
* 四元数“差”表示一个方位到另一个方位的角位移。
* 注意不能除以四元数，只能乘以它们的逆来达到目的。*/

Quaternion QuaterDiff(Quaternion q1, Quaternion q2);
/**
功能：将四元数的某个轴旋转一个角度
参数：q要旋转的四元数
	  axis 旋转的轴  值为 'x','y','z'
	  angle 旋转的角度
返回值：返回旋转后的四元数
*/
Quaternion QuaterAxisRotation(Quaternion q,char axis,float angle);
//功能：将四元数转换欧拉角  
//euler 是float[3] 数组，顺序是0-x,1-y,2-z
//Roll-x      Pitch - y    Yaw - z
void QuaterToEuler(Quaternion quate,float *euler);
//四元数转360度的欧拉角
//euler 是float[3] 数组，顺序是0-x,1-y,2-z
void QuaterToEuler360deg(Quaternion quate,float *euler);
//四元数插值算法
//qa：起始位置 
//qb：结束位置  
//t : 插入的位置 0<= t <= 1
//返回值：插入位置的四元数
Quaternion QuaterSlerp(Quaternion qa, Quaternion qb, float t);
////本文件使用举例测试
//void QuaterTest()
//{
//	float x_roll, y_pitch, z_yaw;
//	Quaternion quaterA = QuaterCreat(0.012294f, 0.954789f, 0.296638f, 0.015280f);//定义四元数变量并赋值
//	Quaternion quaterB = QuaterCreat(-0.106354f, 0.729195f, 0.097532f, 0.668917f);//定义四元数变量并赋值
//	Quaternion QuaterX = QuaterFromEuler(90, 0, 0);      //从欧拉角获取四元数
//	Quaternion QuaterY = QuaterFromEuler(0, 90, 0);
//	Quaternion QuaterZ = QuaterFromEuler(0, 0, 90);
//	Quaternion quater_add = QuaterAdd(quaterA, quaterB);           //两个四元数相加
//	Quaternion Quater_sub = QuaterSub(quaterA, quaterB);           //两个四元数相减
//	Quaternion Quater_mul = QuaterMul(quaterA, quaterB);           //两个四元数相乘
//	float        Quater_dot = QuaterDot(quaterA, quaterB);           //两个四元数点乘
//	float        Quater_angle = QuaterAngle(quaterA, quaterB);       //两个四元数的角度
//	Quaternion Quater_div_num = QuaterDivNum(quaterA, 0.5);        //四元数除以一个数
//	float        Quater_norm_pow = QuaterNormPow(quaterA);           //四元数的范数平方
//	float        Quater_norm = QuaterNorm(quaterA);                  //四元数的范数
//	Quaternion Quater_conjugate = QuaterConjugate(quaterA);        //四元数的共轭
//	Quaternion Quater_inverse = QuaterInverse(quaterA);            //四元数的
//	Quaternion Quater_diff = QuaterDiff(quaterA, quaterB);         //两个四元数的空间角度差
//	Quaternion Quater_slerp = QuaterSlerp(quaterA, quaterB, 0.2f);  //A转到距离B的1/5处的四元数
//	Quaternion Quater_rotation = QuaterAxisRotation(quaterA, 'x', 90);//x轴旋转90度
//}

#endif // ! _Quaternion_
