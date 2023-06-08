#ifndef USERCTRL_H
#define USERCTRL_H
#include "datactrl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void text_in(Editor* ed);

void move_cursor(cursor* cs, char dir);
void normal_edit(Editor* ed);
void visual_edit(Editor* ed);

void editor_drive(char* file_dir);
#endif