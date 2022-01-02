#pragma once
#include "Game2pi.h"
#include "include.h"
#include "Mino.h"
#include "MinoRow.h"
#include "Field2pi.h"
#include "BaseScene.h"
#include "DxLib.h"

Game2pi::Game2pi(ISceneChanger* changer) : BaseScene(changer)
{
    x = 380, y = 30;

    field = new Field2pi(x, y - BLOCK_SIZE * 3);
    mino = new Mino(field);
    ghost_mino = new Mino(field);
    hold_mino = new StaticMino(x - STATIC_BLOCK_SIZE * 4, y + STATIC_BLOCK_SIZE * 1.5 + 10);

    for (int i = 0; i < NEXT_REFER_SIZE; i++) {
        next_mino[i] = new StaticMino(x + FIELD_SIDE_X * BLOCK_SIZE + 70, y + i * STATIC_BLOCK_SIZE * 2.8 + 35);
        next[i] = 0;
    }

    if (fopen_s(&fp, "SaveData/highscore_normal.txt", "r") != NULL) {
        //�t�@�C���I�[�v�����s
    }
    else {
        fscanf_s(fp, "%d", &highscore);
        fclose(fp);
    }

    background_handle = LoadGraph("Images/Back_Cyber_1920.jpg");
    score_font = CreateFontToHandle("�j�R�p", 25, 1, DX_FONTTYPE_ANTIALIASING);
    pause_font = CreateFontToHandle("�j�R�p", 40, 1, DX_FONTTYPE_ANTIALIASING);
    count_font = CreateFontToHandle("�j�R�p", 75, 1, DX_FONTTYPE_ANTIALIASING);

    bgm_handle = LoadSoundMem("Sound/tetlis2pi.mp3");
    pauseSE = LoadSoundMem("SE/�x����1.mp3");

    countSE = LoadSoundMem("SE/Countdown06-1.mp3");
    moveSE = LoadSoundMem("SE/�J�[�\���ړ�2.mp3");
    rotateSE = LoadSoundMem("SE/����A�{�^������35.mp3");
    holdSE = LoadSoundMem("SE/����A�{�^������40.mp3");
    dropSE = LoadSoundMem("SE/�J�[�\���ړ�7.mp3");
    levelupSE = LoadSoundMem("SE/������ ���ʉ� �����|�C���g11.mp3");

    int SEVol = 70;
    ChangeVolumeSoundMem(255 * SEVol / 100, moveSE);
    ChangeVolumeSoundMem(255 * SEVol * 0.85 / 100, rotateSE);
    ChangeVolumeSoundMem(255 * SEVol / 100, holdSE);
    ChangeVolumeSoundMem(255 * SEVol / 100, dropSE);
    ChangeVolumeSoundMem(255 * SEVol / 100, levelupSE);

    gnrt_mx = 5, gnrt_my = 2;
    opening_count = 3;
    iscount = true;
    isGamestart = true;
    level = 1;
    score = 0;
    ren_num = 0;
    t = -1;
    lockdown_count = 0;
    autorepeat_count = 0;
    isautorepeat = false;
    new_gnrt_mino = 0;
    can_generate = false;
    can_transcribe = false;
    can_incrrow = false;
    erase_linenum = 0;
    sum_linenum = 0;
    levelup_count = 0;
    drop_speed = 60;
    isbottom = false;
    row_num = 0;
    hold_mino_num = -1;
    hold_enable = true;
    isGameOver = false;
    isGameClear = false;
    max_linenum = 100;
    ispause = false;
    NowSelect = ePause_Continue;

    pause_y = 0;
}

void Game2pi::Initialize()
{
    selectSE = LoadSoundMem("SE/�J�[�\���ړ�2.mp3");
    decisionSE = LoadSoundMem("SE/����A�{�^������26.mp3");

    menuBGM = LoadSoundMem("Sound/����A�{�^������24.mp3");
    ChangeVolumeSoundMem(255 * 80 / 100, menuBGM);
    PlaySoundMem(countSE, DX_PLAYTYPE_BACK);
}

