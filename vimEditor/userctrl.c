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
        case 0: // ��ͨģʽ���������������
        {
            normal_edit(ed,file_dir);

        }
        break;
        case 1: // �༭ģʽ
        {
            text_in(ed);
        }
        break;
        case 2: // ���ӻ�ģʽ
        {
            visual_edit(ed);
        }
        break;
        }
    }


}
void text_in(Editor* ed) // ����ı༭ģʽ
{
    int loop = 1;
    char ch;
    LineHead* lh_cur = NULL;
    while (loop) {
        ch = _getch();
        switch (ch)
        {

        case '\b':
            // ����
        {
            move_l(1);    // ���������һ��
            del_ch();     // ɾ���������λ�õ��ַ�
            
            //���û���ַ�������Ҫɾ�����У�ͬʱ�������
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
                case 72: // �ϼ�ͷ
                   // printf("�ϼ�ͷ");
                    move_cursor(ed->cs, 'u');
                    break;

                case 80: // �¼�ͷ
                    //printf("�¼�ͷ");
                    move_cursor(ed->cs, 'd');
                    break;

                case 75: // ���ͷ
                    //printf("���ͷ");
                {
                    move_cursor(ed->cs, 'l');
                    move_l(1);

                }
                break;
                case 77: // �Ҽ�ͷ
                    //printf("�Ҽ�ͷ");
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
            loop = 0; // ����ѭ��
            ed->mode = 0;
        }
            break;
        default:
        {
       
            lh_cur = ed->cur;
            if(Add_char(lh_cur,ed->cs, ch)== NULL) // �����β��
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

        
        case 'j': // �����ƶ�һ��
            break;
        case 'k': // �����ƶ�һ��
            break;
        case 'h': // �����ƶ�һ���ַ�
            break;
        case 'l': // �����ƶ�һ���ַ�
            break;
        
        case 27: // esc
        {
            loop = 0; // ����ѭ��
            ed->mode = 0;
        }
        break;
        //default:
        //{

        //    lh_cur = ed->cur;
        //    if (Add_char(lh_cur, ed->cs, ch) == NULL) // �����β��
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
        cs->charn = cs->charn->pre; // �����ǰ��ָ��һЩ�����ַ�������ǰ�ƶ�

    switch (dir) {
    case 'u':
    {
        // ��ʱ�������б��������г���
        int counter = 0;
        if (cs->lineh->pre->line_length == 0) {
            move_u(0); // �ƶ�����һ�У����У��Ŀ�ͷ
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
            cs->charn = cs->lineh->chartail->pre;//��ȥ�����β�ַ�

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
            cs->charn = cs->lineh->chartail->pre;// �����ո�
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
            case 72: // �ϼ�ͷ
               // printf("�ϼ�ͷ");
                move_cursor(ed->cs, 'u');
                break;

            case 80: // �¼�ͷ
                //printf("�¼�ͷ");
                move_cursor(ed->cs, 'd');
                break;

            case 75: // ���ͷ
                //printf("���ͷ");
            {
                move_cursor(ed->cs, 'l');
                move_l(1);

            }
            break;
            case 77: // �Ҽ�ͷ
                //printf("�Ҽ�ͷ");
            {
                move_cursor(ed->cs, 'r');
                move_r(1);
            }
            break;
            }
        }
                break;
        case 'j': // �����ƶ�һ��
            move_cursor(ed->cs, 'd');
            break;
        case 'k': // �����ƶ�һ��
            move_cursor(ed->cs, 'u');
            break;
        case 'h': // �����ƶ�һ���ַ�
            move_cursor(ed->cs, 'l');
            move_l(1);
            break;
        case 'l': // �����ƶ�һ���ַ�
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
            }//�ƶ����
            move_d(0);
            scanf("%s", str);
            if (strcmp(str, "q!") == 0) {
                del_editor(ed);
                loop = 0;  // �˳��༭��
            }
            else if (strcmp(str, "w") == 0) {
                // �����ļ�,��Ҫ���ļ�д��
                save_data_file(ed, file_dir);
                //printf("File saved.\n");
            }
            else if (strcmp(str, "wq") == 0) {
                // ���沢�˳� д���ļ�֮��
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

