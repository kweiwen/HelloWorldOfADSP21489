/*
 * AT.cpp
 * https://www.cnblogs.com/T0213-ZH/p/14336116.html
 *  Created on: 2022年3月11日
 *      Author: EGO
 */
#include "AT.h"
#include <stddef.h>
#include "UART0_isr.h"


/* AT指令表 */
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

/* 指令执行函数 */
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

/* 查找指令表中对应的指令 */
char AT_cmd_search(char *p, char len){

    char ret = 0;
    char *pstr;
    char i, n;

    for(i=1; at_cmd_func[i].cmd != AT_END; i++){

        n = mstrlen(at_cmd_func[i].str);			//獲取指令長度
        if(!mstrncmp(p, at_cmd_func[i].str, n)){	//匹配指令,前n个字符相同，返回0
            ret = i;
            break;
        }
    }
    return ret;
}

/* AT指令解析 */
char at_cmd_parse(char *p, unsigned char len){

    char ret = AT_SUCCESS;
    char index = 0;

    if(len < 4) return AT_ERR; /* 不符合指令最小长度 */

    if((p[0] == 'A') && (p[1] == 'T') && (p[len-2] == 0x0D) && (p[len-1] == 0x0A)){//‘A’‘T’ 開頭, 換行+回車 結束
        if(len == 4){  /* 测试指令 */
            if(at_cmd_func[AT_CMD_TEST].cb != NULL){
            	at_cmd_func[AT_CMD_TEST].cb(NULL, 0);  /* 执行测试指令 */
            }
        }else if(p[2] == '+'){ /* 执行指令解析 */
            index = AT_cmd_search(p, len); /* 查找匹配的执行指令，0=已匹配，!0=未匹配 */
            if(index){
                if(at_cmd_func[index].cb != NULL){  /* 判断指令对应执行函数是否存在 */
                    char n;
                    n = mstrlen(at_cmd_func[index].str);
                    ret = at_cmd_func[index].cb(p+n, len-n); /* 执行对应的指令函数, p+n:将指令参数传输执行函数，len-n-2:指令参数有效长度 */
                }else
                    ret = AT_ERR_FUN_UNUSED; /* 没有可执行函数 */
            }else{
                ret = AT_ERR_UNINVAIL; /* 未找到匹配的指令 */
            }
        }
    }else{/* 格式不匹配 */
        return AT_ERR;
    }

    return ret;
}

/*
 * 上面代码中使用了2个类C库函数 mstrlen, mstrncmp (与C库中strlen, strncmp功能一致)，
 * 主要是为了移植是脱离平台，编译时使用代码占用空间尽量少
 *
 * */
static unsigned int mstrlen(char *s){

	char *ss = s;

    while (*ss)
        ss++;
    return ss - s;
}

//若str1与str2的前n个字符相同，则返回0
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

