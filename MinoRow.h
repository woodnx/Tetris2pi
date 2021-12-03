#pragma once
#define BAG_NUM 2
#define NEXT_REFER_SIZE 6

struct Bag {
	int body[7] = { 1,2,3,4,5,6,7 };
	int nowelem = 0;
	bool isShuffled = false;
};

class MinoRow
{
public:
	MinoRow();

	void InitMinoRow();
	void incrSeaqNum();
	int getMinoNum(int refer_row);

private:
	void RowShuffle(Bag* _bag);
	void shiftRow();

	Bag bag[BAG_NUM];
	bool now_bagnum = 0;
};

