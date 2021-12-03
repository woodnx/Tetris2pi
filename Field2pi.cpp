#include "Field2pi.h"
#include "DxLib.h"
#include "Field.h"

Field2pi::Field2pi(int x, int y)
{
	int i, j;
	for (i = 0; i < FIELD_ASIDE_Y; i++) {
		for (j = 0; j < FIELD_ASIDE_X; j++) {
			if ((j == 0 || j == FIELD_ASIDE_X - 1 || i == FIELD_ASIDE_Y - 1)) {
				this->body[i][j] = -1;
			}
		}
	}
	this->x = x, this->y = y;
	color = GetColor(255, 255, 255);
	grid_color = GetColor(64, 64, 64);
}

void Field2pi::fieldRotate(int rotate_num, bool right_flag)
{
	int x, y;
	int X = 0, Y = 0;
	int lr = 1;
	float a = 0, b = 0;
	Field2pi tmp_field = *this;

	a = 7, b = 7;

	if (right_flag) { lr = -1; }

	//rotate_dir change
	if (right_flag) {
		rotate_dir += rotate_num;
		if (rotate_dir >= 4) { rotate_dir = rotate_dir % 4; }
	}
	else {
		rotate_dir -= rotate_num;
		if (rotate_dir <= -1) { rotate_dir = (4 + rotate_dir) % 4; }
	}

	// Initialize _mino
	for (x = 0; x < FIELD_ASIDE_Y; x++) {
		for (y = 0; y < FIELD_ASIDE_Y; y++) {
			body[y][x] = 0;
		}
	}
	if (rotate_dir != 2) {
		for (int i = 0; i < rotate_num; i++) {
			for (y = 0; y < FIELD_ASIDE_Y; y++) {
				for (x = 0; x < FIELD_ASIDE_Y; x++) {
					// ‰ñ“]s—ñ‚ðŽg‚Á‚Ä‰ñ“]
					// •ÏŠ·Ž®‚Í‰º‚Ì’Ê‚èi x -> X, y -> Y )
					// --------------------------------------------------
					// | X:  cos(90)( x - a ) - sin(90)( y - b ) + a	|
					// |	 = -y + b + a								|
					// |												|
					// | Y:  sin(90)( x - a ) + cos(90)( y - b ) + b	|
					// |	 =  x - a + b								|
					// --------------------------------------------------

					X = (int)(lr * (-y + a) + b);
					Y = (int)(lr * (x - a) + b);
					if (X < 0 || Y < 0) continue;

					this->body[y][x] = tmp_field.body[Y][X];
				}
			}
		}
	}
	else {
		for (y = 0; y < FIELD_ASIDE_Y; y++) {
			for (x = 0; x < FIELD_ASIDE_Y; x++) {
				X = FIELD_SIDE_X - x;
				Y = FIELD_SIDE_Y - y;
				if (X < 0 || Y < 0) continue;

				this->body[y][x] = tmp_field.body[Y][X];
			}
		}
	}

}

int Field2pi::LineEraseAndShift()
{

}
