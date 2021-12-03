#pragma once
#include "MinoRow.h"
#include "DxLib.h"

MinoRow::MinoRow()
{
    RowShuffle(&bag[now_bagnum]);
}

void MinoRow::InitMinoRow()
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 7; j++) {
            this->bag[i].body[j] = j + 1;
        }

        this->bag[i].nowelem = 0;
        this->bag[i].isShuffled = false;
    }
    now_bagnum = 0;

    this->RowShuffle(&bag[now_bagnum]);
}

void MinoRow::incrSeaqNum()
{
    bag[now_bagnum].nowelem++;
    if (bag[now_bagnum].nowelem == 7) {
        shiftRow();
    }
    if (bag[now_bagnum].nowelem + NEXT_REFER_SIZE == 7) {
        RowShuffle(&bag[!now_bagnum]);
    }
}

int MinoRow::getMinoNum(int elem)
{
    int refelem = bag[now_bagnum].nowelem + elem;

    if (refelem < 7) {
        return bag[now_bagnum].body[refelem] - 1;
    }
    else {
        int ref_bagnum = !now_bagnum;
        return bag[ref_bagnum].body[refelem % 7] - 1;
    }
}

void MinoRow::RowShuffle(Bag* _bag)
{
    for (int i = 6; i > 0; i--) {
        int r = GetRand(6);
        int tmp = _bag->body[i];
        _bag->body[i] = _bag->body[r];
        _bag->body[r] = tmp;
    }
    _bag[now_bagnum].isShuffled = true;
}

void MinoRow::shiftRow()
{   
    now_bagnum = !now_bagnum;
    bag[now_bagnum].nowelem = 0;
}
