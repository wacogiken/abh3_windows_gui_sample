/*
 ******************************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2023, Waco Giken Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the Waco Giken nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
*/

//
#include "pch.h"
#include "resource.h"
#include "framework.h"
#include "gui_canABH3.h"
#include "gui_canABH3Doc.h"
#include "gui_canABH3View.h"
#include "ChildFrm.h"

#ifdef _DEBUG
 #define new DEBUG_NEW
#endif

//最低画面サイズ：1440 x 1080
//推奨画面サイズ：1920 x 1080

//色テーブル
static IDCOLOR g_color_tbl[] = {
	//colorNum				textColor				backColor
	{0,						COLOR_WHITE,			COLOR_BLUE},
	{1,						COLOR_WHITE,			COLOR_DARKGREEN},
	{2,						COLOR_WHITE,			COLOR_DARKBLUE},
	{3,						COLOR_WHITE,			COLOR_DARKMAGENTA},
	{4,						COLOR_WHITE,			COLOR_SKYBLUE},
	{5,						COLOR_WHITE,			COLOR_GREEN},
	{6,						COLOR_BLACK,			COLOR_YELLOW},
	{7,						COLOR_WHITE,			COLOR_RED},
	{8,						COLOR_WHITE,			COLOR_BLACK},
	{9,						COLOR_BLACK,			COLOR_WHITE},
	{-1,					0,						0},
	};

//固定色アイテム
static IDCOLOR1 g_textcolor_tbl[] = {
	//beginID				endID					colorNum
	{IDC_TITLE_A,			0,						1},
	{IDC_TITLE_B,			0,						2},
	{IDC_TITLE_VALUE0,		0,						1},
	{IDC_TITLE_VALUE1,		0,						2},
	{IDC_TITLE_VALUE2,		0,						1},
	{IDC_TITLE_VALUE3,		0,						2},
	{IDC_TITLE_VALUE4,		0,						1},
	{IDC_TITLE_VALUE5,		0,						2},
	{IDC_TITLE_VALUE6,		0,						1},
	{IDC_TITLE_VALUE7,		0,						2},
	{IDC_TITLE_VALUE8,		0,						1},
	{IDC_TITLE_VALUE9,		0,						2},
	{IDC_TITLE_VALUE10,		0,						1},
	{IDC_TITLE_VALUE11,		0,						2},
	{IDC_TITLE_VALUE12,		IDC_TITLE_VALUE13,		6},
	{IDC_TITLE_VALUE14,		IDC_TITLE_VALUE15,		3},
	{IDC_TITLE_VALUE16,		IDC_TITLE_VALUE17,		8},
	{IDC_TITLE_CTRL,		0,						0},
	{IDC_TITLE_INPUT,		0,						0},
	{IDC_TITLE_REQUEST,		0,						0},
	{IDC_TITLE_IO,			0,						0},
	{IDC_TITLE_WARN,		0,						6},
	{IDC_TITLE_ERR,			0,						7},
	{IDC_TITLE_CYCLE,		0,						8},
	{IDC_TITLE_CTRLID0,		IDC_TITLE_CTRLID7,		1},
	{IDC_TITLE_CTRLID8,		IDC_TITLE_CTRLID15,		2},
	{IDC_TITLE_CTRLID16,	0,						1},
	{IDC_TITLE_CTRLID17,	0,						2},
	{IDC_TITLE_CTRLID18,	IDC_TITLE_CTRLID19,		6},
	{IDC_TITLE_CTRLID20,	0,						1},
	{IDC_TITLE_CTRLID21,	0,						2},
	{IDC_TITLE_CTRLID22,	0,						7},
	{IDC_TITLE_CTRLID23,	IDC_TITLE_CTRLID31,		9},
	{0,					0,					0},
	};

//汎用アイテムのテキスト
static IDTEXT1 g_anyitem_tbl[] = {
	//ID				{textEN					textJP}
	{IDC_TITLE_A,		{_T("A/Y order"),		_T("A/Y指令")}},
	{IDC_TITLE_B,		{_T("B/X order"),		_T("B/X指令")}},
	{IDC_TITLE_CYCLE,	{_T("Cycle execute"),	_T("周期実行対象")}},
	{IDC_FPS,			{_T("Functions"),		_T("機能")}},
	{IDC_TITLE_CTRL,	{_T("Operation flag"),	_T("操作フラグ")}},
	{IDC_TITLE_INPUT,	{_T("Input flag"),		_T("入力フラグ")}},
	{IDC_TITLE_REQUEST,	{_T("Control flag"),	_T("制御フラグ")}},
	{IDC_TITLE_IO,		{_T("I/O flag"),		_T("I/Oフラグ")}},
	{IDC_TITLE_WARN,	{_T("Warning"),			_T("警告フラグ")}},
	{IDC_TITLE_ERR,		{_T("Error"),			_T("異常フラグ")}},
	{0,					{NULL,					NULL}},
	};

//周期要求要素（最大16要素）
static IDTEXT5 g_request_tbl[] = {
	//ID				text			packetType						adrs
	{IDC_REQUESTDP0R,	_T("DP0"),		PACKETTYPE::SINGLE_PACKET,		0},
	{IDC_REQUESTBR0,	_T("BR0"),		PACKETTYPE::BROADCAST_PACKET,	0},
	{IDC_REQUESTBR1,	_T("BR1"),		PACKETTYPE::BROADCAST_PACKET,	1},
	{IDC_REQUESTBR2,	_T("BR2"),		PACKETTYPE::BROADCAST_PACKET,	2},
	{IDC_REQUESTBR3,	_T("BR3"),		PACKETTYPE::BROADCAST_PACKET,	3},
	{IDC_REQUESTBR4,	_T("BR4"),		PACKETTYPE::BROADCAST_PACKET,	4},
	{IDC_REQUESTBR5,	_T("BR5"),		PACKETTYPE::BROADCAST_PACKET,	5},
	{IDC_REQUESTBR6,	_T("BR6"),		PACKETTYPE::BROADCAST_PACKET,	6},
	{0,					_T(""),			PACKETTYPE::UNKNOWN_PACKET,		0},
	};

//各ステータス値
static IDTEXT4 g_valueid_tbl[] = {
	//titleID			{textEN						textJP}					valueID
	{IDC_TITLE_VALUE0,	{_T("[A/Y]Order.return"),	_T("A/Y帰還")},			IDC_VALUE0},
	{IDC_TITLE_VALUE1,	{_T("[B/X]Order.return"),	_T("B/X帰還")},			IDC_VALUE1},
	{IDC_TITLE_VALUE2,	{_T("[A/Y]Speed.order"),	_T("A/Y速度指令")},		IDC_VALUE2},
	{IDC_TITLE_VALUE3,	{_T("[B/X]Speed.order"),	_T("B/X速度指令")},		IDC_VALUE3},
	{IDC_TITLE_VALUE4,	{_T("[A/Y]Speed.return"),	_T("A/Y速度帰還")},		IDC_VALUE4},
	{IDC_TITLE_VALUE5,	{_T("[B/X]Speed.return"),	_T("B/X速度帰還")},		IDC_VALUE5},
	{IDC_TITLE_VALUE6,	{_T("[A/Y]Current.order"),	_T("A/Y電流指令")},		IDC_VALUE6},
	{IDC_TITLE_VALUE7,	{_T("[B/X]Current.order"),	_T("B/X電流指令")},		IDC_VALUE7},
	{IDC_TITLE_VALUE8,	{_T("[A]Load"),				_T("A負荷率")},			IDC_VALUE8},
	{IDC_TITLE_VALUE9,	{_T("[B]Load"),				_T("B負荷率")},			IDC_VALUE9},
	{IDC_TITLE_VALUE10,	{_T("[A]Pulse"),			_T("Aパルス積算値")},	IDC_VALUE10},
	{IDC_TITLE_VALUE11,	{_T("[B]Pulse"),			_T("Bパルス積算値")},	IDC_VALUE11},
	{IDC_TITLE_VALUE12,	{_T("Analog0"),				_T("アナログ入力0")},	IDC_VALUE12},
	{IDC_TITLE_VALUE13,	{_T("Analog1"),				_T("アナログ入力1")},	IDC_VALUE13},
	{IDC_TITLE_VALUE14,	{_T("Power(main)"),			_T("主電源電圧")},		IDC_VALUE14},
	{IDC_TITLE_VALUE15,	{_T("Power(control)"),		_T("制御電源電圧")},	IDC_VALUE15},
	{IDC_TITLE_VALUE16,	{_T("Monitor0"),			_T("モニタ0データ")},	IDC_VALUE16},
	{IDC_TITLE_VALUE17,	{_T("Monitor1"),			_T("モニタ1データ")},	IDC_VALUE17},
	{0,					{NULL,						NULL},					0},
	};

