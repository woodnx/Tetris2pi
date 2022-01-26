#pragma once
#include "Game1P.h"
#include "include.h"
#include "Mino.h"
#include "MinoRow.h"
#include "Field.h"
#include "BaseScene.h"
#include "DxLib.h"

Game1P::Game1P(ISceneChanger* changer) : BaseScene(changer)
{
    x = 380, y = 50;

	field = new Field(x, y - BLOCK_SIZE * 5);
	mino = new Mino(field);
    ghost_mino = new Mino(field);
    hold_mino = new StaticMino(x - STATIC_BLOCK_SIZE * 4, y + STATIC_BLOCK_SIZE * 1.5 + 10);

    for (int i = 0; i < NEXT_REFER_SIZE; i++) {
        next_mino[i] = new StaticMino(x + FIELD_SIDE_X * BLOCK_SIZE + 70, y + i * STATIC_BLOCK_SIZE * 2.8 + 35);
        next[i] = 0;
    }

    if (fopen_s(&fp, "SaveData/highscore_normal.txt", "r") != NULL) {
        //ファイルオープン失敗
    }
    else {
        fscanf_s(fp, "%d", &highscore);
        fclose(fp);
    }

    background_handle = LoadGraph("Images/Back_Cyber_1920.jpg");
    score_font = CreateFontToHandle("ニコ角", 25, 1, DX_FONTTYPE_ANTIALIASING);
    pause_font = CreateFontToHandle("ニコ角", 40, 1, DX_FONTTYPE_ANTIALIASING);
    count_font = CreateFontToHandle("ニコ角", 75, 1, DX_FONTTYPE_ANTIALIASING);

    bgm_handle = LoadSoundMem("Sound/tetlis2pi.mp3");
    pauseSE = LoadSoundMem("SE/警告音1.mp3");
    
    countSE = LoadSoundMem("SE/Countdown06-1.mp3");
    moveSE = LoadSoundMem("SE/カーソル移動2.mp3");
    rotateSE = LoadSoundMem("SE/決定、ボタン押下35.mp3");
    holdSE = LoadSoundMem("SE/決定、ボタン押下40.mp3");
    dropSE = LoadSoundMem("SE/カーソル移動7.mp3");
    levelupSE = LoadSoundMem("SE/魔王魂 効果音 ワンポイント11.mp3");

    int SEVol = 70;
    ChangeVolumeSoundMem(255 * SEVol / 100, moveSE);
    ChangeVolumeSoundMem(255 * SEVol * 0.85 / 100, rotateSE);
    ChangeVolumeSoundMem(255 * SEVol / 100, holdSE);
    ChangeVolumeSoundMem(255 * SEVol / 100, dropSE);
    ChangeVolumeSoundMem(255 * SEVol / 100, levelupSE);

    gnrt_mx = FIELD_SIDE_X / 2, gnrt_my = 4;
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

void Game1P::Initialize()
{
    selectSE = LoadSoundMem("SE/カーソル移動2.mp3");
    decisionSE = LoadSoundMem("SE/決定、ボタン押下26.mp3");

    menuBGM = LoadSoundMem("Sound/決定、ボタン押下24.mp3");
    ChangeVolumeSoundMem(255 * 80 / 100, menuBGM);
    PlaySoundMem(countSE, DX_PLAYTYPE_BACK);
}

void Game1P::StartCountDown()
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
            mino->generateMinoWithPos(row.getMinoNum(0), gnrt_mx, gnrt_my);
            row.increase();
        }
        isGamestart = false;
    }
    t++;
}

void Game1P::RestartGame()
{
    mino->initialize();
    field->InitField();
    row.initialize();
    ghost_mino->initialize();
    hold_mino->initialize();

    for (int i = 0; i < NEXT_REFER_SIZE; i++) {
        next_mino[i]->initialize();
        next[i] = 0;
    }

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

    PlaySoundMem(countSE, DX_PLAYTYPE_BACK);
}

void Game1P::Update()
{
    if (Key[KEY_INPUT_ESCAPE] == 1 && !iscount) {
        PlaySoundMem(pauseSE, DX_PLAYTYPE_BACK);
        ispause = true;
        iscount = false;
    }

    if (ispause) pause();
    else if (iscount) StartCountDown();
    else if (isGameClear) GameClear();
    else if (isGameOver) GameOver();
    else {
        GameResult();
        installMino();
        controlMino();
        dropMino();
        holdMino();
        makeGhost();
        setNext();

        t++;
    }
}

