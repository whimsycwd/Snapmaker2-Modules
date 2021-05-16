#include <stdio.h>
//#include "stm32f10x.h"
#include "Quaternion.h"
#include <math.h>


//求根号
float QuaterSqrt(float num)
{
	return (float)sqrt(num);
}
//求acos
float QuaterAcos(float num)
{
	return (float)acos(num);//acos銆?
}
//求cos
float QuaterCos(float num)
{
	return (float)cos(num);
}
float QuaterSin(float num)
{
	return (float)sin(num);//sin銆?
}
//求绝对值
float QuaterAbs(float num)
{
	if (num < 0)
		return -num;
	else
		return num;
}
//获取符号
int QuaterSign(float num)
{
	if (num < 0)
		return -1;
	else if(num > 0)
		return 1;
	else
		return 0;
}
/*
x 指定两个点横坐标的差
y 指定两个点纵坐标的差

计算出来的结果angel是一个弧度值，要换算成角度，也必须乘以180/PI。
*/
float QuaterAtan2(float y,float x)
{
	return (float)atan2(y,x);
}

float QuaterAtan(float x)
{
	return (float)atan(x);
}
//求反正弦函数
float QuaterAsin(float x)
{
	return (float)asin(x);
}
//角度制转弧度制
float QuateDegToRad(float deg) {
  deg /= 180/PI;
  return deg;
}
//四元数赋值函数
int QuaterSet(Quaternion *q, float w, float x, float y, float z)
{
	if (q == NULL)  return -1;
	q->w = w;
	q->x = x;
	q->y = y;
	q->z = z;

	return 0;
}
//创建四元数，调用处要用变量接收一下
Quaternion QuaterCreat(float w, float x, float y, float z)
{
	Quaternion quate;
	quate.w = w;
	quate.x = x;
	quate.y = y;
	quate.z = z;
	return quate;
}
//功能：将欧拉角转成四元数
//Roll-x      Pitch - y    Yaw - z
Quaternion QuaterFromEuler(float x_roll, float y_pitch, float z_yaw)
{
	// Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
	// and multiply those together.
	// the calculation below does the same, just shorter
	float p = y_pitch * PIOVER180 / 2.0f;
	float y = z_yaw * PIOVER180   / 2.0f;
	float r = x_roll * PIOVER180  / 2.0f;

	float sinp = QuaterSin(p);
	float siny = QuaterSin(y);
	float sinr = QuaterSin(r);
	float cosp = QuaterCos(p);
	float cosy = QuaterCos(y);
	float cosr = QuaterCos(r);

	return QuaterCreat(cosr * cosp * cosy + sinr * sinp * siny,
		sinr * cosp * cosy - cosr * sinp * siny,
		cosr * sinp * cosy + sinr * cosp * siny,
		cosr * cosp * siny - sinr * sinp * cosy
	);
}


//加法
Quaternion QuaterAdd(Quaternion q1, Quaternion q2)
{
	float w1 = q1.w, x1 = q1.x, y1 = q1.y, z1 = q1.z;
	float w2 = q2.w, x2 = q2.x, y2 = q2.y, z2 = q2.z;
	return QuaterCreat(w1 + w2, x1 + x2, y1 + y2, z1 + z2);
}
//减法
Quaternion QuaterSub(Quaternion q1, Quaternion q2)
{
		float w1 = q1.w, x1 = q1.x, y1 = q1.y, z1 = q1.z;
		float w2 = q2.w, x2 = q2.x, y2 = q2.y, z2 = q2.z;
		return QuaterCreat(w1 - w2, x1 - x2, y1 - y2, z1 - z2);
}
//乘法，q1*q2
Quaternion QuaterMul(Quaternion q1,Quaternion q2)
{
	float w1 = q1.w, w2 = q2.w, w_new = 0;
	float x1 = q1.x, x2 = q2.x, x_new = 0;
	float y1 = q1.y, y2 = q2.y, y_new = 0;
	float z1 = q1.z, z2 = q2.z, z_new = 0;

	w_new = (w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2);
	x_new = (w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2);
	y_new = (w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2);
	z_new = (w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2);

	return QuaterCreat(w_new, x_new, y_new, z_new);
}
/* 四元数的点乘
* 点乘结果是标量，两个单位四元数点乘的结果区间为[-1,1]。
* 点乘结果的绝对值越大，相乘的两个四元数代表的角位移越相似。
*/
float QuaterDot(Quaternion q1, Quaternion q2)
{
	float w1 = q1.w, x1 = q1.x, y1 = q1.y, z1 = q1.z;
	float w2 = q2.w, x2 = q2.x, y2 = q2.y, z2 = q2.z;
	return w1 * w2 + x1 * x2 + y1 * y2 + z1 * z2;
}