//操作フラグ類のタイトル（操作、制御、入力フラグの混合物）
static IDTEXT g_ctrlid_title[] = {
	//bit	ID						{textEN					textJP}
	{0,		IDC_TITLE_CTRLID0,		{_T("[A/Y]Servo"),		_T("A/Y サーボ")}},
	{1,		IDC_TITLE_CTRLID1,		{_T("[A/Y]Start"),		_T("A/Y スタート")}},
	{2,		IDC_TITLE_CTRLID2,		{_T("[A/Y]Order.pol"),	_T("A/Y 指令極性")}},
	{3,		IDC_TITLE_CTRLID3,		{_T("[A/Y]Add.adjust"),	_T("A/Y 補正加算")}},
	{4,		IDC_TITLE_CTRLID4,		{_T("[A/Y]Data.sel0"),	_T("A/Y データ選択0")}},
	{5,		IDC_TITLE_CTRLID5,		{_T("[A/Y]Data.sel1"),	_T("A/Y データ選択1")}},
	{6,		IDC_TITLE_CTRLID6,		{_T("[A/Y]Data.sel2"),	_T("A/Y データ選択2")}},
	{7,		IDC_TITLE_CTRLID7,		{_T("[A/Y]Adjust.pol"),	_T("A/Y 補正極性")}},
	{8,		IDC_TITLE_CTRLID8,		{_T("[B/X]Servo"),		_T("B/X サーボ")}},
	{9,		IDC_TITLE_CTRLID9,		{_T("[B/X]Start"),		_T("B/X スタート")}},
	{10,	IDC_TITLE_CTRLID10,		{_T("[B/X]Order.pol"),	_T("B/X 指令極性")}},
	{11,	IDC_TITLE_CTRLID11,		{_T("[B/X]Add.adjust"),	_T("B/X 補正加算")}},
	{12,	IDC_TITLE_CTRLID12,		{_T("[B/X]Data.sel0"),	_T("B/X データ選択0")}},
	{13,	IDC_TITLE_CTRLID13,		{_T("[B/X]Data.sel1"),	_T("B/X データ選択1")}},
	{14,	IDC_TITLE_CTRLID14,		{_T("[B/X]Data.sel2"),	_T("B/X データ選択2")}},
	{15,	IDC_TITLE_CTRLID15,		{_T("[B/X]Adjust.pol"),	_T("B/X 補正極性")}},
	{16,	IDC_TITLE_CTRLID16,		{_T("[A/Y]Speed/Trq"),	_T("A/Y 速度/トルク")}},
	{17,	IDC_TITLE_CTRLID17,		{_T("[B/X]Speed/Trq"),	_T("B/X 速度/トルク")}},
	{18,	IDC_TITLE_CTRLID18,		{_T("Master/Slave"),	_T("マスタ/スレーブ")}},
	{19,	IDC_TITLE_CTRLID19,		{_T("Break"),			_T("ブレーキ")}},
	{20,	IDC_TITLE_CTRLID20,		{_T("[A]Pulse.clear"),	_T("A軸積算クリア")}},
	{21,	IDC_TITLE_CTRLID21,		{_T("[B]Pulse.clear"),	_T("B軸積算クリア")}},
	{22,	IDC_TITLE_CTRLID22,		{_T("Reset error"),		_T("エラーリセット")}},
	{23,	IDC_TITLE_CTRLID23,		{_T(""),				_T("")}},
	{24,	IDC_TITLE_CTRLID24,		{_T(""),				_T("")}},
	{25,	IDC_TITLE_CTRLID25,		{_T("[A/Y]Ready"),		_T("A/Y軸 レディ")}},
	{26,	IDC_TITLE_CTRLID26,		{_T("[A/Y]Busy"),		_T("A/Y軸 ビジー")}},
	{27,	IDC_TITLE_CTRLID27,		{_T("[B/X]Ready"),		_T("B/X軸 レディ")}},
	{28,	IDC_TITLE_CTRLID28,		{_T("[B/X]Busy"),		_T("B/X軸 ビジー")}},
	{29,	IDC_TITLE_CTRLID29,		{_T("Control model"),	_T("制御モデル")}},
	{30,	IDC_TITLE_CTRLID30,		{_T("Release break"),	_T("ブレーキ解放動作")}},
	{31,	IDC_TITLE_CTRLID31,		{_T("Detected error"),	_T("エラー発生")}},
	{0,		0,						{NULL,					NULL}},
	};

//操作フラグのボタン情報
static IDTEXT2 g_ctrlid_ctrl[] =	{
	//bit	btnOffID			{offtextEN					offtextJP}					btnOnID				{ontextEN				ontextJP}
	{0,		IDC_OFF_CTRLID0,	{_T("ServoOFF"),			_T("サーボOFF")},			IDC_ON_CTRLID0,		{_T("ServoON"),			_T("サーボON")}},
	{1,		IDC_OFF_CTRLID1,	{_T("Disable.order"),		_T("指令無効")},			IDC_ON_CTRLID1,		{_T("Enable.order"),	_T("指令有効")}},
	{2,		IDC_OFF_CTRLID2,	{_T("Through"),				_T("スルー")},				IDC_ON_CTRLID2,		{_T("Reverse"),			_T("反転")}},
	{3,		IDC_OFF_CTRLID3,	{_T("Off"),					_T("オフ")},				IDC_ON_CTRLID3,		{_T("On"),				_T("オン")}},
	{4,		IDC_OFF_CTRLID4,	{_T("bit0"),				_T("bit0")},				IDC_ON_CTRLID4,		{_T("0"),				_T("0")}},
	{5,		IDC_OFF_CTRLID5,	{_T("bit1"),				_T("bit1")},				IDC_ON_CTRLID5,		{_T("0"),				_T("0")}},
	{6,		IDC_OFF_CTRLID6,	{_T("bit2"),				_T("bit2")},				IDC_ON_CTRLID6,		{_T("0"),				_T("0")}},
	{7,		IDC_OFF_CTRLID7,	{_T("Add"),					_T("加算")},				IDC_ON_CTRLID7,		{_T("Sub"),				_T("減算")}},
	{8,		IDC_OFF_CTRLID8,	{_T("ServoOFF"),			_T("サーボOFF")},			IDC_ON_CTRLID8,		{_T("ServoON"),			_T("サーボON")}},
	{9,		IDC_OFF_CTRLID9,	{_T("Disable.order"),		_T("指令無効")},			IDC_ON_CTRLID9,		{_T("Enable.order"),	_T("指令有効")}},
	{10,	IDC_OFF_CTRLID10,	{_T("Through"),				_T("スルー")},				IDC_ON_CTRLID10,	{_T("Reverse"),			_T("反転")}},
	{11,	IDC_OFF_CTRLID11,	{_T("Off"),					_T("オフ")},				IDC_ON_CTRLID11,	{_T("On"),				_T("オン")}},
	{12,	IDC_OFF_CTRLID12,	{_T("bit0"),				_T("bit0")},				IDC_ON_CTRLID12,	{_T("0"),				_T("0")}},
	{13,	IDC_OFF_CTRLID13,	{_T("bit1"),				_T("bit1")},				IDC_ON_CTRLID13,	{_T("0"),				_T("0")}},
	{14,	IDC_OFF_CTRLID14,	{_T("bit2"),				_T("bit2")},				IDC_ON_CTRLID14,	{_T("0"),				_T("0")}},
	{15,	IDC_OFF_CTRLID15,	{_T("Add"),					_T("加算")},				IDC_ON_CTRLID15,	{_T("Sub"),				_T("減算")}},
	{16,	IDC_OFF_CTRLID16,	{_T("Speed/[A/Y]master"),	_T("速度・A/Y軸マスタ")},	IDC_ON_CTRLID16,	{_T("Trq/[B/X]master"),	_T("トルク/B/X軸マスタ")}},
	{17,	IDC_OFF_CTRLID17,	{_T("Speed"),				_T("速度")},				IDC_ON_CTRLID17,	{_T("Trq"),				_T("トルク")}},
	{18,	IDC_OFF_CTRLID18,	{_T("Off"),					_T("オフ")},				IDC_ON_CTRLID18,	{_T("On"),				_T("オン")}},
	{19,	IDC_OFF_CTRLID19,	{_T("Release.off"),			_T("解放操作オフ")},		IDC_ON_CTRLID19,	{_T("Release on"),		_T("解放操作オン")}},
	{20,	IDC_OFF_CTRLID20,	{_T("Never"),				_T("非動作")},				IDC_ON_CTRLID20,	{_T("Clear"),			_T("クリア")}},
	{21,	IDC_OFF_CTRLID21,	{_T("Never"),				_T("非動作")},				IDC_ON_CTRLID21,	{_T("Clear"),			_T("クリア")}},
	{22,	IDC_OFF_CTRLID22,	{_T("Never"),				_T("非動作")},				IDC_ON_CTRLID22,	{_T("Clear"),			_T("クリア")}},
	{23,	IDC_OFF_CTRLID23,	{_T(""),					_T("")},					IDC_ON_CTRLID23,	{_T(""),				_T("")}},
	{23,	IDC_OFF_CTRLID24,	{_T(""),					_T("")},					IDC_ON_CTRLID24,	{_T(""),				_T("")}},
	{23,	IDC_OFF_CTRLID25,	{_T(""),					_T("")},					IDC_ON_CTRLID25,	{_T(""),				_T("")}},
	{23,	IDC_OFF_CTRLID26,	{_T(""),					_T("")},					IDC_ON_CTRLID26,	{_T(""),				_T("")}},
	{23,	IDC_OFF_CTRLID27,	{_T(""),					_T("")},					IDC_ON_CTRLID27,	{_T(""),				_T("")}},
	{23,	IDC_OFF_CTRLID28,	{_T(""),					_T("")},					IDC_ON_CTRLID28,	{_T(""),				_T("")}},
	{23,	IDC_OFF_CTRLID29,	{_T(""),					_T("")},					IDC_ON_CTRLID29,	{_T(""),				_T("")}},
	{23,	IDC_OFF_CTRLID30,	{_T(""),					_T("")},					IDC_ON_CTRLID30,	{_T(""),				_T("")}},
	{23,	IDC_OFF_CTRLID31,	{_T(""),					_T("")},					IDC_ON_CTRLID31,	{_T(""),				_T("")}},
	{0,		0,					{NULL,						NULL},						0,					{NULL,					NULL}},
	};

