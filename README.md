# ClipboardContentChange


--update 2021.2.26

AltRun等软件设置


/**
功能:
1. 对"每行单独成段"，合并每一段，去除'\r\n'，行间看情况加空格
有时它可以删除网页链接，图片
///- analog set key(copy)
///- get data from clipboard
///- operate data(delete or not)
///- write data to clipboard
///- analog set key(paste)

Use AltRun or write VBS and set shortcut

其它:
Ditto 纯文本粘贴

注意:
1. 再次生成exe，修改才生效
**/
#include <stdio.h>
#include <assert.h>
#include <windows.h>

#ifdef _MSC_VER
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif

int mode_space=1;
/**
一般都加
0: 行间不加空格
1: 行间加空格
2: 换行也加上
**/

int main()
{

    char *url,*pData,*pr;
    size_t length,len;
    int i;

    ///- analog set key(copy) (使用AltRun，当前窗口并非是AltRun，ok；或者设置属性中的快捷键)
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
    ///input: 字符串url
    ///output: 字符串pr
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
            一行末尾有'-'，而被获取的，是我们看到的下一行的
                因而这种情况较少: 如果把一行行尾，下一行行头拼接起来，能组成单词，那么进行合并
            一行行尾，下一行行头都是单词

            是否有多行密码的情况? 复制的时候会连在一起吗?
            **/
            if (mode_space==1)
                pr[len++]=' ';
            else if (mode_space==0)
                ///空语句
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









===============================================

给从论文复制的文本设置正确的格式(可设置快捷键)
心路历程 & 使用方法
in https://www.cnblogs.com/cmyg/p/11442696.html

该功能：
1.有可能出现反应慢的情况，按几次才成功（不知道为什么）
2.可能在windows注册，会更好

另外：
"纯文本粘贴"功能，在Ditto等软件中存在。
一个比较好的使用方法：
Ditto+Deepl 翻译 （按两次快捷键，第一次激活Ditto，第二次Ditto纯文本粘贴，复制到Deepl）