float QuaterAngle(Quaternion q1, Quaternion q2)
{
	float cos_theta = QuaterDot(q1, q2);
	float theta;
	// if B is on opposite hemisphere from A, use -B instead
	/*如果四元数点积的结果是负值，那么后面的插值就会在4D球面上绕远路，
	* 这并不是我们想要的。为了解决这个问题，我们测试点积的结果，
	* 当结果是负值时，我们将2个四元数的其中一个取反，取反它的系数和向量部分，
	* 并不会改变它代表的朝向。而经过这一步操作，可以保证这个旋转走的是最短路径。*/	
	if (cos_theta < 0)
	{
		cos_theta = -cos_theta;
	}
	theta = QuaterAcos(cos_theta);
	return 2 * Rad2Deg * theta;

}

//四元数除以一个浮点数
Quaternion QuaterDivNum(Quaternion q, float num)
{
	float w = q.w, x = q.x, y = q.y, z = q.z;
	if (num == 0) return QuaterCreat(1, 0, 0, 0);
	w /= num;
	x /= num;
	y /= num;
	z /= num;
	return QuaterCreat(w, x, y, z);
}

//范数的平方
float QuaterNormPow(Quaternion q)
{
	float w = q.w, x = q.x, y = q.y, z = q.z;
	return w * w + x * x + y * y + z * z;//姹傚嚭鏉ヨ寖鏁扮殑骞虫柟
}
//范数 
float QuaterNorm(Quaternion q)
{
	return QuaterSqrt(QuaterNormPow(q));
}
//规范化
Quaternion QuaterNormalize(Quaternion q)
{
	float norm = QuaterNorm(q);
	return QuaterDivNum(q, norm);
}
/*共轭
* 四元数的共轭就是让四元数的向量部分取负
* 四元数和它的共轭代表相反的角位移，因为相当于旋转轴反向。*/
Quaternion QuaterConjugate(Quaternion q)
{
	float w = q.w, x = q.x, y = q.y, z = q.z;
	return QuaterCreat(w, -x, -y, -z);
}
/* 四元数的逆
* 四元数的逆定义为四元数的共轭除以它的模
* 一般使用单位四元数，此时它的逆和共轭其实是相等的。*/
Quaternion QuaterInverse(Quaternion q)
{
	return QuaterDivNum(QuaterConjugate(q), QuaterNormPow(q));
}

/**求两个四元数的差
* *利用四元数的乘法和逆，可以计算两个四元数的“差”，
* 四元数“差”表示一个方位到另一个方位的角位移。
* 注意不能除以四元数，只能乘以它们的逆来达到目的。*/