//入力フラグの詳細情報
static IDTEXT3 g_inputid_tbl[] =	{
	//bit	ID					{offtextEN					offtextJP}					{ontextEN					ontextJP}
	{0,		IDC_INPUTID0,		{_T("ServoOFF"),			_T("サーボOFF")},			{_T("ServoON"),				_T("サーボON")}},
	{1,		IDC_INPUTID1,		{_T("Disable order"),		_T("指令無効")},			{_T("Enable order"),		_T("指令有効")}},
	{2,		IDC_INPUTID2,		{_T("Through"),				_T("スルー")},				{_T("Reverse"),				_T("反転")}},
	{3,		IDC_INPUTID3,		{_T("Off"),					_T("オフ")},				{_T("On"),					_T("オン")}},
	{4,		IDC_INPUTID4,		{_T("0"),					_T("0")},					{_T("bit0"),				_T("bit0")}},
	{5,		IDC_INPUTID5,		{_T("0"),					_T("0")},					{_T("bit1"),				_T("bit1")}},
	{6,		IDC_INPUTID6,		{_T("0"),					_T("0")},					{_T("bit2"),				_T("bit2")}},
	{7,		IDC_INPUTID7,		{_T("Add"),					_T("加算")},				{_T("Sub"),					_T("減算")}},
	{8,		IDC_INPUTID8,		{_T("ServoOFF"),			_T("サーボOFF")},			{_T("ServoON"),				_T("サーボON")}},
	{9,		IDC_INPUTID9,		{_T("Disable order"),		_T("指令無効")},			{_T("Enable order"),		_T("指令有効")}},
	{10,	IDC_INPUTID10,		{_T("Through"),				_T("スルー")},				{_T("Reverse"),				_T("反転")}},
	{11,	IDC_INPUTID11,		{_T("Off"),					_T("オフ")},				{_T("On"),					_T("オン")}},
	{12,	IDC_INPUTID12,		{_T("0"),					_T("0")},					{_T("bit0"),				_T("bit0")}},
	{13,	IDC_INPUTID13,		{_T("0"),					_T("0")},					{_T("bit1"),				_T("bit1")}},
	{14,	IDC_INPUTID14,		{_T("0"),					_T("0")},					{_T("bit2"),				_T("bit2")}},
	{15,	IDC_INPUTID15,		{_T("Add"),					_T("加算")},				{_T("Sub"),					_T("減算")}},
	{16,	IDC_INPUTID16,		{_T("Speed/[A/Y]master"),	_T("速度・A/Y軸マスタ")},	{_T("Trq/[B/X]master"),		_T("トルク/B/X軸マスタ")}},
	{17,	IDC_INPUTID17,		{_T("Speed"),				_T("速度")},				{_T("Trq"),					_T("トルク")}},
	{18,	IDC_INPUTID18,		{_T("Off"),					_T("オフ")},				{_T("On"),					_T("オン")}},
	{19,	IDC_INPUTID19,		{_T("ReleaseOFF"),			_T("解放操作オフ")},		{_T("ReleaseON"),			_T("解放操作オン")}},
	{20,	IDC_INPUTID20,		{_T("No operation"),		_T("非動作")},				{_T("Clear"),				_T("クリア")}},
	{21,	IDC_INPUTID21,		{_T("No operation"),		_T("非動作")},				{_T("Clear"),				_T("クリア")}},
	{22,	IDC_INPUTID22,		{_T("No operation"),		_T("非動作")},				{_T("Clear"),				_T("クリア")}},
	{23,	IDC_INPUTID23,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{24,	IDC_INPUTID24,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{25,	IDC_INPUTID25,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{26,	IDC_INPUTID26,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{27,	IDC_INPUTID27,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{28,	IDC_INPUTID28,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{29,	IDC_INPUTID29,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{30,	IDC_INPUTID30,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{31,	IDC_INPUTID31,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{0,		0,					{NULL,						NULL},						{NULL,						NULL}},
	};

//制御フラグ詳細
static IDTEXT3 g_resultid_tbl[] =	{
	//bit	ID					offText						onText
	{0,		IDC_RESULTID0,		{_T("ServoOFF"),			_T("サーボOFF")},			{_T("ServoON"),				_T("サーボON")}},
	{1,		IDC_RESULTID1,		{_T("Disable order"),		_T("指令無効")},			{_T("Enable order"),		_T("指令有効")}},
	{2,		IDC_RESULTID2,		{_T("Through"),				_T("スルー")},				{_T("Reverse"),				_T("反転")}},
	{3,		IDC_RESULTID3,		{_T("Off"),					_T("オフ")},				{_T("On"),					_T("オン")}},
	{4,		IDC_RESULTID4,		{_T("0"),					_T("0")},					{_T("bit0"),				_T("bit0")}},
	{5,		IDC_RESULTID5,		{_T("0"),					_T("0")},					{_T("bit1"),				_T("bit1")}},
	{6,		IDC_RESULTID6,		{_T("0"),					_T("0")},					{_T("bit2"),				_T("bit2")}},
	{7,		IDC_RESULTID7,		{_T("Add"),					_T("加算")},				{_T("Sub"),					_T("減算")}},
	{8,		IDC_RESULTID8,		{_T("ServoOFF"),			_T("サーボOFF")},			{_T("ServoON"),				_T("サーボON")}},
	{9,		IDC_RESULTID9,		{_T("Disable order"),		_T("指令無効")},			{_T("Enable order"),		_T("指令有効")}},
	{10,	IDC_RESULTID10,		{_T("Through"),				_T("スルー")},				{_T("Reverse"),				_T("反転")}},
	{11,	IDC_RESULTID11,		{_T("Off"),					_T("オフ")},				{_T("On"),					_T("オン")}},
	{12,	IDC_RESULTID12,		{_T("0"),					_T("0")},					{_T("bit0"),				_T("bit0")}},
	{13,	IDC_RESULTID13,		{_T("0"),					_T("0")},					{_T("bit1"),				_T("bit1")}},
	{14,	IDC_RESULTID14,		{_T("0"),					_T("0")},					{_T("bit2"),				_T("bit2")}},
	{15,	IDC_RESULTID15,		{_T("Add"),					_T("加算")},				{_T("Sub"),					_T("減算")}},
	{16,	IDC_RESULTID16,		{_T("Speed/[A/Y]master"),	_T("速度・A/Y軸マスタ")},	{_T("Trq/[B/X]master"),		_T("トルク/B/X軸マスタ")}},
	{17,	IDC_RESULTID17,		{_T("Speed"),				_T("速度")},				{_T("Trq"),					_T("トルク")}},
	{18,	IDC_RESULTID18,		{_T("Off"),					_T("オフ")},				{_T("On"),					_T("オン")}},
	{19,	IDC_RESULTID19,		{_T("ReleaseOFF"),			_T("解放操作オフ")},		{_T("ReleaseON"),			_T("解放操作オン")}},
	{20,	IDC_RESULTID20,		{_T(""),					_T("非動作")},				{_T("Clear"),				_T("クリア")}},
	{21,	IDC_RESULTID21,		{_T(""),					_T("非動作")},				{_T("Clear"),				_T("クリア")}},
	{22,	IDC_RESULTID22,		{_T(""),					_T("非動作")},				{_T("Clear"),				_T("クリア")}},
	{23,	IDC_RESULTID23,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{24,	IDC_RESULTID24,		{_T(""),					_T("")},					{_T(""),					_T("")}},
	{25,	IDC_RESULTID25,		{_T("Status ServoOFF"),		_T("サーボOFF状態")},		{_T("Status ServoON"),		_T("サーボON状態")}},
	{26,	IDC_RESULTID26,		{_T("Order=0"),				_T("指令=0")},				{_T("Moving"),				_T("駆動中")}},
	{27,	IDC_RESULTID27,		{_T("Status ServoOFF"),		_T("サーボOFF状態")},		{_T("Status ServoON"),		_T("サーボON状態")}},
	{28,	IDC_RESULTID28,		{_T("Order=0"),				_T("指令=0")},				{_T("Moving"),				_T("駆動中")}},
	{29,	IDC_RESULTID29,		{_T("Axis motor"),			_T("モータ軸")},			{_T("Axis tire"),			_T("走行軸")}},
	{30,	IDC_RESULTID30,		{_T("Enable break"),		_T("ブレーキ保持")},		{_T("Release break"),		_T("ブレーキ解放")}},
	{31,	IDC_RESULTID31,		{_T("Normal"),				_T("異常無し")},			{_T("Abnormal"),			_T("異常有り")}},
	{0,		0,					{NULL,						NULL},						{NULL,						NULL}},
	};

//標準ABH3のI/Oフラグ
static IDTEXT g_io_normal_tbl[] =	{
	//bit	ID					{textEN						textJP}
	{0,		IDC_IO0,			{_T("Error.out"),			_T("エラー出力")}},
	{1,		IDC_IO1,			{_T("Alarm code0"),			_T("アラームコード0")}},
	{2,		IDC_IO2,			{_T("Alarm code1"),			_T("アラームコード1")}},
	{3,		IDC_IO3,			{_T("Alarm code2"),			_T("アラームコード2")}},
	{4,		IDC_IO4,			{_T("Ready.out"),			_T("レディ出力")}},
	{5,		IDC_IO5,			{_T("Busy.out"),			_T("ビジー出力")}},
	{6,		IDC_IO6,			{_T("Alarm code"),			_T("アラームコード")}},
	{7,		IDC_IO7,			{_T("Alarm.out"),			_T("アラーム出力")}},
	{8,		IDC_IO8,			{_T("[A/Y]ServoON"),		_T("A/YサーボON")}},
	{9,		IDC_IO9,			{_T("[A/Y]Start"),			_T("A/Yスタート")}},
	{10,	IDC_IO10,			{_T("Break"),				_T("ブレーキ")}},
	{11,	IDC_IO11,			{_T("[A/Y]Speed/Trq"),		_T("A/Y速度/トルク")}},
	{12,	IDC_IO12,			{_T("[A/Y]Select.data0"),	_T("A/Yデータ選択0")}},
	{13,	IDC_IO13,			{_T("[A/Y]Select.data1"),	_T("A/Yデータ選択1")}},
	{14,	IDC_IO14,			{_T("[A/Y]Adjust.pol"),		_T("A/Y補正極性")}},
	{15,	IDC_IO15,			{_T("[A/Y]Adjust.add"),		_T("A/Y補正加算")}},
	{16,	IDC_IO16,			{_T("[A/Y]Select.data2"),	_T("A/Yデータ選択2")}},
	{17,	IDC_IO17,			{_T("[A/Y]Order.pol"),		_T("A/Y指令極性")}},
	{18,	IDC_IO18,			{_T("[B/X]ServoON"),		_T("B/XサーボON")}},
	{19,	IDC_IO19,			{_T("[B/X]Start"),			_T("B/Xスタート")}},
	{20,	IDC_IO20,			{_T("Master/Slave"),		_T("マスタ/スレーブ")}},
	{21,	IDC_IO21,			{_T("[B/X]Speed/Trq"),		_T("B/X速度/トルク")}},
	{22,	IDC_IO22,			{_T("[B/X]Select.data0"),	_T("B/Xデータ選択0")}},
	{23,	IDC_IO23,			{_T("[B/X]Select.data1"),	_T("B/Xデータ選択1")}},
	{24,	IDC_IO24,			{_T("[B/X]Adjust.pol"),		_T("B/X補正極性")}},
	{25,	IDC_IO25,			{_T("[B/X]Adjust.add"),		_T("B/X補正加算")}},
	{26,	IDC_IO26,			{_T("[B/X]Select.data2"),	_T("B/Xデータ選択2")}},
	{27,	IDC_IO27,			{_T("[B/X]Order.pol"),		_T("B/X指令極性")}},
	{28,	IDC_IO28,			{_T(""),					_T("")}},
	{29,	IDC_IO29,			{_T(""),					_T("")}},
	{30,	IDC_IO30,			{_T(""),					_T("")}},
	{31,	IDC_IO31,			{_T("エラーリセット入力"),	_T("エラーリセット入力")}},
	{0,		0,					{NULL,						NULL}},
	};

//小型ABH3のI/Oフラグ
static IDTEXT g_io_small_tbl[] =	{
	//bit	ID					text
	{0,		IDC_IO0,			_T("Error.out"),			_T("エラー出力")},
	{1,		IDC_IO1,			_T("Alarm.out"),			_T("アラーム出力")},
	{2,		IDC_IO2,			_T("Ready.out"),			_T("レディ出力")},
	{3,		IDC_IO3,			_T("Busy.out"),				_T("ビジー出力")},
	{4,		IDC_IO4,			_T(""),						_T("")},
	{5,		IDC_IO5,			_T(""),						_T("")},
	{6,		IDC_IO6,			_T(""),						_T("")},
	{7,		IDC_IO7,			_T(""),						_T("")},
	{8,		IDC_IO8,			_T("[A/Y]ServoON"),			_T("A/YサーボON")},
	{9,		IDC_IO9,			_T("[A/Y]Start"),			_T("A/Yスタート")},
	{10,	IDC_IO10,			_T("[A/Y]Order.pol"),		_T("A/Y指令極性")},
	{11,	IDC_IO11,			_T("[B/X]ServoON"),			_T("B/XサーボON")},
	{12,	IDC_IO12,			_T("[B/X]Start"),			_T("B/Xスタート")},
	{13,	IDC_IO13,			_T("[B/X]Order.pol"),		_T("B/X指令極性")},
	{14,	IDC_IO14,			_T("Select.data0"),			_T("データ選択0")},
	{15,	IDC_IO15,			_T("Select.data1"),			_T("データ選択1")},
	{16,	IDC_IO16,			_T("Select.data2"),			_T("データ選択2")},
	{17,	IDC_IO17,			_T("Break"),				_T("ブレーキ")},
	{18,	IDC_IO18,			_T(""),						_T("")},
	{19,	IDC_IO19,			_T(""),						_T("")},
	{20,	IDC_IO20,			_T(""),						_T("")},
	{21,	IDC_IO21,			_T(""),						_T("")},
	{22,	IDC_IO22,			_T(""),						_T("")},
	{23,	IDC_IO23,			_T(""),						_T("")},
	{24,	IDC_IO24,			_T(""),						_T("")},
	{25,	IDC_IO25,			_T(""),						_T("")},
	{26,	IDC_IO26,			_T(""),						_T("")},
	{27,	IDC_IO27,			_T(""),						_T("")},
	{28,	IDC_IO28,			_T(""),						_T("")},
	{29,	IDC_IO29,			_T(""),						_T("")},
	{30,	IDC_IO30,			_T(""),						_T("")},
	{31,	IDC_IO31,			_T("Reset.err"),			_T("エラーリセット入力")},
	{0,		0,					NULL,						NULL},
	};

//警告とエラー
static IDTEXT2 g_warnerr_tbl[] =	{
	//bit	warnID			{warntextEN					warnTextJP}					errID					{errtextEN				errTextJP}
	{0,		IDC_WARN_BIT0,	{_T(""),					_T("")},					IDC_ERR_BIT0,		{_T("AxisA Lock.mecha"),	_T("A軸 メカロック")}},
	{1,		IDC_WARN_BIT1,	{_T(""),					_T("")},					IDC_ERR_BIT1,		{_T("AxisB Lock.mecha"),	_T("B軸 メカロック")}},
	{2,		IDC_WARN_BIT2,	{_T(""),					_T("")},					IDC_ERR_BIT2,		{_T("Overheat"),			_T("ドライバ過熱")}},
	{3,		IDC_WARN_BIT3,	{_T(""),					_T("")},					IDC_ERR_BIT3,		{_T("Abnormal break"),		_T("ブレーキ異常")}},
	{4,		IDC_WARN_BIT4,	{_T(""),					_T("")},					IDC_ERR_BIT4,		{_T("AxisA resolver"),		_T("A軸 レゾルバ")}},
	{5,		IDC_WARN_BIT5,	{_T(""),					_T("")},					IDC_ERR_BIT5,		{_T("AxisB resolver"),		_T("B軸 レゾルバ")}},
	{6,		IDC_WARN_BIT6,	{_T(""),					_T("")},					IDC_ERR_BIT6,		{_T("AxisA Over current"),	_T("A軸 過電流")}},
	{7,		IDC_WARN_BIT7,	{_T(""),					_T("")},					IDC_ERR_BIT7,		{_T("AxisB Over current"),	_T("B軸 過電流")}},
	{8,		IDC_WARN_BIT8,	{_T(""),					_T("")},					IDC_ERR_BIT8,		{_T("Lower power.control"),	_T("制御電源 電圧低下")}},
	{9,		IDC_WARN_BIT9,	{_T(""),					_T("")},					IDC_ERR_BIT9,		{_T("Abnormal parameter"),	_T("パラメータ")}},
	{10,	IDC_WARN_BIT10,	{_T(""),					_T("")},					IDC_ERR_BIT10,		{_T("AxisA PDU"),			_T("A軸 PDU")}},
	{11,	IDC_WARN_BIT11,	{_T(""),					_T("")},					IDC_ERR_BIT11,		{_T("AxisB PDU"),			_T("B軸 PDU")}},
	{12,	IDC_WARN_BIT12,	{_T("AxisA SoftThermal"),	_T("A軸 電子サーマル")},	IDC_ERR_BIT12,		{_T("AxisA SoftThermal"),	_T("A軸 電子サーマル")}},
	{13,	IDC_WARN_BIT13,	{_T("AxisB SoftThermal"),	_T("B軸 電子サーマル")},	IDC_ERR_BIT13,		{_T("AxisB SoftThermal"),	_T("B軸 電子サーマル")}},
	{14,	IDC_WARN_BIT14,	{_T("Lower power.main"),	_T("主電源 電圧低下")},		IDC_ERR_BIT14,		{_T(""),					_T("")}},
	{15,	IDC_WARN_BIT15,	{_T(""),					_T("")},					IDC_ERR_BIT15,		{_T("Over power"),			_T("電源 過電圧")}},
	{16,	IDC_WARN_BIT16,	{_T(""),					_T("")},					IDC_ERR_BIT16,		{_T("AxisA Over accurate"),	_T("A軸 過速度")}},
	{17,	IDC_WARN_BIT17,	{_T(""),					_T("")},					IDC_ERR_BIT17,		{_T("AxisB Over accurate"),	_T("B軸 過速度")}},
	{18,	IDC_WARN_BIT18,	{_T("AxisA Limit speed"),	_T("A軸 速度リミット")},	IDC_ERR_BIT18,		{_T(""),					_T("")}},
	{19,	IDC_WARN_BIT19,	{_T("AxisB Limit speed"),	_T("B軸 速度リミット")},	IDC_ERR_BIT19,		{_T(""),					_T("")}},
	{20,	IDC_WARN_BIT20,	{_T("AxisA Limit current"),	_T("A軸 電流リミット")},	IDC_ERR_BIT20,		{_T(""),					_T("")}},
	{21,	IDC_WARN_BIT21,	{_T("AxisB Limit current"),	_T("B軸 電流リミット")},	IDC_ERR_BIT21,		{_T(""),					_T("")}},
	{22,	IDC_WARN_BIT22,	{_T("Timeout.comm"),		_T("通信タイムアウト")},	IDC_ERR_BIT22,		{_T("Timeout.comm"),		_T("通信タイムアウト")}},
	{23,	IDC_WARN_BIT23,	{_T(""),					_T("")},					IDC_ERR_BIT23,		{_T("Over comminucation"),	_T("通信トラフィック過大")}},
	{24,	IDC_WARN_BIT24,	{_T(""),					_T("")},					IDC_ERR_BIT24,		{_T(""),					_T("")}},
	{25,	IDC_WARN_BIT25,	{_T(""),					_T("")},					IDC_ERR_BIT25,		{_T(""),					_T("")}},
	{26,	IDC_WARN_BIT26,	{_T(""),					_T("")},					IDC_ERR_BIT26,		{_T(""),					_T("")}},
	{27,	IDC_WARN_BIT27,	{_T(""),					_T("")},					IDC_ERR_BIT27,		{_T(""),					_T("")}},
	{28,	IDC_WARN_BIT28,	{_T("Code.Alarm0"),			_T("アラームコード0")},		IDC_ERR_BIT28,		{_T("Code Error0"),			_T("エラーコード0")}},
	{29,	IDC_WARN_BIT29,	{_T("Code.Alarm1"),			_T("アラームコード1")},		IDC_ERR_BIT29,		{_T("Code Error1"),			_T("エラーコード1")}},
	{30,	IDC_WARN_BIT30,	{_T("Code.Alarm2"),			_T("アラームコード2")},		IDC_ERR_BIT30,		{_T("Code Error2"),			_T("エラーコード2")}},
	{31,	IDC_WARN_BIT31,	{_T("Code.Alarm3"),			_T("アラームコード3")},		IDC_ERR_BIT31,		{_T("Code Error3"),			_T("エラーコード3")}},
	{0,		0,				{NULL,						NULL},						0,					{NULL,						NULL}},
	};

//
IMPLEMENT_DYNCREATE(CguicanABH3View,CFormView)

//
BEGIN_MESSAGE_MAP(CguicanABH3View,CFormView)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_CONTROL_RANGE(EN_CHANGE,IDC_EDIT0,IDC_EDIT1,&CguicanABH3View::OnEnChangeEdit)
	ON_COMMAND_RANGE(IDC_OFF_CTRLID0,IDC_OFF_CTRLID22,&CguicanABH3View::OnBnClickedOffCtrlid)
	ON_COMMAND_RANGE(IDC_ON_CTRLID0,IDC_ON_CTRLID22,&CguicanABH3View::OnBnClickedOnCtrlid)
	ON_COMMAND(IDC_ENABLE_CYCLE,&CguicanABH3View::OnEnableCycle)
	ON_COMMAND(IDC_DISABLE_CYCLE,&CguicanABH3View::OnDisableCycle)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REQUESTDP0R,&CguicanABH3View::OnBnClickedRequestItem)
	ON_BN_CLICKED(IDC_REQUESTBR0,&CguicanABH3View::OnBnClickedRequestItem)
	ON_BN_CLICKED(IDC_REQUESTBR1,&CguicanABH3View::OnBnClickedRequestItem)
	ON_BN_CLICKED(IDC_REQUESTBR2,&CguicanABH3View::OnBnClickedRequestItem)
	ON_BN_CLICKED(IDC_REQUESTBR3,&CguicanABH3View::OnBnClickedRequestItem)
	ON_BN_CLICKED(IDC_REQUESTBR4,&CguicanABH3View::OnBnClickedRequestItem)
	ON_BN_CLICKED(IDC_REQUESTBR5,&CguicanABH3View::OnBnClickedRequestItem)
	ON_BN_CLICKED(IDC_REQUESTBR6,&CguicanABH3View::OnBnClickedRequestItem)
END_MESSAGE_MAP()

//コンストラクタ
CguicanABH3View::CguicanABH3View() noexcept
	: CFormView(IDD_GUI_CANABH3_FORM)
	{
	//内部変数初期化
	::ZeroMemory(&m_var,sizeof(VIEW_VAR));

	//描画用ブラシ構築
	m_brush.CreateSolidBrush(COLOR_BLUE);
	}

//デストラクタ
CguicanABH3View::~CguicanABH3View()
	{
	//描画用ブラシ解放
	m_brush.DeleteObject();
	}

//診断
#ifdef _DEBUG
void CguicanABH3View::AssertValid() const
	{
	CFormView::AssertValid();
	}
void CguicanABH3View::Dump(CDumpContext& dc) const
	{
	CFormView::Dump(dc);
	}
CguicanABH3Doc* CguicanABH3View::GetDocument() const // デバッグ以外のバージョンはインラインです。
	{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CguicanABH3Doc)));
	return (CguicanABH3Doc*)m_pDocument;
	}
#endif //_DEBUG

//DDE
void CguicanABH3View::DoDataExchange(CDataExchange* pDX)
	{
	CFormView::DoDataExchange(pDX);

	//ID
	DDX_Control(pDX,IDC_TITLE_ID,m_id);
	//FPS
	DDX_Control(pDX,IDC_FPS,m_fps);
	//指令
	DDX_Control(pDX,IDC_EDIT0,m_edit[0]);
	DDX_Control(pDX,IDC_EDIT1,m_edit[1]);
	//32項目系
	IDTEXT item;
	IDTEXT2 item2;
	IDTEXT3 item3;
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		//制御フラグ
		item3 = g_resultid_tbl[nLoop];
		DDX_Control(pDX,item3.nUid,m_result_bit[nLoop]);
		//入力フラグ
		item3 = g_inputid_tbl[nLoop];
		DDX_Control(pDX,item3.nUid,m_input_bit[nLoop]);
		//I/Oフラグ
		if(theConfig.getABH3type(GetDocument()->GetID()) == 0)
			item = g_io_normal_tbl[nLoop];
		else
			item = g_io_small_tbl[nLoop];
		DDX_Control(pDX,item.nUid,m_io_bit[nLoop]);
		//警告とエラー
		item2 = g_warnerr_tbl[nLoop];
		DDX_Control(pDX,item2.nOffUid,m_warn_bit[nLoop]);
		DDX_Control(pDX,item2.nOnUid,m_err_bit[nLoop]);
		//操作フラグ
		item2 = g_ctrlid_ctrl[nLoop];
		DDX_Control(pDX,item2.nOffUid,m_ctrl_off_bit[nLoop]);
		DDX_Control(pDX,item2.nOnUid,m_ctrl_on_bit[nLoop]);
		}
	}

//
BOOL CguicanABH3View::PreCreateWindow(CREATESTRUCT& cs)
	{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CFormView::PreCreateWindow(cs);
	}

//ダイアログ初期化時に呼び出されます
void CguicanABH3View::OnInitialUpdate()
	{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	//ダイアログアイテムの再配置（間隔を一定にする為）
	ArrangementItem();

	//シートタブ名とフォーム左上のタイトルを更新
	UpdateTitle();

	//指令入力箇所の設定（変更時に白文字赤背景、初期値"0.0"）
	for(int nLoop = 0;nLoop < 2;nLoop++)
		{
		m_edit[nLoop].SetChangeColor(true,COLOR_WHITE,COLOR_RED);
		m_edit[nLoop].SetWindowText(_T("0.0"));
		}

	//ダイアログアイテムに文字列設定
	SetTextTbl(g_anyitem_tbl);
	SetTextTbl(g_request_tbl);
	SetTextTbl(g_valueid_tbl);
	SetTextTbl(g_ctrlid_title);
	SetTextTbl(g_ctrlid_ctrl);
	if(theConfig.getABH3type(GetDocument()->GetID()) == 0)
		SetTextTbl(g_io_normal_tbl);	//標準タイプ
	else
		SetTextTbl(g_io_small_tbl);		//小型タイプ
	SetTextTbl(g_warnerr_tbl);

	//ボタンの初期値
	SetButtonTbl(g_request_tbl,false);
	SetButtonTbl(g_ctrlid_ctrl,false);

	//周期転送の復帰
	RestoreSendButton(GetDocument()->GetID());

	//データ表示更新
	UpdateView(true);

	//タイマー割り込み
	uint32_t nInterval = theConfig.getInterval();
	m_var.nTimerNum = SetTimer(theApp.GetTimerNum(),nInterval,NULL);
	m_var.n1sec = SetTimer(theApp.GetTimerNum(),1000,NULL);

	//非同期送信スレッド
	m_var.thread.hThread = (HANDLE)_beginthreadex(NULL,0,CguicanABH3View::SendThread,this,0,&m_var.thread.nUid);
	}

//ダイアログアイテムの色制御時に呼び出されます
HBRUSH CguicanABH3View::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)
	{
	bool bDraw = false;
	COLORREF nTextColor = COLOR_BLACK;
	COLORREF nBackColor = COLOR_WHITE;

	//色付け対象か？
	if(DrawCheck(pWnd,nTextColor,nBackColor))
		{
		//
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(nBackColor);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(nBackColor);
		pDC->SetTextColor(nTextColor);
		return(m_brush);
		}

	//標準処理
	HBRUSH hbr = CFormView::OnCtlColor(pDC,pWnd,nCtlColor);
	return hbr;
	}

//画面上の周期要求要素を取り込み
uint32_t CguicanABH3View::disp2cycle()
	{
	//戻り値
	uint32_t nResult = 0;
	//値の取得
	int nLoop = 0;
	while(-1)
		{
		//テーブルから情報取得
		IDTEXT5 item = g_request_tbl[nLoop];
		//終端？
		if(item.nUid == 0)
			break;
		//ボタン状態を取得して指定bitに格納
		CButton* pBtn = (CButton*)GetDlgItem(item.nUid);
		if(pBtn->GetCheck())
			nResult = nResult | (1 << nLoop);
		//次の項目
		++nLoop;
		}
	//
	return(nResult);
	}


//タイマー割り込み時に呼び出されます
void CguicanABH3View::OnTimer(UINT_PTR nIDEvent)
	{
	static bool m_bTimerRunning = false;
	if(!m_bTimerRunning)
		{
		m_bTimerRunning = true;

		//現在、このビューがアクティブかどうか（親フレーム側に処理実装されている）
		//注意：100ms周期程度で通信してないと通信エラーでサーボOFFされるので、周期処理間隔は変えない
		bool bActive = ((CChildFrame*)GetParentFrame())->IsActivate();


		//1秒割り込み？
		if(nIDEvent == m_var.n1sec)
			{
			//フレームレート表示有り？
			if(theConfig.GetConfig()->nFPS)
				{
				//このウィンドウがアクティブ（最前面）？
				if(bActive)
					{
					//現在の値を表示（英語固定）
					CString sText("");
					sText.Format(_T("SEND(%d)"),m_var.nSendCounter);
					FastSetText(IDC_FPS,sText);
					}
				//周期割り込みカウンタ初期化
				m_var.nFPS = 0;
				//送信カウンタ初期化
				m_var.nSendCounter = 0;
				}
			}
		else if(nIDEvent == m_var.nTimerNum)
			{
			//表示更新回数加算
			++m_var.nFPS;

			//最前面に表示されていれば、表示物を更新
			if(bActive)
				UpdateView();

			//周期要求要素の取り込み
			m_var.cycle.nCycleRequest = disp2cycle();

			//周期要求
			m_var.thread.bRequest = true;

			//周期要求の結果を確認（今回の要求結果とは限らない）
			uint32_t nResult = m_var.thread.nResult;
			if(nResult)
				{
				//エラーになった箇所をチェック無しにする
				IDTEXT5 item = g_request_tbl[nResult - 1];
				SetCheckbox(item.nUid,false);
				//送信エラーを設定
				theApp.SetErrorID(GetDocument()->GetID());
				//再開
				m_var.thread.nResult = 0;
				}
			}
		//
		m_bTimerRunning = false;
		}
	else
		{
		//
		}

	CFormView::OnTimer(nIDEvent);
	}

//非同期送信スレッド
 unsigned __stdcall CguicanABH3View::SendThread(void* pParam)
	{
	//
	CguicanABH3View* pClass = (CguicanABH3View*)pParam;
	volatile CguicanABH3View::pVIEW_VAR pVar = &pClass->m_var;

	while(!pVar->thread.bQuit)
		{
		//送信要求があるか？
		if(pVar->thread.bRequest)
			{
			//要求受理したのでフラグ解除（送信中に再設定される場合も有る）
			pVar->thread.bRequest = false;
			//エラー状態では無いか？
			if(pVar->thread.nResult == 0)
				{
				//回線を開いているか？
				if(theABH3.IsOpenInterface())
					{
					//送信処理
					pVar->thread.nResult = pClass->ExecCycleRequest(pVar->cycle.nCycleRequest);
					//送信カウンタ更新
					++pVar->nSendCounter;
					}
				}
			}

		Sleep(1);
		}

	//
	_endthreadex(0);
	return(0);
	}

//「全ての周期送信を有効化」を選択すると呼び出されます
void CguicanABH3View::OnEnableCycle()
	{
	SetButtonTbl(g_request_tbl,true);
	}

//「全ての周期送信を無効化」を選択すると呼び出されます
void CguicanABH3View::OnDisableCycle()
	{
	SetButtonTbl(g_request_tbl,false);
	}

//テーブルを使用してダイアログアイテムに文字列を設定
void CguicanABH3View::SetTextTbl(pIDTEXT pIDtbl)
	{
	int nLoop = 0;
	while(pIDtbl->nUid)
		{
		GetDlgItem(pIDtbl->nUid)->SetWindowText(theApp.GetLangText(&pIDtbl->text));
		++pIDtbl;
		}
	}
void CguicanABH3View::SetTextTbl(pIDTEXT1 pIDtbl)
	{
	int nLoop = 0;
	while(pIDtbl->nUid)
		{
		GetDlgItem(pIDtbl->nUid)->SetWindowText(theApp.GetLangText(&pIDtbl->text));
		++pIDtbl;
		}
	}
void CguicanABH3View::SetTextTbl(pIDTEXT2 pIDtbl)
	{
	int nLoop = 0;
	while(pIDtbl->nOffUid)
		{
		GetDlgItem(pIDtbl->nOffUid)->SetWindowText(theApp.GetLangText(&pIDtbl->offtext));
		GetDlgItem(pIDtbl->nOnUid)->SetWindowText(theApp.GetLangText(&pIDtbl->ontext));
		++pIDtbl;
		}
	}
void CguicanABH3View::SetTextTbl(pIDTEXT4 pIDtbl)
	{
	int nLoop = 0;
	while(pIDtbl->nTitleUid)
		{
		GetDlgItem(pIDtbl->nTitleUid)->SetWindowText(theApp.GetLangText(&pIDtbl->text));
		++pIDtbl;
		}
	}
void CguicanABH3View::SetTextTbl(pIDTEXT5 pIDtbl)
	{
	int nLoop = 0;
	while(pIDtbl->nUid)
		{
		GetDlgItem(pIDtbl->nUid)->SetWindowText(pIDtbl->pText);
		++pIDtbl;
		}
	}

//指定テーブルのボタンを操作
void CguicanABH3View::SetButtonTbl(pIDTEXT2 pIDtbl,bool bOn)
	{
	int nLoop = 0;
	while(pIDtbl->nOffUid)
		{
		CButton* pON = (CButton*)GetDlgItem(pIDtbl->nOnUid);
		CButton* pOFF = (CButton*)GetDlgItem(pIDtbl->nOffUid);

		if(bOn)
			{
			pON->SetCheck(1);
			pOFF->SetCheck(0);
			}
		else
			{
			pON->SetCheck(0);
			pOFF->SetCheck(1);
			}
		++pIDtbl;
		}
	}
void CguicanABH3View::SetButtonTbl(pIDTEXT5 pIDtbl,bool bOn)
	{
	int nLoop = 0;
	while(pIDtbl->nUid)
		{
		CButton* pBtn = (CButton*)GetDlgItem(pIDtbl->nUid);
		pBtn->SetCheck((bOn != false) ? 1 : 0);
		++pIDtbl;
		}
	}


//色制御確認
bool CguicanABH3View::DrawCheck(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor)
	{

	//戻り値（描画フラグ）
	if(DrawCheck_0(pWnd,nTextColor,nBackColor))
		return(true);
	else if(DrawCheck_1(pWnd,nTextColor,nBackColor))
		return(true);
	else if(DrawCheck_2(pWnd,nTextColor,nBackColor))
		return(true);
	else if(DrawCheck_3(pWnd,nTextColor,nBackColor))
		return(true);
	else if(DrawCheck_4(pWnd,nTextColor,nBackColor))
		return(true);
	else if(DrawCheck_5(pWnd,nTextColor,nBackColor))
		return(true);
	return(false);
	}

//固定テキストアイテムの色制御確認
bool CguicanABH3View::DrawCheck_0(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor)
	{
	//ID取得
	UINT nUid = pWnd->GetDlgCtrlID();

	//ID
	if(pWnd == &m_id)
		{
		if(theApp.isErrorID(GetDocument()->GetID()))
			{
			//エラー発生中
			nTextColor = COLOR_WHITE;
			nBackColor = COLOR_RED;
			}
		else
			{
			//正常動作中
			nTextColor = COLOR_WHITE;
			nBackColor = COLOR_BLUE;
			}
		return(true);
		}

	//FPS
	if(pWnd == &m_fps)
		{
		if(theConfig.GetConfig()->nFPS)
			{
			//フレームレート表示中
			nTextColor = COLOR_BLACK;
			nBackColor = COLOR_YELLOW;
			}
		else
			{
			//通常時
			nTextColor = COLOR_WHITE;
			nBackColor = COLOR_BLUE;
			}
		return(true);
		}

	//固定色表示テーブル
	int nLoop = 0;
	while(-1)
		{
		IDCOLOR1 info = g_textcolor_tbl[nLoop];
		if(info.nBeginUid == 0)
			break;
		else if(((info.nBeginUid == nUid) && (info.nEndUid == 0)) || ((info.nBeginUid <= nUid) && (info.nEndUid >= nUid)))
			{
			nTextColor = g_color_tbl[info.nColorIndex].nText;
			nBackColor = g_color_tbl[info.nColorIndex].nBack;
			return(true);
			}
		++nLoop;
		}
	return(false);
	}

//入力フラグの色制御確認
bool CguicanABH3View::DrawCheck_1(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor)
	{
	//描画対象が警告部か確認
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		//該当？
		if(pWnd == &m_input_bit[nLoop])
			{
			//該当箇所の情報を取得
			IDTEXT3 item3 = g_inputid_tbl[nLoop];				
			uint32_t nValue = m_var.lastdata.BR1.nCtrlBit;
			//成立中？
			if(IsBit(nValue,item3.nBit))
				{
				nTextColor = RGB(255,255,255);
				nBackColor = RGB(0,0,255);
				return(true);
				}
			return(false);
			}
		}
	return(false);
	}

//制御フラグの色制御確認
bool CguicanABH3View::DrawCheck_2(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor)
	{
	//描画対象が警告部か確認
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		//該当？
		if(pWnd == &m_result_bit[nLoop])
			{
			//該当箇所の情報を取得
			IDTEXT3 item3 = g_resultid_tbl[nLoop];				
			uint32_t nValue = m_var.lastdata.DP0R.nCtrlBit;
			//成立中？
			if(IsBit(nValue,item3.nBit))
				{
				nTextColor = RGB(255,255,255);
				nBackColor = RGB(0,0,255);
				return(true);
				}
			return(false);
			}
		}
	return(false);
	}

//I/Oフラグの色制御確認
bool CguicanABH3View::DrawCheck_3(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor)
	{
	//
	IDTEXT item;
	//描画対象が警告部か確認
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		//該当？
		if(pWnd == &m_io_bit[nLoop])
			{
			//該当箇所の情報を取得
			if(theConfig.getABH3type(GetDocument()->GetID()) == 0)
				item = g_io_normal_tbl[nLoop];				
			else
				item = g_io_small_tbl[nLoop];				
			uint32_t nValue = m_var.lastdata.BR1.nIoBit;
			//成立中？
			if(IsBit(nValue,item.nBit))
				{
				nTextColor = RGB(255,255,255);
				nBackColor = RGB(0,0,255);
				return(true);
				}
			return(false);
			}
		}
	return(false);
	}

//警告フラグの色制御確認
bool CguicanABH3View::DrawCheck_4(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor)
	{
	//描画対象が警告部か確認
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		//該当？
		if(pWnd == &m_warn_bit[nLoop])
			{
			//該当箇所の情報を取得
			IDTEXT2 item2 = g_warnerr_tbl[nLoop];				
			uint32_t nValue = m_var.lastdata.BR0.nWarnBit;
			//成立中？
			if(IsBit(nValue,item2.nBit))
				{
				nTextColor = RGB(0,0,0);
				nBackColor = RGB(255,255,0);
				return(true);
				}
			return(false);
			}
		}
	return(false);
	}