void Game2pi::StartCountDown()
{
    StopSoundMem(bgm_handle);
    if (t % 45 == 0) {
        opening_count--;
    }
    if (opening_count < 0) {
        t = -1;
        iscount = false;
        opening_count = 2;
        PlaySoundMem(bgm_handle, DX_PLAYTYPE_LOOP);

        if (isGamestart) {
            mino->generateMino(row.getMinoNum(0), gnrt_mx, gnrt_my);
            row.incrSeaqNum();
        }
        isGamestart = false;
    }
    t++;
}

void Game2pi::Update()
{
    if (Key[PAD_12] == 1 && !iscount) {
        PlaySoundMem(pauseSE, DX_PLAYTYPE_BACK);
        ispause = true;
        iscount = false;
    }

    if (ispause) pause();
    else if (iscount) StartCountDown();
    else if (isGameClear) GameClear();
    else if (isGameOver) GameOver();
    else {
        installMino();
        controlMino();
        dropMino();
        holdMino();
        makeGhost();
        setNext();

        GameResult();
        t++;
    }
}

void Game2pi::controlMino()
{
    //���E�ړ�����
    if (Key[PAD_LEFT] >= 1) {
        lockdown_count = 0;

        if (autorepeat_count == 0) {
            mino->moveMinoWithCollision(-1, 0);
            PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
            autorepeat_count++;
        }
        else if (isautorepeat && t % 3 == 0) {
            mino->moveMinoWithCollision(-1, 0);
            PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
        }
        else if (autorepeat_count == 9) {
            isautorepeat = true;
        }
        else {
            autorepeat_count++;
        }
    }

    if (Key[PAD_RIGHT] >= 1) {
        lockdown_count = 0;

        if (autorepeat_count == 0) {
            mino->moveMinoWithCollision(1, 0);
            PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
            autorepeat_count++;
        }
        else if (isautorepeat && t % 3 == 0) {
            mino->moveMinoWithCollision(1, 0);
            PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
        }
        else if (autorepeat_count == 9) {
            isautorepeat = true;
        }
        else {
            autorepeat_count++;
        }
    }

    //��]����iSRS�t���j
    if (Key[PAD_4] == 1)
    {
        mino->rotateMinoWithCollision(true);
        PlaySoundMem(rotateSE, DX_PLAYTYPE_BACK);
        lockdown_count = 0;
    }
    if (Key[PAD_3] == 1)
    {
        mino->rotateMinoWithCollision(false);
        PlaySoundMem(rotateSE, DX_PLAYTYPE_BACK);
        lockdown_count = 0;
    }

    if (!CheckHitKeyAll(DX_CHECKINPUT_PAD)) {
        autorepeat_count = 0;
        isautorepeat = false;
    }
}

void Game2pi::dropMino()
{
    if (Key[PAD_UP] == 1) {    //�n�[�h�h���b�v

        mino->dropToMaxBottom();
        new_gnrt_mino = row.getMinoNum(0);
        can_generate = true;
        can_transcribe = true;
        can_incrrow = true;
        hold_enable = true;
    }
    else if (Key[PAD_DOWN] >= 1 && t % 3 == 0) {   //�\�t�g�h���b�v
        mino->moveMinoWithCollision(0, 1);
        PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
    }
    else if (t % drop_speed == 0) {      //�i�`�������h���b�v
        if (!mino->collisionField(0, 1)) {
            mino->moveMino(0, 1);
            isbottom = false;
        }
        else isbottom = true;
    }

    if (isbottom) {
        if (lockdown_count == 30) {
            new_gnrt_mino = row.getMinoNum(0);
            can_generate = true;
            can_transcribe = true;
            can_incrrow = true;
            hold_enable = true;

            lockdown_count = 0;

            isbottom = false;
        }
        else {
            lockdown_count++;
        }
    }
}

