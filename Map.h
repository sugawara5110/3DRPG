//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Map_Header
#define Class_Map_Header

#include "MovieSoundManager.h"
#include "Position.h"

#define POS_CE  (posz * mxy.y * mxy.x + posy * mxy.x + posx)
#define POSY_U1 (posz * mxy.y * mxy.x + (posy + 1) * mxy.x + posx)
#define POSY_D1 (posz * mxy.y * mxy.x + (posy - 1) * mxy.x + posx)
#define POSX_U1 (posz * mxy.y * mxy.x + posy * mxy.x + (posx + 1))
#define POSX_D1 (posz * mxy.y * mxy.x + posy * mxy.x + (posx - 1))

class Map{

private:
	static int map_no_s;    //マップナンバー
	int map_no;            //オブジェクト内部用マップナンバー
	static MapStPos MPos;//マップスタート位置
	static int boss_killed[5];//各ボス撃破履歴
	Dx9Process *dx;
	typedef struct{
		int *m;
		int x;
		int y;
		int z;
	}mapxy;
	mapxy mxy;

	float cax1;
	float cax2;
	float cay1;
	float cay2;
	float src_theta;//現在の向いてる方向

	int posx;
	int posy;
	int posz;
	int blockcount;   //壁用立方体の個数
	int squarecount; //壁用板個数
	int r_point_count;//回復ポイント個数
	int mo_count;    //動画テクスチャ,松明個数
	int f_wall_count;//動画テクスチャ炎壁個数
	int boss_count; //ボス出現ポイント個数
	int Elevator_count;//エレベーター個数
	int map_text_f;//マップ上のテキスト表示フラグ
	char m_tx[30];//表示内容
	bool recover_p_f;//リカバーポイント到達履歴
	bool boss_p_f;  //ボス出現ポイント到達履歴
	bool elevator_UP; 
	bool elevator_DOWN;
	float elevator_step;

	bool moving;
	Directionkey direction_move;
	float m_theta;
	float stepx;
	float stepy;

	Dx9Process::PolygonData poWall, poWall1, poF_Wall, poGround, poCeiling, poBackground,
		poRain, poEXIT, poENTER, poRecover, poRecoverLine, poMo, poBoss, poElevator;
	Position::E_Pos e_pos[4];
	Position::H_Pos h_pos;

	//フォグパラメーター
	float StartPos;
	float EndPos;
	DWORD r, g, b;

	void Debug();//デバック用
	Map();
	void Mapfilter(Position::H_Pos *h_p);
	void Mapcreate_Wall1();
	void Mapcreate_Wall();
	void Mapcreate_F_Wall();
	void Mapcreate_Ground(float st, float end);
	void Mapcreate_Ceiling(float st, float end, float height);
	void Mapcreate_Background(float st, float end, float height);
	void Mapcreate_Rain();
	void Mapcreate_ENTER(float x, float y, float z, float xsize);
	void Mapcreate_EXIT(float x, float y, float z, float xsize);
	void Mapcreate_Recover();
	void Mapcreate_Ds();
	void Mapcreate_BossPoint();
	void Mapcreate_Elevator();
	Encount Move(MapState *mapstate, Directionkey direction);
	void MapText(char str[30]);

public:
	static int GetMapNo();
	static void SetMapNo(int no);
	static void SetBossKilled(int i, int f);
	static int *GetBossKilled();
	Map(Position::H_Pos *h_p);
	Encount Mapdraw(MapState *mapstate, Directionkey direction, Encount encount, bool menu, bool title);
	Position::E_Pos *Getposition(int p);
	Position::H_Pos *Getposition();
	~Map();
};

#endif