//異常フラグの色制御確認
bool CguicanABH3View::DrawCheck_5(CWnd* pWnd,COLORREF& nTextColor,COLORREF& nBackColor)
	{
	//描画対象が警告部か確認
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		//該当？
		if(pWnd == &m_err_bit[nLoop])
			{
			//該当箇所の情報を取得
			IDTEXT2 item2 = g_warnerr_tbl[nLoop];				
			uint32_t nValue = m_var.lastdata.BR0.nErrorBit;
			//成立中？
			if(IsBit(nValue,item2.nBit))
				{
				//発生中
				nTextColor = RGB(255,255,255);
				nBackColor = RGB(255,0,0);
				return(true);
				}
			return(false);
			}
		}
	return(false);
	}

//
void CguicanABH3View::UpdateView(bool bForce /* false */)
	{
	//このビューに割り当てられたID
	uint8_t nID = GetDocument()->GetID();

	//最終受信データを取得
	theABH3.abh3_can_copylastdata(nID,&m_var.lastdata);

	//各ビュー要素の更新
	UpdateView_0(bForce);
	UpdateView_1(bForce);
	UpdateView_2(bForce);
	UpdateView_3(bForce);
	UpdateView_4(bForce);
	UpdateView_5(bForce);
	UpdateView_6(bForce);
	UpdateView_7(bForce);

	//タイトル更新
	UpdateTitle();
	}

