#pragma once
#define LAYOUT_SIZE 4
#define STATIC_BLOCK_SIZE 20

class StaticMino
{
public:
	StaticMino(int cx, int cy);

	void InitMino();

	void generateMino(int generate_mino_num);

	void drawMino(bool is_ghost);
	void drawMino(int alpha);

private:
	int coord_x, coord_y;
	int layout[LAYOUT_SIZE][LAYOUT_SIZE];	//ƒ~ƒm–{‘Ì
	int num = -1;					// each Mino unique number
	int color = -1;					// Mino color
};

