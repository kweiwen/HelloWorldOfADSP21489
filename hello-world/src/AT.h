/*
 * AT.h
 *
 *  Created on: 2022年3月11日
 *      Author: EGO
 */

#ifndef AT_CMD_AT_H_
#define AT_CMD_AT_H_

typedef enum{
    AT_CMD_TEST = 0,     /* 测试指令 */
    AT_CMD_NAME,         /* 设置名称 */
    AT_CMD_READ_IN,    	/* 讀取輸入有效值 */
    AT_CMD_READ_OUT,    /* 讀取輸出有效值 */
    AT_SET_PARA,
    AT_GET_VER,           /* 获取版本号 */
	AT_END
}AT_Cmd;

typedef char (*pFunc)(char *ptr, unsigned char len);

typedef struct {
    AT_Cmd cmd;			/* 指令序号 */
    char *str;			/* 指令内容 */
    pFunc  cb;			/* 指令执行 */
}AT_cmd_func;



#define AT_SUCCESS         (0) /* 指令正常 */
#define AT_ERR             (1) /* 指令异常 */
#define AT_ERR_UNINVAIL    (2) /* 没有对应指令 */
#define AT_ERR_FUN_UNUSED  (3) /* 没有可执行函数 */

/* 返回值参数内容如上， p-指向解析的指令，len-解析指令长度 */
char at_cmd_parse(char *p, unsigned char len);
char at_cmd_test(char *p, unsigned char len);
char at_cmd_name(char *p, unsigned char len);
char at_cmd_get_value(char *p, unsigned char len);
char at_cmd_set_para(char *p, unsigned char len);

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL   ((void *)0)
#endif
#endif


#endif /* AT_CMD_AT_H_ */