//帰還・制御フラグの更新(要素0)
void CguicanABH3View::UpdateView_0(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 0;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//========================================
	//帰還
	//========================================
	FastSetText(g_valueid_tbl[0].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.DP0R.nBackAY)));
	FastSetText(g_valueid_tbl[1].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.DP0R.nBackBX)));

	//========================================
	//制御フラグ
	//========================================

	//現在の値を取得
	uint32_t nValue = m_var.lastdata.DP0R.nCtrlBit;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);

	//全項目処理
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		//コントロール変数番号nLoopに対する情報を取得し、状態の文字列を設定
		IDTEXT3 item = g_resultid_tbl[nLoop];
		if(IsBit(nValue,item.nBit))
			FastSetText(item.nUid,theApp.GetLangText(&item.ontext));		//ON
		else
			FastSetText(item.nUid,theApp.GetLangText(&item.offtext));	//OFF
		}

	//========================================
	//更新フラグ解除
	//========================================
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);
	}

//異常・警告フラグの更新(要素1)
void CguicanABH3View::UpdateView_1(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 1;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//========================================
	//異常フラグ
	//========================================
	//再描画処理
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		IDTEXT2 item = g_warnerr_tbl[nLoop];
		GetDlgItem(item.nOffUid)->Invalidate();
		}

	//========================================
	//警告フラグ
	//========================================
	//再描画処理
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		IDTEXT2 item = g_warnerr_tbl[nLoop];
		GetDlgItem(item.nOnUid)->Invalidate();
		}

	//========================================
	//更新フラグ解除
	//========================================
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);
	}

