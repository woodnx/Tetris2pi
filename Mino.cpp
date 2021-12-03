#pragma once
#include "Mino.h"
#include "Field.h"
#include "include.h"
#include "DxLib.h"

Mino::Mino(int cx, int cy)
{
	memset(layout, 0, sizeof(layout));

	field = NULL;
	this->coord_x = cx, this->coord_y = cy;
	this->field_x = 0, this->field_y = 0;
	this->num = -1;
	this->color = -1;
	this->rotate_center_x = -1;
	this->rotate_center_y = -1;
	this->rotate_dir = 0;
	this->is_entity = false;
}

Mino::Mino(Field* _field)
{
	memset(layout, 0, sizeof(layout));

	field = _field;
	this->coord_x = 0, this->coord_y = 0;
	this->field_x = 0, this->field_y = 0;
	this->num = -1;
	this->color = -1;
	this->rotate_center_x = -1;
	this->rotate_center_y = -1;
	this->is_entity = false;
}

void Mino::InitMino()
{
	memset(layout, 0, sizeof(layout));
	this->num = -1;
	this->color = -1;
	this->rotate_center_x = -1;
	this->rotate_center_y = -1;
	this->rotate_dir = 0;
	this->is_entity = false;
}

int Mino::getMinoNum()
{
	return num - 1;
}

int Mino::getMinoCoordX()
{
	return this->coord_x;
}

int Mino::getMinoCoordY()
{
	return this->coord_y;
}

// genarate _mino
// 指定された gnrt__mino_num (_mino.num - 1) にあったミノを生成する
// 0:I, 1:L, 2:J, 3:S, 4:Z, 5:O, 6:T
void Mino::generateMino(int generate_mino_num)
{
	memset(layout, 0, sizeof(layout));

	//LoadDivGraph("Images/mino.png", 8, 8, 1, 16, 16, image);
	switch (generate_mino_num) {
	case 0:	//I
		this->num = 1;
		//			 Y  X
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;
		this->layout[1][3] = this->num;

		this->color = GetColor(0, 191, 255);
		this->rotate_center_x = 1.5f;
		this->rotate_center_y = 1.5f;
		this->is_entity = true;
		break;

	case 1:	//L
		this->num = 2;
		//			 Y  X
		this->layout[0][2] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;

		this->color = GetColor(255, 165, 0);
		this->rotate_center_x = 1.0f;
		this->rotate_center_y = 1.0f;
		this->is_entity = true;
		break;

	case 2:	//J
		this->num = 3;
		//			 Y  X
		this->layout[0][0] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;

		this->color = GetColor(65, 105, 225);
		this->rotate_center_x = 1.0f;
		this->rotate_center_y = 1.0f;
		this->is_entity = true;
		break;

	case 3:	//S
		this->num = 4;
		//			 Y  X
		this->layout[0][1] = this->num;
		this->layout[0][2] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;

		this->color = GetColor(50, 205, 50);
		this->rotate_center_x = 1.0f;
		this->rotate_center_y = 1.0f;
		this->is_entity = true;
		break;

	case 4:	//Z
		this->num = 5;
		//			 Y  X
		this->layout[0][0] = this->num;
		this->layout[0][1] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;

		this->color = GetColor(255, 99, 71);
		this->rotate_center_x = 1.0f;
		this->rotate_center_y = 1.0f;
		this->is_entity = true;
		break;

	case 5:	//O
		this->num = 6;
		//			 Y  X
		this->layout[0][0] = this->num;
		this->layout[0][1] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;

		this->color = GetColor(0xff, 0xff, 0x66);
		this->rotate_center_x = 0.5f;
		this->rotate_center_y = 0.5f;
		this->is_entity = true;
		break;

	case 6:	//T
		this->num = 7;
		//			 Y  X
		this->layout[0][1] = this->num;
		this->layout[1][0] = this->num;
		this->layout[1][1] = this->num;
		this->layout[1][2] = this->num;

		this->color = GetColor(218, 112, 214);
		this->rotate_center_x = 1;
		this->rotate_center_y = 1;
		this->is_entity = true;
		break;

	defalt:
		this->num = -1;
		this->color = -1;
		this->rotate_center_x = -1;
		this->rotate_center_y = -1;
		this->is_entity = false;
		break;
	}
}

void Mino::generateMino(int generate_mino_num, int field_x, int field_y)
{
	memset(layout, 0, sizeof(layout));
	this->field_x = field_x, this->field_y = field_y;
	this->coord_x = field->elemXToCoordX(this->field_x);
	this->coord_y = field->elemYToCoordY(this->field_y);
	this->rotate_dir = 0;

	generateMino(generate_mino_num);
}

