/**
����:
1. ��"ÿ�е����ɶ�"���ϲ�ÿһ�Σ�ȥ��'\r\n'���м俴����ӿո�
��ʱ������ɾ����ҳ���ӣ�ͼƬ
///- analog set key(copy)
///- get data from clipboard
///- operate data(delete or not)
///- write data to clipboard
///- analog set key(paste)

Use AltRun or write VBS and set shortcut

����:
Ditto ���ı�ճ��

ע��:
1. �ٴ�����exe���޸Ĳ���Ч
**/
#include <stdio.h>
#include <assert.h>
#include <windows.h>

#ifdef _MSC_VER
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif

int mode_space=1;
/**
һ�㶼��
0: �м䲻�ӿո�
1: �м�ӿո�
2: ����Ҳ����
**/

int main()
{

    char *url,*pData,*pr;
    size_t length,len;
    int i;

    ///- analog set key(copy) (ʹ��AltRun����ǰ���ڲ�����AltRun��ok���������������еĿ�ݼ�)
    keybd_event(VK_CONTROL,0,0,0);
    keybd_event('C',0,0,0);
    keybd_event('C',0,KEYEVENTF_KEYUP,0);
    keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
//    return 0;

    ///- get data from clipboard
    ///from https://zhidao.baidu.com/question/682834457977004412.html
    OpenClipboard(NULL);
    HANDLE hData=GetClipboardData(CF_TEXT);
//    assert(hData!=NULL);
    length=GlobalSize(hData);
    url=(char*)malloc(length+1);
    pData=(char*)GlobalLock(hData);
    strcpy(url,pData);
    GlobalUnlock(hData);
    CloseClipboard();
    url[length]=0;
    ///input: �ַ���url
    ///output: �ַ���pr
//    printf("%s\n",url);

//    for (i=0;i<length;i++)
//        printf("%c",url[i]);
//    printf("\n\n\n");

//    printf("%s\n",url);
//    return 0;

    ///- operate data(delete or not)
    len=0;
    pr=(char*)malloc(length+1);
    for (i=0;i<length;i++)
        ///'\r\n' replace with ' '
        if (url[i]!='\n' && url[i]!='\r')
            pr[len++]=url[i];
        else if (url[i]=='\n')
        {
            /**
            һ��ĩβ��'-'��������ȡ�ģ������ǿ�������һ�е�
                ��������������: �����һ����β����һ����ͷƴ������������ɵ��ʣ���ô���кϲ�
            һ����β����һ����ͷ���ǵ���

            �Ƿ��ж�����������? ���Ƶ�ʱ�������һ����?
            **/
            if (mode_space==1)
                pr[len++]=' ';
            else if (mode_space==0)
                ///�����
                ;
            else if (mode_space==2)
                pr[len++]=url[i];
        }
        else if (url[i]=='\r')
        {
            if (mode_space==2)
                pr[len++]=url[i];
        }

    pr[len]=0;

////    printf("%s\n",pr);
////    return 0;


//    for (i=0;i<len;i++)
//        printf("%c",pr[i]);
//    printf("\n\n\n");

    ///- write data to clipboard
    ///from https://zhidao.baidu.com/question/647698977954822045.html
    if(OpenClipboard(NULL))
    {
        EmptyClipboard();
        HGLOBAL clipbuffer;
        char *buffer;
        clipbuffer = GlobalAlloc(GMEM_DDESHARE, len+1);
        buffer = (char *)GlobalLock(clipbuffer);
        strcpy(buffer, pr);
        GlobalUnlock(clipbuffer);
        SetClipboardData(CF_TEXT, clipbuffer);
        CloseClipboard();
    }

    ///- analog set key(paste)
    ///- Ctrl+V
//    keybd_event(VK_CONTROL,0,0,0);
//    keybd_event('V',0,0,0);
//    keybd_event('V',0,KEYEVENTF_KEYUP,0);
//    keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);

    return 0;
}