//I/Oフラグ・入力フラグの更新(要素2)
void CguicanABH3View::UpdateView_2(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 2;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//========================================
	//I/Oフラグ
	//========================================
	//再描画処理
	IDTEXT item;
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		if(theConfig.getABH3type(GetDocument()->GetID()) == 0)
			item = g_io_normal_tbl[nLoop];
		else
			item = g_io_small_tbl[nLoop];
		GetDlgItem(item.nUid)->Invalidate();
		}

	//========================================
	//入力フラグ
	//========================================
	//現在の値を取得
	uint32_t nValue = m_var.lastdata.BR1.nCtrlBit;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);

	//全項目処理
	for(int nLoop = 0;nLoop < 32;nLoop++)
		{
		//コントロール変数番号nLoopに対する情報を取得し、状態の文字列を設定
		IDTEXT3 item = g_inputid_tbl[nLoop];
		if(IsBit(nValue,item.nBit))
			FastSetText(item.nUid,theApp.GetLangText(&item.ontext));	//ON
		else
			FastSetText(item.nUid,theApp.GetLangText(&item.offtext));	//OFF
		}

	//========================================
	//更新フラグ解除
	//========================================
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);
	}

//速度指令・速度帰還の更新(要素3)
void CguicanABH3View::UpdateView_3(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 3;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//========================================
	//速度指令
	//========================================
	FastSetText(g_valueid_tbl[2].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.BR2.nOrderSpeedAY)));
	FastSetText(g_valueid_tbl[3].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.BR2.nOrderSpeedBX)));

	//========================================
	//速度帰還
	//========================================
	FastSetText(g_valueid_tbl[4].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.BR2.nBackSpeedAY)));
	FastSetText(g_valueid_tbl[5].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.BR2.nBackSpeedBX)));

	//========================================
	//更新フラグ解除
	//========================================
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);
	}

