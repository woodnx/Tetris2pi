#pragma once
#include "Field.h"
#include "include.h"
#include "DxLib.h"
#include <vector>
#define LAYOUT_SIZE 4

using namespace std;

class Mino
{
public:
	Mino(int cx, int cy);
	Mino(Field* _field);

	void InitMino();

	int getMinoNum();
	int getMinoCoordX();
	int getMinoCoordY();

	void generateMino(int generate_mino_num);
	void generateMino(int generate_mino_num, int field_x, int field_y);

	void rotateMino(bool right_flag);				// rotate Mino
	void rotateMinoWithCollision(bool right_flag);
	bool superRotation(int dir_old, bool right_flag);

	void moveMino(int dx, int dy);
	void moveMinoWithCollision(int dx, int dy);
	void dropToMaxBottom();

	bool collisionField();
	bool collisionField(int dx, int dy);

	void transcribeMinoToField();

	void drawMino(bool is_ghost);
	void drawMino(int alpha);
	void drawStatus();
private:
	Field* field;
	int coord_x, coord_y;
	int field_x, field_y;
	//int layout[LAYOUT_SIZE][LAYOUT_SIZE];	//ƒ~ƒm–{‘Ì
	vector<vector<int>> layout = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};
	int num = -1;					// each Mino unique number
	int color = -1;					// Mino color
	int image[8] = { 0 };
	int rotate_dir = 0;
	float rotate_center_x;			// an central array element when mino rotates
	float rotate_center_y;			// an central array element when mino rotates
	bool is_entity = false;			// judge NULL

	int fillLayout(int val);
};
