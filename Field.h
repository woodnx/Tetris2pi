#pragma once
#define FIELD_SIDE_X 10
#define FIELD_SIDE_Y 24
#define FIELD_ASIDE_X FIELD_SIDE_X + 2
#define FIELD_ASIDE_Y FIELD_SIDE_Y + 2
#define BLOCK_SIZE 30

class Field
{
public:
	Field();
	Field(int x, int y);

	void InitField();

	bool judgeLineFull(int line_num);
	void OneLineErase(int line_num);
	void shiftLine(int line_num);
	virtual int LineEraseAndShift();

	int elemXToCoordX(int elem_x);
	int elemYToCoordY(int elem_y);
	int coordXToElemX(int coord_x);
	int coordYToElemY(int coord_y);

	int  getFieldValue(int elem_x, int elem_y);
	void setFieldValue(int elem_x, int elem_y, int value);

	bool containMino(int elem_x, int elem_y);

	int selectColor(int mino_num);

	void drawField();

private:
	int x, y;
	int body[FIELD_ASIDE_Y][FIELD_ASIDE_X] = { 0 };
	int color;
	int grid_color;
	int rotate_dir = 0;
};