//電流指令・負荷率の更新(要素4)
void CguicanABH3View::UpdateView_4(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 4;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//========================================
	//電流指令
	//========================================
	FastSetText(g_valueid_tbl[6].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Cur(m_var.lastdata.BR3.nOrderCurrentAY)));
	FastSetText(g_valueid_tbl[7].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Cur(m_var.lastdata.BR3.nOrderCurrentBX)));

	//========================================
	//負荷率
	//========================================
	FastSetText(g_valueid_tbl[8].nValueUid,float2text(_T("%.0f"),theABH3.cnvCAN2Load(m_var.lastdata.BR3.nLoadA)));
	FastSetText(g_valueid_tbl[9].nValueUid,float2text(_T("%.0f"),theABH3.cnvCAN2Load(m_var.lastdata.BR3.nLoadB)));

	//========================================
	//更新フラグ解除
	//========================================
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);
	}

//パルス積算値の更新(要素5)
void CguicanABH3View::UpdateView_5(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 5;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//========================================
	//パルス積算値
	//========================================
	FastSetText(g_valueid_tbl[10].nValueUid,uint2text(_T("%d"),m_var.lastdata.BR4.nCountPulseA));
	FastSetText(g_valueid_tbl[11].nValueUid,uint2text(_T("%d"),m_var.lastdata.BR4.nCountPulseB));

	//========================================
	//更新フラグ解除
	//========================================
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);
	}

//アナログ入力・電源電圧の更新(要素6)
void CguicanABH3View::UpdateView_6(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 6;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//========================================
	//アナログ入力
	//========================================
	FastSetText(g_valueid_tbl[12].nValueUid,float2text(_T("%.2f"),theABH3.cnvCAN2Analog(m_var.lastdata.BR5.nAnalog0)));
	FastSetText(g_valueid_tbl[13].nValueUid,float2text(_T("%.2f"),theABH3.cnvCAN2Analog(m_var.lastdata.BR5.nAnalog1)));

	//========================================
	//電源
	//========================================
	FastSetText(g_valueid_tbl[14].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Volt(m_var.lastdata.BR5.nPowerMain)));
	FastSetText(g_valueid_tbl[15].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Volt(m_var.lastdata.BR5.nPowerCtrl)));

	//========================================
	//更新フラグ解除
	//========================================
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);
	}

