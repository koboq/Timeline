
// MIT License
// Copyright (c) 2025 こぼ
//
// See the LICENSE file in the project root for full license information.

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include "timeline.h"

using namespace Modigs;

int main() {

	Timeline tl;

	//-------------------------------------------
	// デフォルトのコールバック設定
	//-------------------------------------------

	tl.SetGlobalCallback([](int id, const std::vector<double>& params) {
		// イベントIDとパラメータを出力する
		std::cout << "[GLOBAL] id=" << id << " params=[";
		for (size_t i = 0; i < params.size(); ++i) {
			std::cout << std::fixed << std::setprecision(2) << params[i];
			if (i + 1 < params.size()) std::cout << ", ";
		}
		std::cout << "]\n";
	});
	


	//-------------------------------------------
	// イベント登録
	// 複数のパターンを試す
	//-------------------------------------------

	// Case 1. 
	//    発火時間: 1.0秒
	//    関数指定: グローバルコールバック
	//    引数:     有
	tl.AddEvent(1.0, 100, {1.0});


	// Case 2.
	//    発火時間: 2.0秒
	//    関数指定: 個別コールバック
	//    引数:     有
	tl.AddEvent(2.0, 101,
		{0.5, 2.0},
		[](int id, const std::vector<double>& params) {
			std::cout << "[CALL1] id=" << id << " params=[";
			for (size_t i = 0; i < params.size(); ++i) {
			std::cout << std::fixed << std::setprecision(2) << params[i];
				if (i + 1 < params.size()) std::cout << ", ";
			}
			std::cout << "]\n";
		});


	// Caee 3.
	//    発火時間: 4.5秒
	//    関数指定: グローバルコールバック
	//    引数:     無
	tl.AddEvent(4.5, 200);


	// Case 4.
	//    発火時間: 3.2秒
	//    関数指定: 個別コールバック
	//    引数:     無
	// ※ 前の時間を指定しても自動でソートされる
	tl.AddEvent(3.2, 300,
		{},
		[](int id, const std::vector<double>& params) {
			std::cout << "[CALL2] id=" << id << " params=[";
			for (size_t i = 0; i < params.size(); ++i) {
			std::cout << std::fixed << std::setprecision(2) << params[i];
				if (i + 1 < params.size()) std::cout << ", ";
			}
			std::cout << "]\n";
		});



	//-------------------------------------------
	// タイムラインの期間
	// 未指定なら最大イベント時刻=4.5秒
	//-------------------------------------------
	//tl.SetDuration(4.0);



	//-------------------------------------------
	// ループ指定
	//-------------------------------------------
	tl.SetLoop(true);// 3.0秒区切りでループ



	//-------------------------------------------
	// 再生開始
	//-------------------------------------------
	tl.Play();



	//-------------------------------------------
	// 再生
	// 0.1 x 70 = 7秒実行する 
	//-------------------------------------------
	for (int i = 0; i < 70; ++i) {
	    tl.Update(0.1);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}