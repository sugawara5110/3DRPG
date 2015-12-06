//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Battleクラス                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Battle_Header
#define Class_Battle_Header

#include "MovieSoundManager.h"
#include "Position.h"
#define METER_MAX 25000

//前方宣言
class Map;
class Parameter;
class Hero;  
class Enemy; 
class EnemySide; 
class EnemyBoss; 
//前方宣言

class Battle{

private:
	Dx9Process *dx;
	Dx9Process::MY_VERTEX2 *command, *h_select;//コマンド選択ウインドウ, 回復選択カーソル
	Dx9Process::PolygonData E_select;//敵選択カーソル
	Enemy *enemy;
	EnemySide *enemyside;
	EnemyBoss *enemyboss;
	int e_num;            //敵出現数
	bool command_run_first_flg;//コマンド選択権無の状態 == FALSE
	bool time_stop_flg;       //バトル中時間ストップフラグ
	int Escape_f;            //エスケープ表示フラグ,座標
	bool Escape_s;          //エスケープ成功

	CommandSelect com_select;   //コマンド入力
	CommandSelect E_com_select;//敵用
	MagicSelect MAG_select;   //選択したマジック
	MagicSelect E_MAG_select;//敵用
	int select_obj;           //選択対象番号敵,プレイヤー共通,エフェクト選択(4==全体)
	int E_select_obj;        //敵用(エフェクト選択用)(4==全体)

	typedef struct{
		float AGmeter;        //メーター
		Action action;
		MagicSelect Magrun;//選択マジック
		int RCVdrawY;       //回復表示フラグ,表示座標
		int RCVdata;       //回復数保管
		int DMdrawY;      //ダメージ表示フラグ,表示座標
		int DMdata;      //ダメージ数保管
		int draw_x;
		bool command_run;  //コマンドアクセス権状態,攻撃スタートフラグ
		bool LOST_fin;    //LOSTアクション終了フラグ(敵のみ)
		MenuSelect manu;//現選択メニュー(以下コマンド選択関連)
		int M_select;    //メインメニューカーソル位置
		int A_select;   //攻撃対象カーソル位置
		int MA_select; //マジック対象カーソル位置
		int R_select; //回復対象カーソル位置 
	}Draw;
	Draw *h_draw, *e_draw;
	Position::E_Pos *e_pos;
	Position::H_Pos *h_pos;

	void Debug(Enemy *enemy);//デバック用
	Battle();
	void Menucreate();
	void Cursor_h(int no);
	void Cursor_e(int select);
	void SelectPermissionMove(Hero *hero);
	CommandSelect E_AT_select(Hero *hero);
	CommandSelect Menu_main(Hero *hero, int i, Directionkey direction);
	CommandSelect Menu_AT(int i, Directionkey direction);
	CommandSelect Menu_MAG_AT(int i, Directionkey direction);
	CommandSelect Menu_RCV(Hero *hero, int i, Directionkey direction);
	CommandSelect H_AT_select(Hero *hero, int i, Directionkey direction);
	bool Escapedraw();

	template<typename T_dm, typename T_att>
	void ATprocess(T_dm *dm, T_att *att, Draw *d, Draw *at){
		if (dm->Dieflg() == FALSE){
			at->action = ATTACK;
			d->DMdata = att->GetAttack();
			time_stop_flg = TRUE;
		}
		at->AGmeter = 0;
	}

