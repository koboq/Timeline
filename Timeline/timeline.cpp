
// MIT License
// Copyright (c) 2025 こぼ
//
// See the LICENSE file in the project root for full license information.

#include "timeline.h"

#include <stdexcept>
#include <utility>
#include <algorithm>
#include <cmath>

namespace Modigs {

	/// <summary>
	/// 指定時刻・ID・パラメータ・オプションのコールバックを持つイベントを追加し、
	/// ソートおよびインデックスを再構築します。
	/// </summary>
	/// <param name="timeSec">イベントが発火する時刻（秒）</param>
	/// <param name="eventId">イベント識別子</param>
	/// <param name="params">イベントに渡す任意パラメータ</param>
	/// <param name="cb">個別コールバック（nullptr 可）</param>
	void Timeline::AddEvent(double timeSec, int eventId, std::vector<double> params, Callback cb) {
		events_.push_back(Event{ timeSec, eventId, std::move(params), std::move(cb) });
		sortEvents_();
		rebuildIndex_();
	}



	/// <summary>
	/// 個別コールバックなしでイベントを追加します（AddEvent の委譲版）。
	/// </summary>
	/// <param name="timeSec">イベントが発火する時刻（秒）</param>
	/// <param name="eventId">イベント識別子</param>
	/// <param name="params">イベントに渡す任意パラメータ</param>
	void Timeline::AddEvent(double timeSec, int eventId, std::vector<double> params) {
		AddEvent(timeSec,eventId, std::move(params), nullptr);
	}



	/// <summary>
	/// パラメータ・コールバック無しでイベントを追加します（AddEvent の委譲版）。
	/// </summary>
	/// <param name="timeSec">イベントが発火する時刻（秒）</param>
	/// <param name="eventId">イベント識別子</param>
	void Timeline::AddEvent(double timeSec, int eventId) {
		AddEvent(timeSec,eventId, {}, nullptr);
	}



	/// <summary>
	/// 登録されているイベント数を返します。
	/// </summary>
	/// <returns>イベント数（int）</returns>
	int Timeline::GetEventCount() const {
		return events_.size();
	}



	/// <summary>
	/// 全イベントを削除し、時刻とカーソルをリセットします。
	/// </summary>
	void Timeline::Clear() {
		events_.clear();
		currentTime_ = 0.0;
		cursor_ = 0;
	}



	/// <summary>
	/// 再生を開始します（isPlaying_ を true に設定）。
	/// Update の呼び出しでイベント発火が行われます。
	/// </summary>
	void Timeline::Play() {
		isPlaying_ = true;
	}



	/// <summary>
	/// 再生を一時停止します（isPlaying_ を false に設定）。
	/// </summary>
	void Timeline::Pause() {
		isPlaying_ = false;
	}



	/// <summary>
	/// 再生停止と時刻リセットを行います（Pause と Reset を実行）。
	/// </summary>
	void Timeline::Stop() {
		Pause();
		Reset();
	}



	/// <summary>
	/// 現在時刻を 0.0 に戻し、イベントインデックスを再構築します。
	/// </summary>
	void Timeline::Reset() {
		currentTime_ = 0.0;
		rebuildIndex_();
	}



	/// <summary>
	/// ループ有効/無効を設定します。
	/// </summary>
	/// <param name="enable">true でループ有効、false で無効</param>
	void Timeline::SetLoop(bool enable) {
		isLoop_ = enable;
	}



	/// <summary>
	/// 再生中かどうかを返します。
	/// </summary>
	/// <returns>true: 再生中, false: 停止中</returns>
	bool Timeline::IsPlaying() const {
		return isPlaying_;
	}



	/// <summary>
	/// ループが有効かどうかを返します。
	/// </summary>
	/// <returns>true: ループ有効, false: 無効</returns>
	bool Timeline::IsLoop() const {
		return isLoop_;
	}


	/// <summary>
	/// タイムラインを進め、進行区間内のイベントを発火します。
	/// - isPlaying_ が false、events_ が空、または deltaTimeSec <= 0 の場合は何もしません。
	/// - duration がほぼ 0 の場合はループ処理を行わず一度だけ dispatch します（無限ループ防止）。
	/// - 非ループ時は [prev, min(next, dur)] を発火（開始を包含）。
	/// - ループ時は複数周にまたがる発火を分割して処理します。
	/// </summary>
	/// <param name="deltaTimeSec">進める秒数（正の値）</param>
	void Timeline::Update(double deltaTimeSec) {

		if (!isPlaying_ || events_.empty()) {
			return;
		}

		if (deltaTimeSec <= 0.0) {
			return;
		}

		double prev = currentTime_;
		double next = currentTime_ + deltaTimeSec;
		const double dur = effectiveDuration_();

		if (dur < 0.0) {
			currentTime_ = next;
			return;
		}

		if (!isLoop_) {

			// 非ループ：終端を超えた分も含めて [prev, min(next, dur)] を発火（開始を包含）
			const double end = (next > dur) ? dur : next;
			if (end > prev - EPS) {
				dispatchInRange_(prev, end);
			}
			currentTime_ = (next > dur) ? dur : next;
			// 終端に到達しても停止はしない（必要なら外側でPause/Stop）

		} else {

			// ループ：何周跨いでもOK
			double remaining = deltaTimeSec;
			double localPrev = prev;
		
			while (remaining > EPS) {

				double localEnd = localPrev + remaining;

				if (localEnd < dur + EPS) {

					// ループ境界を跨がない
					dispatchInRange_(localPrev, localEnd);
					if (dur < EPS) {
						currentTime_ = localEnd;
						cursor_ = 0; // 周回したので先頭から
					} else {
						currentTime_ = (localEnd >= dur) ? std::fmod(localEnd, dur) : localEnd;
					}
					remaining = 0.0;

				} else {

					// 境界を跨ぐ：(localPrev, dur] と [0, localEnd - dur] に分割
					dispatchInRange_(localPrev, dur);
					if (dur < EPS) {
						// durationが0の場合は無限ループになるので抜ける
						remaining = 0.0;
					} else { 
						remaining = localEnd - dur;
					}
					localPrev = 0.0;
					cursor_ = 0; // 周回したので先頭から
				}
			}
		}
	}