void Game2pi::holdMino()
{
    if ((Key[PAD_5] == 1 || Key[PAD_7] == 1) && hold_enable) {
        PlaySoundMem(holdSE, DX_PLAYTYPE_BACK);
        if (hold_mino_num == -1) {
            hold_mino_num = mino->getMinoNum();
            hold_mino->generateMino(hold_mino_num);

            new_gnrt_mino = row.getMinoNum(0);
            can_generate = true;
            can_transcribe = false;
            can_incrrow = true;
        }
        else {
            int tmp_mino_num = hold_mino_num;
            hold_mino_num = mino->getMinoNum();
            hold_mino->generateMino(hold_mino_num);

            new_gnrt_mino = tmp_mino_num;
            can_generate = true;
            can_transcribe = false;
            can_incrrow = false;
        }
        hold_enable = false;
    }
}

void Game2pi::makeGhost()
{
    *ghost_mino = *mino;
    ghost_mino->dropToMaxBottom();
}

void Game2pi::setNext()
{
    for (int i = 0; i < NEXT_REFER_SIZE; i++) {
        next_mino[i]->generateMino(row.getMinoNum(i));
    }
}

int Game2pi::calcScore(int _level, int _drop_speed)
{
    int _score = 0;

    int tmp_erase_linenum = erase_linenum;
    erase_linenum = field->LineEraseAndShift();
    if (tmp_erase_linenum != 0 && erase_linenum != 0)
        ren_num++;

    int drop_score = (20 * _drop_speed / 60 - t / 60);
    int line_score = _level * 100 * erase_linenum;

    _score = (drop_score + line_score) * (ren_num + 1);

    return _score;
}

void Game2pi::levelControl()
{
    int tmp_level = level;
    sum_linenum += erase_linenum;
    level = sum_linenum / 10 + 1;
    if (level != tmp_level) {
        PlaySoundMem(levelupSE, DX_PLAYTYPE_BACK);
        if (drop_speed != 1) {
            drop_speed /= 1.3;
        }
    }
}

void Game2pi::pause()
{
    if (ispause) {
        if (Key[PAD_DOWN] == 1) {                 //���L�[��������Ă�����
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + 1) % ePause_Num;   //�I����Ԃ��������
        }
        if (Key[PAD_UP] == 1) {//��L�[��������Ă�����
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + (ePause_Num - 1)) % ePause_Num;    //�I����Ԃ���グ��
        }
        if (Key[PAD_4] == 1) {    //�G���^�[�L�[�������ꂽ��
            PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK);
            switch (NowSelect) {        //���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
            case ePause_Continue:
                iscount = true;
                ispause = false;
                PlaySoundMem(countSE, DX_PLAYTYPE_BACK);
                break;
            case ePause_Restart:
                StopSoundMem(bgm_handle);
                mSceneChanger->ChangeScene(eScene_Normal);
                break;
            case ePause_End://�ݒ�I�𒆂Ȃ�
                StopSoundMem(bgm_handle);
                mSceneChanger->ChangeScene(eScene_Menu);
                break;
            }
        }
    }
}

void Game2pi::installMino()
{
    if (can_generate) {
        if (can_transcribe) {
            PlaySoundMem(dropSE, DX_PLAYTYPE_BACK);
            mino->transcribeMinoToField();
        }

        delete(mino);
        mino = new Mino(field);
        mino->generateMino(new_gnrt_mino, gnrt_mx, gnrt_my);

        if (can_incrrow)row.incrSeaqNum();

        score += calcScore(level, drop_speed);

        levelControl();
        erase_linenum = 0;

        can_generate = false;
        can_transcribe = false;
        can_incrrow = false;
        t = 0;
    }
}

void Game2pi::GameResult()
{
    if (field->containMino(gnrt_mx, gnrt_my) || field->containMino(gnrt_mx + 1, gnrt_my)) {
        StopSoundMem(bgm_handle);
        PlaySoundMem(menuBGM, DX_PLAYTYPE_BACK);
        isGameOver = true;
    }
    else if (sum_linenum >= max_linenum) {
        StopSoundMem(bgm_handle);
        PlaySoundMem(menuBGM, DX_PLAYTYPE_BACK);
        isGameClear = true;
    }
}

