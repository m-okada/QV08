/*
 * sdram.c
 *
 *  Created on: 2016/08/21
 *      Author: Manabu Okada
 */

#include "cpu.h"
#include "sdram_port.h"
#include "sdram.h"

/*

初期化
モードレジスタ設定
バーストリード
ライト
リフレッシュ

プリチャージ

読み込みキャッシュ
ライトスルー

*/

/*
初期化シーケンス

電源投入
1. VDD, VDDQ を同時に投入します。このときCKE はローレベルを保持します。
2. 電源安定を待ちます。
3. クロックを供給します。また、CKE をハイレベルにします。
　注意　電源投入時は、入力ピンのレベルは VDD+0.3V を超えないようにして下さい。

初期化
4. 電源およびクロックの安定後、最小200μs 待ちます。
5. プリチャージオールコマンド(PALL)を発行します。
6. tRP 経過後、オート・リフレッシュ・コマンドを8 回以上実行します。
7. モード・レジスタの初期設定のため，モード・レジスタ設定コマンドを実行します。
　備考　初期化シーケンス中は、出力がハイ・インピーダンス状態であることを保証するため，CKE とDQM を
　　　　ハイレベルの状態に保持することを推奨します。

*/

/*

No コマンド                  略号  CKE      /CS /RAS /CAS /WE DQM Address
                                   n–1 n                          BA0, A10 A11,
                                                                  BA1      A9-A0
 1 モード・レジスタ設定      MRS   H   x    L   L    L    L   x   L    L   V
 2 CBR（オート）リフレッシュ REF   H   H    L   L    L    H   x   x    x   x
 3 セルフ・リフレッシュ開始  SELF  H   L    L   L    L    H   x   x    x   x
 4 セルフ・リフレッシュ終了  －    L   H    L   H    H    H   x   x    x   x
                                   L   H    H   x    x    x   x   x    x   x
 5 選択バンクのプリチャージ  PRE   H   x    L   L    H    L   x   V    L   x
 6 全バンクのプリチャージ    PALL  H   x    L   L    H    L   x   x    H   x
 7 バンク・アクティブ        ACT   H   x    L   L    H    H   x   V    V   V
 8 ライト                    WRIT  H   x    L   H    L    L   x   V    L   V
 9 ライト(AP)                WRITA H   x    L   H    L    L   x   V    H   V
10 リード                    READ  H   x    L   H    L    H   x   V    L   V
11 リード(AP)                READA H   x    L   H    L    H   x   V    H   V
12 バースト・ストップ        BST   H   x    L   H    H    L   x   x    x   x
13 ノー・オペレーション      NOP   H   x    L   H    H    H   x   x    x   x
14 デバイス非選択            DESL  H   x    H   x    x    x   x   x    x   x
15 データ入出力可能          −     H   x    x   x    x    x   L   x    x   x
16 データ・マスク            −     H   x    x   x    x    x   H   x    x   x

備考 H：ハイ・レベル，L：ロウ・レベル，
     x：ハイ・レベルまたはロウ・レベル（Don't care）
     V：有効データ
(AP) (オート・プリチャージ付き)


Row Address: RA0 ~ RA11, Column Address: CA0 ~ CA8
*/
static void sdram_set_cmd(int cmd, int op) ;

void sdram_init(void){
	;

	// A9:1single write
	//   A6-A4:010 CL2
	//     A3:0 Sequential
	//       A2-A0:011 Burst Length 8
	sdram_set_cmd(SDR_MRS, 0) ;
	sdramport_setmode(0x00000223UL) ;
}

static void sdram_set_cmd(int cmd, int option){
	switch(cmd){
	case SDR_MRS:
		break ;
	case SDR_REF:
		break ;
	case SDR_SELF:
		break ;
	case SDR_END_SELF:
		break ;
	case SDR_PRE:
		break ;
	case SDR_PALL:
		break ;
	case SDR_ACT:
		break ;
	case SDR_WRIT:
		break ;
	case SDR_WRITA:
		break ;
	case SDR_READ:
		break ;
	case SDR_READA:
		break ;
	case SDR_BST:
		break ;
	case SDR_NOP:
		break ;
	case SDR_DSEL:
		break ;
	default:
		break ;
	}
}

int sdram_read_byte(int addr){
	;

	return 0 ;
}

void sdram_refresh(int start_clmn){
	;
}