//モニタデータの更新(要素7)
void CguicanABH3View::UpdateView_7(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 7;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//========================================
	//モニタデータ（正体不明）
	//========================================
	FastSetText(g_valueid_tbl[16].nValueUid,float2text(_T("%f"),m_var.lastdata.BR6.nMonitor0));
	FastSetText(g_valueid_tbl[17].nValueUid,float2text(_T("%f"),m_var.lastdata.BR6.nMonitor1));

	//========================================
	//更新フラグ解除
	//========================================
	theABH3.abh3_can_resetlastdata(GetDocument()->GetID(),nPos);
	}

//周期要求設定の実行
uint32_t CguicanABH3View::ExecCycleRequest(uint32_t nCycleRequest)
	{
	//
	uint32_t nResult = 0;
	int32_t nLoop = 0;
	IDTEXT5 item;
	//
	while(-1)
		{
		//情報取り出しと終端判断
		item = g_request_tbl[nLoop];;
		if(item.nUid == 0)
			break;

		//周期処理要求あり？
		else if(nCycleRequest & (1 << nLoop))
			{
			//シングルパケット？
			if(item.nType == PACKETTYPE::SINGLE_PACKET)
				{
				//指令の変換
				//	仕様がいまいち不明なので、速度扱いで変換する
				uint16_t nA = theABH3.cnvVel2CAN(m_var.DP0R.nValue[0]);
				uint16_t nB = theABH3.cnvVel2CAN(m_var.DP0R.nValue[1]);
				//シングルパケット・指令と操作を一括
				if(theABH3.abh3_can_cmdAndopSet(
					GetDocument()->GetID(),
					nA,
					nB,
					m_var.DP0R.nRequest,
					0,
					NULL
					))
					{
					//エラー
					nResult = nLoop + 1;
					break;
					}
				}
			//ブロードキャスト？
			else if(item.nType == PACKETTYPE::BROADCAST_PACKET)
				{
				//グループとアドレスからコードを作る
				uint8_t nCode = (GetDocument()->GetGroup() << 3) | (item.nAdrs & 0x07);

				if(theABH3.abh3_can_reqBRD(
					GetDocument()->GetID(),
					nCode,
					0,
					NULL))
					{
					nResult = nLoop + 1;
					break;
					}
				}
			}
		//
		++nLoop;
		}

	//正常終了
	return(nResult);
	}

//指令が更新されると呼び出されます
void CguicanABH3View::OnEnChangeEdit(UINT nUid)
	{
	//
	UINT nEdit = nUid - IDC_EDIT0;

	//確定した？
	if(m_edit[nEdit].IsSetData())
		{
		//確定したので、入力文字を取得し格納
		CString sText("");
		m_edit[nEdit].GetWindowText(sText);
		float nValue = (float)::_tcstod((LPCTSTR)sText,NULL);
		m_var.DP0R.nValue[nEdit] = nValue;

		//入力値をCANで扱う値に変換
		//int16_t nValue16 = theABH3.cnvVel2CAN(nValue);
		////
		//int32_t nResult = theABH3.abh3_can_cmdAY(GetDocument()->GetID(),nValue16,NULL);
		}
	}

//操作フラグのOFFボタンが押されると呼び出されます
void CguicanABH3View::OnBnClickedOffCtrlid(UINT nUid)
	{
	//押されたボタン
	UINT nBtn = nUid - IDC_OFF_CTRLID0;
	//指定ビットを解除
	m_var.DP0R.nRequest &= ~(1 << nBtn);
	}

//操作フラグのONボタンが押されると呼び出されます
void CguicanABH3View::OnBnClickedOnCtrlid(UINT nUid)
	{
	//押されたボタン
	UINT nBtn = nUid - IDC_ON_CTRLID0;
	//指定ビットを設定
	m_var.DP0R.nRequest |= (1 << nBtn);
	}

//画面アイテムの再配置と表示
void CguicanABH3View::ArrangementItem()
	{
	//ダイアログ上のアイテムを再配置
	CRect rect1 = ArrangementItem_sub(IDC_TITLE_A,IDC_TITLE_B);
	ArrangementItem_sub(IDC_EDIT0,IDC_EDIT1);
	ArrangementItem_sub(IDC_TITLE_VALUE0,IDC_TITLE_VALUE17);
	ArrangementItem_sub(IDC_VALUE0,IDC_VALUE17);
	ArrangementItem_sub(IDC_TITLE_CTRLID0,IDC_TITLE_CTRLID31);
	ArrangementItem_sub(IDC_OFF_CTRLID0,IDC_OFF_CTRLID31);
	ArrangementItem_sub(IDC_ON_CTRLID0,IDC_ON_CTRLID31);
	ArrangementItem_sub(IDC_INPUTID0,IDC_INPUTID31);
	ArrangementItem_sub(IDC_RESULTID0,IDC_RESULTID31);
	ArrangementItem_sub(IDC_IO0,IDC_IO31);
	ArrangementItem_sub(IDC_WARN_BIT0,IDC_WARN_BIT31);
	CRect rect2 = ArrangementItem_sub(IDC_ERR_BIT0,IDC_ERR_BIT31);
	CRect rect3 = MoveItem(IDC_TITLE_CYCLE,CPoint(rect1.left,rect2.bottom + (rect1.Height() / 2)));
	MoveItem(IDC_REQUESTDP0R,CPoint(rect3.right + (rect1.Height() / 2),rect3.top));

	//周期要求要素は横に並べる
	ArrangementItem_sub(IDC_REQUESTDP0R,IDC_REQUESTBR6,false);

	//フォームの全アイテムを表示する（フォーム上のアイテムは、初期状態は全て非表示）
	CWnd* pWnd = GetTopWindow();
	while(pWnd)
		{
		pWnd->ShowWindow(SW_SHOW);
		pWnd = pWnd->GetNextWindow();
		}
	}

//ダイアログアイテムを詰めて並べる
CRect CguicanABH3View::ArrangementItem_sub(UINT nBaseUID,UINT nLastUID,bool bVert /* true */)
	{
	//概要
	//	ダイアログアイテムを詰めて並べる
	//パラメータ
	//	nBaseID		基準となるダイアログアイテムのID
	//	nLastID		移動対象となる最後のダイアログアイテムのID
	//	bVert		trueで下方向に並べ、falseで右方向に並べる
	//戻り値
	//	最後の移動アイテムの位置をクライアント座標で戻す
	//注意
	//	配置するダイアログアイテムのIDは、連続している事が前提となる。
	//	ダイアログアイテムのIDは、resource.hを直接編集する事が前提となる
	//

	//
	CRect itemRect,moveRect;
	//基準のアイテム位置を取得
	GetDlgItem(nBaseUID)->GetWindowRect(itemRect);
	ScreenToClient(itemRect);
	moveRect = itemRect;
	int nHeight = itemRect.Height();
	int nWidth = itemRect.Width();
	for(UINT nUid = nBaseUID + 1;nUid <= nLastUID;nUid++)
		{
		if(bVert)
			{
			moveRect.top = moveRect.bottom;
			moveRect.bottom = moveRect.top + nHeight;
			}
		else
			{
			moveRect.left = moveRect.right;
			moveRect.right = moveRect.left + nWidth;
			}
		GetDlgItem(nUid)->MoveWindow(moveRect);
		}
	//最後のアイテム位置を戻す
	return(moveRect);
	}

//ウィンドウタイトルを更新
void CguicanABH3View::UpdateTitle()
	{
	//このビューに割り当てられたID
	uint8_t nID = GetDocument()->GetID();

	//タイトル（タブシート名を兼ねる）設定
	CString sText("");
	sText.Format(_T("ID = %02Xh"),nID);

	//エラー有り？
	if(theApp.isErrorID(nID))
		sText += _T(" (Detected I/F error)");

	//シートタブ名
	GetParentFrame()->SetWindowText(sText);

	//フォームアイテム
	GetDlgItem(IDC_TITLE_ID)->SetWindowText(sText);
	}

//ウィンドウが破棄される時に呼び出されます
void CguicanABH3View::OnDestroy()
	{
	CFormView::OnDestroy();

	//非同期送信スレッド停止
	m_var.thread.bQuit = true;
	::WaitForSingleObject(m_var.thread.hThread,INFINITE);
	}

//周期転送の復帰
void CguicanABH3View::RestoreSendButton(uint8_t nID)
	{
	uint32_t nValue = theConfig.GetConfig()->send256[nID];
	uint32_t nLoop = 0;
	while(-1)
		{
		IDTEXT5 item = g_request_tbl[nLoop];
		if(item.nUid == 0)
			break;
		CButton* pBtn = (CButton*)GetDlgItem(item.nUid);
		pBtn->SetCheck((1 << nLoop) & nValue ? TRUE : FALSE);
		++nLoop;
		}
	}


//周期送信ボタンが操作されると呼び出されます
void CguicanABH3View::OnBnClickedRequestItem()
	{
	SaveSendButton(GetDocument()->GetID());
	}

//周期転送の保存
void CguicanABH3View::SaveSendButton(uint8_t nID)
	{
	//表示を取り込み
	uint32_t nValue = 0;
	uint32_t nLoop = 0;
	while(-1)
		{
		IDTEXT5 item = g_request_tbl[nLoop];
		if(item.nUid == 0)
			break;
		CButton* pBtn = (CButton*)GetDlgItem(item.nUid);
		if(pBtn->GetCheck())
			nValue |= (1 << nLoop);
		++nLoop;
		}
	//環境設定内に保存
	theConfig.GetConfig()->send256[nID] = nValue;
	//システムへ保存
	theConfig.reg2sys();
	}