	/// <summary>
	/// 現在時刻を任意の値に設定し、インデックスを再構築します。
	/// </summary>
	/// <param name="tSec">設定する時刻（秒）</param>
	void Timeline::SetCurrentTime(double tSec) {
		currentTime_ = tSec;// (tSec < 0.0) ? 0.0 : tSec;
		rebuildIndex_();
	}



	/// <summary>
	/// 現在時刻を取得します。
	/// </summary>
	/// <returns>現在時刻（秒）</returns>
	double Timeline::GetCurrentTime() const {
	    return currentTime_;
	}



	/// <summary>
	/// タイムラインの明示的な長さを設定します。正の値を設定すると effectiveDuration_ に反映されます。
	/// </summary>
	/// <param name="durationSec">長さ（秒）</param>
	void Timeline::SetDuration(double durationSec) {
		duration_ = durationSec;
	}



	/// <summary>
	/// 有効なタイムライン長を返します。duration_ が正ならそれを返し、そうでなければ最後のイベント時刻を返します。
	/// </summary>
	/// <returns>有効な長さ（秒）</returns>
	double Timeline::GetDuration() const {
		return effectiveDuration_();
	}



	/// <summary>
	/// グローバルコールバック（個別コールバックが未設定のイベント時に使用）を設定します。
	/// </summary>
	/// <param name="cb">コールバック関数</param>
	void Timeline::SetGlobalCallback(Callback cb) {
		globalCallback_ = cb;
	}

	//******************************************************************
	//
	//  以下、プライベートメソッド
	//
	//******************************************************************

	/// <summary>
	/// イベント配列を時刻順（同時刻は ID 昇順）でソートします。
	/// </summary>
	void Timeline::sortEvents_() {
		std::sort(events_.begin(), events_.end(),
				[](const Event& a, const Event& b) {
				    if (a.time == b.time) {
						return a.id < b.id;
					}
				    return a.time < b.time;
				});
	}



	/// <summary>
	/// currentTime_ より小さいイベントをスキップし、currentTime_ と等しいイベントは残す位置にカーソルを合わせます。
	/// </summary>
	void Timeline::rebuildIndex_() {
		cursor_ = 0;
	    while (cursor_ < static_cast<int>(events_.size()) &&
	           (events_[cursor_].time < currentTime_ - EPS)) {
	        ++cursor_;
	    }
	}


	/// <summary>
	/// duration_ が正ならそれを返し、そうでなければ登録済みイベントの最大時刻を返します。
	/// </summary>
	/// <returns>有効なタイムライン長（秒）</returns>
	double Timeline::effectiveDuration_() const {

		if (duration_ > 0.0) {
			return duration_;
		}

		if (events_.empty()) {
			return 0.0;
		}

		return events_.back().time;
	}



	/// <summary>
	/// 指定区間 [startInclusive, endInclusive] にあるイベントを順次コールします。
	/// 個別コールバックがあればそれを優先して呼び、なければグローバルコールバックを使用します。
	/// </summary>
	/// <param name="startInclusive">開始時刻（包含）</param>
	/// <param name="endInclusive">終了時刻（包含）</param>
	void Timeline::dispatchInRange_(double startInclusive, double endInclusive) {

		// [startInclusive, endInclusive] にあるイベントを順次コール
		while (cursor_ < static_cast<int>(events_.size())) {

			const auto& e = events_[cursor_];

			// 開始時刻より小さいものは既に過ぎているのでスキップ
			if (e.time < startInclusive - EPS) {
			    ++cursor_;
			    continue;
			}

			// 終了時刻より大きければ処理終了（残りは未来）
			if (e.time > endInclusive + EPS) {
			    break;
			}

			// 範囲内のイベントをコール（個別コールバック優先）
			if (e.callback) {
				e.callback(e.id, e.params);
			} else if (globalCallback_) {
				globalCallback_(e.id, e.params);
			}

			++cursor_;
		}
	}

}  // namespace Modigs