void Game2pi::GameClear()
{
    if (isGameClear) {

        if (Key[PAD_DOWN] == 1) {                 //���L�[��������Ă�����
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + 1) % eResult_Num;   //�I����Ԃ��������
        }
        if (Key[PAD_UP] == 1) {//��L�[��������Ă�����
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + (eResult_Num - 1)) % eResult_Num;    //�I����Ԃ���グ��
        }
        if (Key[PAD_4] == 1) {    //�G���^�[�L�[�������ꂽ��
            PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK);
            switch (NowSelect) {        //���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
            case eResult_Restart:
                mSceneChanger->ChangeScene(eScene_Normal);
                break;
            case eResult_End://�ݒ�I�𒆂�
                mSceneChanger->ChangeScene(eScene_Menu);
                break;
            }
            StopSoundMem(menuBGM);
        }
    }
}

void Game2pi::GameOver()
{
    if (isGameOver) {
        if (Key[PAD_DOWN] == 1) {                 //���L�[��������Ă�����
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + 1) % eResult_Num;   //�I����Ԃ��������
        }
        if (Key[PAD_UP] == 1) {//��L�[��������Ă�����
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + (eResult_Num - 1)) % eResult_Num;    //�I����Ԃ���グ��
        }
        if (Key[PAD_4] == 1) {    //�G���^�[�L�[�������ꂽ��
            PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK);
            switch (NowSelect) {        //���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
            case eResult_Restart:
                mSceneChanger->ChangeScene(eScene_Normal);
                break;
            case eResult_End://�ݒ�I�𒆂Ȃ�
                mSceneChanger->ChangeScene(eScene_Menu);
                break;
            }
            StopSoundMem(menuBGM);
        }
    }
}

