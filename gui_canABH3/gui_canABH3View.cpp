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

//注意点
//	ダイアログに256以上のアイテムを配置している関係か、以下の関数を呼び出すと
//	正常動作しなくなります。
//		AfxMessageBox
//	本Viewクラスでは使わないで下さい。

//
#include "pch.h"
#include "resource.h"
#include "framework.h"
#include "gui_canABH3.h"
#include "gui_canABH3Doc.h"
#include "gui_canABH3View.h"
#include "ChildFrm.h"
#include "SelectID.h"
//#include "MainFrm.h"

#ifdef _DEBUG
 #define new DEBUG_NEW
#endif

//================================================================================
//固定色アイテムの色設定テーブル
//
//概要
//	色が変化しないダイアログアイテムの定義
//
//注意
//	終了側IDが無い（単一指定の場合）は、終了側IDを0に設定
//	列挙子の説明は pch.h に有る
//	設定色自体は pch.cpp に定義が有る
//================================================================================
static IDCOLOR g_textcolor_tbl[] = {
	//beginID				endID					APPCOLOR
	{IDC_TITLE_A,			0,						APPCOLOR::APPC_AXISA},
	{IDC_TITLE_B,			0,						APPCOLOR::APPC_AXISB},
	{IDC_TITLE_VALUE0,		0,						APPCOLOR::APPC_AXISA},
	{IDC_TITLE_VALUE1,		0,						APPCOLOR::APPC_AXISB},
	{IDC_TITLE_VALUE2,		0,						APPCOLOR::APPC_AXISA},
	{IDC_TITLE_VALUE3,		0,						APPCOLOR::APPC_AXISB},
	{IDC_TITLE_VALUE4,		0,						APPCOLOR::APPC_AXISA},
	{IDC_TITLE_VALUE5,		0,						APPCOLOR::APPC_AXISB},
	{IDC_TITLE_VALUE6,		0,						APPCOLOR::APPC_AXISA},
	{IDC_TITLE_VALUE7,		0,						APPCOLOR::APPC_AXISB},
	{IDC_TITLE_VALUE8,		0,						APPCOLOR::APPC_AXISA},
	{IDC_TITLE_VALUE9,		0,						APPCOLOR::APPC_AXISB},
	{IDC_TITLE_VALUE10,		0,						APPCOLOR::APPC_AXISA},
	{IDC_TITLE_VALUE11,		0,						APPCOLOR::APPC_AXISB},
	{IDC_TITLE_VALUE12,		IDC_TITLE_VALUE13,		APPCOLOR::APPC_ANALOG},
	{IDC_TITLE_VALUE14,		IDC_TITLE_VALUE15,		APPCOLOR::APPC_POWER},
	{IDC_TITLE_VALUE16,		IDC_TITLE_VALUE17,		APPCOLOR::APPC_MONITOR},
	{IDC_TITLE_CTRL,		0,						APPCOLOR::APPC_NORMAL},
	{IDC_TITLE_INPUT,		0,						APPCOLOR::APPC_NORMAL},
	{IDC_TITLE_REQUEST,		0,						APPCOLOR::APPC_NORMAL},
	{IDC_TITLE_IO,			0,						APPCOLOR::APPC_NORMAL},
	{IDC_TITLE_WARNERR,		0,						APPCOLOR::APPC_NORMAL},
	{IDC_TITLE_WARN,		0,						APPCOLOR::APPC_WARNING},
	{IDC_TITLE_ERR,			0,						APPCOLOR::APPC_ERROR},
	{IDC_TITLE_ORDER,		0,						APPCOLOR::APPC_NORMAL},
	{IDC_TITLE_INTERVAL,	0,						APPCOLOR::APPC_NORMAL},
	{IDC_TITLE_WARNERR0,	IDC_TITLE_WARNERR31,	APPCOLOR::APPC_CTRL4},
	{IDC_INFO,				0,						APPCOLOR::APPC_INFO},
	{0,						0,						APPCOLOR::APPC_NOCOLOR},
	};

//================================================================================
//汎用アイテム（表示が変化しない物）の設定テーブル
//
//アイテムIDに対する英文と日本文を定義している
//================================================================================
static IDTEXT1 g_anyitem_tbl[] = {
	//ID				textEN						textJP}
	{IDC_TITLE_INTERVAL,{_T("Send Interval[ms]"),	_T("送信周期[ms]")}},
	{IDC_OFF_INTERVAL,	{_T("Send stop"),			_T("送信停止")}},
	{IDC_ON_INTERVAL,	{_T("Send start"),			_T("送信開始")}},
	{IDC_TITLE_ORDER,	{_T("Order mode"),			_T("指令モード")}},
	{IDC_ORDER1,		{_T("Speed"),				_T("速度")}},
	{IDC_ORDER2,		{_T("Torque"),				_T("トルク")}},
	{IDC_TITLE_A,		{_T("A/Y order"),			_T("A/Y指令")}},
	{IDC_TITLE_B,		{_T("B/X order"),			_T("B/X指令")}},
	{IDC_TITLE_CTRL,	{_T("Operation flag"),		_T("操作フラグ")}},
	{IDC_TITLE_INPUT,	{_T("Input flag"),			_T("入力フラグ")}},
	{IDC_TITLE_REQUEST,	{_T("Control flag"),		_T("制御フラグ")}},
	{IDC_TITLE_IO,		{_T("I/O flag"),			_T("I/Oフラグ")}},
	{IDC_TITLE_WARNERR,	{_T("Warn/Err"),			_T("警告/異常")}},
	{IDC_TITLE_WARN,	{_T("Warn"),				_T("警告")}},
	{IDC_TITLE_ERR,		{_T("Err"),					_T("異常")}},
	{0,					{NULL,						NULL}},
	};

//================================================================================
//周期要求用テーブル
//
//概要
//	周期送信処理で使用する送信許可チェックボックスの定義
//
//注意
//	文字列以外変更禁止
//================================================================================
static IDTEXT5 g_request_tbl[] = {
	//ID				subID				textEN				textJP						packetType						adrs
	{IDC_REQUESTDP0R,	IDC_STATUSDP0R,		{_T("Singlepacket"),_T("シングルパケット")},	PACKETTYPE::SINGLE_PACKET,		0},
	{IDC_REQUESTBR0,	IDC_STATUSBR0,		{_T("Broadcast0"),	_T("ブロードキャスト0")},	PACKETTYPE::BROADCAST_PACKET,	0},
	{IDC_REQUESTBR1,	IDC_STATUSBR1,		{_T("Broadcast1"),	_T("ブロードキャスト1")},	PACKETTYPE::BROADCAST_PACKET,	1},
	{IDC_REQUESTBR2,	IDC_STATUSBR2,		{_T("Broadcast2"),	_T("ブロードキャスト2")},	PACKETTYPE::BROADCAST_PACKET,	2},
	{IDC_REQUESTBR3,	IDC_STATUSBR3,		{_T("Broadcast3"),	_T("ブロードキャスト3")},	PACKETTYPE::BROADCAST_PACKET,	3},
	{IDC_REQUESTBR4,	IDC_STATUSBR4,		{_T("Broadcast4"),	_T("ブロードキャスト4")},	PACKETTYPE::BROADCAST_PACKET,	4},
	{IDC_REQUESTBR5,	IDC_STATUSBR5,		{_T("Broadcast5"),	_T("ブロードキャスト5")},	PACKETTYPE::BROADCAST_PACKET,	5},
	{IDC_REQUESTBR6,	IDC_STATUSBR6,		{_T("Broadcast6"),	_T("ブロードキャスト6")},	PACKETTYPE::BROADCAST_PACKET,	6},
	{0,					0,					{_T(""),			_T("")},					PACKETTYPE::UNKNOWN_PACKET,		0},
	};

//================================================================================
//ステータス表示用テーブル
//
//概要
//	シングルパケット又はブロードキャストパケットで取得可能な要素の内、数値表示する物
//
//注意
//	並び替え禁止
//	文字列以外変更禁止
//	値の表示は表示フォーマットを含め、プログラム側で個別実装している事に注意
//	標準・小型共通
//================================================================================
static IDTEXT4 g_valueid_tbl[] = {
	//titleID			textEN						textJP					valueID
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
	{IDC_TITLE_VALUE10,	{_T("[A]Pulse"),			_T("Aパルス積算")},		IDC_VALUE10},
	{IDC_TITLE_VALUE11,	{_T("[B]Pulse"),			_T("Bパルス積算")},		IDC_VALUE11},
	{IDC_TITLE_VALUE12,	{_T("Analog0"),				_T("アナログ入力0")},	IDC_VALUE12},
	{IDC_TITLE_VALUE13,	{_T("Analog1"),				_T("アナログ入力1")},	IDC_VALUE13},
	{IDC_TITLE_VALUE14,	{_T("Power(main)"),			_T("主電源電圧")},		IDC_VALUE14},
	{IDC_TITLE_VALUE15,	{_T("Power(control)"),		_T("制御電源電圧")},	IDC_VALUE15},
	{IDC_TITLE_VALUE16,	{_T("AnalogMonitor0"),		_T("アナログモニタ0")},	IDC_VALUE16},
	{IDC_TITLE_VALUE17,	{_T("AnalogMonitor1"),		_T("アナログモニタ1")},	IDC_VALUE17},
	{0,					{NULL,						NULL},					0},
	};

//================================================================================
//操作フラグ定義（標準ABH3用）
//
//概要
//	シングルパケットで送信される操作フラグの定義
//
//注意点
//	並びを変えたい場合は、IDの順を変えずに他の項目を入れ替える事。
//================================================================================
static IDTEXT2 g_ctrlid_normal_ctrl[] =	{
	//bit	btnOffID			offTextEN					offTextJP}					btnOnID				onTextEN				onTextJP}
	{0,		IDC_OFF_CTRLID0,	{_T("A/Y servoOFF"),		_T("A/YサーボOFF")},		IDC_ON_CTRLID0,		{_T("A/Y servoON"),		_T("A/YサーボON")}},
	{1,		IDC_OFF_CTRLID1,	{_T("A/Y stop"),			_T("A/Yストップ")},			IDC_ON_CTRLID1,		{_T("A/Y start"),		_T("A/Yスタート")}},
	{2,		IDC_OFF_CTRLID2,	{_T("A/Y fwd.order"),		_T("A/Y指令正転")},			IDC_ON_CTRLID2,		{_T("A/Y rev.order"),	_T("A/Y指令反転")}},
	{3,		IDC_OFF_CTRLID3,	{_T("A/Y adj.off"),			_T("A/Y補正加算オフ")},		IDC_ON_CTRLID3,		{_T("A/Y adj.add"),		_T("A/Y補正加算オン")}},
	{4,		IDC_OFF_CTRLID4,	{_T("A/Y sel0=0"),			_T("A/Y選択0=0")},			IDC_ON_CTRLID4,		{_T("A/Y sel0=1"),		_T("A/Y選択0=1")}},
	{5,		IDC_OFF_CTRLID5,	{_T("A/Y sel1=0"),			_T("A/Y選択1=0")},			IDC_ON_CTRLID5,		{_T("A/Y sel1=1"),		_T("A/Y選択1=1")}},
	{6,		IDC_OFF_CTRLID6,	{_T("A/Y sel2=0"),			_T("A/Y選択2=0")},			IDC_ON_CTRLID6,		{_T("A/Y sel2=1"),		_T("A/Y選択2=1")}},
	{7,		IDC_OFF_CTRLID7,	{_T("A/Y adj.add"),			_T("A/Y補正加算")},			IDC_ON_CTRLID7,		{_T("A/Y adj.sub"),		_T("A/Y補正減算")}},
	{8,		IDC_OFF_CTRLID8,	{_T("B/X servoOFF"),		_T("B/XサーボOFF")},		IDC_ON_CTRLID8,		{_T("B/X servoON"),		_T("B/XサーボON")}},
	{9,		IDC_OFF_CTRLID9,	{_T("B/X stop"),			_T("B/Xストップ")},			IDC_ON_CTRLID9,		{_T("B/X start"),		_T("B/Xスタート")}},
	{10,	IDC_OFF_CTRLID10,	{_T("B/X fwd.order"),		_T("B/X指令正転")},			IDC_ON_CTRLID10,	{_T("B/X rev.order"),	_T("B/X指令反転")}},
	{11,	IDC_OFF_CTRLID11,	{_T("B/X adj.off"),			_T("B/X補正加算オフ")},		IDC_ON_CTRLID11,	{_T("B/X adj.add"),		_T("B/X補正加算オン")}},
	{12,	IDC_OFF_CTRLID12,	{_T("B/X sel0=0"),			_T("B/X選択0=0")},			IDC_ON_CTRLID12,	{_T("B/X sel0=1"),		_T("B/X選択0=1")}},
	{13,	IDC_OFF_CTRLID13,	{_T("B/X sel1=0"),			_T("B/X選択1=0")},			IDC_ON_CTRLID13,	{_T("B/X sel1=1"),		_T("B/X選択1=1")}},
	{14,	IDC_OFF_CTRLID14,	{_T("B/X sel2=0"),			_T("B/X選択2=0")},			IDC_ON_CTRLID14,	{_T("B/X sel2=1"),		_T("B/X選択2=1")}},
	{15,	IDC_OFF_CTRLID15,	{_T("B/X adj.add"),			_T("B/X補正加算")},			IDC_ON_CTRLID15,	{_T("B/X adj.sub"),		_T("B/X補正減算")}},
	{16,	IDC_OFF_CTRLID16,	{_T("A/Y speed"),			_T("A/Y速度")},				IDC_ON_CTRLID16,	{_T("A/Y torque"),		_T("A/Yトルク")}},
	{17,	IDC_OFF_CTRLID17,	{_T("B/X speed"),			_T("B/X速度")},				IDC_ON_CTRLID17,	{_T("B/X torque"),		_T("B/Xトルク")}},
	{18,	IDC_OFF_CTRLID18,	{_T("Disable mst/slv"),		_T("マスタ/スレーブ無効")},	IDC_ON_CTRLID18,	{_T("Enable mst/slv"),	_T("マスタ/スレーブ有効")}},
	{19,	IDC_OFF_CTRLID19,	{_T("Lock break"),			_T("ブレーキ保持")},		IDC_ON_CTRLID19,	{_T("Release break"),	_T("ブレーキ解放")}},
	{20,	IDC_OFF_CTRLID20,	{_T("Never"),				_T("非動作")},				IDC_ON_CTRLID20,	{_T("AxisA clear"),		_T("A軸積算クリア")}},
	{21,	IDC_OFF_CTRLID21,	{_T("Never"),				_T("非動作")},				IDC_ON_CTRLID21,	{_T("AxisB clear"),		_T("B軸積算クリア")}},
	{22,	IDC_OFF_CTRLID22,	{_T("Never"),				_T("非動作")},				IDC_ON_CTRLID22,	{_T("Reset error"),		_T("エラーリセット")}},
	{23,	IDC_OFF_CTRLID23,	{_T(""),					_T("")},					IDC_ON_CTRLID23,	{_T(""),				_T("")}},
	{24,	IDC_OFF_CTRLID24,	{_T(""),					_T("")},					IDC_ON_CTRLID24,	{_T(""),				_T("")}},
	{25,	IDC_OFF_CTRLID25,	{_T(""),					_T("")},					IDC_ON_CTRLID25,	{_T(""),				_T("")}},
	{26,	IDC_OFF_CTRLID26,	{_T(""),					_T("")},					IDC_ON_CTRLID26,	{_T(""),				_T("")}},
	{27,	IDC_OFF_CTRLID27,	{_T(""),					_T("")},					IDC_ON_CTRLID27,	{_T(""),				_T("")}},
	{28,	IDC_OFF_CTRLID28,	{_T(""),					_T("")},					IDC_ON_CTRLID28,	{_T(""),				_T("")}},
	{29,	IDC_OFF_CTRLID29,	{_T(""),					_T("")},					IDC_ON_CTRLID29,	{_T(""),				_T("")}},
	{30,	IDC_OFF_CTRLID30,	{_T(""),					_T("")},					IDC_ON_CTRLID30,	{_T(""),				_T("")}},
	{31,	IDC_OFF_CTRLID31,	{_T(""),					_T("")},					IDC_ON_CTRLID31,	{_T(""),				_T("")}},
	{0,		0,					{NULL,						NULL},						0,					{NULL,					NULL}},
	};