void Game1P::controlMino()
{
    //左右移動操作
    //PAD PAD_LEFT
    if (Key[KEY_INPUT_A] >= 1) {
        if (mino->getMinoCoordX() != pre_mino_coordx && mino->getMinoCoordY() != pre_mino_coordy)
            lockdown_count = 0;

        if (autorepeat_count == 0) {
            if (!mino->collisionField(-1, 0)) {
                mino->moveMino(-1, 0);
                PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
            }
            autorepeat_count++;
        }
        else if (isautorepeat && t % 3 == 0) {
            if (!mino->collisionField(-1, 0)) {
                mino->moveMino(-1, 0);
                PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
            }
        }
        else if (autorepeat_count == 9) {
            isautorepeat = true;
        }
        else {
            autorepeat_count++;
        }
    }
    //PAD PAD_RIGHT
    if (Key[KEY_INPUT_D] >= 1) {
        if(mino->getMinoCoordX() != pre_mino_coordx && mino->getMinoCoordY() != pre_mino_coordy)
            lockdown_count = 0;

        if (autorepeat_count == 0) {
            if (!mino->collisionField(1, 0)) {
                mino->moveMino(1, 0);
                PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
            }
            autorepeat_count++;
        }
        else if (isautorepeat && t % 3 == 0) {
            if (!mino->collisionField(1, 0)) {
                mino->moveMino(1, 0);
                PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
            }
        }
        else if (autorepeat_count == 9) {
            isautorepeat = true;
        }
        else {
            autorepeat_count++;
        }
    }

    //回転操作（SRS付き）
    //PAD PAD_4
    if (Key[KEY_INPUT_L] == 1)
    {
        mino->rotateMinoWithCollision(true);
        PlaySoundMem(rotateSE, DX_PLAYTYPE_BACK);
        lockdown_count = 0;
    }
    //PAD PAD_3
    if (Key[KEY_INPUT_K] == 1)
    {
        mino->rotateMinoWithCollision(false);
        PlaySoundMem(rotateSE, DX_PLAYTYPE_BACK);
        lockdown_count = 0;
    }

    if (!CheckHitKeyAll(DX_CHECKINPUT_KEY)) {
        autorepeat_count = 0;
        isautorepeat = false;
    }
}

void Game1P::dropMino()
{
    //PAD PAD_UP
    if (Key[KEY_INPUT_W] == 1) {    //ハードドロップ

        mino->dropToMaxBottom();
        new_gnrt_mino = row.getMinoNum(0);
        can_generate = true;
        can_transcribe = true;
        can_incrrow = true;
        hold_enable = true;
    }//PAD PAD_DOWN
    else if (Key[KEY_INPUT_S] >= 1 && t % 3 == 0) {   //ソフトドロップ
        if (!mino->collisionField(0, 1)) {
            mino->moveMino(0, 1);
            PlaySoundMem(moveSE, DX_PLAYTYPE_BACK);
        }
        else {
            isbottom = true;
        }
    }
    else if (t % drop_speed == 0) {      //ナチュラルドロップ
        if (!mino->collisionField(0, 1)) {
            mino->moveMino(0, 1);
            isbottom = false;
        }
        else {
            isbottom = true;
            pre_mino_coordx = mino->getMinoCoordX();
            pre_mino_coordy = mino->getMinoCoordY();
        }
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
        else if (!mino->collisionField(0, 1)) {
            can_generate = false;
            can_transcribe = false;
            can_incrrow = false;

            lockdown_count = 0;

            isbottom = false;
        }
        else {
            lockdown_count++;
        }
    }
}

