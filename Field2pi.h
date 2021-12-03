#pragma once
#include "Field.h"
#define FIELD_SIDE_X 15
#define FIELD_SIDE_Y 15
#define FIELD_ASIDE_X FIELD_SIDE_X + 2
#define FIELD_ASIDE_Y FIELD_SIDE_Y + 2
#define BLOCK_SIZE 30

class Field2pi :public Field
{
public:
	Field2pi(int x, int y);
	void fieldRotate(int rotate_num, bool right_flag);
	int LineEraseAndShift() override;

private:
	int body[FIELD_ASIDE_Y][FIELD_ASIDE_X] = { 0 };
	int rotate_dir = 0;
};