//ミノの回転
//回転行列を使って回転させる
void Mino::rotateMino(bool right_flag)
{
	int x, y;
	int X = 0, Y = 0;
	int lr = 1;
	float a = 0, b = 0;
	Mino tmp_mino = *this;

	a = this->rotate_center_x;
	b = this->rotate_center_y;

	if (right_flag) { lr = -1; }

	// Initialize _mino
	for (x = 0; x < LAYOUT_SIZE; x++) {
		for (y = 0; y < LAYOUT_SIZE; y++) {
			this->layout[y][x] = 0;
		}
	}

	for (y = 0; y < LAYOUT_SIZE; y++) {
		for (x = 0; x < LAYOUT_SIZE; x++) {
			// 回転行列を使って回転
			// 変換式は下の通り（ x -> X, y -> Y )
			// --------------------------------------------------
			// | X:  cos(90)( x - a ) - sin(90)( y - b ) + a	|
			// |	 = -y + b + a								|
			// |												|
			// | Y:  sin(90)( x - a ) + cos(90)( y - b ) + b	|
			// |	 =  x - a + b								|
			// --------------------------------------------------

			X = (int)(lr * (-y + a) + b);
			Y = (int)(lr * ( x - a) + b);
			if (X < 0 || Y < 0) continue;

			this->layout[y][x] = tmp_mino.layout[Y][X];
		}
	}
	if (right_flag) {
		rotate_dir++;
		if (rotate_dir >= 4) { rotate_dir = rotate_dir % 4; }
	}
	else {
		rotate_dir--;
		if (rotate_dir <= -1) { rotate_dir = (4 + rotate_dir) % 4; }
	}
}

void Mino::rotateMinoWithCollision(bool right_flag)
{
	Mino tmp_mino = *this;
	int tmp_rota_dir = this->rotate_dir;
	tmp_mino.rotateMino(right_flag);

	if (!tmp_mino.collisionField()) {
		this->rotateMino(right_flag);
	}
	else if(tmp_mino.superRotation(tmp_rota_dir, right_flag)) {
		this->rotateMino(right_flag);
		this->superRotation(tmp_rota_dir, right_flag);
	}
}

