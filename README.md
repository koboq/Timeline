# Modigs Timeline (C++)
![license](https://img.shields.io/badge/license-MIT-blue)
![cpp](https://img.shields.io/badge/C%2B%2B-17%2B-informational)
![build](https://img.shields.io/github/actions/workflow/status/<user>/<repo>/ci.yaml?label=CI)


### JP
シンプルで高速なイベントタイムライン（C++）。  
毎フレーム Updateするだけ。フレーム落ち・ループ跨ぎ・任意ジャンプにも強い。
時刻指定のイベント登録・再生・ループ・グローバル／個別コールバックに対応。  
Visual Studio対応・MITライセンス。

### EN
A simple and fast event timeline library in C++.  
Lightweight C++ timeline engine for frame-based apps.  
Supports time-based event registration, playback, looping, and both global and per-event callbacks.  
Designed for Visual Studio. Released under the MIT License.

---

## 特徴 / Features
### JP
- `AddEvent(time, id, params, callback)` でイベント登録  
- `Play/Pause/Stop/Reset`、`SetLoop(true/false)`  
- `Update(deltaTimeSec)` で時間進行＆区間内イベントを安全に発火  
- 個別コールバック優先 → 無ければグローバルコールバック  

### EN
- Register events with `AddEvent(time, id, params, callback)`
- Control playback with `Play`, `Pause`, `Stop`, `Reset`, and `SetLoop(true/false)`
- Advance time using `Update(deltaTimeSec)` to safely trigger events within the interval
- Per-event callbacks take priority; if none are set, the global callback is used

---

## 用途 / Use Cases
### JP
- ゲーム演出（UIフェード、SE、カットシーンの簡易制御）
- AV/ツールの同期（効果音やエフェクトのタイミング合わせ）
- GUIアニメーション（tooltip/indicator の時限制御）
- IoT/シミュレーションの時限イベント管理

### EN
- Game presentation (UI fades, SFX, simple cutscene control)
- AV/tools synchronization (aligning sound effects and visual effects timing)
- GUI animations (time-based control for tooltips/indicators)
- Timed event management for IoT/simulations

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
```

---

## ビルド / Build
### JP
- Visual Studio 2022（C++17 以上を推奨）
- timeline.h / timeline.cpp をプロジェクトに追加

### EN
- Visual Studio 2022 (C++17 or later recommended)
- Add timeline.h and timeline.cpp to your project

---

## ライセンス / License
### JP
MIT License - 詳細は LICENSE を参照。

### EN
MIT License - see the LICENSE file for details.

---

## サポート / Supports
### JP
- 本リポジトリは 原則サポートなし です（Issues はバグ報告用のみ）。
- このリポジトリは **最小API/安定運用** を目的としています。
- 質問は **GitHub Issues のみ** で受け付けます（メール/DMは不可）。
- バグ報告は **最小再現コード** と **使用環境** を添付してください。
- 新機能の提案は歓迎ですが、**コアはシンプル維持**を優先します。

### EN
- This repository is provided with no official support (Issues are for bug reports only).
- The project aims for a minimal API and stable operation.
- Questions are accepted via GitHub Issues only (email/DMs are not supported).
- When reporting bugs, please include a minimal reproducible example and your environment details.
- Feature requests are welcome, but keeping the core simple has priority.

---

## FAQ (JP)

### Q1. なぜイベントの発火は「(prev, now] 区間」で判定するのですか？
フレーム時間は常に一定ではありません。重い瞬間に deltaTime が伸びると、時間が 1.00 → 1.05 のように「一気に進む」ことがあります。  
このとき **1.02秒** のイベントを取りこぼさないために、**直前時刻 prev から現在時刻 now までの区間 (prev, now]** に入ったイベントをすべて実行します。  
同様に、**ループ終端の跨ぎ**（例：2.9→3.2秒）も、(2.9, 3.0] と [0.0, 0.2] の2区間に分割して確実に拾います。

### Q2. `SetTime()` で任意時刻へジャンプした後の挙動は？
内部カーソルを再計算し、**次の `Update()`** で (prev, now] に入るイベントを順に発火します。  
「ジャンプ直後に過去のイベントが勝手に発火する」ことはありません。

### Q3. 同一時刻に複数イベントがある場合の順序は？
**time → id** の順でソートしており、同一 time では **id の昇順** で発火します。

### Q4. 期間（duration）の扱いは？
`SetDuration(d)` 未設定時は、**最大イベント時刻**を duration として採用します。  
`SetLoop(true)` のときは、この duration を1周として循環します。

### Q5. 浮動小数点の誤差対策は？
判定は **(prev + EPS, now + EPS]** のように小さな `EPS` を加味して行います。  
「1.0 ちょうど」を等値で拾わないのは、浮動小数では厳密一致が期待できないためです。

### Q6. スレッドセーフですか？
**現時点では非対応**です。単一スレッドで `Update()` する想定です。

### Q7. マルチトラックや条件付きイベントは？
**v0.2+** のロードマップ項目として検討中です。コアのシンプルさを保ちつつ拡張します。

---

## FAQ (EN)
### Q1. Why are events triggered based on the (prev, now] interval?
Frame delta times are not constant. When a frame takes longer, time can jump forward (for example from 1.00 to 1.05).  
To avoid missing an event at 1.02 seconds, we trigger all events whose times fall within the interval (prev, now].  
Similarly, when crossing a loop boundary (for example 2.9 → 3.2), we split the check into two intervals?(2.9, 3.0] and [0.0, 0.2]?so events are not missed.

### Q2. What happens after jumping to an arbitrary time with `SetTime()`?
We recalculate the internal cursor and then, on the next `Update()`, trigger events that fall into (prev, now] in order.  
Events from the past will not be fired immediately upon jumping.

### Q3. What is the order when multiple events share the same time?
Events are sorted by time, then by id. For the same time value, events are fired in ascending id order.

### Q4. How is duration handled?
If `SetDuration(d)` is not called, the duration defaults to the maximum event time.  
When `SetLoop(true)` is set, the timeline cycles over that duration.

### Q5. How do you handle floating-point precision issues?
Time comparisons include a small `EPS` offset, e.g. (prev + EPS, now + EPS], to mitigate floating-point inaccuracies.  
We avoid relying on exact equality (e.g. exactly 1.0) because floating-point values are not guaranteed to match exactly.

### Q6. Is the timeline thread-safe?
Not at this time. The timeline is designed to be used from a single thread, calling `Update()` there.

### Q7. What about multi-track or conditional events?
These features are under consideration for the v0.2+ roadmap. We aim to extend functionality while keeping the core simple.