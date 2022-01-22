#pragma once
#include "Field.h"
#include "Blocks.h"
#include "include.h"
#include "DxLib.h"
#define LAYOUT_SIZE 4

class Mino : public Blocks
{
public:
	Mino(Field* _field);

	void InitMino();

	int getMinoNum();
	int getMinoCoordX();
	int getMinoCoordY();

	void generateMino(int generate_mino_num);
	void generateMino(int generate_mino_num, int field_x, int field_y);

	void rotateMinoWithCollision(bool right_flag);
	bool superRotation(int dir_old, bool right_flag);

	void moveMino(int dx, int dy);
	void moveMinoWithCollision(int dx, int dy);
	void dropToMaxBottom();

	bool collisionField();
	bool collisionField(int dx, int dy);

	void transcribeMinoToField();
	void drawStatus();

private:
	Field* field;
	int field_x, field_y;
	int num = -1;					// each Mino unique number
	int color = -1;					// Mino color
};