//================================================================================
//操作フラグ定義（小型ABH3用）
//
//概要
//	シングルパケットで送信される操作フラグの定義
//
//注意点
//	並びを変えたい場合は、IDの順を変えずに他の項目を入れ替える事。
//================================================================================
static IDTEXT2 g_ctrlid_small_ctrl[] =	{
	//bit	btnOffID			offTextEN					offTextJP}					btnOnID				onTextEN				onTextJP}
	{0,		IDC_OFF_CTRLID0,	{_T("A/Y servoOFF"),		_T("A/YサーボOFF")},		IDC_ON_CTRLID0,		{_T("A/Y servoON"),		_T("A/YサーボON")}},
	{1,		IDC_OFF_CTRLID1,	{_T("A/Y stop"),			_T("A/Yストップ")},			IDC_ON_CTRLID1,		{_T("A/Y start"),		_T("A/Yスタート")}},
	{2,		IDC_OFF_CTRLID2,	{_T("A/Y fwd.order"),		_T("A/Y指令正転")},			IDC_ON_CTRLID2,		{_T("A/Y rev.order"),	_T("A/Y指令反転")}},
	{3,		IDC_OFF_CTRLID3,	{_T(""),					_T("")},					IDC_ON_CTRLID3,		{_T(""),				_T("")}},
	{4,		IDC_OFF_CTRLID4,	{_T("A/Y sel0=0"),			_T("A/Y選択0=0")},			IDC_ON_CTRLID4,		{_T("A/Y sel0=1"),		_T("A/Y選択0=1")}},
	{5,		IDC_OFF_CTRLID5,	{_T("A/Y sel1=0"),			_T("A/Y選択1=0")},			IDC_ON_CTRLID5,		{_T("A/Y sel1=1"),		_T("A/Y選択1=1")}},
	{6,		IDC_OFF_CTRLID6,	{_T("A/Y sel2=0"),			_T("A/Y選択2=0")},			IDC_ON_CTRLID6,		{_T("A/Y sel2=1"),		_T("A/Y選択2=1")}},
	{7,		IDC_OFF_CTRLID7,	{_T(""),					_T("")},					IDC_ON_CTRLID7,		{_T(""),				_T("")}},
	{8,		IDC_OFF_CTRLID8,	{_T("B/X servoOFF"),		_T("B/XサーボOFF")},		IDC_ON_CTRLID8,		{_T("B/X servoON"),		_T("B/XサーボON")}},
	{9,		IDC_OFF_CTRLID9,	{_T("B/X stop"),			_T("B/Xストップ")},			IDC_ON_CTRLID9,		{_T("B/X start"),		_T("B/Xスタート")}},
	{10,	IDC_OFF_CTRLID10,	{_T("B/X fwd.order"),		_T("B/X指令正転")},			IDC_ON_CTRLID10,	{_T("B/X rev.order"),	_T("B/X指令反転")}},
	{11,	IDC_OFF_CTRLID11,	{_T(""),					_T("")},					IDC_ON_CTRLID11,	{_T(""),				_T("")}},
	{12,	IDC_OFF_CTRLID12,	{_T("B/X sel0=0"),			_T("B/X選択0=0")},			IDC_ON_CTRLID12,	{_T("B/X sel0=1"),		_T("B/X選択0=1")}},
	{13,	IDC_OFF_CTRLID13,	{_T("B/X sel1=0"),			_T("B/X選択1=0")},			IDC_ON_CTRLID13,	{_T("B/X sel1=1"),		_T("B/X選択1=1")}},
	{14,	IDC_OFF_CTRLID14,	{_T("B/X sel2=0"),			_T("B/X選択2=0")},			IDC_ON_CTRLID14,	{_T("B/X sel2=1"),		_T("B/X選択2=1")}},
	{15,	IDC_OFF_CTRLID15,	{_T(""),					_T("")},					IDC_ON_CTRLID15,	{_T(""),				_T("")}},
	{16,	IDC_OFF_CTRLID16,	{_T("A/Y speed"),			_T("A/Y速度")},				IDC_ON_CTRLID16,	{_T("A/Y torque"),		_T("A/Yトルク")}},
	{17,	IDC_OFF_CTRLID17,	{_T("B/X speed"),			_T("B/X速度")},				IDC_ON_CTRLID17,	{_T("B/X torque"),		_T("B/Xトルク")}},
	{18,	IDC_OFF_CTRLID18,	{_T("Disable mst/slv"),		_T("マスタ/スレーブ無効")},	IDC_ON_CTRLID18,	{_T("Enable mst/slv"),	_T("マスタ/スレーブ有効")}},
	{19,	IDC_OFF_CTRLID19,	{_T("Lock break"),			_T("ブレーキ保持")},		IDC_ON_CTRLID19,	{_T("Release break"),	_T("ブレーキ解放")}},
	{20,	IDC_OFF_CTRLID20,	{_T("Never"),				_T("非動作")},				IDC_ON_CTRLID20,	{_T("AxisA clear"),		_T("A軸積算クリア")}},
	{21,	IDC_OFF_CTRLID21,	{_T("Never"),				_T("非動作")},				IDC_ON_CTRLID21,	{_T("AxisB clear"),		_T("B軸積算クリア")}},
	{22,	IDC_OFF_CTRLID22,	{_T("Never"),				_T("非動作")},				IDC_ON_CTRLID22,	{_T("Reset error"),		_T("エラーリセット")}},
	{23,	IDC_OFF_CTRLID23,	{_T(""),					_T("")},					IDC_ON_CTRLID23,	{_T(""),				_T("")}},
	{24,	IDC_OFF_CTRLID24,	{_T(""),					_T("")},					IDC_ON_CTRLID24,	{_T(""),				_T("")}},
	{25,	IDC_OFF_CTRLID25,	{_T(""),					_T("")},					IDC_ON_CTRLID25,	{_T(""),				_T("")}},
	{26,	IDC_OFF_CTRLID26,	{_T(""),					_T("")},					IDC_ON_CTRLID26,	{_T(""),				_T("")}},
	{27,	IDC_OFF_CTRLID27,	{_T(""),					_T("")},					IDC_ON_CTRLID27,	{_T(""),				_T("")}},
	{28,	IDC_OFF_CTRLID28,	{_T(""),					_T("")},					IDC_ON_CTRLID28,	{_T(""),				_T("")}},
	{29,	IDC_OFF_CTRLID29,	{_T(""),					_T("")},					IDC_ON_CTRLID29,	{_T(""),				_T("")}},
	{30,	IDC_OFF_CTRLID30,	{_T(""),					_T("")},					IDC_ON_CTRLID30,	{_T(""),				_T("")}},
	{31,	IDC_OFF_CTRLID31,	{_T(""),					_T("")},					IDC_ON_CTRLID31,	{_T(""),				_T("")}},
	{0,		0,					{NULL,						NULL},						0,					{NULL,					NULL}},
	};