bool Mino::superRotation(int dir_old, bool right_flag)
{
	//参考（ほぼ丸パクリ）:http://www.terasol.co.jp/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0/6335
	//参考２（Iミノ部分の実装）：https://tetrisch.github.io/main/srs.html
	int dx = 0, dy = 0;
	int dir = this->rotate_dir;

	if (this->num != 1) //Iミノ以外の場合
	{
		// 1. 軸を左右に動かす
		// 0が90度（B）の場合は左，-90度（D）の場合は右へ移動
		// 0が0度（A），180度（C）の場合は回転前の方向の逆方向へ移動
		switch (dir) {
		case 1:
			dx = -1;
			break;
		case 3:
			dx = 1;
			break;
		case 0:
		case 2:
			switch (dir_old){
			case 1: // 回転前が右向き
				dx = 1;
				break;
			case 3: // 回転前が左向き
				dx = -1;
				break;
			}
			break;
		}
		if (this->collisionField(dx, dy)) {
			// 2.その状態から軸を上下に動かす
			// 0が90度（B），-90度（D）の場合は上へ移動
			// 0が0度（A），180度（C）の場合は下へ移動
			switch (dir){
			case 1:
			case 3:
				dy = -1;
				break;
			case 0:
			case 2:
				dy = 1;
				break;
			}
			if (this->collisionField(dx, dy)) {
				// 3.元に戻し、軸を上下に2マス動かす
				// 0が90度（B），-90度（D）の場合は下へ移動
				// 0が0度（A），180度（C）の場合は上へ移動
				dx = 0; dy = 0;
				switch (this->rotate_dir){
				case 1:
				case 3:
					dy = 2;
					break;
				case 0:
				case 2:
					dy = -2;
					break;
				}
				if (this->collisionField(dx, dy)) {
					// 4.その状態から軸を左右に動かす
					// 0が90度（B）の場合は左，-90度（D）の場合は右へ移動
					// 0が0度（A），180度（C）の場合は回転した方向の逆へ移動
					switch (dir)
					{
					case 1:
						dx = -1;
						break;
					case 3:
						dx = 1;
						break;
					case 0:
					case 2:
						switch (dir_old)
						{
						case 1: // 回転前が右向き
							dx = 1;
							break;
						case 3: // 回転前が左向き
							dx = -1;
							break;
						}
						break;
					}
					if (this->collisionField(dx, dy))return false;
				}
			}
		}
	}
	else {
		int p1dx, p2dx;
	
		// 1. 軸を左右に動かす
		// 0が90度（B）の場合は右，-90度（D）の場合は左へ移動（枠にくっつく）
		// 0が0度（A），180度（C）の場合は回転した方向の逆へ移動 0度は２マス移動
		switch (dir) {
		case 1:
			if (dir_old == 0)dx = -2;
			else dx = 1;
			break;
		case 3:
			if (dir_old == 2)dx = 2;
			else dx = -1;
			break;
		case 0:
		case 2:
			switch (dir_old) {
			case 1: // 回転前が右向き
				dx = -1;
				break;
			case 3: // 回転前が左向き
				dx = 1;
				break;
			}
			if (dir == 0)dx *= -2;
			break;
		}
		p1dx = dx;
		if (this->collisionField(dx, dy)) {
			// 2. 軸を左右に動かす
			// 0が90度（B）の場合は左，-90度（D）の場合は右へ移動（枠にくっつく）
			// 0が0度（A），180度（C）の場合は回転した方向へ移動 180度は２マス移動
			switch (dir) {
			case 1:
				if (dir_old == 2)dx = -2;
				else dx = 1;
				break;
			case 3:
				if (dir_old == 0)dx = 2;
				else dx = -1;
				break;
			case 0:
			case 2:
				switch (dir_old) {
				case 1: // 回転前が右向き
					dx = -1;
					break;
				case 3: // 回転前が左向き
					dx = 1;
					break;
				}
				if (dir == 2)dx *= -2;
				break;
			}
			p2dx = dx;
			if (this->collisionField(dx, dy)) {
				// 3. 軸を上下に動かす
				// 0が90度（B）の場合は1を下，-90度（D）の場合は1を上へ移動
				// 0が0度（A），180度（C）の場合は
				// 回転前のミノが右半分にある（B）なら1を上へ
				// 回転前のミノが左半分にある（D）なら2を下へ移動
				// 左回転なら２マス動かす
				switch (dir) {
				case 1:
					dx = p1dx;
					dy = 1;
					if ((dir_old == 0 && dir == 3) || (dir_old == 3 && dir == 2) ||
						(dir_old == 2 && dir == 1) || (dir_old == 1 && dir == 0))
						dy *= 2;
					break;
				case 3:
					dx = p1dx;
					dy = -1;
					if ((dir_old == 0 && dir == 3) || (dir_old == 3 && dir == 2) ||
						(dir_old == 2 && dir == 1) || (dir_old == 1 && dir == 0))
						dy *= 2;
					break;
				case 0:
				case 2:
					switch (dir_old) {
					case 1: // 回転前が右向き
						dx = p1dx;
						dy = -1;
						break;
					case 3: // 回転前が左向き
						dx = p2dx;
						dy = 1;
						break;
					}
					if ((dir_old == 3 && dir == 0) || (dir_old == 0 && dir == 1) ||
						(dir_old == 1 && dir == 2) || (dir_old == 2 && dir == 3))
						dy *= 2;
					break;
				}
				
				if (this->collisionField(dx, dy)) {
					// 4. 軸を上下に動かす
					// 0が90度（B）の場合は2を上，-90度（D）の場合は2を下へ移動
					// 0が0度（A），180度（C）の場合は
					// 回転前のミノが右半分にある（B）なら2を下へ
					// 回転前のミノが左半分にある（D）なら1を上へ移動
					// 右回転なら２マス動かす
					switch (dir) {
					case 1:
						dx = p2dx;
						dy = -1;
						if ((dir_old == 3 && dir == 0) || (dir_old == 0 && dir == 1) ||
							(dir_old == 1 && dir == 2) || (dir_old == 2 && dir == 3))
							dy *= 2;
						break;
					case 3:
						dx = p2dx;
						dy = 1;
						if ((dir_old == 3 && dir == 0) || (dir_old == 0 && dir == 1) ||
							(dir_old == 1 && dir == 2) || (dir_old == 2 && dir == 3))
							dy *= 2;
						break;
					case 0:
					case 2:
						switch (dir_old) {
						case 1: // 回転前が右向き
							dx = p2dx;
							dy = 1;
							break;
						case 3: // 回転前が左向き
							dx = p1dx;
							dy = -1;
							break;
						}
						if ((dir_old == 0 && dir == 3) || (dir_old == 3 && dir == 2) ||
							(dir_old == 2 && dir == 1) || (dir_old == 1 && dir == 0))
							dy *= 2;
						break;
					}
					if (this->collisionField(dx, dy))return false;
				}
			}
		}
	}
	this->moveMino(dx, dy);
	
	return true;
}

