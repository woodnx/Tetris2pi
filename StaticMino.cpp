#include "StaticMino.h"
#include "DxLib.h"

StaticMino::StaticMino(int cx, int cy)
{
	this->coord_x = cx, this->coord_y = cy;
	memset(layout, 0, sizeof(layout));
	this->num = -1;
	this->color = -1;
}

void StaticMino::InitMino()
{
	memset(layout, 0, sizeof(layout));
	this->num = -1;
	this->color = -1;
}

void StaticMino::generateMino(int generate_mino_num)
{
	memset(layout, 0, sizeof(layout));
	switch (generate_mino_num) {
	case 0:	//I
		this->num = 1;
		//			 Y  X
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;
		this->layout[1][3] = this->num;

		this->color = GetColor(0, 191, 255);
		break;

	case 1:	//L
		this->num = 2;
		//			 Y  X
		this->layout[0][2] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;

		this->color = GetColor(255, 165, 0);
		break;

	case 2:	//J
		this->num = 3;
		//			 Y  X
		this->layout[0][0] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;

		this->color = GetColor(65, 105, 225);
		break;

	case 3:	//S
		this->num = 4;
		//			 Y  X
		this->layout[0][1] = this->num;
		this->layout[0][2] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;

		this->color = GetColor(50, 205, 50);
		break;

	case 4:	//Z
		this->num = 5;
		//			 Y  X
		this->layout[0][0] = this->num;
		this->layout[0][1] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;

		this->color = GetColor(255, 99, 71);
		break;

	case 5:	//O
		this->num = 6;
		//			 Y  X
		this->layout[0][0] = this->num;
		this->layout[0][1] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;

		this->color = GetColor(0xff, 0xff, 0x66);
		break;

	case 6:	//T
		this->num = 7;
		//			 Y  X
		this->layout[0][1] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;

		this->color = GetColor(218, 112, 214);
		break;

	defalt:
		this->num = -1;
		this->color = -1;
		break;
	}
}

void StaticMino::drawMino(bool is_ghost)
{
	int i, j;	// roop variables
	int fillflag = is_ghost ? FALSE : TRUE;

	for (i = 0; i < LAYOUT_SIZE; i++) {
		for (j = 0; j < LAYOUT_SIZE; j++) {
			if (this->layout[i][j] > 0)
			{
				DrawBox(this->coord_x + STATIC_BLOCK_SIZE * j, this->coord_y + STATIC_BLOCK_SIZE * i,
					this->coord_x + STATIC_BLOCK_SIZE * (j + 1) - 1, this->coord_y + STATIC_BLOCK_SIZE * (i + 1) - 1,
					this->color, fillflag);
				//DrawGraph(this->coord_x + BLOCK_SIZE * j, this->coord_y + BLOCK_SIZE * i, image[layout[i][j] - 1], FALSE);
				//DrawExtendGraph(this->coord_x + BLOCK_SIZE * j, this->coord_y + BLOCK_SIZE * i,
				//	this->coord_x + BLOCK_SIZE * (j + 1) - 1, this->coord_y + BLOCK_SIZE * (i + 1) - 1, image[num - 1], FALSE);
			}
		}
	}
}


void StaticMino::drawMino(int alpha)
{
	int i, j;	// roop variables

	for (i = 0; i < LAYOUT_SIZE; i++) {
		for (j = 0; j < LAYOUT_SIZE; j++) {
			if (this->layout[i][j] > 0) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
				DrawBox(this->coord_x + STATIC_BLOCK_SIZE * j, this->coord_y + STATIC_BLOCK_SIZE * i,
					this->coord_x + STATIC_BLOCK_SIZE * (j + 1) - 1, this->coord_y + STATIC_BLOCK_SIZE * (i + 1) - 1,
					this->color, TRUE);
				/*DrawExtendGraph(this->coord_x + BLOCK_SIZE * j, this->coord_y + BLOCK_SIZE * i,
					this->coord_x + BLOCK_SIZE * (j + 1) - 1, this->coord_y + BLOCK_SIZE * (i + 1) - 1, image[0], TRUE);*/
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//ブレンドモードをオフ
			}
		}
	}
}