//================================================================================
//入力フラグ定義（標準ABH3用）
//
//概要
//	ブロードキャストパケットで取得する、入力フラグの定義
//
//注意点
//	並びを変えたい場合は、IDの順を変えずに他の項目を入れ替える事。
//	但し、操作フラグ定義とビットの並びを同一にする事
//================================================================================
static IDTEXT3 g_inputid_normal_tbl[] =	{
	//bit	ID				offTextEN					offTextJP						onTextEN				onTextJP
	{0,		IDC_INPUTID0,	{_T("A.Y servoOFF"),		_T("A/YサーボOFF")},			{_T("A/Y servoON"),		_T("A/YサーボON")}},
	{1,		IDC_INPUTID1,	{_T("A/Y stop"),			_T("A/Yストップ")},				{_T("A/Y start"),		_T("A/Yスタート")}},
	{2,		IDC_INPUTID2,	{_T("A/Y fwd.order"),		_T("A/Y指令正転")},				{_T("A/Y rev.order"),	_T("A/Y指令反転")}},
	{3,		IDC_INPUTID3,	{_T("A/Y adj.off"),			_T("A/Y補正加算オフ")},			{_T("A/Y adj.add"),		_T("A/Y補正加算オン")}},
	{4,		IDC_INPUTID4,	{_T("A/Y sel0=0"),			_T("A/Y選択0=0")},				{_T("A/Y sel0=1"),		_T("A/Y選択0=1")}},
	{5,		IDC_INPUTID5,	{_T("A/Y sel1=0"),			_T("A/Y選択1=0")},				{_T("A/Y sel1=1"),		_T("A/Y選択1=1")}},
	{6,		IDC_INPUTID6,	{_T("A/Y sel2=0"),			_T("A/Y選択2=0")},				{_T("A/Y sel2=1"),		_T("A/Y選択2=1")}},
	{7,		IDC_INPUTID7,	{_T("A/Y adj.add"),			_T("A/Y補正加算")},				{_T("A/Y adj.sub"),		_T("A/Y補正減算")}},
	{8,		IDC_INPUTID8,	{_T("B/X servoOFF"),		_T("B/XサーボOFF")},			{_T("B/X servoON"),		_T("B/XサーボON")}},
	{9,		IDC_INPUTID9,	{_T("B/X stop"),			_T("B/Xストップ")},				{_T("B/X start"),		_T("B/Xスタート")}},
	{10,	IDC_INPUTID10,	{_T("B/X fwd.order"),		_T("B/X指令正転")},				{_T("B/X rev.order"),	_T("B/X指令反転")}},
	{11,	IDC_INPUTID11,	{_T("B/X adj.off"),			_T("B/X補正加算オフ")},			{_T("B/X adj.add"),		_T("B/X補正加算オン")}},
	{12,	IDC_INPUTID12,	{_T("B/X sel0=0"),			_T("B/X選択0=0")},				{_T("B/X sel0=1"),		_T("B/X選択0=1")}},
	{13,	IDC_INPUTID13,	{_T("B/X sel1=0"),			_T("B/X選択1=0")},				{_T("B/X sel1=1"),		_T("B/X選択1=1")}},
	{14,	IDC_INPUTID14,	{_T("B/X sel2=0"),			_T("B/X選択2=0")},				{_T("B/X sel2=1"),		_T("B/X選択2=1")}},
	{15,	IDC_INPUTID15,	{_T("B/X adj.add"),			_T("B/X補正加算")},				{_T("B/X adj.sub"),		_T("B/X補正減算")}},
	{16,	IDC_INPUTID16,	{_T("A/Y speed"),			_T("A/Y速度")},					{_T("A/Y torque"),		_T("A/Yトルク")}},
	{17,	IDC_INPUTID17,	{_T("B/X speed"),			_T("B/X速度")},					{_T("B/X torque"),		_T("B/Xトルク")}},
	{18,	IDC_INPUTID18,	{_T("Disable mst/slv"),		_T("マスタ/スレーブ無効")},		{_T("Enable mst/slv"),	_T("マスタ/スレーブ有効")}},
	{19,	IDC_INPUTID19,	{_T("Lock break"),			_T("ブレーキ保持")},			{_T("Release break"),	_T("ブレーキ解放")}},
	{20,	IDC_INPUTID20,	{_T("Never"),				_T("非動作")},					{_T("AxisA clear"),		_T("A軸積算クリア")}},
	{21,	IDC_INPUTID21,	{_T("Never"),				_T("非動作")},					{_T("AxisB clear"),		_T("B軸積算クリア")}},
	{22,	IDC_INPUTID22,	{_T("Never"),				_T("非動作")},					{_T("Reset error"),		_T("エラーリセット")}},
	{23,	IDC_INPUTID23,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{24,	IDC_INPUTID24,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{25,	IDC_INPUTID25,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{26,	IDC_INPUTID26,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{27,	IDC_INPUTID27,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{28,	IDC_INPUTID28,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{29,	IDC_INPUTID29,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{30,	IDC_INPUTID30,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{31,	IDC_INPUTID31,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{0,		0,				{NULL,						NULL},							{NULL,					NULL}},
	};

//================================================================================
//入力フラグ定義（小型ABH3用）
//
//概要
//	ブロードキャストパケットで取得する、入力フラグの定義
//
//注意点
//	並びを変えたい場合は、IDの順を変えずに他の項目を入れ替える事。
//	但し、操作フラグ定義とビットの並びを同一にする事
//================================================================================
static IDTEXT3 g_inputid_small_tbl[] =	{
	//bit	ID				offTextEN					offTextJP						onTextEN				onTextJP
	{0,		IDC_INPUTID0,	{_T("A.Y servoOFF"),		_T("A/YサーボOFF")},			{_T("A/Y servoON"),		_T("A/YサーボON")}},
	{1,		IDC_INPUTID1,	{_T("A/Y stop"),			_T("A/Yストップ")},				{_T("A/Y start"),		_T("A/Yスタート")}},
	{2,		IDC_INPUTID2,	{_T("A/Y fwd.order"),		_T("A/Y指令正転")},				{_T("A/Y rev.order"),	_T("A/Y指令反転")}},
	{3,		IDC_INPUTID3,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{4,		IDC_INPUTID4,	{_T("A/Y sel0=0"),			_T("A/Y選択0=0")},				{_T("A/Y sel0=1"),		_T("A/Y選択0=1")}},
	{5,		IDC_INPUTID5,	{_T("A/Y sel1=0"),			_T("A/Y選択1=0")},				{_T("A/Y sel1=1"),		_T("A/Y選択1=1")}},
	{6,		IDC_INPUTID6,	{_T("A/Y sel2=0"),			_T("A/Y選択2=0")},				{_T("A/Y sel2=1"),		_T("A/Y選択2=1")}},
	{7,		IDC_INPUTID7,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{8,		IDC_INPUTID8,	{_T("B/X servoOFF"),		_T("B/XサーボOFF")},			{_T("B/X servoON"),		_T("B/XサーボON")}},
	{9,		IDC_INPUTID9,	{_T("B/X stop"),			_T("B/Xストップ")},				{_T("B/X start"),		_T("B/Xスタート")}},
	{10,	IDC_INPUTID10,	{_T("B/X fwd.order"),		_T("B/X指令正転")},				{_T("B/X rev.order"),	_T("B/X指令反転")}},
	{11,	IDC_INPUTID11,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{12,	IDC_INPUTID12,	{_T("B/X sel0=0"),			_T("B/X選択0=0")},				{_T("B/X sel0=1"),		_T("B/X選択0=1")}},
	{13,	IDC_INPUTID13,	{_T("B/X sel1=0"),			_T("B/X選択1=0")},				{_T("B/X sel1=1"),		_T("B/X選択1=1")}},
	{14,	IDC_INPUTID14,	{_T("B/X sel2=0"),			_T("B/X選択2=0")},				{_T("B/X sel2=1"),		_T("B/X選択2=1")}},
	{15,	IDC_INPUTID15,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{16,	IDC_INPUTID16,	{_T("A/Y speed"),			_T("A/Y速度")},					{_T("A/Y torque"),		_T("A/Yトルク")}},
	{17,	IDC_INPUTID17,	{_T("B/X speed"),			_T("B/X速度")},					{_T("B/X torque"),		_T("B/Xトルク")}},
	{18,	IDC_INPUTID18,	{_T("Disable mst/slv"),		_T("マスタ/スレーブ無効")},		{_T("Enable mst/slv"),	_T("マスタ/スレーブ有効")}},
	{19,	IDC_INPUTID19,	{_T("Lock break"),			_T("ブレーキ保持")},			{_T("Release break"),	_T("ブレーキ解放")}},
	{20,	IDC_INPUTID20,	{_T("Never"),				_T("非動作")},					{_T("AxisA clear"),		_T("A軸積算クリア")}},
	{21,	IDC_INPUTID21,	{_T("Never"),				_T("非動作")},					{_T("AxisB clear"),		_T("B軸積算クリア")}},
	{22,	IDC_INPUTID22,	{_T("Never"),				_T("非動作")},					{_T("Reset error"),		_T("エラーリセット")}},
	{23,	IDC_INPUTID23,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{24,	IDC_INPUTID24,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{25,	IDC_INPUTID25,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{26,	IDC_INPUTID26,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{27,	IDC_INPUTID27,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{28,	IDC_INPUTID28,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{29,	IDC_INPUTID29,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{30,	IDC_INPUTID30,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{31,	IDC_INPUTID31,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{0,		0,				{NULL,						NULL},							{NULL,					NULL}},
	};

//================================================================================
//制御フラグ定義（標準ABH3用）
//
//概要
//	シングルパケットで取得する、制御フラグの定義
//
//注意点
//	並びを変えたい場合は、IDの順を変えずに他の項目を入れ替える事。
//	但し、操作フラグ定義とビットの並びを同一にする事
//================================================================================
static IDTEXT3 g_resultid_normal_tbl[] =	{
	//bit	ID				offTextEN					offTextJP						onTextEN				onTextJP
	{0,		IDC_RESULTID0,	{_T("A.Y servoOFF"),		_T("A/YサーボOFF")},			{_T("A/Y servoON"),		_T("A/YサーボON")}},
	{1,		IDC_RESULTID1,	{_T("A/Y stop"),			_T("A/Yストップ")},				{_T("A/Y start"),		_T("A/Yスタート")}},
	{2,		IDC_RESULTID2,	{_T("A/Y fwd.order"),		_T("A/Y指令正転")},				{_T("A/Y rev.order"),	_T("A/Y指令反転")}},
	{3,		IDC_RESULTID3,	{_T("A/Y adj.off"),			_T("A/Y補正加算オフ")},			{_T("A/Y adj.add"),		_T("A/Y補正加算オン")}},
	{4,		IDC_RESULTID4,	{_T("A/Y sel0=0"),			_T("A/Y選択0=0")},				{_T("A/Y sel0=1"),		_T("A/Y選択0=1")}},
	{5,		IDC_RESULTID5,	{_T("A/Y sel1=0"),			_T("A/Y選択1=0")},				{_T("A/Y sel1=1"),		_T("A/Y選択1=1")}},
	{6,		IDC_RESULTID6,	{_T("A/Y sel2=0"),			_T("A/Y選択2=0")},				{_T("A/Y sel2=1"),		_T("A/Y選択2=1")}},
	{7,		IDC_RESULTID7,	{_T("A/Y adj.add"),			_T("A/Y補正加算")},				{_T("A/Y adj.sub"),		_T("A/Y補正減算")}},
	{8,		IDC_RESULTID8,	{_T("B/X servoOFF"),		_T("B/XサーボOFF")},			{_T("B/X servoON"),		_T("B/XサーボON")}},
	{9,		IDC_RESULTID9,	{_T("B/X stop"),			_T("B/Xストップ")},				{_T("B/X start"),		_T("B/Xスタート")}},
	{10,	IDC_RESULTID10,	{_T("B/X fwd.order"),		_T("B/X指令正転")},				{_T("B/X rev.order"),	_T("B/X指令反転")}},
	{11,	IDC_RESULTID11,	{_T("B/X adj.off"),			_T("B/X補正加算オフ")},			{_T("B/X adj.add"),		_T("B/X補正加算オン")}},
	{12,	IDC_RESULTID12,	{_T("B/X sel0=0"),			_T("B/X選択0=0")},				{_T("B/X sel0=1"),		_T("B/X選択0=1")}},
	{13,	IDC_RESULTID13,	{_T("B/X sel1=0"),			_T("B/X選択1=0")},				{_T("B/X sel1=1"),		_T("B/X選択1=1")}},
	{14,	IDC_RESULTID14,	{_T("B/X sel2=0"),			_T("B/X選択2=0")},				{_T("B/X sel2=1"),		_T("B/X選択2=1")}},
	{15,	IDC_RESULTID15,	{_T("B/X adj.add"),			_T("B/X補正加算")},				{_T("B/X adj.sub"),		_T("B/X補正減算")}},
	{16,	IDC_RESULTID16,	{_T("A/Y speed"),			_T("A/Y速度")},					{_T("A/Y torque"),		_T("A/Yトルク")}},
	{17,	IDC_RESULTID17,	{_T("B/X speed"),			_T("B/X速度")},					{_T("B/X torque"),		_T("B/Xトルク")}},
	{18,	IDC_RESULTID18,	{_T("Disable mst/slv"),		_T("マスタ/スレーブ無効")},		{_T("Enable mst/slv"),	_T("マスタ/スレーブ有効")}},
	{19,	IDC_RESULTID19,	{_T("Lock break"),			_T("ブレーキ保持")},			{_T("Release break"),	_T("ブレーキ解放")}},
	{20,	IDC_RESULTID20,	{_T("Never"),				_T("非動作")},					{_T("AxisA clear"),		_T("A軸積算クリア")}},
	{21,	IDC_RESULTID21,	{_T("Never"),				_T("非動作")},					{_T("AxisB clear"),		_T("B軸積算クリア")}},
	{22,	IDC_RESULTID22,	{_T("Never"),				_T("非動作")},					{_T("Reset error"),		_T("エラーリセット")}},
	{23,	IDC_RESULTID23,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{24,	IDC_RESULTID24,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{25,	IDC_RESULTID25,	{_T("A/Y servoOFF"),		_T("A/YサーボOFF")},			{_T("A/Y servoON"),		_T("A/YサーボON")}},
	{26,	IDC_RESULTID26,	{_T("A/Y order=0"),			_T("A/Y指令=0")},				{_T("A/X order"),		_T("A/Y指令≠0")}},
	{27,	IDC_RESULTID27,	{_T("B/X servoOFF"),		_T("B/XサーボOFF")},			{_T("B/X servoON"),		_T("B/XサーボON")}},
	{28,	IDC_RESULTID28,	{_T("B/X order=0"),			_T("B/X指令=0")},				{_T("B/X order"),		_T("B/X指令≠0")}},
	{29,	IDC_RESULTID29,	{_T("Control motor"),		_T("モータ軸制御")},			{_T("Control tire"),	_T("走行軸制御")}},
	{30,	IDC_RESULTID30,	{_T("Lock break"),		_T("ブレーキ解除出力OFF")},			{_T("Release break"),	_T("ブレーキ解放出力ON")}},
	{31,	IDC_RESULTID31,	{_T("Normal"),				_T("異常なし")},				{_T("Abnormal"),		_T("異常発生")}},
	{0,		0,				{NULL,						NULL},							{NULL,					NULL}},
	};

//================================================================================
//制御フラグ定義（小型ABH3用）
//
//概要
//	シングルパケットで取得する、制御フラグの定義
//
//注意点
//	並びを変えたい場合は、IDの順を変えずに他の項目を入れ替える事。
//	但し、操作フラグ定義とビットの並びを同一にする事
//================================================================================
static IDTEXT3 g_resultid_small_tbl[] =	{
	//bit	ID				offTextEN					offTextJP						onTextEN				onTextJP
	{0,		IDC_RESULTID0,	{_T("A.Y servoOFF"),		_T("A/YサーボOFF")},			{_T("A/Y servoON"),		_T("A/YサーボON")}},
	{1,		IDC_RESULTID1,	{_T("A/Y stop"),			_T("A/Yストップ")},				{_T("A/Y start"),		_T("A/Yスタート")}},
	{2,		IDC_RESULTID2,	{_T("A/Y fwd.order"),		_T("A/Y指令正転")},				{_T("A/Y rev.order"),	_T("A/Y指令反転")}},
	{3,		IDC_RESULTID3,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{4,		IDC_RESULTID4,	{_T("A/Y sel0=0"),			_T("A/Y選択0=0")},				{_T("A/Y sel0=1"),		_T("A/Y選択0=1")}},
	{5,		IDC_RESULTID5,	{_T("A/Y sel1=0"),			_T("A/Y選択1=0")},				{_T("A/Y sel1=1"),		_T("A/Y選択1=1")}},
	{6,		IDC_RESULTID6,	{_T("A/Y sel2=0"),			_T("A/Y選択2=0")},				{_T("A/Y sel2=1"),		_T("A/Y選択2=1")}},
	{7,		IDC_RESULTID7,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{8,		IDC_RESULTID8,	{_T("B/X servoOFF"),		_T("B/XサーボOFF")},			{_T("B/X servoON"),		_T("B/XサーボON")}},
	{9,		IDC_RESULTID9,	{_T("B/X stop"),			_T("B/Xストップ")},				{_T("B/X start"),		_T("B/Xスタート")}},
	{10,	IDC_RESULTID10,	{_T("B/X fwd.order"),		_T("B/X指令正転")},				{_T("B/X rev.order"),	_T("B/X指令反転")}},
	{11,	IDC_RESULTID11,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{12,	IDC_RESULTID12,	{_T("B/X sel0=0"),			_T("B/X選択0=0")},				{_T("B/X sel0=1"),		_T("B/X選択0=1")}},
	{13,	IDC_RESULTID13,	{_T("B/X sel1=0"),			_T("B/X選択1=0")},				{_T("B/X sel1=1"),		_T("B/X選択1=1")}},
	{14,	IDC_RESULTID14,	{_T("B/X sel2=0"),			_T("B/X選択2=0")},				{_T("B/X sel2=1"),		_T("B/X選択2=1")}},
	{15,	IDC_RESULTID15,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{16,	IDC_RESULTID16,	{_T("A/Y speed"),			_T("A/Y速度")},					{_T("A/Y torque"),		_T("A/Yトルク")}},
	{17,	IDC_RESULTID17,	{_T("B/X speed"),			_T("B/X速度")},					{_T("B/X torque"),		_T("B/Xトルク")}},
	{18,	IDC_RESULTID18,	{_T("Disable mst/slv"),		_T("マスタ/スレーブ無効")},		{_T("Enable mst/slv"),	_T("マスタ/スレーブ有効")}},
	{19,	IDC_RESULTID19,	{_T("Lock break"),			_T("ブレーキ保持")},			{_T("Release break"),	_T("ブレーキ解放")}},
	{20,	IDC_RESULTID20,	{_T("Never"),				_T("非動作")},					{_T("AxisA clear"),		_T("A軸積算クリア")}},
	{21,	IDC_RESULTID21,	{_T("Never"),				_T("非動作")},					{_T("AxisB clear"),		_T("B軸積算クリア")}},
	{22,	IDC_RESULTID22,	{_T("Never"),				_T("非動作")},					{_T("Reset error"),		_T("エラーリセット")}},
	{23,	IDC_RESULTID23,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{24,	IDC_RESULTID24,	{_T(""),					_T("")},						{_T(""),				_T("")}},
	{25,	IDC_RESULTID25,	{_T("A/Y servoOFF"),		_T("A/YサーボOFF")},			{_T("A/Y servoON"),		_T("A/YサーボON")}},
	{26,	IDC_RESULTID26,	{_T("A/Y order=0"),			_T("A/Y指令=0")},				{_T("A/X order"),		_T("A/Y指令≠0")}},
	{27,	IDC_RESULTID27,	{_T("B/X servoOFF"),		_T("B/XサーボOFF")},			{_T("B/X servoON"),		_T("B/XサーボON")}},
	{28,	IDC_RESULTID28,	{_T("B/X order=0"),			_T("B/X指令=0")},				{_T("B/X order"),		_T("B/X指令≠0")}},
	{29,	IDC_RESULTID29,	{_T("Control motor"),		_T("モータ軸制御")},			{_T("Control tire"),	_T("走行軸制御")}},
	{30,	IDC_RESULTID30,	{_T("Lock break"),		_T("ブレーキ解除出力OFF")},			{_T("Release break"),	_T("ブレーキ解放出力ON")}},
	{31,	IDC_RESULTID31,	{_T("Normal"),				_T("異常なし")},				{_T("Abnormal"),		_T("異常発生")}},
	{0,		0,				{NULL,						NULL},							{NULL,					NULL}},
	};

//================================================================================
//I/Oフラグ定義（標準ABH3用）
//
//概要
//	ブロードキャストパケットで取得する、I/Oフラグ（標準ABH3用）の定義
//
//注意点
//	並びを変えたい場合は、IDの順を変えずに他の項目を入れ替える事。
//	但し、操作フラグ定義とビットの並びを同一にする事
//================================================================================
static IDTEXT g_io_normal_tbl[] =	{
	//bit	ID					{textEN						textJP}
	{0,		IDC_IO0,			{_T("Output #0"),			_T("汎用出力 #0")}},
	{1,		IDC_IO1,			{_T("Output #1"),			_T("汎用出力 #1")}},
	{2,		IDC_IO2,			{_T("Output #2"),			_T("汎用出力 #2")}},
	{3,		IDC_IO3,			{_T("Output #3"),			_T("汎用出力 #3")}},
	{4,		IDC_IO4,			{_T("Output #4"),			_T("汎用出力 #4")}},
	{5,		IDC_IO5,			{_T("Output #5"),			_T("汎用出力 #5")}},
	{6,		IDC_IO6,			{_T("Output #6"),			_T("汎用出力 #6")}},
	{7,		IDC_IO7,			{_T("Output #7"),			_T("汎用出力 #7")}},
	{8,		IDC_IO8,			{_T("Input #0"),			_T("汎用入力 #0")}},
	{9,		IDC_IO9,			{_T("Input #1"),			_T("汎用入力 #1")}},
	{10,	IDC_IO10,			{_T("Input #2"),			_T("汎用入力 #2")}},
	{11,	IDC_IO11,			{_T("Input #3"),			_T("汎用入力 #3")}},
	{12,	IDC_IO12,			{_T("Input #4"),			_T("汎用入力 #4")}},
	{13,	IDC_IO13,			{_T("Input #5"),			_T("汎用入力 #5")}},
	{14,	IDC_IO14,			{_T("Input #6"),			_T("汎用入力 #6")}},
	{15,	IDC_IO15,			{_T("Input #7"),			_T("汎用入力 #7")}},
	{16,	IDC_IO16,			{_T("Input #8"),			_T("汎用入力 #8")}},
	{17,	IDC_IO17,			{_T("Input #9"),			_T("汎用入力 #9")}},
	{18,	IDC_IO18,			{_T("Input #10"),			_T("汎用入力 #10")}},
	{19,	IDC_IO19,			{_T("Input #11"),			_T("汎用入力 #11")}},
	{20,	IDC_IO20,			{_T("Input #12"),			_T("汎用入力 #12")}},
	{21,	IDC_IO21,			{_T("Input #13"),			_T("汎用入力 #13")}},
	{22,	IDC_IO22,			{_T("Input #14"),			_T("汎用入力 #14")}},
	{23,	IDC_IO23,			{_T("Input #15"),			_T("汎用入力 #15")}},
	{24,	IDC_IO24,			{_T("Input #16"),			_T("汎用入力 #16")}},
	{25,	IDC_IO25,			{_T("Input #17"),			_T("汎用入力 #17")}},
	{26,	IDC_IO26,			{_T("Input #18"),			_T("汎用入力 #18")}},
	{27,	IDC_IO27,			{_T("Input #19"),			_T("汎用入力 #19")}},
	{28,	IDC_IO28,			{_T(""),					_T("")}},
	{29,	IDC_IO29,			{_T(""),					_T("")}},
	{30,	IDC_IO30,			{_T(""),					_T("")}},
	{31,	IDC_IO31,			{_T("Input reset-error"),	_T("エラーリセット入力")}},
	{0,		0,					{NULL,						NULL}},
	};

//================================================================================
//I/Oフラグ定義（小型ABH3用）
//
//概要
//	ブロードキャストパケットで取得する、I/Oフラグ（小型ABH3用）の定義
//
//注意点
//	並びを変えたい場合は、IDの順を変えずに他の項目を入れ替える事。
//	但し、操作フラグ定義とビットの並びを同一にする事
//================================================================================
static IDTEXT g_io_small_tbl[] =	{
	//bit	ID					text
	{0,		IDC_IO0,			{_T("Output #0"),			_T("汎用出力 #0")}},
	{1,		IDC_IO1,			{_T("Output #1"),			_T("汎用出力 #1")}},
	{2,		IDC_IO2,			{_T("Output #2"),			_T("汎用出力 #2")}},
	{3,		IDC_IO3,			{_T("Output #3"),			_T("汎用出力 #3")}},
	{4,		IDC_IO4,			{_T(""),					_T("")}},
	{5,		IDC_IO5,			{_T(""),					_T("")}},
	{6,		IDC_IO6,			{_T(""),					_T("")}},
	{7,		IDC_IO7,			{_T(""),					_T("")}},
	{8,		IDC_IO8,			{_T("Input #0"),			_T("汎用入力 #0")}},
	{9,		IDC_IO9,			{_T("Input #1"),			_T("汎用入力 #1")}},
	{10,	IDC_IO10,			{_T("Input #2"),			_T("汎用入力 #2")}},
	{11,	IDC_IO11,			{_T("Input #3"),			_T("汎用入力 #3")}},
	{12,	IDC_IO12,			{_T("Input #4"),			_T("汎用入力 #4")}},
	{13,	IDC_IO13,			{_T("Input #5"),			_T("汎用入力 #5")}},
	{14,	IDC_IO14,			{_T("Input #6"),			_T("汎用入力 #6")}},
	{15,	IDC_IO15,			{_T("Input #7"),			_T("汎用入力 #7")}},
	{16,	IDC_IO16,			{_T("Input #8"),			_T("汎用入力 #8")}},
	{17,	IDC_IO17,			{_T("Input #9"),			_T("汎用入力 #9")}},
	{18,	IDC_IO18,			{_T(""),					_T("")}},
	{19,	IDC_IO19,			{_T(""),					_T("")}},
	{20,	IDC_IO20,			{_T(""),					_T("")}},
	{21,	IDC_IO21,			{_T(""),					_T("")}},
	{22,	IDC_IO22,			{_T(""),					_T("")}},
	{23,	IDC_IO23,			{_T(""),					_T("")}},
	{24,	IDC_IO24,			{_T(""),					_T("")}},
	{25,	IDC_IO25,			{_T(""),					_T("")}},
	{26,	IDC_IO26,			{_T(""),					_T("")}},
	{27,	IDC_IO27,			{_T(""),					_T("")}},
	{28,	IDC_IO28,			{_T(""),					_T("")}},
	{29,	IDC_IO29,			{_T(""),					_T("")}},
	{30,	IDC_IO30,			{_T(""),					_T("")}},
	{31,	IDC_IO31,			{_T("Input reset-error"),	_T("エラーリセット入力")}},
	{0,		0,					{NULL,						NULL}},
	};

//================================================================================
//警告とエラー定義
//
//概要
//	ブロードキャストパケットで取得する、警告フラグ・異常フラグの定義
//
//注意点
//	並び替え禁止
//================================================================================
static IDTEXT g_warnerr_tbl[] = {
	//bit	ID						textEN				textJP
	{0,		IDC_TITLE_WARNERR0,		{_T("AxisA Lock.mecha"),	_T("A軸 メカロック")}},
	{1,		IDC_TITLE_WARNERR1,		{_T("AxisB Lock.mecha"),	_T("B軸 メカロック")}},
	{2,		IDC_TITLE_WARNERR2,		{_T("Overheat"),			_T("ドライバ過熱")}},
	{3,		IDC_TITLE_WARNERR3,		{_T("Abnormal break"),		_T("ブレーキ異常")}},
	{4,		IDC_TITLE_WARNERR4,		{_T("AxisA resolver"),		_T("A軸 レゾルバ")}},
	{5,		IDC_TITLE_WARNERR5,		{_T("AxisB resolver"),		_T("B軸 レゾルバ")}},
	{6,		IDC_TITLE_WARNERR6,		{_T("AxisA Over current"),	_T("A軸 過電流")}},
	{7,		IDC_TITLE_WARNERR7,		{_T("AxisB Over current"),	_T("B軸 過電流")}},
	{8,		IDC_TITLE_WARNERR8,		{_T("Lower power.control"),	_T("制御電源 電圧低下")}},
	{9,		IDC_TITLE_WARNERR9,		{_T("Abnormal parameter"),	_T("パラメータ")}},
	{10,	IDC_TITLE_WARNERR10,	{_T("AxisA PDU"),			_T("A軸 PDU")}},
	{11,	IDC_TITLE_WARNERR11,	{_T("AxisB PDU"),			_T("B軸 PDU")}},
	{12,	IDC_TITLE_WARNERR12,	{_T("AxisA SoftThermal"),	_T("A軸 電子サーマル")}},
	{13,	IDC_TITLE_WARNERR13,	{_T("AxisB SoftThermal"),	_T("B軸 電子サーマル")}},
	{14,	IDC_TITLE_WARNERR14,	{_T("Lower power.main"),	_T("主電源 電圧低下")}},
	{15,	IDC_TITLE_WARNERR15,	{_T("Over power"),			_T("制/主電源過電圧")}},
	{16,	IDC_TITLE_WARNERR16,	{_T("AxisA Over accurate"),	_T("A軸 過速度")}},
	{17,	IDC_TITLE_WARNERR17,	{_T("AxisB Over accurate"),	_T("B軸 過速度")}},
	{18,	IDC_TITLE_WARNERR18,	{_T("AxisA Limit speed"),	_T("A軸 速度リミット")}},
	{19,	IDC_TITLE_WARNERR19,	{_T("AxisB Limit speed"),	_T("B軸 速度リミット")}},
	{20,	IDC_TITLE_WARNERR20,	{_T("AxisA Limit current"),	_T("A軸 電流リミット")}},
	{21,	IDC_TITLE_WARNERR21,	{_T("AxisB Limit current"),	_T("B軸 電流リミット")}},
	{22,	IDC_TITLE_WARNERR22,	{_T("Timeout.comm"),		_T("CAN通信タイムアウト")}},
	{23,	IDC_TITLE_WARNERR23,	{_T("Over comminucation"),	_T("CAN通信トラフィック過大")}},
	{24,	IDC_TITLE_WARNERR24,	{_T(""),					_T("")}},
	{25,	IDC_TITLE_WARNERR25,	{_T(""),					_T("")}},
	{26,	IDC_TITLE_WARNERR26,	{_T(""),					_T("")}},
	{27,	IDC_TITLE_WARNERR27,	{_T(""),					_T("")}},
	{28,	IDC_TITLE_WARNERR28,	{_T(""),					_T("")}},
	{29,	IDC_TITLE_WARNERR29,	{_T(""),					_T("")}},
	{30,	IDC_TITLE_WARNERR30,	{_T(""),					_T("")}},
	{31,	IDC_TITLE_WARNERR31,	{_T(""),					_T("")}},
	{0,		0,						{NULL,						NULL}},
	};

//================================================================================
//ハートビート状態表示用文字列定義
//
//概要
//	受信状態を表示する所に表示する文字列の定義
//
//注意点
//	並び替え禁止
//================================================================================
static LANGTEXT g_heartbeat_text[] = {
	//textEN							textJP
	{_T("Receiving"),					_T("受信中")},
	{_T("No received"),					_T("受信無し")},
	{NULL,								NULL},
	};

//================================================================================
//ステータス状態表示用文字列定義
//
//概要
//	ステータス状態を表示する所に表示する文字列の定義
//
//注意点
//	並び替え禁止
//================================================================================
static LANGTEXT g_status_text[] = {
	//textEN							textJP
	{_T("Normal operation."),			_T("正常動作中")},
	{_T("Detected error of interface."),_T("インターフェースのエラーを検出中")},
	{_T("Interface not connected."),	_T("インターフェースは未接続です")},
	{NULL,								NULL},
	};

//================================================================================
//ホストモード（ホスト側としてで動作している機器の送信を表示）用表示アイテム定義
//
//概要
//	機種で「ホスト」を選んだ場合に画面に表示するダイアログアイテムのIDを定義
//
//================================================================================
static UINT g_hostitem_tbl[] = {
	//開始ID			終了ID
	IDC_INFO,			IDC_INFO2,
	IDC_TITLE_ORDER,	0,
	IDC_ORDER1,			IDC_ORDER2,
	IDC_TITLE_A,		IDC_TITLE_B,
	IDC_VALUE0,			IDC_VALUE1,
	IDC_TITLE_CTRL,		0,
	IDC_REQUESTDP0R,	IDC_REQUESTBR6,
	IDC_STATUSDP0R,		IDC_STATUSBR6,
	IDC_OFF_CTRLID0,	IDC_OFF_CTRLID31,
	IDC_ON_CTRLID0,		IDC_ON_CTRLID31,
	0,					0
	};	

//
IMPLEMENT_DYNCREATE(CguicanABH3View,CFormView)

//
BEGIN_MESSAGE_MAP(CguicanABH3View,CFormView)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(EN_CHANGE,IDC_EDIT0,IDC_EDIT1,&CguicanABH3View::OnEnChangeEdit)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_OFF_CTRLID0,IDC_OFF_CTRLID31,&CguicanABH3View::OnBnClickedOffCtrlid)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_ON_CTRLID0,IDC_ON_CTRLID31,&CguicanABH3View::OnBnClickedOnCtrlid)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_ORDER1,IDC_ORDER2,&CguicanABH3View::OnBnClickedOrder)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_REQUESTDP0R,IDC_REQUESTBR6,&CguicanABH3View::OnBnClickedRequestTarget)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_OFF_INTERVAL,IDC_ON_INTERVAL,&CguicanABH3View::OnBnClickedInterval)
	ON_EN_CHANGE(IDC_INTERVAL,&CguicanABH3View::OnEnChangeInterval)
	ON_UPDATE_COMMAND_UI(IDC_ON_INTERVAL,&CguicanABH3View::OnUpdateOnInterval)
	ON_UPDATE_COMMAND_UI(IDC_OFF_INTERVAL,&CguicanABH3View::OnUpdateOffInterval)
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

	DDX_Control(pDX,IDC_EDIT0,m_edit[0]);
	DDX_Control(pDX,IDC_EDIT1,m_edit[1]);
	DDX_Control(pDX,IDC_INTERVAL,m_interval);
	DDX_Control(pDX,IDC_ORDER1,m_order_value[0]);
	DDX_Control(pDX,IDC_ORDER2,m_order_value[1]);
	DDX_Control(pDX,IDC_OFF_INTERVAL,m_interval_btn[0]);
	DDX_Control(pDX,IDC_ON_INTERVAL,m_interval_btn[1]);
	}

//ウィンドウ作成時に呼び出されます
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

	//ドキュメントクラスから設定取得
	::CopyMemory(&m_var.config,GetDocument()->GetIDSET(),sizeof(IDSET));

	//画面構築
	CreateScreen();

	//設定の復帰
	reg2disp();

	//タイマー割り込み
	m_var.nTimerNum = SetTimer(theApp.GetTimerNum(),100,NULL);	//表示は固定周期100ms
	m_var.n1sec = SetTimer(theApp.GetTimerNum(),1000,NULL);		//その他の周期は1000ms

	//非同期送信用スレッド
	m_var.thread.hThread = (HANDLE)_beginthreadex(NULL,0,CguicanABH3View::SendThread,this,0,&m_var.thread.nUid);
	}


//================================================================================
//ハートビート関連（受信の有無判断）
//================================================================================

//ハートビートが有った場合に呼び出します
void CguicanABH3View::UpHeartBeat(int nNum)
	{
	if(nNum < QTY_HEARTBEAT)
		{
		//前の値
		uint8_t nOld = m_var.heartbeat.lifeTime[nNum];

		//新しい寿命を設定
		m_var.heartbeat.lifeTime[nNum] = 10;

		//前の値が0だった？
		if(nOld == 0)
			{
			//表示文字を設定し、再描画させる
			CStatic* pStatic = (CStatic*)GetDlgItem(g_request_tbl[nNum].nSubUid);
			pStatic->SetWindowText(theApp.GetLangText(&g_heartbeat_text[0]));
			pStatic->Invalidate();
			}
		m_var.heartbeat.lifeTime[nNum] = 10;
		}
	}

//全体のハートビートカウンタを減らします
void CguicanABH3View::DownHeartBeat()
	{
	for(int nNum = 0;nNum < QTY_HEARTBEAT;nNum++)
		{
		uint8_t nOld = m_var.heartbeat.lifeTime[nNum];
		if(nOld)
			{
			--nOld;
			if(nOld == 0)
				{
				//再描画させる
				CStatic* pStatic = (CStatic*)GetDlgItem(g_request_tbl[nNum].nSubUid);
				pStatic->SetWindowText(theApp.GetLangText(&g_heartbeat_text[1]));
				pStatic->Invalidate();
				}
			m_var.heartbeat.lifeTime[nNum] = nOld;
			}
		}
	}

//================================================================================
//画面構築関連
//================================================================================

//画面構築
void CguicanABH3View::CreateScreen()
	{
	//現在の機種
	MTYPE nType = GetType();

	//ダイアログアイテムの再配置（間隔を一定にする為）
	ArrangeScreenItem();

	//ウィンドウタブとシートのタイトル更新
	UpdateWindowTitle();
	UpdateSheetInfo();

	//汎用アイテムの文字列設定
	SetTextTbl(g_anyitem_tbl);

	//周期設定入力箇所の設定（変更時に白文字赤背景、初期値"100"）
	m_interval.SetChangeColor(true,COLOR_WHITE,COLOR_RED);
	RestoreInterval();

	//送信停止/送信開始ボタンを送信停止に設定
	CtrlButtonInterval(false);

	//指令選択のボタンを速度に設定
	CtrlButtonOrder(true);

	//指令入力箇所の設定（変更時に白文字赤背景、初期値"0"）
	for(int nLoop = 0;nLoop < 2;nLoop++)
		{
		m_edit[nLoop].SetChangeColor(true,COLOR_WHITE,COLOR_RED);
		m_edit[nLoop].SetWindowText(_T("0"));
		}

	//送信対象のチェックボックスに文字列設定し、チェック無しに設定
	SetTextTbl(g_request_tbl);
	SetButtonTbl(g_request_tbl,false);

	//数値表示項目のタイトルに文字列設定
	SetTextTbl(g_valueid_tbl);

	//操作フラグのボタンに文字列設定し、ボタンをOFF側に設定
	if(nType != MTYPE::MTYPE_SMALL)
		{
		//標準タイプとホストモード
		SetTextTbl(g_ctrlid_normal_ctrl);
		SetButtonTbl(g_ctrlid_normal_ctrl,false);
		}
	else
		{
		//小型タイプ
		SetTextTbl(g_ctrlid_small_ctrl);
		SetButtonTbl(g_ctrlid_small_ctrl,false);
		}

	//I/Oフラグの表示領域に文字列設定
	if(nType == MTYPE::MTYPE_NORMAL)
		SetTextTbl(g_io_normal_tbl);	//標準タイプ
	else if(nType == MTYPE::MTYPE_SMALL)
		SetTextTbl(g_io_small_tbl);		//小型タイプ
	else
		{
		//未指定
		}

	//警告/異常のタイトル領域に文字列設定
	SetTextTbl(g_warnerr_tbl);
	}

//画面アイテムの再配置と表示
void CguicanABH3View::ArrangeScreenItem()
	{
	//現在の設定機種
	MTYPE nType = GetType();

	//
	CRect baseRect,itemRect,moveRect,tmpRect1,tmpRect2;
	CSize baseSize,btnSize;
	GetDlgItem(IDC_TITLE_INTERVAL)->GetWindowRect(baseRect);
	baseSize.cx = baseRect.Width();
	baseSize.cy = baseRect.Height();

	GetDlgItem(IDC_OFF_CTRLID0)->GetWindowRect(itemRect);
	btnSize.cx = baseSize.cx;
	btnSize.cy = itemRect.Height();

	int nSepa = baseRect.Height() / 4;				//セパレータ距離
	int nSepa2 = (nSepa * 2);
	int nSepa4 = (nSepa * 4);

	//配置位置の計算
	int posX[10],posY[64];
	posX[0] = nSepa;
	posX[1] = posX[0] + baseSize.cx + nSepa;
	posX[2] = posX[1] + baseSize.cx + (nSepa * 2);
	posX[3] = posX[2] + baseSize.cx;
	posX[4] = posX[3] + baseSize.cx + (nSepa * 2);
	posX[5] = posX[4] + baseSize.cx + (nSepa * 4);
	posX[6] = posX[5] + baseSize.cx + nSepa;
	posX[7] = posX[6] + baseSize.cx + (nSepa * 4);
	posX[8] = posX[7] + ((baseSize.cx * 2) * 70 / 100);
	posX[9] = posX[8] + ((baseSize.cx * 2) * 15 / 100);
	posY[0] = nSepa;

	//2023/09/27 yo0043 ステータス表示を最上段左に移動 （伊佐地殿リクエスト）
	if(nType == MTYPE::MTYPE_HOST)
		{
		//Y位置の開始位置を2段移動
		//nRowを操作しようかと思ったが、指令モーdに影響した為保留
		posY[0] = nSepa + (baseSize.cy * 2);
		}


	posY[1] = posY[0] + baseSize.cy + (nSepa * 2);
	for(int nLoop = 2;nLoop < 64;nLoop++)
		posY[nLoop] = posY[nLoop - 1] + baseSize.cy;
	int nLen1 = posX[8] - posX[7];	//異常/警告タイトルの幅
	int nLen2 = posX[9] - posX[8];	//異常/警告表示部の幅

	//========================================
	//最上段（送信周期・送信ボタン）
	//========================================
	MoveItem(IDC_TITLE_INTERVAL,CPoint(posX[0],posY[0]),baseSize);
	MoveItem(IDC_INTERVAL,CPoint(posX[1],posY[0]),baseSize);
	MoveItem(IDC_OFF_INTERVAL,CPoint(posX[2],posY[0]),baseSize);
	MoveItem(IDC_ON_INTERVAL,CPoint(posX[3],posY[0]),baseSize);
	MoveItem(IDC_INFO,CPoint(posX[7],posY[0]),CSize(nLen1 + nLen2 + nLen2,baseSize.cy));
	MoveItem(IDC_INFO2,CPoint(posX[4],posY[0]),CSize(posX[7] - posX[4] - nSepa4,baseSize.cy));

	//========================================
	//カラム0（指令と数値表示）
	//========================================
	//開始行
	int nRow = 1;	//ホスト以外の初期位置
	if(nType == MTYPE::MTYPE_HOST)
		nRow = 0;	//ホストの初期位置

	//DP0R:チェックボックスとステータス表示
	MoveItem(IDC_REQUESTDP0R,CPoint(posX[0],posY[nRow]),baseSize);
	MoveItem(IDC_STATUSDP0R,CPoint(posX[1],posY[nRow++]),CSize(posX[4] - posX[1] - nSepa2 ,baseSize.cy));

	//指令モード
	MoveItem(IDC_TITLE_ORDER,CPoint(posX[0],posY[nRow]),CSize(baseSize.cx,baseSize.cy * 2));
	MoveItem(IDC_ORDER1,CPoint(posX[1],posY[nRow++]),baseSize);
	MoveItem(IDC_ORDER2,CPoint(posX[1],posY[nRow++]),baseSize);

	if(nType == MTYPE::MTYPE_HOST)
		{
		//2023/09/27 yo0043 ステータス表示を最上段左に移動 （伊佐地殿リクエスト）
		MoveItem(IDC_INFO,CPoint(posX[1],nSepa),CSize(posX[4] - posX[1] - nSepa2 ,baseSize.cy));
		MoveItem(IDC_INFO2,CPoint(posX[1],nSepa + baseSize.cy),CSize(posX[4] - posX[1] - nSepa2 ,baseSize.cy));

		//帰還を指令扱いにする（表示用）
		MoveItem(IDC_TITLE_A,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_VALUE0,CPoint(posX[1],posY[nRow++]),baseSize);
		MoveItem(IDC_TITLE_B,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_VALUE1,CPoint(posX[1],posY[nRow++]),baseSize);

		++nRow;

		//BR0:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR0,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR0,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR1:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR1,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR1,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR2:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR2,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR2,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR3:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR3,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR3,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR4:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR4,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR4,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR5:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR5,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR5,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR6:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR6,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR6,CPoint(posX[1],posY[nRow++]),baseSize);

		//チェックボックスを全て無効化する
		GetDlgItem(IDC_REQUESTDP0R)->EnableWindow(FALSE);
		GetDlgItem(IDC_REQUESTBR0)->EnableWindow(FALSE);
		GetDlgItem(IDC_REQUESTBR1)->EnableWindow(FALSE);
		GetDlgItem(IDC_REQUESTBR2)->EnableWindow(FALSE);
		GetDlgItem(IDC_REQUESTBR3)->EnableWindow(FALSE);
		GetDlgItem(IDC_REQUESTBR4)->EnableWindow(FALSE);
		GetDlgItem(IDC_REQUESTBR5)->EnableWindow(FALSE);
		GetDlgItem(IDC_REQUESTBR6)->EnableWindow(FALSE);

		}
	else
		{
		//指令入力
		MoveItem(IDC_TITLE_A,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_EDIT0,CPoint(posX[1],posY[nRow++]),baseSize);
		MoveItem(IDC_TITLE_B,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_EDIT1,CPoint(posX[1],posY[nRow++]),baseSize);
		//帰還
		MoveItem(IDC_TITLE_VALUE0,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_VALUE0,CPoint(posX[1],posY[nRow++]),baseSize);
		MoveItem(IDC_TITLE_VALUE1,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_VALUE1,CPoint(posX[1],posY[nRow++]),baseSize);
		++nRow;
		//BR2:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR2,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR2,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR2:4項目表示
		for(int nLoop = 0;nLoop < 4;nLoop++)
			{
			MoveItem(IDC_TITLE_VALUE2 + nLoop,CPoint(posX[0],posY[nRow]),baseSize);
			MoveItem(IDC_VALUE2 + nLoop,CPoint(posX[1],posY[nRow++]),baseSize);
			}
		++nRow;
		//BR3:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR3,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR3,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR3:4項目表示
		for(int nLoop = 0;nLoop < 4;nLoop++)
			{
			MoveItem(IDC_TITLE_VALUE6 + nLoop,CPoint(posX[0],posY[nRow]),baseSize);
			MoveItem(IDC_VALUE6 + nLoop,CPoint(posX[1],posY[nRow++]),baseSize);
			}
		++nRow;
		//BR4:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR4,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR4,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR4:2項目表示
		for(int nLoop = 0;nLoop < 2;nLoop++)
			{
			MoveItem(IDC_TITLE_VALUE10 + nLoop,CPoint(posX[0],posY[nRow]),baseSize);
			MoveItem(IDC_VALUE10 + nLoop,CPoint(posX[1],posY[nRow++]),baseSize);
			}
		++nRow;
		//BR5:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR5,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR5,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR5:4項目表示
		for(int nLoop = 0;nLoop < 4;nLoop++)
			{
			MoveItem(IDC_TITLE_VALUE12 + nLoop,CPoint(posX[0],posY[nRow]),baseSize);
			MoveItem(IDC_VALUE12 + nLoop,CPoint(posX[1],posY[nRow++]),baseSize);
			}
		++nRow;
		//BR6:チェックボックスとステータス表示
		MoveItem(IDC_REQUESTBR6,CPoint(posX[0],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR6,CPoint(posX[1],posY[nRow++]),baseSize);
		//BR6:2項目表示
		for(int nLoop = 0;nLoop < 2;nLoop++)
			{
			MoveItem(IDC_TITLE_VALUE16 + nLoop,CPoint(posX[0],posY[nRow]),baseSize);
			MoveItem(IDC_VALUE16 + nLoop,CPoint(posX[1],posY[nRow++]),baseSize);
			}
		++nRow;
		}

	//========================================
	//カラム1（操作ボタン）
	//========================================
	//開始行
	nRow = 2;

	if(nType == MTYPE::MTYPE_HOST)
		nRow = 1;

	//タイトル
	MoveItem(IDC_TITLE_CTRL,CPoint(posX[2],posY[nRow++]),CSize(baseSize.cx * 2,baseSize.cy));
	//ボタン
	pIDTEXT2 pTbl2 = g_ctrlid_normal_ctrl;
	while(pTbl2->nOffUid)
		{
		MoveItem(pTbl2->nOffUid,CPoint(posX[2],posY[nRow]),baseSize);
		MoveItem(pTbl2->nOnUid,CPoint(posX[3],posY[nRow++]),baseSize);
		++pTbl2;
		}

	//========================================
	//カラム2（制御フラグ）
	//========================================
	//開始行
	nRow = 2;
	//タイトル
	MoveItem(IDC_TITLE_REQUEST,CPoint(posX[4],posY[nRow++]),baseSize);
	//表示領域
	pIDTEXT3 pTbl3 = g_resultid_normal_tbl;
	while(pTbl3->nUid)
		{
		MoveItem(pTbl3->nUid,CPoint(posX[4],posY[nRow++]),baseSize);
		++pTbl3;
		}

	//========================================
	//カラム3（入力フラグとI/Oフラグ）
	//========================================
	//開始行
	nRow = 1;
	//BR1:チェックボックスとステータス表示
	if(nType != MTYPE::MTYPE_HOST)
		{
		MoveItem(IDC_REQUESTBR1,CPoint(posX[5],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR1,CPoint(posX[6],posY[nRow++]),baseSize);
		}
	//タイトル
	MoveItem(IDC_TITLE_INPUT,CPoint(posX[5],posY[nRow]),baseSize);
	MoveItem(IDC_TITLE_IO,CPoint(posX[6],posY[nRow++]),baseSize);
	//表示領域
	pTbl3 = g_inputid_normal_tbl;
	pIDTEXT pTbl = g_io_normal_tbl;
	while(pTbl3->nUid)
		{
		MoveItem(pTbl3->nUid,CPoint(posX[5],posY[nRow]),baseSize);
		MoveItem(pTbl->nUid,CPoint(posX[6],posY[nRow++]),baseSize);
		++pTbl3;
		++pTbl;
		}

	//========================================
	//カラム4（警告/異常）
	//========================================
	//開始行
	nRow = 1;
	//BR0:チェックボックスとステータス表示
	if(nType != MTYPE::MTYPE_HOST)
		{
		MoveItem(IDC_REQUESTBR0,CPoint(posX[7],posY[nRow]),baseSize);
		MoveItem(IDC_STATUSBR0,CPoint(posX[7] + baseSize.cx,posY[nRow++]),baseSize);
		}
	//タイトル
	MoveItem(IDC_TITLE_WARNERR,CPoint(posX[7],posY[nRow]),CSize(nLen1,baseSize.cy));
	MoveItem(IDC_TITLE_WARN,CPoint(posX[8],posY[nRow]),CSize(nLen2,baseSize.cy));
	MoveItem(IDC_TITLE_ERR,CPoint(posX[9],posY[nRow++]),CSize(nLen2,baseSize.cy));
	//表示領域
	pTbl = g_warnerr_tbl;
	UINT nWarnID = IDC_WARN_BIT0;
	UINT nErrID = IDC_ERR_BIT0;
	while(pTbl->nUid)
		{
		MoveItem(pTbl->nUid,CPoint(posX[7],posY[nRow]),CSize(nLen1,baseSize.cy));
		MoveItem(nWarnID,CPoint(posX[8],posY[nRow]),CSize(nLen2,baseSize.cy));
		MoveItem(nErrID,CPoint(posX[9],posY[nRow++]),CSize(nLen2,baseSize.cy));
		++pTbl;
		++nWarnID;
		++nErrID;
		}

	//========================================
	//表示処理
	//========================================
	//ホスト以外？
	if(nType != MTYPE::MTYPE_HOST)
		{

		//フォームの全アイテムを表示する（フォーム上のアイテムは、初期状態が非表示設定）
		CWnd* pWnd = GetTopWindow();
		while(pWnd)
			{
			pWnd->ShowWindow(SW_SHOW);
			pWnd = pWnd->GetNextWindow();
			}
		}
	else
		{
		//機種がホストの場合は特別扱いの要素のみ表示
		int nPt = 0;
		while(g_hostitem_tbl[nPt * 2])
			{
			UINT nID1 = g_hostitem_tbl[nPt * 2 + 0];
			UINT nID2 = g_hostitem_tbl[nPt * 2 + 1];
			if(nID2 == 0)
				nID2 = nID1;
			for(UINT nLoop = nID1;nLoop <= nID2;nLoop++)
				{
				CWnd* pWnd = GetDlgItem(nLoop);
				pWnd->ShowWindow(SW_SHOW);
				}
			++nPt;
			}
		}
	}

//ウィンドウタイトルを更新
void CguicanABH3View::UpdateWindowTitle()
	{
	//情報を取得
	uint8_t nID = GetID();
	uint8_t nGroup = GetGroup();
	CString sType = GetTypeName();

	//タブシート名をタブに設定する
	CString sText("");
	sText.Format(_T("ID = %d (%s)"),nID,(LPCTSTR)sType);
	GetParentFrame()->SetWindowText(sText);

	//シート右上の情報表示も更新
	sText.Format(_T("ID = %d (%s)  GROUP = %d"),nID,(LPCTSTR)sType,nGroup);
	FastSetText(IDC_INFO,sText);
	}

//ステータス表示を更新
void CguicanABH3View::UpdateSheetInfo()
	{
	//このビューに割り当てられたID
	uint8_t nID = GetID();

	if(!theABH3.IsOpenInterface())
		{
		//未接続
		FastSetText(IDC_INFO2,theApp.GetLangText(&g_status_text[2]));
		}
	//エラー状態の取得
	else if(theApp.isErrorID(nID))
		{
		//エラー有り
		FastSetText(IDC_INFO2,theApp.GetLangText(&g_status_text[1]));
		}
	else
		{
		//正常動作中
		FastSetText(IDC_INFO2,theApp.GetLangText(&g_status_text[0]));
		}
	}

//テーブルを元にテキストを一括設定
void CguicanABH3View::SetTextTbl(pIDTEXT pIDtbl)
	{
	while(pIDtbl->nUid)
		{
		GetDlgItem(pIDtbl->nUid)->SetWindowText(theApp.GetLangText(&pIDtbl->text));
		++pIDtbl;
		}
	}
void CguicanABH3View::SetTextTbl(pIDTEXT1 pIDtbl)
	{
	while(pIDtbl->nUid)
		{
		GetDlgItem(pIDtbl->nUid)->SetWindowText(theApp.GetLangText(&pIDtbl->text));
		++pIDtbl;
		}
	}
void CguicanABH3View::SetTextTbl(pIDTEXT2 pIDtbl)
	{
	while(pIDtbl->nOffUid)
		{
		GetDlgItem(pIDtbl->nOffUid)->SetWindowText(theApp.GetLangText(&pIDtbl->offtext));
		GetDlgItem(pIDtbl->nOnUid)->SetWindowText(theApp.GetLangText(&pIDtbl->ontext));
		++pIDtbl;
		}
	}
void CguicanABH3View::SetTextTbl(pIDTEXT4 pIDtbl)
	{
	while(pIDtbl->nTitleUid)
		{
		GetDlgItem(pIDtbl->nTitleUid)->SetWindowText(theApp.GetLangText(&pIDtbl->text));
		++pIDtbl;
		}
	}
void CguicanABH3View::SetTextTbl(pIDTEXT5 pIDtbl)
	{
	while(pIDtbl->nUid)
		{
		GetDlgItem(pIDtbl->nUid)->SetWindowText(theApp.GetLangText(&pIDtbl->text));
		++pIDtbl;
		}
	}

//テーブルを元にボタンを一括操作
void CguicanABH3View::SetButtonTbl(pIDTEXT2 pIDtbl,bool bOn)
	{
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
	while(pIDtbl->nUid)
		{
		CButton* pBtn = (CButton*)GetDlgItem(pIDtbl->nUid);
		pBtn->SetCheck((bOn != false) ? 1 : 0);
		++pIDtbl;
		}
	}

//指令切り替えのボタン制御
void CguicanABH3View::CtrlButtonOrder(bool bSpeed)
	{
	//ボタン制御及びフラグ制御を行う
	m_order_value[0].SetCheck((bSpeed != false) ? 1 : 0);
	m_order_value[1].SetCheck((bSpeed == false) ? 1 : 0);
	uint8_t nID = GetID();
	CConfigDlg::pCFG_ORDER pOrder = &theConfig.GetConfig()->order256[nID];
	pOrder->nType = (bSpeed != false) ? 0 : 1;	
	}

//周期送信のボタン制御
void CguicanABH3View::CtrlButtonInterval(bool bON)
	{
	//ボタン制御及びフラグ制御を行う
	m_interval_btn[0].SetCheck((bON == false) ? 1 : 0);
	m_interval_btn[1].SetCheck((bON != false) ? 1 : 0);
	m_var.thread.bRequest = bON;
	}

//色制御確認
bool CguicanABH3View::DrawCheck(CWnd* pWnd,COLORITEM& colorItem)
	{
	//描画対象のID
	UINT nItemID = pWnd->GetDlgCtrlID();

	//戻り値（描画フラグ）
	if(DrawCheck_0(nItemID,colorItem))
		return(true);
	else if(DrawCheck_1(nItemID,colorItem))
		return(true);
	else if(DrawCheck_2(nItemID,colorItem))
		return(true);
	else if(DrawCheck_3(nItemID,colorItem))
		return(true);
	else if(DrawCheck_4(nItemID,colorItem))
		return(true);
	else if(DrawCheck_5(nItemID,colorItem))
		return(true);
	return(false);
	}

//固定テキスト類アイテムの色制御確認
bool CguicanABH3View::DrawCheck_0(UINT nItemID,COLORITEM& colorItem)
	{
	//情報表示
	if(nItemID == IDC_INFO2)
		{
		//未接続？
		if(!theABH3.IsOpenInterface())
			colorItem = GetAppColor(APPCOLOR::APPC_ERROR);
		//エラー有り？
		else if(!theApp.isErrorID(GetID()))
			colorItem = GetAppColor(APPCOLOR::APPC_INFO);
		else
			colorItem = GetAppColor(APPCOLOR::APPC_ERROR);
		return(true);
		}

	//固定色表示テーブル
	pIDCOLOR pTbl = g_textcolor_tbl;

	//該当するかチェック
	while(pTbl->nBeginUid)
		{
		//該当？
		if(((pTbl->nBeginUid == nItemID) && (pTbl->nEndUid == 0))
			|| ((pTbl->nBeginUid <= nItemID) && (pTbl->nEndUid >= nItemID)))
			{
			colorItem = GetAppColor(pTbl->index);
			return(true);
			}
		++pTbl;
		}
	//非該当
	return(false);
	}

//入力フラグの色制御確認
bool CguicanABH3View::DrawCheck_1(UINT nItemID,COLORITEM& colorItem)
	{
	//比較対象のテーブル
	pIDTEXT3 pTbl = g_inputid_normal_tbl;

	//該当するかチェック
	while(pTbl->nUid)
		{
		//該当？
		if(pTbl->nUid == nItemID)
			{
			//評価用の値を取得
			uint32_t nValue = m_var.lastdata.BR1.nCtrlBit;
			//成立中？
			if(IsBit(nValue,pTbl->nBit))
				{
				//ONの時の色を戻す
				colorItem = GetAppColor(APPCOLOR::APPC_CTRL2);
				return(true);
				}
			//OFF状態
			return(false);
			}
		//次エントリ
		++pTbl;
		}
	//非該当
	return(false);
	}

//制御フラグの色制御確認
bool CguicanABH3View::DrawCheck_2(UINT nItemID,COLORITEM& colorItem)
	{
	//比較対象のテーブル
	pIDTEXT3 pTbl = g_resultid_normal_tbl;

	//該当するかチェック
	while(pTbl->nUid)
		{
		if(pTbl->nUid == nItemID)
			{
			uint32_t nValue = m_var.lastdata.DP0R.nCtrlBit;
			//成立中？
			if(IsBit(nValue,pTbl->nBit))
				{
				//ONの時の色を戻す
				colorItem = GetAppColor(APPCOLOR::APPC_CTRL1);
				return(true);
				}
			//OFF状態
			return(false);
			}
		++pTbl;
		}
	//非該当
	return(false);
	}

//I/Oフラグの色制御確認
bool CguicanABH3View::DrawCheck_3(UINT nItemID,COLORITEM& colorItem)
	{
	//機種
	MTYPE nType = GetType();

	//機種に対するテーブル
	pIDTEXT pTbl = g_io_normal_tbl;
	if(nType != MTYPE::MTYPE_NORMAL)
		pTbl = g_io_small_tbl;

	//テーブルから一致する物が有るか調べる
	while(pTbl->nUid)
		{
		if(pTbl->nUid == nItemID)
			{
			//比較対象の値
			uint32_t nValue = m_var.lastdata.BR1.nIoBit;

			//成立中？
			if(IsBit(nValue,pTbl->nBit))
				{
				//ONの時の色を戻す
				colorItem = GetAppColor(APPCOLOR::APPC_CTRL3);
				return(true);
				}
			return(false);
			}
		++pTbl;
		}
	//該当しない
	return(false);
	}

//警告と異常フラグの色制御確認
bool CguicanABH3View::DrawCheck_4(UINT nItemID,COLORITEM& colorItem)
	{
	//調査
	bool bError = false;	//false..warning  true..error
	UINT nOffset = 0;
	if((nItemID >= IDC_WARN_BIT0) && (nItemID <= IDC_WARN_BIT31))
		{
		bError = false;	//警告が描画対象
		nOffset = nItemID - IDC_WARN_BIT0;
		}
	else if((nItemID >= IDC_ERR_BIT0) && (nItemID <= IDC_ERR_BIT31))
		{
		bError = true;
		nOffset = nItemID - IDC_ERR_BIT0;
		}
	else
		return(false);	//非該当

	//テーブルのIDとの比較用ID
	//UINT nDrawID = IDC_TITLE_WARNERR0 + nOffset;

	//探す
	pIDTEXT pTbl = &g_warnerr_tbl[nOffset];

	//ビット番号
	int nBit = pTbl->nBit;

	//比較対象の値
	uint32_t nValue = m_var.lastdata.BR0.nWarnBit;
	if(bError)
		nValue = m_var.lastdata.BR0.nErrorBit;

	//成立中？
	if(IsBit(nValue,nBit))
		{
		//ONの時の色を戻す
		if(!bError)
			colorItem = GetAppColor(APPCOLOR::APPC_WARNING);
		else
			colorItem = GetAppColor(APPCOLOR::APPC_ERROR);
		return(true);
		}
	//OFF状態
	return(false);
	}

//ハートビート関連
bool CguicanABH3View::DrawCheck_5(UINT nItemID,COLORITEM& colorItem)
	{
	int nNum = 0;
	pIDTEXT5 pItem5 = g_request_tbl;
	while(pItem5->nUid)
		{
		if(pItem5->nSubUid == nItemID)
			{
			//
			if(IsHeartBeat(nNum))
				{
				//有効
				colorItem = GetAppColor(APPCOLOR::APPC_HEARTBEAT);
				return(true);
				}
			//無効
			colorItem = GetAppColor(APPCOLOR::APPC_WARNING);
			return(true);
			}
		++nNum;
		++pItem5;
		}
	return(false);
	}

//全データ部分の更新
void CguicanABH3View::UpdateView(bool bForce /* false */)
	{
	//最終受信データを取得
	theABH3.abh3_can_copylastdata(GetID(),&m_var.lastdata);

//m_var.lastdata.DP0R.nCtrlBit = 1;
//m_var.lastdata.BR0.nWarnBit = 2;
//m_var.lastdata.BR0.nErrorBit = 4;
//m_var.lastdata.BR1.nCtrlBit = 8;
//m_var.lastdata.BR1.nIoBit = 16;
//m_var.lastdata.update[0].nUpdate = 1;
//m_var.lastdata.update[1].nUpdate = 1;
//m_var.lastdata.update[2].nUpdate = 1;


	//各ビュー要素の更新
	UpdateView_0(bForce);
	UpdateView_1(bForce);
	UpdateView_2(bForce);
	UpdateView_3(bForce);
	UpdateView_4(bForce);
	UpdateView_5(bForce);
	UpdateView_6(bForce);
	UpdateView_7(bForce);
	}

//帰還・制御フラグの更新(要素0)
void CguicanABH3View::UpdateView_0(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 0;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetID(),nPos);

	//ハートビート設定
	UpHeartBeat(nPos);

	//現在の機種
	MTYPE nType = GetType();

	//表示対象がホスト以外？
	if(nType != MTYPE::MTYPE_HOST)
		{
		//表示対象は通常（標準・小型）表示用として扱う

		//========================================
		//帰還
		//========================================
		FastSetText(g_valueid_tbl[0].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.DP0R.nBackAY)));
		FastSetText(g_valueid_tbl[1].nValueUid,float2text(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.DP0R.nBackBX)));

		//========================================
		//制御フラグ
		//========================================
		uint32_t nValue = m_var.lastdata.DP0R.nCtrlBit;
		//使用テーブルの切り替え
		pIDTEXT3 pTbl = g_resultid_normal_tbl;
		if(nType != MTYPE::MTYPE_NORMAL)
			pTbl = g_resultid_small_tbl;
		while(pTbl->nUid)
			{
			if(IsBit(nValue,pTbl->nBit))
				FastSetText(pTbl->nUid,theApp.GetLangText(&pTbl->ontext));		//ON
			else
				FastSetText(pTbl->nUid,theApp.GetLangText(&pTbl->offtext));	//OFF
			++pTbl;
			}
		}
	else
		{
		//表示対象はホスト表示用として扱う

		//========================================
		//指令の値（帰還の場所に格納）を表示、表示先は帰還表示位置（指令の場所に位置移動済み）
		//========================================
		CString sText1(""),sText2("");
		if(GetOrderType() == 0)
			{
			//速度扱い
			sText1.Format(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.DP0R.nBackAY));
			sText2.Format(_T("%.1f"),theABH3.cnvCAN2Vel(m_var.lastdata.DP0R.nBackBX));
			}
		else
			{
			//トルク扱い
			sText1.Format(_T("%.1f"),theABH3.cnvCAN2Cur(m_var.lastdata.DP0R.nBackAY));
			sText2.Format(_T("%.1f"),theABH3.cnvCAN2Cur(m_var.lastdata.DP0R.nBackBX));
			}
		FastSetText(IDC_VALUE0,sText1);
		FastSetText(IDC_VALUE1,sText2);

		//========================================
		//制御フラグの値を操作フラグのボタンに反映
		//
		//操作/入力/制御/I/Oフラグはbitの並びを同じにしている為、
		//テーブルのID順が一致（IDC_*0 -> IDC_*31）になっている事が前提となっている
		//========================================
		uint32_t nValue = m_var.lastdata.DP0R.nCtrlBit;
		pIDTEXT3 pTbl3 = g_resultid_normal_tbl;
		pIDTEXT2 pTbl2 = g_ctrlid_normal_ctrl;
		while(pTbl3->nUid)
			{
			CButton* pON = (CButton*)GetDlgItem(pTbl2->nOnUid);
			CButton* pOFF = (CButton*)GetDlgItem(pTbl2->nOffUid);

			if(IsBit(nValue,pTbl3->nBit))
				{
				pON->SetCheck(1);
				pOFF->SetCheck(0);
				//m_ctrl_on_bit[nPt].SetCheck(1);
				//m_ctrl_off_bit[nPt].SetCheck(0);
				}
			else
				{
				pON->SetCheck(0);
				pOFF->SetCheck(1);
				//m_ctrl_on_bit[nPt].SetCheck(0);
				//m_ctrl_off_bit[nPt].SetCheck(1);
				}
			++pTbl3;
			++pTbl2;
			}
		}
	}

//異常・警告フラグの更新(要素1)
void CguicanABH3View::UpdateView_1(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 1;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetID(),nPos);

	//ハートビート設定
	UpHeartBeat(nPos);

	//現在の機種がホストなら何も描画しない
	if(GetType() == MTYPE::MTYPE_HOST)
		return;

	//========================================
	//異常フラグと警告フラグの再描画
	//========================================
	pIDTEXT pTbl = g_warnerr_tbl;
	while(pTbl->nUid)
		{
		UINT nOffset = pTbl->nUid - IDC_TITLE_WARNERR0;
		GetDlgItem(IDC_WARN_BIT0 + nOffset)->Invalidate();
		GetDlgItem(IDC_ERR_BIT0 + nOffset)->Invalidate();
		++pTbl;
		}
	}

//I/Oフラグ・入力フラグの更新(要素2)
void CguicanABH3View::UpdateView_2(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 2;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetID(),nPos);

	//ハートビート設定
	UpHeartBeat(nPos);

	//現在の機種がホストなら何も描画しない
	if(GetType() == MTYPE::MTYPE_HOST)
		return;

	//========================================
	//I/Oフラグ
	//========================================
	//再描画処理
	pIDTEXT pTbl = g_io_normal_tbl;
	//機種切り替え
	if(GetType() == MTYPE::MTYPE_SMALL)
		pTbl = g_io_small_tbl;
	while(pTbl->nUid)
		{
		GetDlgItem(pTbl->nUid)->Invalidate();
		++pTbl;
		}

	//========================================
	//入力フラグ
	//========================================
	//現在の値を取得して適切な文字を設定
	uint32_t nValue = m_var.lastdata.BR1.nCtrlBit;
	//機種切り替え
	pIDTEXT3 pTbl2 = g_inputid_normal_tbl;
	if(GetType() == MTYPE::MTYPE_SMALL)
		pTbl2 = g_inputid_small_tbl;
	while(pTbl2->nUid)
		{
		if(IsBit(nValue,pTbl2->nBit))
			FastSetText(pTbl2->nUid,theApp.GetLangText(&pTbl2->ontext));	//ON
		else
			FastSetText(pTbl2->nUid,theApp.GetLangText(&pTbl2->offtext));	//OFF
		++pTbl2;
		}
	}

//速度指令・速度帰還の更新(要素3)
void CguicanABH3View::UpdateView_3(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 3;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetID(),nPos);

	//ハートビート設定
	UpHeartBeat(nPos);

	//現在の機種がホストなら何も描画しない
	if(GetType() == MTYPE::MTYPE_HOST)
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
	}

//電流指令・負荷率の更新(要素4)
void CguicanABH3View::UpdateView_4(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 4;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetID(),nPos);

	//ハートビート設定
	UpHeartBeat(nPos);

	//現在の機種がホストなら何も描画しない
	if(GetType() == MTYPE::MTYPE_HOST)
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
	}

//パルス積算値の更新(要素5)
void CguicanABH3View::UpdateView_5(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 5;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetID(),nPos);

	//ハートビート設定
	UpHeartBeat(nPos);

	//現在の機種がホストなら何も描画しない
	if(GetType() == MTYPE::MTYPE_HOST)
		return;

	//========================================
	//パルス積算値
	//========================================
	FastSetText(g_valueid_tbl[10].nValueUid,uint2text(_T("%d"),m_var.lastdata.BR4.nCountPulseA));
	FastSetText(g_valueid_tbl[11].nValueUid,uint2text(_T("%d"),m_var.lastdata.BR4.nCountPulseB));
	}

//アナログ入力・電源電圧の更新(要素6)
void CguicanABH3View::UpdateView_6(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 6;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetID(),nPos);

	//ハートビート設定
	UpHeartBeat(nPos);

	//現在の機種がホストなら何も描画しない
	if(GetType() == MTYPE::MTYPE_HOST)
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
	}

//モニタデータの更新(要素7)
void CguicanABH3View::UpdateView_7(bool bForce /* false */)
	{
	//更新フラグの場所
	int32_t nPos = 7;

	//更新不用？
	if((bForce | (bool)(m_var.lastdata.update[nPos].nUpdate != 0)) == false)
		return;

	//更新フラグを解除
	theABH3.abh3_can_resetlastdata(GetID(),nPos);

	//ハートビート設定
	UpHeartBeat(nPos);

	//現在の機種がホストなら何も描画しない
	if(GetType() == MTYPE::MTYPE_HOST)
		return;

	//========================================
	//モニタデータ（正体不明）
	//========================================
	FastSetText(g_valueid_tbl[16].nValueUid,float2text(_T("%.2f"),m_var.lastdata.BR6.nMonitor0));
	FastSetText(g_valueid_tbl[17].nValueUid,float2text(_T("%.2f"),m_var.lastdata.BR6.nMonitor1));
	}

//================================================================================
//設定関連
//================================================================================

//設定の復帰
void CguicanABH3View::reg2disp()
	{
	//周期設定と周期送信対象を前の状態に復帰
	RestoreInterval();
	RestoreRequestTarget();

	//指令値と指令モードを前の状態に復帰
	RestoreOrder();

	//送信対象のチェックボックスを前の状態に復帰
	RestoreRequestTarget();

	//データ表示更新
	UpdateView(true);
	}

//指令を設定しシステムへ保存
void CguicanABH3View::SetOrder(int nNum,CString sValue)
	{
	SetOrder(nNum,(float)::_tcstod(sValue,NULL));
	}
void CguicanABH3View::SetOrder(int nNum,float nValue)
	{
	//ID
	uint8_t nID = GetID();
	//環境設定に設定
	CConfigDlg::pCFG_ORDER pOrder = &theConfig.GetConfig()->order256[nID];
	if(nNum == 0)
		pOrder->nA = nValue;
	else if(nNum == 1)
		pOrder->nB = nValue;
	//システムに保存
	theConfig.reg2sys();
	}

//指令モードを設定しシステムへ保存
void CguicanABH3View::SetOrderType(int nMode)
	{
	//ID
	uint8_t nID = GetID();
	//環境設定に設定
	CConfigDlg::pCFG_ORDER pOrder = &theConfig.GetConfig()->order256[nID];
	pOrder->nType = uint8_t(nMode);
	//システムに保存
	theConfig.reg2sys();
	}

//現在の指令モードを取得
int CguicanABH3View::GetOrderType()
	{
	//ID
	uint8_t nID = GetID();
	//環境設定から取得
	CConfigDlg::pCFG_ORDER pOrder = &theConfig.GetConfig()->order256[nID];
	//
	uint8_t nResult = uint8_t(pOrder->nType);
	return(nResult);
	}

//指令と指令選択の復帰
void CguicanABH3View::RestoreOrder()
	{
	//
	uint8_t nID = GetID();

	CString sText("");
	CConfigDlg::pCFG_ORDER pOrder = &theConfig.GetConfig()->order256[nID];

	//指令を表示
	sText.Format(_T("%.1f"),pOrder->nA);
	m_edit[0].SetWindowText(sText);
	sText.Format(_T("%.1f"),pOrder->nB);
	m_edit[1].SetWindowText(sText);
	
	//指令モードを表示
	m_order_value[0].SetCheck((pOrder->nType == 0) ? 1 : 0);
	m_order_value[1].SetCheck((pOrder->nType != 0) ? 1 : 0);
	}


//周期設定を設定しシステムへ保存
void CguicanABH3View::SetInterval(uint32_t nTimeMS)
	{
	//ID
	uint8_t nID = GetID();
	//範囲制限(10-10000)
	if(nTimeMS < 10)
		nTimeMS = 10;
	else if(nTimeMS > 10000)
		nTimeMS = 10000;
	//環境設定に設定
	theConfig.GetConfig()->interval256[nID] = nTimeMS;
	//システムに保存
	theConfig.reg2sys();
	}

//周期設定を復帰
void CguicanABH3View::RestoreInterval()
	{
	//ID
	uint8_t nID = GetID();
	//環境設定から取得
	uint32_t nTimeMS = theConfig.GetConfig()->interval256[nID];
	//保存データ無し？
	if(nTimeMS == 0)
		nTimeMS = 100;	//工場出荷設定
	//範囲制限(10-10000)
	else if(nTimeMS < 10)
		nTimeMS = 10;
	else if(nTimeMS > 10000)
		nTimeMS = 10000;
	//表示
	CString sText("");
	sText.Format(_T("%d"),nTimeMS);
	m_interval.SetWindowText(sText);
	}

//周期送信対象の復帰
void CguicanABH3View::RestoreRequestTarget()
	{
	//
	uint8_t nID = GetID();
	uint32_t nValue = theConfig.GetConfig()->send256[nID];

	int nLoop = 0;
	while(-1)
		{
		IDTEXT5 item = g_request_tbl[nLoop];
		if(item.nUid == 0)
			break;
		CButton* pBtn = (CButton*)GetDlgItem(item.nUid);
		//現在の機種設定がホスト以外？
		if(GetType() != MTYPE::MTYPE_HOST)
			pBtn->SetCheck(((nValue & (1 << nLoop)) != 0) ? 1 : 0);
		else
			pBtn->SetCheck(0);	//ホストモードではチェック非表示

		++nLoop;
		}
	}

//周期送信対象の取り込み
uint32_t CguicanABH3View::StockRequestTarget(bool bNoSave /* false */)
	{
	//
	uint8_t nID = GetID();

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
	//指定が有ればシステムへ保存
	if(!bNoSave)
		theConfig.reg2sys();
	//
	return(nValue);
	}

//================================================================================
//処理系
//================================================================================

//非同期送信スレッド
 unsigned __stdcall CguicanABH3View::SendThread(void* pParam)
	{
	//========================================
	//目的の動作
	//========================================
	//一定時間毎に周期送信処理を行う。
	//処理時間が長い等の理由で、指定時間に処理が開始出来ない場合は、次回時間に持ち越す
	//
	//========================================
	//動作詳細
	//========================================
	//(1)現在時間を取得し、予定時間（次の周期指定送信時間）を超過しているか確認
	//	未達の場合は(5)に移動
	//(2)送信要求が有り、エラーが無く、インターフェースの利用開始状態なら、
	//	周期送信処理を呼び出す。
	//(3)エラーが有った場合、回線が切れているか判断する。
	//	回線が切れている場合はユーザーが切ったと判断し、エラーにしない
	//	そうでない場合はエラーを保存しておく
	//(4)予定時間に周期時間を加算する処理を、現在時間より後の時間になる迄繰り返す。
	//	予定時間が決まったら、(1)に戻る
	//(5)現在時間と予定時間の半分の時間（但し1秒を超えない）を算出し、待つ
	//	待ったら(1)に戻る
	//
	//========================================

	//呼び出し元クラス類と制御用変数へのポインタ類
	CguicanABH3View* pClass = (CguicanABH3View*)pParam;
//	CguicanABH3Doc* pDoc = pClass->GetDocument();
	volatile CguicanABH3View::pVIEW_VAR pVar = &pClass->m_var;
	//時間制御用及び制御対象IDの取得
	uint32_t nNextTime = theABH3.GetTm();
	uint8_t nID = pVar->config.nID;

	//処理ループ
	while(!pVar->thread.bQuit)
		{
		//現在時間
		uint32_t nCurrentTime = theABH3.GetTm();

		//送信予定時間超過？
		if(nCurrentTime >= nNextTime)
			{
			//送信要求があるか？
			if(pVar->thread.bRequest)
				{
				//エラー状態では無いか？
				if(pVar->thread.nResult == 0)
					{
					//回線を開いているか？
					if(theABH3.IsOpenInterface())
						{
						//送信処理
						uint32_t nResult = pClass->ExecCycleRequest();//pVar->cycle.nCycleRequest);
						//エラー有り？
						if(nResult)
							{
							//もしかしてユーザーに回線切られた？
							if(!theABH3.IsOpenInterface())
								{
								//エラーにしない
								nResult = 0;
								}
							}
						//結果格納
						pVar->thread.nResult = nResult;
						}
					}
				}
			//次回時間
			uint32_t nStepTime = theConfig.GetConfig()->interval256[nID];
			if(nStepTime < 10)
				nStepTime = 10;
			while(nNextTime <= nCurrentTime)
				{
				nNextTime += nStepTime;
				}
			}
		//予定時間前
		else
			{
			//残り時間の半分を待つ、但し最大1秒
			uint32_t nRemain = nNextTime - nCurrentTime;
			uint32_t nWaitMS = nRemain / 2;
			if(nWaitMS > 1000)
				nWaitMS = 1000;
			Sleep(nWaitMS);
			}
		}

	//スレッド終了処理
	_endthreadex(0);
	return(0);
	}

//周期要求設定の実行
uint32_t CguicanABH3View::ExecCycleRequest()	//uint32_t nCycleRequest)
	{
	//
	uint32_t nResult = 0;
	int32_t nLoop = 0;
	IDTEXT5 item;
	uint8_t nID = GetID();
	uint32_t nRequest = theConfig.GetConfig()->send256[nID];

	//
	while(-1)
		{
		//情報取り出しと終端判断
		item = g_request_tbl[nLoop];;
		if(item.nUid == 0)
			{
			break;
			}

		//周期処理要求あり？
		else if(nRequest & (1 << nLoop))
			{
			//シングルパケット？
			if(item.nType == PACKETTYPE::SINGLE_PACKET)
				{
				//
				CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();
				CConfigDlg::pCFG_ORDER pOrder = &pConfig->order256[nID];
	
				//現在の指令値を取得して、指定された指令モード用に値を変換する
				uint16_t nA = 0;
				uint16_t nB = 0;
				//速度モード？
				if(pOrder->nType == 0)
					{
					//速度
					nA = theABH3.cnvVel2CAN(pOrder->nA);
					nB = theABH3.cnvVel2CAN(pOrder->nB);
					}
				else
					{
					//トルク
					nA = theABH3.cnvCur2CAN(pOrder->nA);
					nB = theABH3.cnvCur2CAN(pOrder->nB);
					}

				//シングルパケット・指令と操作を一括送信・受信無し
				if(theABH3.abh3_can_cmdAndopSet(nID,nA,nB,m_var.userctrl.nRequestFlag,0,NULL))
					{
					//エラー
					nResult = nLoop + 1;
					break;
					}
				}
			//ブロードキャスト？
			else if(item.nType == PACKETTYPE::BROADCAST_PACKET)
				{
				//グループとアドレスからコードを作り、送信のみ行う
				uint8_t nCode = (GetGroup() << 3) | (item.nAdrs & 0x07);
				if(theABH3.abh3_can_reqBRD(nID,nCode,0,NULL))
					{
					nResult = nLoop + 1;
					break;
					}
				}
			}
		//次の送信対象
		++nLoop;
		}

	//正常終了
	return(nResult);
	}

//================================================================================
//イベントハンドラ
//================================================================================

//ダイアログアイテムの色制御時に呼び出されます
HBRUSH CguicanABH3View::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)
	{
	//
	COLORITEM colorItem = GetAppColor(APPCOLOR::APPC_NORMAL);

	//色付け対象か？
	if(DrawCheck(pWnd,colorItem))
		{
		//
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(colorItem.nBack);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(colorItem.nBack);
		pDC->SetTextColor(colorItem.nText);
		return(m_brush);
		}

	//標準処理
	HBRUSH hbr = CFormView::OnCtlColor(pDC,pWnd,nCtlColor);
	return hbr;
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

		//1秒周期割り込み？
		if(nIDEvent == m_var.n1sec)
			{
			UpdateSheetInfo();

			//非アクティブウィンドウは、この割り込み周期で画面更新
			UpdateView();
			}
		//100ms周期割り込み？
		else if(nIDEvent == m_var.nTimerNum)
			{
			//ハートビートの寿命処理
			DownHeartBeat();

			//最前面に表示されていれば、表示物を更新
			if(bActive)
				UpdateView();

			//周期要求要素の取り込み
//			m_var.cycle.nCycleRequest = StockRequestTarget(true);

			//周期要求の結果を確認（今回の要求結果とは限らない）
			uint32_t nResult = m_var.thread.nResult;
			//エラー有り？
			if(nResult)
				{
				//周期送信を強制停止させる
				CtrlButtonInterval(false);
				//送信エラーを設定
				theApp.SetErrorID(GetID());
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

//周期設定が更新されると呼び出されます
void CguicanABH3View::OnEnChangeInterval()
	{
	//確定した？
	if(m_interval.IsSetData())
		{
		//確定したので、入力文字を取得し数値変換
		CString sText("");
		m_interval.GetWindowText(sText);
		uint32_t nTimeMS = (uint32_t)::_tcstoul((LPCTSTR)sText,NULL,10);
		//格納
		SetInterval(nTimeMS);
		//再表示（値の制限を考慮）
		RestoreInterval();
		}
	}

//指令が更新されると呼び出されます
void CguicanABH3View::OnEnChangeEdit(UINT nUid)
	{
	//
	UINT nEdit = nUid - IDC_EDIT0;

	//確定した？
	if(m_edit[nEdit].IsSetData())
		{
		//
		uint8_t nID = GetID();
		//環境設定へのポインタ取得
		CConfigDlg::pCONFIGDLG_CONFIG pConfig = theConfig.GetConfig();

		//確定したので、入力文字を取得し格納
		CString sText("");
		m_edit[nEdit].GetWindowText(sText);
		if(nEdit == 0)
			pConfig->order256[nID].nA = (float)::_tcstod((LPCTSTR)sText,NULL);
		else if(nEdit == 1)
			pConfig->order256[nID].nB = (float)::_tcstod((LPCTSTR)sText,NULL);

		//システムへ保存する
		theConfig.reg2sys();
		}
	}

//操作フラグのOFFボタンが押されると呼び出されます
void CguicanABH3View::OnBnClickedOffCtrlid(UINT nUid)
	{
	//注意点
	//	ボタン自体は自動扱いなので、ON/OFFの制御不要

	//確認
	if((nUid >= IDC_OFF_CTRLID0) && (nUid <= IDC_OFF_CTRLID31))
		{
		//押されたボタン
		UINT nBtn = nUid - IDC_OFF_CTRLID0;
		//指定ビットを解除
		m_var.userctrl.nRequestFlag &= ~(1 << nBtn);
		}
	}

//操作フラグのONボタンが押されると呼び出されます
void CguicanABH3View::OnBnClickedOnCtrlid(UINT nUid)
	{
	//注意点
	//	ボタン自体は自動扱いなので、ON/OFFの制御不要

	//確認
	if((nUid >= IDC_ON_CTRLID0) && (nUid <= IDC_ON_CTRLID31))
		{
		//押されたボタン
		UINT nBtn = nUid - IDC_ON_CTRLID0;
		//指定ビットを設定
		m_var.userctrl.nRequestFlag |= (1 << nBtn);
		}
	}

//ウィンドウが破棄される時に呼び出されます
void CguicanABH3View::OnDestroy()
	{
	CFormView::OnDestroy();

	//非同期送信スレッド停止
	m_var.thread.bQuit = true;
	::WaitForSingleObject(m_var.thread.hThread,INFINITE);
	}

//周期送信対象チェックボックスが操作されると呼び出されます
void CguicanABH3View::OnBnClickedRequestTarget(UINT nUid)
	{
	UNUSED_ALWAYS(nUid);
	//周期送信対象を取り込んでシステムへ保存する
	StockRequestTarget();
	}

//指令選択を切り替えると呼び出されます
void CguicanABH3View::OnBnClickedOrder(UINT nUid)
	{
	uint32_t nBtn = nUid - IDC_ORDER1;
	if((nBtn >= 0) && (nBtn <= 1))
		{
		//モードを保存
		SetOrderType(int(nBtn));

		//現在の機種設定がホスト以外？
		if(GetType() != MTYPE::MTYPE_HOST)
			{
			//指令を初期化してシステムに保存
			SetOrder(0,0);
			SetOrder(1,0);
			//指令の再表示
			RestoreOrder();
			}
		}
	}

//「送信開始」「送信停止」メニューが選択されると呼び出されます
void CguicanABH3View::OnBnClickedInterval(UINT nUid)
	{
	//注意点
	//	ボタン自体は手動扱いなので、ON/OFFの制御が必要

	//機種を取得
	MTYPE nType = GetType();

	//ホスト以外？
	if(nType != MTYPE::MTYPE_HOST)
		{
		//設定
		m_var.thread.bRequest = (nUid == IDC_ON_INTERVAL) ? true : false;
		//ボタンに反映
		CtrlButtonInterval(m_var.thread.bRequest);
		}
	}

//送信開始のメニュー制御の為、呼び出されます
void CguicanABH3View::OnUpdateOnInterval(CCmdUI* pCmdUI)
	{
	//機種を取得
	MTYPE nType = GetType();

	//
	if(nType == MTYPE::MTYPE_HOST)
		pCmdUI->Enable(FALSE);	//常に使用不可
	else
		pCmdUI->Enable((m_var.thread.bRequest != false) ? FALSE : TRUE);
	}

//送信停止のメニュー制御の為、呼び出されます
void CguicanABH3View::OnUpdateOffInterval(CCmdUI* pCmdUI)
	{
	//機種を取得
	MTYPE nType = GetType();

	//ホスト？
	if(nType == MTYPE::MTYPE_HOST)
		pCmdUI->Enable(FALSE);	//常に使用不可
	else
		pCmdUI->Enable((m_var.thread.bRequest == false) ? FALSE : TRUE);
	}