void Mino::moveMino(int dx, int dy)
{
	this->coord_x = field->elemXToCoordX(field_x += dx);
	this->coord_y = field->elemYToCoordY(field_y += dy);
}

void Mino::moveMinoWithCollision(int dx, int dy)
{
	if (!this->collisionField(dx,dy)) {
		this->moveMino(dx, dy);
	}
}

void Mino::dropToMaxBottom()
{
	while (!this->collisionField(0, 1)) 
	{
		this->moveMino(0, 1);
	}
}

bool Mino::collisionField()
{
	int x, y;
	for (y = 0; y < LAYOUT_SIZE; y++) {
		for (x = 0; x < LAYOUT_SIZE; x++) {
			int coll_x = field->coordXToElemX(this->coord_x + BLOCK_SIZE * x);
			int coll_y = field->coordYToElemY(this->coord_y + BLOCK_SIZE * y);
			if (this->layout[y][x] > 0 && field->getFieldValue(coll_x, coll_y) != 0) {
				return true;
			}
		}
	}
	return false;
}

bool Mino::collisionField(int dx, int dy)
{
	int x, y;
	for (y = 0; y < LAYOUT_SIZE; y++) {
		for (x = 0; x < LAYOUT_SIZE; x++) {
			int coll_x = field->coordXToElemX(this->coord_x + BLOCK_SIZE * x) + dx;
			int coll_y = field->coordYToElemY(this->coord_y + BLOCK_SIZE * y) + dy;
			if (this->layout[y][x] > 0 && field->getFieldValue(coll_x, coll_y) != 0) {
				return true;
			}
		}
	}
	return false;
}

void Mino::transcribeMinoToField()
{
	int i, j;
	for (i = 0; i < LAYOUT_SIZE; i++) {
		for (j = 0; j < LAYOUT_SIZE; j++) {
			int elem_x = field->coordXToElemX(this->coord_x) + j;
			int elem_y = field->coordYToElemY(this->coord_y) + i;
			field->setFieldValue(elem_x, elem_y, layout[i][j]);
		}
	}
}

void Mino::drawMino(bool is_ghost)
{
	int i, j;	// roop variables
	int fillflag = is_ghost ? FALSE : TRUE;

	for (i = 0; i < LAYOUT_SIZE; i++) {
		for (j = 0; j < LAYOUT_SIZE; j++) {
			if (this->layout[i][j] > 0) 
			{
				DrawBox(this->coord_x + BLOCK_SIZE * j, this->coord_y + BLOCK_SIZE * i,
					this->coord_x + BLOCK_SIZE * (j + 1) - 1, this->coord_y + BLOCK_SIZE * (i + 1) - 1,
					this->color, fillflag);
				//DrawGraph(this->coord_x + BLOCK_SIZE * j, this->coord_y + BLOCK_SIZE * i, image[layout[i][j] - 1], FALSE);
				//DrawExtendGraph(this->coord_x + BLOCK_SIZE * j, this->coord_y + BLOCK_SIZE * i,
				//	this->coord_x + BLOCK_SIZE * (j + 1) - 1, this->coord_y + BLOCK_SIZE * (i + 1) - 1, image[num - 1], FALSE);
			}
		}
	}
}

void Mino::drawMino(int alpha)
{
	int i, j;	// roop variables

	for (i = 0; i < LAYOUT_SIZE; i++) {
		for (j = 0; j < LAYOUT_SIZE; j++) {
			if (this->layout[i][j] > 0) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
				DrawBox(this->coord_x + BLOCK_SIZE * j, this->coord_y + BLOCK_SIZE * i,
					this->coord_x + BLOCK_SIZE * (j + 1) - 1, this->coord_y + BLOCK_SIZE * (i + 1) - 1,
					this->color, TRUE);
				/*DrawExtendGraph(this->coord_x + BLOCK_SIZE * j, this->coord_y + BLOCK_SIZE * i,
					this->coord_x + BLOCK_SIZE * (j + 1) - 1, this->coord_y + BLOCK_SIZE * (i + 1) - 1, image[0], TRUE);*/
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//ブレンドモードをオフ
			}
		}
	}
}

void Mino::drawStatus()
{
	//DrawFormatString(250, 0, GetColor(255, 255, 255), "coord_x:%d, coord_y:%d", coord_x, coord_y);
	//DrawFormatString(250, 20, GetColor(255, 255, 255), "field_x:%d, field_y:%d", field_x, field_y);
	DrawFormatString(250, 30, GetColor(255, 255, 255), "rotate_dir :%d",rotate_dir);
}
