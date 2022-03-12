/*
 * AT.h
 *
 *  Created on: 2022��3��11��
 *      Author: EGO
 */

#ifndef AT_CMD_AT_H_
#define AT_CMD_AT_H_

typedef enum{
    AT_CMD_TEST = 0,     /* ����ָ�� */
    AT_CMD_NAME,         /* �������� */
    AT_CMD_READ_IN,    	/* �xȡݔ����Чֵ */
    AT_CMD_READ_OUT,    /* �xȡݔ����Чֵ */
    AT_SET_PARA,
    AT_GET_VER,           /* ��ȡ�汾�� */
	AT_END
}AT_Cmd;

typedef char (*pFunc)(char *ptr, unsigned char len);

typedef struct {
    AT_Cmd cmd;			/* ָ����� */
    char *str;			/* ָ������ */
    pFunc  cb;			/* ָ��ִ�� */
}AT_cmd_func;



#define AT_SUCCESS         (0) /* ָ������ */
#define AT_ERR             (1) /* ָ���쳣 */
#define AT_ERR_UNINVAIL    (2) /* û�ж�Ӧָ�� */
#define AT_ERR_FUN_UNUSED  (3) /* û�п�ִ�к��� */

/* ����ֵ�����������ϣ� p-ָ�������ָ�len-����ָ��� */
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