void Game1P::holdMino()
{
    //PAD Key[PAD_5] == 1 || Key[PAD_7] == 1
    if (Key[KEY_INPUT_SPACE] && hold_enable) {
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

void Game1P::makeGhost()
{
    *ghost_mino = *mino;
    ghost_mino->dropToMaxBottom();
}

void Game1P::setNext()
{
    for (int i = 0; i < NEXT_REFER_SIZE; i++) {
        next_mino[i]->generateMino(row.getMinoNum(i));
    }
}

int Game1P::calcScore(int _level, int _drop_speed)
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

void Game1P::levelControl()
{
    int tmp_level = level;
    sum_linenum += erase_linenum;
    level = sum_linenum / 10 + 1;
    if (level != tmp_level) {
        PlaySoundMem(levelupSE, DX_PLAYTYPE_BACK);
        if (drop_speed != 1) {
            drop_speed /= 1.5;
        }
    }
}

void Game1P::pause()
{
    if (ispause) {
        if (Key[KEY_INPUT_S] == 1) {                 //下キーが押されていたら
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + 1) % ePause_Num;   //選択状態を一つ下げる
        }
        if (Key[KEY_INPUT_W] == 1) {//上キーが押されていたら
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + (ePause_Num - 1)) % ePause_Num;    //選択状態を一つ上げる
        }
        if (Key[KEY_INPUT_SPACE] == 1) {    //エンターキーが押されたら
            PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK);
            switch (NowSelect) {        //現在選択中の状態によって処理を分岐
            case ePause_Continue: 
                iscount = true;
                ispause = false;
                PlaySoundMem(countSE, DX_PLAYTYPE_BACK);
                break;                
            case ePause_Restart:
                StopSoundMem(bgm_handle);
                RestartGame();
                break;
            case ePause_End://設定選択中なら
                StopSoundMem(bgm_handle);
                mSceneChanger->ChangeScene(eScene_Menu);
                break;
            }
        }
    }
}

void Game1P::installMino()
{
    if (can_generate){
        if (can_transcribe) {
            PlaySoundMem(dropSE, DX_PLAYTYPE_BACK);
            mino->transcribeMinoToField();
        }

        mino->generateMinoWithPos(new_gnrt_mino, gnrt_mx, gnrt_my);

        if(can_incrrow)row.increase();

        score += calcScore(level, drop_speed);

        levelControl();
        erase_linenum = 0;

        can_generate = false;
        can_transcribe = false;
        can_incrrow = false;
        t = 0;
    }
}

void Game1P::GameResult()
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

void Game1P::GameClear()
{
    if (isGameClear) {

        if (Key[KEY_INPUT_S] == 1) {                 //下キーが押されていたら
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + 1) % eResult_Num;   //選択状態を一つ下げる
        }
        if (Key[KEY_INPUT_W] == 1) {//上キーが押されていたら
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + (eResult_Num - 1)) % eResult_Num;    //選択状態を一つ上げる
        }
        if (Key[KEY_INPUT_SPACE] == 1) {    //エンターキーが押されたら
            PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK);
            switch (NowSelect) {        //現在選択中の状態によって処理を分岐
            case eResult_Restart:
                RestartGame();
                break;
            case eResult_End://設定選択中な
                mSceneChanger->ChangeScene(eScene_Menu);
                break;
            }
            StopSoundMem(menuBGM);
        }
    }
}

void Game1P::GameOver()
{
    if (isGameOver) {
        if (Key[KEY_INPUT_S] == 1) {                 //下キーが押されていたら
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + 1) % eResult_Num;   //選択状態を一つ下げる
        }
        if (Key[KEY_INPUT_W] == 1) {//上キーが押されていたら
            PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);
            NowSelect = (NowSelect + (eResult_Num - 1)) % eResult_Num;    //選択状態を一つ上げる
        }
        if (Key[KEY_INPUT_SPACE] == 1) {    //エンターキーが押されたら
            PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK);
            switch (NowSelect) {        //現在選択中の状態によって処理を分岐
            case eResult_Restart:
                RestartGame();
                break;
            case eResult_End://設定選択中なら
                mSceneChanger->ChangeScene(eScene_Menu);
                break;
            }
            StopSoundMem(menuBGM);
        }
    }
}