	template<typename T_dm, typename T_att>
	void MAGprocess(T_dm *p_dm, T_att *p_att, T_att *att, Draw *at, Draw *p_at, Draw *p_d, int *select_ob, MagicSelect *select_M, TemplateType type){
		int att_n;
		int dm_n;
		if (type == E_ATT){
			att_n = e_num;
			dm_n = 4;
		}
		if (type == H_ATT){
			att_n = 4;
			dm_n = e_num;
		}
		int cnt = 0;
		at->action = MAGIC;
		//MP処理
		if (att->DownMP(*select_M) == FALSE){
			//MP足りない場合の処理
			at->AGmeter = 0;
			at->Magrun = NOSEL;
			time_stop_flg = TRUE;
			return;
		}
		//マジック攻撃処理
		switch (*select_M){
		case FLAME:
			att->s_Fp(1);
			//全体攻撃
			if (*select_ob == 4){
				for (int i = 0; i < dm_n; i++){
					if (p_dm[i].Dieflg() == FALSE)cnt++;
				}
				for (int i = 0; i < dm_n; i++){
					if (p_dm[i].Dieflg() == TRUE)continue;
					p_d[i].DMdata = att->GetMagic(FLAME, cnt);
				}
			}
			else{
				//単体攻撃
				if (p_dm[*select_ob].Dieflg() == TRUE){
					//選択後攻撃直前に対象LOSTの場合の処理
					at->action = NORMAL; at->AGmeter = 0; return;
				}
				p_d[*select_ob].DMdata = att->GetMagic(FLAME, 1);
			}
			break;
		case HEAL:
			att->s_Hp(1);
			//全体攻撃
			if (*select_ob == 4){
				for (int i = 0; i < att_n; i++){
					if (p_att[i].Dieflg() == FALSE)cnt++;
				}
				for (int i = 0; i < att_n; i++){
					if (p_att[i].Dieflg() == TRUE)continue;
					p_at[i].RCVdata = att->GetMagic(HEAL, cnt);
				}
			}
			else{
				//単体攻撃
				if (p_att[*select_ob].Dieflg() == TRUE){
					//選択後回復直前に対象LOSTの場合の処理
					at->action = NORMAL;  at->AGmeter = 0; return;
				}
				p_at[*select_ob].RCVdata = att->GetMagic(HEAL, 1);
			}
			break;
		case RECOV:
			att->s_Rp(1);
			//戦闘不能者選択:復活  それ以外選択:小回復
			p_at[*select_ob].RCVdata = att->GetMagic(RECOV, 1);
			if (p_att[*select_ob].Dieflg() == TRUE){
				p_att[*select_ob].Dieflg(FALSE);
				p_at[*select_ob].action = RECOVER;
				p_at[*select_ob].LOST_fin = FALSE;
			}
			break;
		}
		at->AGmeter = 0;
		time_stop_flg = TRUE;
	}

	template<typename T_rcv>
	void RCVdraw(T_rcv *rcv, Draw *at, int DMdrawYMAX){
		char str[30];
		if (at->RCVdrawY != 0){
			if (at->RCVdrawY++ < DMdrawYMAX){
				sprintf(str, "%d", at->RCVdata);
				dx->text(str, at->draw_x, at->RCVdrawY, FALSE, 0xff00ffff);
			}
			else{
				if (at->RCVdrawY >= DMdrawYMAX && at->RCVdrawY < DMdrawYMAX + 10){
					sprintf(str, "%d", at->RCVdata);
					dx->text(str, at->draw_x, DMdrawYMAX, FALSE, 0xff00ffff);
				}
				else {
					rcv->UpHP(at->RCVdata);
					at->RCVdrawY = 0;//ここで初期化しないと数値表示されっぱなし
					time_stop_flg = FALSE;
				}
			}
		}
	}

	template<typename T_dm>
	void DMdraw(T_dm *dm, Draw *d, int DMdrawYMAX){
		char str[30];
		if (d->DMdrawY != 0){
			if (d->DMdrawY++ < DMdrawYMAX){
				sprintf(str, "%d", d->DMdata);
				dx->text(str, d->draw_x, d->DMdrawY, FALSE, 0xffffffff);
			}
			else{
				if (d->DMdrawY >= DMdrawYMAX && d->DMdrawY < DMdrawYMAX + 10){
					sprintf(str, "%d", d->DMdata);
					dx->text(str, d->draw_x, DMdrawYMAX, FALSE, 0xffffffff);
				}
				else {
					dm->DownHP(d->DMdata);
					d->DMdrawY = 0;//ここで初期化しないと数値表示されっぱなし
					time_stop_flg = FALSE;
				}
			}
		}
	}

public:
	Battle(Position::E_Pos *e_pos, Position::H_Pos *h_pos, Encount encount, int no, int e_nu);
	Result Fight(Hero *he, Directionkey direction, Result result);
	bool GetH_DM(int element);
	bool GetH_RCV(int element);
	bool GetE_DM(int element);
	bool GetE_RCV(int element);
	~Battle();
};

#endif