void Game2pi::Draw()
{
    DrawGraph(0, 0, background_handle, TRUE);
    int drx = x - STATIC_BLOCK_SIZE * 5, dry = y;
    DrawRoundRect(drx, dry, drx + STATIC_BLOCK_SIZE * 5 + 10, dry + STATIC_BLOCK_SIZE * 4 + 10, 10, 10, GetColor(0, 0, 0), TRUE);
    drx = x + FIELD_SIDE_X * BLOCK_SIZE + 50, dry = y;
    DrawRoundRect(drx, dry, drx + STATIC_BLOCK_SIZE * 5 + 10, dry + STATIC_BLOCK_SIZE * 6 * 3, 10, 10, GetColor(0, 0, 0), TRUE);

    field->drawField();
    ghost_mino->drawMino(true);
    mino->drawMino(false);
    if (hold_enable)
        hold_mino->drawMino(false);
    else
        hold_mino->drawMino(100);
    for (int i = 0; i < NEXT_REFER_SIZE; i++) {
        next_mino[i]->drawMino(false);
    }

    DrawStringToHandle(x - STATIC_BLOCK_SIZE * 5 + 5, y + 5, "HOLD", GetColor(255, 255, 255), score_font);
    DrawStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 50 + 5, y + 5, "NEXT", GetColor(255, 255, 255), score_font);
    DrawStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 50, y + STATIC_BLOCK_SIZE * 6 * 3, "SCORE", GetColor(255, 255, 255), score_font);
    DrawFormatStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 70, y + STATIC_BLOCK_SIZE * 6 * 3 + 20, GetColor(255, 255, 255), pause_font, "%d", score);
    DrawStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 50, y + STATIC_BLOCK_SIZE * 6 * 3 + 60, "HIGH SCORE", GetColor(255, 255, 255), score_font);
    DrawFormatStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 70, y + STATIC_BLOCK_SIZE * 6 * 3 + 80, GetColor(255, 255, 255), pause_font, "%d", highscore);
    DrawStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 50, y + STATIC_BLOCK_SIZE * 6 * 3 + 120, "LEVEL", GetColor(255, 255, 255), score_font);
    DrawFormatStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 70, y + STATIC_BLOCK_SIZE * 6 * 3 + 140, GetColor(255, 255, 255), pause_font, "%d", level);
    DrawStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 50, y + STATIC_BLOCK_SIZE * 6 * 3 + 180, "LINES", GetColor(255, 255, 255), score_font);
    DrawFormatStringToHandle(x + FIELD_SIDE_X * BLOCK_SIZE + 70, y + STATIC_BLOCK_SIZE * 6 * 3 + 200, GetColor(255, 255, 255), pause_font, "%d", sum_linenum);

    if (iscount) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//�u�����h���[�h���I�t
        if (opening_count > 0) {
            DrawFormatStringToHandle(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2 - 50, GetColor(255, 255, 255), count_font, "%d", opening_count);
        }
        else DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, WINDOW_SIZE_Y / 2 - 50, "START", GetColor(255, 255, 255), count_font);
    }

    if (ispause) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//�u�����h���[�h���I�t

        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, CONTINUE_Y, "�Â���", GetColor(255, 255, 255), pause_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, RESTART_Y, "���Ȃ���", GetColor(255, 255, 255), pause_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, END_Y, "��߂�", GetColor(255, 255, 255), pause_font);

        switch (NowSelect) {//���݂̑I����Ԃɏ]���ď����𕪊�
        case ePause_Continue://�Q�[���I�𒆂Ȃ�
            pause_y = CONTINUE_Y;    //�Q�[���̍��W���i�[
            break;
        case ePause_Restart://�ݒ�I�𒆂Ȃ�
            pause_y = RESTART_Y;    //�ݒ�̍��W���i�[
            break;
        case ePause_End://�ݒ�I�𒆂Ȃ�
            pause_y = END_Y;    //�ݒ�̍��W���i�[
            break;
        }
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, pause_y, "��", GetColor(255, 255, 255), pause_font);
    }
    else if (isGameClear) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//�u�����h���[�h���I�t

        if (score > highscore) {

            if (fopen_s(&fp, "SaveData/highscore_normal.txt", "w") != NULL) {
                //�t�@�C���I�[�v�����s
            }
            else {
                fprintf(fp, "%d", score);
                fclose(fp);
            }
            DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, 180, "HIGH SCORE", GetColor(255, 255, 255), pause_font);
        }


        DrawStringToHandle(WINDOW_SIZE_X / 2 - 200, 100, "GAME CLEAR", GetColor(255, 255, 255), count_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, CONTINUE_Y, "���������ǗV��", GetColor(255, 255, 255), pause_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, END_Y, "���j���[�ɖ߂�", GetColor(255, 255, 255), pause_font);

        switch (NowSelect) {//���݂̑I����Ԃɏ]���ď����𕪊�
        case eResult_Restart://�Q�[���I�𒆂Ȃ�
            pause_y = CONTINUE_Y;    //�Q�[���̍��W���i�[
            break;
        case eResult_End://�ݒ�I�𒆂Ȃ�
            pause_y = END_Y;    //�ݒ�̍��W���i�[
            break;
        }
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, pause_y, "��", GetColor(255, 255, 255), pause_font);
    }
    else if (isGameOver) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//�u�����h���[�h���I�t

        DrawStringToHandle(WINDOW_SIZE_X / 2 - 200, 100, "GAME OVER", GetColor(255, 255, 255), count_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, CONTINUE_Y, "���������ǗV��", GetColor(255, 255, 255), pause_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, END_Y, "���j���[�ɖ߂�", GetColor(255, 255, 255), pause_font);

        switch (NowSelect) {//���݂̑I����Ԃɏ]���ď����𕪊�
        case eResult_Restart://�Q�[���I�𒆂Ȃ�
            pause_y = CONTINUE_Y;    //�Q�[���̍��W���i�[
            break;
        case eResult_End://�ݒ�I�𒆂Ȃ�
            pause_y = END_Y;    //�ݒ�̍��W���i�[
            break;
        }
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, pause_y, "��", GetColor(255, 255, 255), pause_font);
    }
}

void Game2pi::Finalize()
{
    delete(mino);
    delete(field);
    delete(ghost_mino);
    delete(hold_mino);

    for (int i = 0; i < NEXT_REFER_SIZE; i++) {
        delete(next_mino[i]);
    }
}