Quaternion QuaterDiff(Quaternion q1, Quaternion q2)
{
	return QuaterMul(QuaterInverse(q1), q2);
}
/**
功能：将四元数的某个轴旋转一个角度
参数：q要旋转的四元数
	  axis 旋转的轴  值为 'x','y','z'
	  angle 旋转的角度
返回值：返回旋转后的四元数
*/
Quaternion QuaterAxisRotation(Quaternion q,char axis,float angle)
{
	Quaternion rotation;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	switch (axis)
	{
	case 'x':x = angle; break;
	case 'y':y = angle; break;
	case 'z':z = angle; break;
	}
	rotation =	QuaterFromEuler(x,y,z);
	return QuaterMul(q, rotation);
}
//功能：将四元数转换欧拉角  
//euler 是float[3] 数组，顺序是0-x,1-y,2-z
//Roll-x      Pitch - y    Yaw - z
void QuaterToEuler(Quaternion quate,float *euler) // Z-Y-X Euler angles
{
	float q0 = quate.w;
	float q1 = quate.x;
	float q2 = quate.y;
	float q3 = quate.z;

	euler[0] = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;                                    // pitch
	euler[1] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;     // roll
	euler[2] = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3; //yaw
// 	float *q = (float *)&quate; // quaternion
// 	float gx, gy, gz; // estimated gravity direction
  
//   gx = 2 * (q[1]*q[3] - q[0]*q[2]);
//   gy = 2 * (q[0]*q[1] + q[2]*q[3]);
//   gz = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
  
//   euler[0] =QuaterAtan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1);
//   euler[1] =QuaterAtan(gx / QuaterSqrt(gy*gy + gz*gz));
//   euler[2] =QuaterAtan(gy / QuaterSqrt(gx*gx + gz*gz));
}
//四元数转360度的欧拉角（角度制）
//euler 是float[3] 数组，顺序是0-x,1-y,2-z
void QuaterToEuler360deg(Quaternion quate,float *euler)
{
  float m11, m12, m21, m31, m32;
  float gx, gz; // estimated gravity direction
  float q0 = quate.w,q1 = quate.x,q2 = quate.y,q3 = quate.z;
  float * angles = euler;
  gx = 2 * (q1*q3 - q0*q2);
//  gy = 2 * (q[0]*q[1] + q[2]*q[3]);
  gz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
    
  m11 = 2.0f*(q1*q2 + q0*q3);
  m12 = q0*q0 + q1*q1 - q2*q2 - q3*q3;
  m21 = -2.0f*(q1*q3 - q0*q2);               
  m31 = 2.0f*(q2*q3 + q0*q1);              
  m32 = q0*q0 - q1*q1 - q2*q2 + q3*q3;

  // find angles for rotations about X, Y, and Z axes
  angles[0] = -QuaterAtan2( m11, m12 ) * 57.2957795f;
  angles[1] = -QuaterAsin( m21 ) * 57.2957795f;
  angles[2] = -QuaterAtan2( m31, m32 ) * 57.2957795f;
    
  //  	Gx	gy	gz
  //0-90	"+"		"+"
  //90-180	"+"		"-"
  //180-270	"-"		"-"
  //270-360	"-"		"+"
    
  if(gx >= 0 && gz < 0)
      angles[1] = 180.0f - angles[1];
     else if(gx < 0 && gz < 0)
       angles[1] = 180.0f - angles[1];
      else if(gx < 0 && gz >=0)
        angles[1] = 360.0f + angles[1];
        
  if(angles[0] < 0) angles[0] = 360.0f + angles[0];
  if(angles[2] < 0) angles[2] = 360.0f + angles[2];
  
  angles[0] = 360 - angles[0];  
}
//四元数转360度的欧拉角（弧度制）
//euler 是float[3] 数组，顺序是0-x,1-y,2-z
void QuaterToEuler360rad(Quaternion quate,float *euler)
{
	int i = 0;
	QuaterToEuler360deg(quate,euler);
	for(i = 0;i < 3; i++) {
		euler[i] = QuateDegToRad(euler[i]);
	}
}

//四元数插值算法
//qa：起始位置 
//qb：结束位置  
//t : 插入的位置 0<= t <= 1
//返回值：插入位置的四元数
Quaternion QuaterSlerp(Quaternion qa, Quaternion qb, float t)
{
	// quaternion to return
	Quaternion qm;
	float sinHalfTheta ;
	float halfTheta ;
	float ratioA  ;
	float ratioB  ;
	// Calculate angle between them.
	float cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (QuaterAbs(cosHalfTheta) >= 1.0)
	{
		qm.w = qa.w; qm.x = qa.x; qm.y = qa.y; qm.z = qa.z;
		return qm;
	}
	// Calculate temporary values.
	halfTheta = QuaterAcos(cosHalfTheta);
	sinHalfTheta = QuaterSqrt(1.0f - cosHalfTheta * cosHalfTheta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	if (QuaterAbs(sinHalfTheta) < 0.001f)
	{ // fabs is floating point absolute
		qm.w = (qa.w * 0.5f + qb.w * 0.5f);
		qm.x = (qa.x * 0.5f + qb.x * 0.5f);
		qm.y = (qa.y * 0.5f + qb.y * 0.5f);
		qm.z = (qa.z * 0.5f + qb.z * 0.5f);
		return qm;
	}
	ratioA = QuaterSin((1 - t) * halfTheta) / sinHalfTheta;
	ratioB = QuaterSin(t * halfTheta) / sinHalfTheta;
	//calculate Quaternion.
	qm.w = (qa.w * ratioA + qb.w * ratioB);
	qm.x = (qa.x * ratioA + qb.x * ratioB);
	qm.y = (qa.y * ratioA + qb.y * ratioB);
	qm.z = (qa.z * ratioA + qb.z * ratioB);
	return qm;
}
////本文件使用举例测试
//void QuaterTest()
//{
//	float x_roll,  y_pitch,  z_yaw;
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
//	Quaternion Quater_slerp = QuaterSlerp(quaterA, quaterB,0.2f);  //A转到距离B的1/5处的四元数
//	Quaternion Quater_rotation = QuaterAxisRotation(quaterA, 'x', 90);//x轴旋转90度
//}
