/*
 * RMS.cpp
 *
 *  Created on: 2022年3月10日
 *      Author: EGO
 *  https://www.zhihu.com/question/270468541
 *  一个系统的基准电平，或称标准工作电平记为0dBr( dBreference,dB 参考系统)。在一个专业音频设备中，
 *  0dBr等于1.23V电压，而在音频音序器软件中，0dBr表示采样值大小为1。由于0dBr是在系统中是电平的上限
 *  https://blog.csdn.net/weixin_43153548/article/details/82905969
 */
#include "RMS.h"
#include <stats.h>
#define 	Vref		2.5
#define 	VFull		5
float rmsf (const float samples[], int sample_length);

float rms_ch1;
float Get_RMS_Voltage(const float samples[], int sample_length)
{
	rms_ch1 = rmsf(samples, sample_length);
	rms_ch1 = rms_ch1 *(1/5.0);
	};
