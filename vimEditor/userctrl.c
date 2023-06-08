#define _CRT_SECURE_NO_WARNINGS
#include "userctrl.h"
#include"display.h"
#include <conio.h>

void editor_drive(char* file_dir) {
    Editor* ed = NULL;

    int loop = 1;
    ed = init_editor(ed);
    system("cls");
    if (!load_data_file(ed, file_dir))
    {
        printf("error");
        return;
    }
    
    while (loop) {
        switch (ed->mode)
        {
        case 0: // 普通模式，用来保存和其他
        {
            normal_edit(ed,file_dir);

        }
        break;
        case 1: // 编辑模式
        {
            text_in(ed);
        }
        break;
        case 2: // 可视化模式
        {
            visual_edit(ed);
        }
        break;
        }
    }


}
void text_in(Editor* ed) // 进入的编辑模式
{
    int loop = 1;
    char ch;
    LineHead* lh_cur = NULL;
    while (loop) {
        ch = _getch();
        switch (ch)
        {

        case '\b':
            // 回退
        {
            move_l(1);    // 将光标左移一格
            del_ch();     // 删除光标所在位置的字符
            
            //如果没有字符，则需要删除整行，同时光标上移
            del_cn(ed->cs);
        }
            break;
        case '\r':// enter
        {
            printf("\n");
            Add_Line(lh_cur, ed);
        }
            break;
        case -32: {
            switch(_getch()){
                case 72: // 上箭头
                   // printf("上箭头");
                    move_cursor(ed->cs, 'u');
                    break;

                case 80: // 下箭头
                    //printf("下箭头");
                    move_cursor(ed->cs, 'd');
                    break;

                case 75: // 左箭头
                    //printf("左箭头");
                {
                    move_cursor(ed->cs, 'l');
                    move_l(1);

                }
                break;
                case 77: // 右箭头
                    //printf("右箭头");
                {
                    move_cursor(ed->cs, 'r');
                    move_r(1);
                }
                break;
            }
        }
            break;
        case 27: // esc
        {
            loop = 0; // 跳出循环
            ed->mode = 0;
        }
            break;
        default:
        {
       
            lh_cur = ed->cur;
            if(Add_char(lh_cur,ed->cs, ch)== NULL) // 如果是尾插
                printf("%c", ch);
            else 
                ist_ch(ch);
        }
            break;
        }
    }
}

void visual_edit(Editor* ed){
    int loop = 1;
    char ch;
    LineHead* lh_cur = NULL;
    while (loop) {
        ch = _getch();
        switch (ch)
        {

        
        case 'j': // 向下移动一行
            break;
        case 'k': // 向上移动一行
            break;
        case 'h': // 向左移动一个字符
            break;
        case 'l': // 向右移动一个字符
            break;
        
        case 27: // esc
        {
            loop = 0; // 跳出循环
            ed->mode = 0;
        }
        break;
        //default:
        //{

        //    lh_cur = ed->cur;
        //    if (Add_char(lh_cur, ed->cs, ch) == NULL) // 如果是尾插
        //        printf("%c", ch);
        //    else
        //        printf("\033[@%c", ch);
        //}
        //break;
        }
    }
}

void move_cursor(cursor* cs, char dir)
{
    if (cs->charn == NULL || cs->lineh == NULL) return;
    if (cs->charn->next->context == '\r' || cs->charn->next->context == '\n')
        cs->charn = cs->charn->pre; // 如果当前是指向一些特殊字符，则往前移动

    switch (dir) {
    case 'u':
    {
        // 此时上面那行比下面这行长；
        int counter = 0;
        if (cs->lineh->pre->line_length == 0) {
            move_u(0); // 移动到上一行（空行）的开头
            cs->lineh = cs->lineh->pre;
            cs->charn = cs->lineh->charhead;
        }
        else if (cs->charn->char_seq <= cs->lineh->pre->line_length) {
            move_u(1);
            counter = cs->charn->char_seq;
            cs->lineh = cs->lineh->pre;
            cs->charn = cs->lineh->charhead;
            while ((counter--)>1) {
                cs->charn = cs->charn->next;
            }
        }
        else {
            move_u(2);
            move_l(1);
            cs->lineh = cs->lineh->pre;
            cs->charn = cs->lineh->chartail->pre;//除去特殊结尾字符

        }
    }
        break;
    case 'd':
    {
        int counter = 0;
        if (cs->lineh->next->line_length == 0) {
            move_d(0);
            cs->lineh = cs->lineh->next;
            cs->charn = cs->lineh->charhead;
        }
        else if (cs->charn->char_seq <= cs->lineh->next->line_length) {
            move_d(1);
            counter = cs->charn->char_seq;
            cs->lineh = cs->lineh->next;
            cs->charn = cs->lineh->charhead;
            while ((counter--) > 1) {
                cs->charn = cs->charn->next;
            }
        }
        else {
            move_d(2);
            move_l(1);
            cs->lineh = cs->lineh->next;
            cs->charn = cs->lineh->chartail->pre;
        }
    }
        break;
    case 'l':
    {
        if (cs->charn->pre != NULL) {
            cs->charn = cs->charn->pre;
        }
        else {
            cs->charn = cs->lineh->chartail->pre;// 跳过空格
        }
       // cs->lineh->charcur = cs->charn;
    }
        break;
    case'r':
    {
        if (cs->charn->next != '\r' || cs->charn->next != '\n')
            cs->charn = cs->charn->next;
        else
            cs->charn = cs->lineh->pre->charhead;
       // cs->lineh->charcur = cs->charn;
    }
        break;
    }
}

void normal_edit(Editor* ed,char* file_dir)
{
    int loop = 1 , n = 0;
    char ch;
    LineHead* lh_cur = NULL;
    while (loop) {
        ch = _getch();
        switch (ch)
        {
        case -32: {
            switch (_getch()) {
            case 72: // 上箭头
               // printf("上箭头");
                move_cursor(ed->cs, 'u');
                break;

            case 80: // 下箭头
                //printf("下箭头");
                move_cursor(ed->cs, 'd');
                break;

            case 75: // 左箭头
                //printf("左箭头");
            {
                move_cursor(ed->cs, 'l');
                move_l(1);

            }
            break;
            case 77: // 右箭头
                //printf("右箭头");
            {
                move_cursor(ed->cs, 'r');
                move_r(1);
            }
            break;
            }
        }
                break;
        case 'j': // 向下移动一行
            move_cursor(ed->cs, 'd');
            break;
        case 'k': // 向上移动一行
            move_cursor(ed->cs, 'u');
            break;
        case 'h': // 向左移动一个字符
            move_cursor(ed->cs, 'l');
            move_l(1);
            break;
        case 'l': // 向右移动一个字符
            move_cursor(ed->cs, 'r');
            move_r(1);
            break;

        case 'i':
        {
            ed->mode = 1;
            loop = 0;
        }
        break;
        case ':':
        {
            char ch_temp, str[8];
            n = ed->lineNums;
            while (n-- > 0) {
                move_d(0);
            }//移动光标
            move_d(0);
            scanf("%s", str);
            if (strcmp(str, "q!") == 0) {
                del_editor(ed);
                loop = 0;  // 退出编辑器
            }
            else if (strcmp(str, "w") == 0) {
                // 保存文件,需要将文件写入
                save_data_file(ed, file_dir);
                //printf("File saved.\n");
            }
            else if (strcmp(str, "wq") == 0) {
                // 保存并退出 写入文件之后
                save_data_file(ed, file_dir);
                del_editor(ed);
                //printf("File saved.\n");
                loop = 0;
            }
           
        }
        break;
        }
    }
}

