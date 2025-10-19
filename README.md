# Modigs Timeline (C++)

シンプルで高速なイベントタイムライン（C++）。  
時刻指定のイベント登録・再生・ループ・グローバル／個別コールバックに対応。  
Visual Studio対応・MITライセンス。

A simple and fast event timeline library in C++.  
Supports time-based event registration, playback, looping, and both global and per-event callbacks.  
Designed for Visual Studio. Released under the MIT License.

---

## 特徴 / Features
- `AddEvent(time, id, params, callback)` でイベント登録  
  Register events with `AddEvent(time, id, params, callback)`
- `Play/Pause/Stop/Reset`、`SetLoop(true/false)`  
  Control playback with `Play`, `Pause`, `Stop`, `Reset`, and `SetLoop(true/false)`
- `Update(deltaTimeSec)` で時間進行＆区間内イベントを安全に発火  
  Advance time using `Update(deltaTimeSec)` to safely trigger events within the interval
- 個別コールバック優先 → 無ければグローバルコールバック  
  Per-event callbacks take priority; if none are set, the global callback is used

---

## 使い方（概要） / Usage Example
```cpp
#include "timeline/timeline.h"
using namespace Modigs;

Timeline tl;
tl.SetGlobalCallback([](int id, const std::vector<double>& p){ /* ... */ });
tl.AddEvent(0.5,  1, {1.0, 2.0});
tl.AddEvent(1.0, 42, {}, [](int id, const auto&){ /* 個別処理 / per-event callback */ });
tl.SetLoop(false);
tl.Play();
tl.Update(0.2); // …ゲーム/アプリのフレームで呼ぶ / call this per frame

tl.Pause(); // …一時停止、Playを呼ぶと止めたところから再開される / pause and resume
tl.Stop();  // …停止、Playを呼ぶと最初から始まる / stop and restart from beginning
tl.Reset(); // …削除、タイムラインが削除される / clear all events



## 動作環境 / Environment
- Visual Studio 2022（C++17 以上を推奨）
  Visual Studio 2022 (C++17 or later recommended)
- timeline.h / timeline.cpp をプロジェクトに追加
  Add timeline.h and timeline.cpp to your project

## ライセンス / License
MIT License - 詳細は LICENSE を参照。
MIT License - see the LICENSE file for details.

## その他 / Notes
- 本リポジトリは 原則サポートなし です（Issues はバグ報告用のみ）。
This repository is provided as-is, with no official support. Please use Issues for bug reports only.

