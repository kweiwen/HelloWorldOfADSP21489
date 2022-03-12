/*
 * AT.cpp
 * https://www.cnblogs.com/T0213-ZH/p/14336116.html
 *  Created on: 2022��3��11��
 *      Author: EGO
 */
#include "AT.h"
#include <stddef.h>
#include "UART0_isr.h"


/* ATָ��� */
const AT_cmd_func at_cmd_func[] = {
    {AT_CMD_TEST,         "AT",                at_cmd_test},
    {AT_CMD_NAME,         "AT+NAME=",          at_cmd_name},
    {AT_CMD_READ_IN,      "AT+READ_IN=",         at_cmd_get_value},
    {AT_CMD_READ_OUT,	  "AT+READ_OUT=",           NULL},
    {AT_SET_PARA,	  	  "AT+SET=",           at_cmd_set_para},
    {AT_GET_VER,    "AT_ADV=",           NULL},
    {AT_END,              NULL,                NULL}
};

static unsigned int mstrlen(char *s);
static int mstrncmp(char *s1, char *s2, int n);

char at_cmd_set_para(char *p, unsigned char len){

}

/* ָ��ִ�к��� */
char at_cmd_get_value(char *p, unsigned char len){

	switch(*p)
	{
	case '1':
    	DBG("read Channel 1\r\n");
	break;

	case '2':
    	DBG("read Channel 2\r\n");
	break;

	case '3':
    	DBG("read Channel 3\r\n");
	break;

	case '4':
    	DBG("read Channel 4\r\n");
	break;

	default:
    	DBG("Channel number error\r\n");
		break;
	};



    return 0;
}

char at_cmd_test(char *p, unsigned char len){

    DBG("AT+OK\r\n");
    return 0;
}

char at_cmd_name(char *p, unsigned char len){

    if(*p == '?'){
    	DBG("This DSP is ADI ADSP21489\r\n");
    }else{
    	DBG("AT+OK at command err\r\n");
    }
    return 0;
}

/* ����ָ����ж�Ӧ��ָ�� */
char AT_cmd_search(char *p, char len){

    char ret = 0;
    char *pstr;
    char i, n;

    for(i=1; at_cmd_func[i].cmd != AT_END; i++){

        n = mstrlen(at_cmd_func[i].str);			//�@ȡָ���L��
        if(!mstrncmp(p, at_cmd_func[i].str, n)){	//ƥ��ָ��,ǰn���ַ���ͬ������0
            ret = i;
            break;
        }
    }
    return ret;
}

/* ATָ����� */
char at_cmd_parse(char *p, unsigned char len){

    char ret = AT_SUCCESS;
    char index = 0;

    if(len < 4) return AT_ERR; /* ������ָ����С���� */

    if((p[0] == 'A') && (p[1] == 'T') && (p[len-2] == 0x0D) && (p[len-1] == 0x0A)){//��A����T�� �_�^, �Q��+��܇ �Y��
        if(len == 4){  /* ����ָ�� */
            if(at_cmd_func[AT_CMD_TEST].cb != NULL){
            	at_cmd_func[AT_CMD_TEST].cb(NULL, 0);  /* ִ�в���ָ�� */
            }
        }else if(p[2] == '+'){ /* ִ��ָ����� */
            index = AT_cmd_search(p, len); /* ����ƥ���ִ��ָ�0=��ƥ�䣬!0=δƥ�� */
            if(index){
                if(at_cmd_func[index].cb != NULL){  /* �ж�ָ���Ӧִ�к����Ƿ���� */
                    char n;
                    n = mstrlen(at_cmd_func[index].str);
                    ret = at_cmd_func[index].cb(p+n, len-n); /* ִ�ж�Ӧ��ָ���, p+n:��ָ���������ִ�к�����len-n-2:ָ�������Ч���� */
                }else
                    ret = AT_ERR_FUN_UNUSED; /* û�п�ִ�к��� */
            }else{
                ret = AT_ERR_UNINVAIL; /* δ�ҵ�ƥ���ָ�� */
            }
        }
    }else{/* ��ʽ��ƥ�� */
        return AT_ERR;
    }

    return ret;
}

/*
 * ���������ʹ����2����C�⺯�� mstrlen, mstrncmp (��C����strlen, strncmp����һ��)��
 * ��Ҫ��Ϊ����ֲ������ƽ̨������ʱʹ�ô���ռ�ÿռ価����
 *
 * */
static unsigned int mstrlen(char *s){

	char *ss = s;

    while (*ss)
        ss++;
    return ss - s;
}

//��str1��str2��ǰn���ַ���ͬ���򷵻�0
static int mstrncmp(char *s1, char *s2, int n){

    const char *c1 = (const char *)s1;
    const char *c2 = (const char *)s2;
    char ch;
    int d = 0;

    while (n--) {
        d = (int)(ch = *c1++) - (int)*c2++;
        if (d || !ch)
            break;
    }

    return d;
}

