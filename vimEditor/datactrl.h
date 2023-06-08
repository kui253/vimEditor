#ifndef DATACTRL_H
#define DATACTRL_H


typedef struct charnode {
	char context;
	struct charnode* next;
	struct charnode* pre;

	int char_seq;
}CharNode;

typedef struct LineHead {
	CharNode* charhead;
	CharNode* chartail;
	//CharNode* charcur;
	struct LineHead* next;
	struct LineHead* pre;
	int line_length;
	int line_seq;
}LineHead;
typedef struct cursor {
	LineHead* lineh;
	CharNode* charn;
}cursor;
typedef struct Editor {
	LineHead* head;
	LineHead* tail;
	LineHead* cur;
	int lineNums;
	cursor* cs ;
	int mode;
}Editor;

//±à¼­Æ÷³õÊ¼»¯
Editor* init_editor(Editor* ed);

//±à¼­Æ÷µÄ²åÈë

LineHead* Add_Line(LineHead* lh,Editor* ed);

CharNode* Add_char(LineHead* lh, cursor* cs, char ch);

void del_cn(cursor* cs);
void del_lh(cursor* cs);
void del_editor(Editor* ed);

int load_data_file(Editor* ed, char* file_dir);
int save_data_file(Editor* ed, char* file_dir);
#endif