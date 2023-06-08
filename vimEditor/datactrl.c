#define _CRT_SECURE_NO_WARNINGS
#include "datactrl.h"
#include "display.h"
#include <malloc.h>
#include<stdio.h>
Editor* init_editor(Editor* ed)
{
	LineHead* lh = NULL;
	ed = malloc(sizeof(Editor));
	if (ed == NULL) {
		printf("out of mem");
		return NULL;
	}
	ed->head = NULL;
	ed->tail = NULL;
	ed->cur = NULL;

	ed->cs =malloc(sizeof(cursor));
	ed->cs->charn = NULL;
	ed->cs->lineh = NULL;
	ed->lineNums = 0;
	ed->mode = 0;
	lh = Add_Line(lh, ed);
	return ed;
}

void notify(LineHead* beg,CharNode* cnbeg,int add_sub) { // add_sub是加还是减
	if (beg != NULL && cnbeg == NULL) {
		LineHead* lh_temp = beg, * lh_next = beg->next;
		while (lh_next != NULL) {
			if (add_sub)
				lh_temp->line_seq++;
			else
				lh_temp->line_seq--;
			lh_temp = lh_next;
			lh_next = lh_next->next;
		}
	}
	else if (beg == NULL && cnbeg == NULL) {
		CharNode* cn_temp = cnbeg, * cn_next = cnbeg->next;
		while (cn_next != NULL) {
			if (add_sub)
				cn_temp->char_seq++;
			else
				cn_temp->char_seq--;
			cn_temp = cn_next;
			cn_next = cn_next->next;
		}
	}
	else {
		printf("wrong input,with beg and cnbeg both null and not null!");
	}
}

LineHead* Add_Line(LineHead* lh,Editor* ed)
{

	lh = malloc(sizeof(LineHead));
	if (lh == NULL) {
		printf("out of mem");
		return NULL;
	}
	lh->charhead = NULL;
	lh->chartail = NULL;
	lh->next = NULL;
	lh->line_length = 0;
	if (ed->head == NULL) {
		ed->head = lh;
		ed->tail = lh;
		ed->cur = lh;
		lh->pre = NULL;
		lh->line_seq = ed->lineNums + 1;
	}
	else{
		if (ed->cur->next == NULL) { //这个是尾插
			ed->tail->next = lh; // 一直跟踪再最后面
			lh->pre = ed->tail;
			ed->tail = lh;
			ed->cur = lh;
			lh->line_seq = lh->pre->line_seq + 1;
		}
		else { // 这个是随便插入一行
			lh->next = ed->cur->next;
			ed->cur->next = lh;
			lh->pre = ed->cur;
			ed->cur = lh;
			lh->line_seq = lh->pre->line_seq + 1;
			notify(lh,NULL,1);
		}
	}

	ed->lineNums++;

	return lh;
}

CharNode* Add_char(LineHead* lh,cursor* cs, char ch)
{
	CharNode* cn = malloc(sizeof(CharNode));
	cn->next = NULL;
	lh->line_length++;
	if (lh->charhead == NULL) {
		lh->charhead = cn;
		lh->chartail = cn;
		//lh->charcur = cn;
		cn->pre = NULL;
		cn->char_seq = lh->line_length;
		
	}
	else {
		if (cs->charn->next == NULL) {
			lh->chartail->next = cn; // 此时charn和chartail指向的是一个东西
			//lh->charcur = cn;
			
			cn->pre = lh->chartail;
			lh->chartail = cn;
			
			cn->char_seq = lh->line_length;
			
		}
		else {
			cn->next = cs->charn->next;
			cs->charn->next = cn;
			cn->pre = cs->charn;
			
			cn->char_seq = cn->pre->char_seq + 1;
			notify(NULL,cn,1);
			
		}

	}
	cs->charn = cn;
	cs->lineh = lh;
	cn->context = ch;
	return cn->next;// 通过这个来判断是不是最后一个
	
}

int load_data_file(Editor* ed,char* file_dir) {
	FILE* file = fopen(file_dir, "r");
	int ch,n = 0;
	if (file == NULL) {
		return 0;
	}
	while ((ch = fgetc(file)) != EOF) {
		switch (ch){
		case '\n': 
		{
			printf("\n");
			Add_Line(ed->tail, ed);
		}
			break;
		case '\r': {
			printf("\n");
			Add_Line(ed->tail, ed);
		}
			break;
		default:
		{
			Add_char(ed->tail, ed->cs, ch);// 如果是尾插
			printf("%c", ch);
		}
			break;
		}
	}
	ed->cs->lineh = ed->head;
	ed->cs->charn = ed->head->charhead;//修改光标
	ed->cur = ed->head;
	n = ed->lineNums;
	while (n-- > 0) {
		move_u(0);
	}//移动光标
	return 1;
}

int save_data_file(Editor* ed, char* file_dir) {
	FILE* file = fopen(file_dir, "w");
	if (file == NULL) {
		return 0;
	}
	LineHead* linecur = ed->head, * linenext = linecur->next;
	CharNode* charcur = linecur->charhead, * charnext = charcur->next;
	while (linenext != NULL) {
		//fputc(linecur->line_seq+48,file);
		
		while (charnext != NULL) {
			fputc(charcur->context,file);
			charcur = charnext;
			charnext = charnext->next;
		}
		linecur = linenext;
		linenext = linenext->next;
	}
	return 1;
}
//void print_again(Editor* ed) {
//	LineHead* linecur = ed->head,*linenext = linecur->next;
//	CharNode* charcur = linecur->charcur, * charnext = charcur->next;
//	while (linenext != NULL) {
//		printf("%d :", linecur->line_seq);
//		while (charnext != NULL) {
//			printf("%c", charcur->context);
//			charcur = charnext;
//			charnext = charnext->next;
//		}
//		linecur = linenext;
//		linenext = linenext->next;
//	}
//}
void del_cn(cursor* cs) {
	CharNode* cnpre = NULL, * cnnext = NULL;
	LineHead* ldel = NULL;
	if (cs->charn == NULL) return;
	if (cs->charn->pre != NULL) {
		cnpre = cs->charn->pre->pre;
		cnnext = cs->charn;
		cnpre->next = cnnext;
		free(cs->charn->pre);
		//cs->charn = cnpre;
		notify(NULL, cs->charn, 0);
		return;
	}
	else {//此时是首元素
		cnnext = cs->charn;
		//cs->lineh->charhead = cnnext;
		cnpre = cs->lineh->pre->chartail;
		ldel = cs->lineh;
		cs->lineh->pre->next = cs->lineh->next;
		cs->lineh = ldel->pre;
		cs->lineh->chartail = ldel->chartail;
		free(ldel);
		
		cs->charn = cnpre->pre;
		cs->charn->next = cnnext;
		free(cnpre);
		cs->charn->char_seq = cs->charn->pre->char_seq + 1;
		notify(NULL, cs->charn, 0);
		notify(cs->lineh->next,NULL, 0);
		return;
	}
}

void del_editor(Editor* ed)
{
	LineHead* linecur = ed->head, * linenext = linecur->next;
	CharNode* charcur = linecur->charhead, * charnext = charcur->next;
	while (linenext != NULL) {
		//fputc(linecur->line_seq+48,file);

		while (charnext != NULL) {
			free(charcur);
			charcur = charnext;
			charnext = charnext->next;
		}
		free(linecur);
		linecur = linenext;
		linenext = linenext->next;
	}
	free(ed);
}

void del_lh(cursor* cs) {

}

