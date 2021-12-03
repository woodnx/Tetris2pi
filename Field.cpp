#pragma once
#include "Field.h"
#include "DxLib.h"

Field::Field()
{
}

Field::Field(int x, int y)
{
	int i, j;
	for (i = 0; i < FIELD_ASIDE_Y; i++) {
		for (j = 0; j < FIELD_ASIDE_X; j++) {
			if ((j == 0 || j == FIELD_ASIDE_X - 1 || i == 0 || i == FIELD_ASIDE_Y - 1)) {
				this->body[i][j] = -1;
			}
		}
	}
	this->x = x, this->y = y;
	color = GetColor(255, 255, 255);
	grid_color = GetColor(64, 64, 64);
}

void Field::InitField()
{
	int i, j;
	for (i = 0; i < FIELD_ASIDE_Y; i++) {
		for (j = 0; j < FIELD_ASIDE_X; j++) {
			if ((j == 0 || j == FIELD_ASIDE_X - 1 || i == 0 || i == FIELD_ASIDE_Y - 1)) {
				this->body[i][j] = -1;
			}
			else {
				this->body[i][j] = 0;
			}
		}
	}
}

bool Field::judgeLineFull(int line_num)
{
	int i = 1;
	while (body[line_num][i] != 0) {
		if (i == FIELD_SIDE_X)
			return true;
		i++;
	}
	return false;
}

void Field::OneLineErase(int line_num)
{
	for (int i = 1; i <= FIELD_SIDE_X; i++) {
		this->body[line_num][i] = 0;
	}
}

void Field::shiftLine(int line_num)
{
	int i, j;
	int tmpline[FIELD_SIDE_X + 1] = { 0 };
	for (i = line_num; i > 0; i--) {
		for (j = 1; j <= FIELD_SIDE_X; j++) {
			tmpline[j] = body[i - 1][j];
			body[i][j] = 0;
			body[i - 1][j] = 0;
			body[i][j] = tmpline[j];
		}
	}
}

/// <summary>
/// 列の消去と移動
/// </summary>
/// <returns>消去した列の数</returns>
int Field::LineEraseAndShift()
{
	int erase_linenum = 0;
	for (int i = 1; i <= FIELD_SIDE_Y; i++) {
		if (judgeLineFull(i)) {
			OneLineErase(i);
			shiftLine(i);
			erase_linenum++;
		}
	}
	return erase_linenum;
}

/// <summary>
/// フィールド配列のx要素をウィンドウx座標に変換
/// </summary>
/// <param name="elem_x">フィールド配列のx要素</param>
/// <returns>ウィンドウx座標</returns>
int Field::elemXToCoordX(int elem_x)
{
	return this->x + elem_x * BLOCK_SIZE;
}

/// <summary>
/// フィールド配列のy要素をウィンドウy座標に変換
/// </summary>
/// <param name="elem_y">フィールド配列のy要素</param>
/// <returns>ウィンドウy座標</returns>
int Field::elemYToCoordY(int elem_y)
{
	return this->y + elem_y * BLOCK_SIZE;
}

/// <summary>
/// ウィンドウx座標をフィールド配列のx要素に変換
/// </summary>
/// <param name="coord_x">ウィンドウx座標</param>
/// <returns>フィールド配列のx要素</returns>
int Field::coordXToElemX(int coord_x)
{
	return (coord_x - this->x) / BLOCK_SIZE;
}

/// <summary>
/// ウィンドウy座標をフィールド配列のy要素に変換
/// </summary>
/// <param name="coord_y">ウィンドウy座標</param>
/// <returns>フィールド配列のy要素</returns>
int Field::coordYToElemY(int coord_y)
{
	return (coord_y - this->y) / BLOCK_SIZE;
}

/// <summary>
/// 指定配列要素のフィールドの値を返す
/// </summary>
/// <param name="elem_x">フィールド配列のx要素</param>
/// <param name="elem_y">フィールド配列のy要素</param>
/// <returns></returns>
int Field::getFieldValue(int elem_x, int elem_y)
{
	if (elem_x < 0 || elem_x > FIELD_ASIDE_X || elem_y < 0 || elem_y > FIELD_ASIDE_Y)return -1;
	else return this->body[elem_y][elem_x];
}

/// <summary>
/// フィールドの指定配列要素に値を書き込む
/// </summary>
/// <param name="elem_x">フィールド配列のx要素</param>
/// <param name="elem_y">フィールド配列のy要素</param>
/// <param name="value">書き込む値</param>
void Field::setFieldValue(int elem_x, int elem_y, int value)
{
	if (value != 0) {
		body[elem_y][elem_x] = value;
	}
}

/// <summary>
/// フィールドの指定要素にミノが存在するか判定
/// </summary>
/// <param name="elem_x">　フィールド配列のx要素</param>
/// <param name="elem_y">　フィールド配列のy要素</param>
/// <returns>
/// ミノが存在する true　ミノが存在しない false
/// </returns>
bool Field::containMino(int elem_x, int elem_y)
{
	if (this->body[elem_y][elem_x] > 0)return true;
	else return false;
}

int Field::selectColor(int mino_num)
{
	switch (mino_num) {
	case 1:	//I
		return GetColor(0, 191, 255);
		break;

	case 2:	//L
		return GetColor(255, 165, 0);
		break;

	case 3:	//J
		return GetColor(65, 105, 225);
		break;

	case 4:	//S
		return GetColor(50, 205, 50);
		break;

	case 5:	//Z
		return GetColor(255, 99, 71);
		break;

	case 6:	//O
		return GetColor(0xff, 0xff, 0x66);
		break;

	case 7:	//T
		return GetColor(218, 112, 214);
		break;
	}
	return GetColor(255, 255, 255);
}

void Field::drawField()
{
	int i, j;

	for (i = 5; i <= FIELD_SIDE_Y; i++) {
		for (j = 1; j <= FIELD_SIDE_X; j++) {
			DrawBox(this->x + BLOCK_SIZE * j, this->y + BLOCK_SIZE * i,
				this->x + BLOCK_SIZE * (j + 1), this->y + BLOCK_SIZE * (i + 1), GetColor(0, 0, 0), TRUE);
			if (this->body[i][j] > 0) {
				DrawBox(this->x + BLOCK_SIZE * j, this->y + BLOCK_SIZE * i,
					this->x + BLOCK_SIZE * (j + 1) - 1, this->y + BLOCK_SIZE * (i + 1) - 1, selectColor(this->body[i][j]), TRUE);
			}
			else if(this->body[i][j] == -1){
				DrawBox(this->x + BLOCK_SIZE * j, this->y + BLOCK_SIZE * i,
					this->x + BLOCK_SIZE * (j + 1) - 1, this->y + BLOCK_SIZE * (i + 1) - 1, GetColor(255,255,255), TRUE);
			}
			DrawBox(this->x + BLOCK_SIZE * j, this->y + BLOCK_SIZE * i,
				this->x + BLOCK_SIZE * (j + 1), this->y + BLOCK_SIZE * (i + 1), this->grid_color, FALSE);
			/*DrawBox(this->x + BLOCK_SIZE * j, this->y + BLOCK_SIZE * i,
				this->x + BLOCK_SIZE * (j + 1), this->y + BLOCK_SIZE * (i + 1), this->grid_color, FALSE);*/
		}
	}
}