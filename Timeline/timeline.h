
// MIT License
// Copyright (c) 2025 こぼ
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace Modigs {

class Timeline {
public:

	using Callback = std::function<void(int /*eventId*/, const std::vector<double>& /*params*/)>;

	struct Event {
		double time;
		int id;
		std::vector<double> params;
		Callback callback;
	};

	void AddEvent(double timeSec, int eventId, std::vector<double> params, Callback cb);
	void AddEvent(double timeSec, int eventId, std::vector<double> params);
	void AddEvent(double timeSec, int eventId);
	int GetEventCount() const;
	void Clear();

	void Play();
	void Pause();
	void Stop();
	void Reset();
	void SetLoop(bool enable);
	bool IsPlaying() const;
	bool IsLoop()    const;

	void Update(double deltaTimeSec);
	void SetCurrentTime(double tSec);
	double GetCurrentTime() const;

	void SetDuration(double durationSec);
	double GetDuration() const;

	void SetGlobalCallback(Callback cb);

void AddEvent(std::string label, std::int64_t timestamp);
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] const Event& at(std::size_t index) const;



private:

	void sortEvents_();
	void rebuildIndex_();
	double effectiveDuration_() const;
	void dispatchInRange_(double startExclusive, double endInclusive);

	std::vector<Event> events_;
	Callback globalCallback_{};

	bool isPlaying_ = false;
	bool isLoop_ = false;
	double currentTime_ = 0.0;
	double duration_ = 0.0;
	int cursor_ = 0;

	static constexpr double EPS = 1e-9;
};

}  // namespace timeline