void Game1P::Draw()
{
    DrawGraph(0, 0, background_handle, TRUE);
    int drx = x - STATIC_BLOCK_SIZE * 5, dry = y;
    DrawRoundRect(drx, dry, drx + STATIC_BLOCK_SIZE * 5 + 10, dry + STATIC_BLOCK_SIZE * 4 + 10, 10, 10, GetColor(0, 0, 0), TRUE);
    drx = x + FIELD_SIDE_X * BLOCK_SIZE + 50, dry = y;
    DrawRoundRect(drx, dry, drx + STATIC_BLOCK_SIZE * 5 + 10, dry + STATIC_BLOCK_SIZE * 6 * 3, 10, 10, GetColor(0, 0, 0), TRUE);

    field->drawField();
    ghost_mino->draw(false);
    mino->draw(true);
    if(hold_enable)
        hold_mino->draw(true);
    else 
        hold_mino->draw(true, 100);
    for (int i = 0; i < NEXT_REFER_SIZE; i++) {
        next_mino[i]->draw(true);
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
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//ブレンドモードをオフ
        if (opening_count > 0) {
            DrawFormatStringToHandle(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2 - 50, GetColor(255, 255, 255), count_font, "%d", opening_count);
        }
        else DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, WINDOW_SIZE_Y / 2 - 50, "START", GetColor(255, 255, 255), count_font);
    }
    
    if (ispause) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//ブレンドモードをオフ

        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, CONTINUE_Y, "つづける", GetColor(255, 255, 255), pause_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, RESTART_Y,  "やりなおす", GetColor(255, 255, 255), pause_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, END_Y,      "やめる", GetColor(255, 255, 255), pause_font);

        switch (NowSelect) {//現在の選択状態に従って処理を分岐
        case ePause_Continue://ゲーム選択中なら
            pause_y = CONTINUE_Y;    //ゲームの座標を格納
            break;
        case ePause_Restart://設定選択中なら
            pause_y = RESTART_Y;    //設定の座標を格納
            break;
        case ePause_End://設定選択中なら
            pause_y = END_Y;    //設定の座標を格納
            break;
        }
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, pause_y, "■", GetColor(255, 255, 255), pause_font);
    }
    else if (isGameClear) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//ブレンドモードをオフ

        if (score > highscore) { 

            if (fopen_s(&fp, "SaveData/highscore_normal.txt", "w") != NULL) {
                //ファイルオープン失敗
            }
            else {
                fprintf(fp, "%d", score);
                fclose(fp);
            }
            DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, 180, "HIGH SCORE", GetColor(255, 255, 255), pause_font);
        }


        DrawStringToHandle(WINDOW_SIZE_X / 2 - 200,100, "GAME CLEAR", GetColor(255, 255, 255), count_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, CONTINUE_Y, "もういちど遊ぶ", GetColor(255, 255, 255), pause_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, END_Y, "メニューに戻る", GetColor(255, 255, 255), pause_font);

        switch (NowSelect) {//現在の選択状態に従って処理を分岐
        case eResult_Restart://ゲーム選択中なら
            pause_y = CONTINUE_Y;    //ゲームの座標を格納
            break;
        case eResult_End://設定選択中なら
            pause_y = END_Y;    //設定の座標を格納
            break;
        }
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, pause_y, "■", GetColor(255, 255, 255), pause_font);
    }
    else if (isGameOver) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(0, 0, WINDOW_SIZE_X - 1, WINDOW_SIZE_Y - 1, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		//ブレンドモードをオフ

        DrawStringToHandle(WINDOW_SIZE_X / 2 - 200, 100, "GAME OVER", GetColor(255, 255, 255), count_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, CONTINUE_Y, "もういちど遊ぶ", GetColor(255, 255, 255), pause_font);
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 50, END_Y, "メニューに戻る", GetColor(255, 255, 255), pause_font);

        switch (NowSelect) {//現在の選択状態に従って処理を分岐
        case eResult_Restart://ゲーム選択中なら
            pause_y = CONTINUE_Y;    //ゲームの座標を格納
            break;
        case eResult_End://設定選択中なら
            pause_y = END_Y;    //設定の座標を格納
            break;
        }
        DrawStringToHandle(WINDOW_SIZE_X / 2 - 100, pause_y, "■", GetColor(255, 255, 255), pause_font);
    }
}

void Game1P::Finalize()
{
    delete(mino);
    delete(field);
    delete(ghost_mino);
    delete(hold_mino);

    InitFontToHandle();

    for (int i = 0; i < NEXT_REFER_SIZE; i++) {
        delete(next_mino[i]);
    }
}
