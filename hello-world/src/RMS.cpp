/*
 * RMS.cpp
 *
 *  Created on: 2022��3��10��
 *      Author: EGO
 *  https://www.zhihu.com/question/270468541
 *  һ��ϵͳ�Ļ�׼��ƽ����Ʊ�׼������ƽ��Ϊ0dBr( dBreference,dB �ο�ϵͳ)����һ��רҵ��Ƶ�豸�У�
 *  0dBr����1.23V��ѹ��������Ƶ����������У�0dBr��ʾ����ֵ��СΪ1������0dBr����ϵͳ���ǵ�ƽ������
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
