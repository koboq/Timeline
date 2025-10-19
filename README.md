# Modigs Timeline (C++)
![license](https://img.shields.io/badge/license-MIT-blue)
![cpp](https://img.shields.io/badge/C%2B%2B-17%2B-informational)
![build](https://img.shields.io/github/actions/workflow/status/<user>/<repo>/ci.yaml?label=CI)


### JP
�V���v���ō����ȃC�x���g�^�C�����C���iC++�j�B  
���t���[�� Update���邾���B�t���[�������E���[�v�ׂ��E�C�ӃW�����v�ɂ������B
�����w��̃C�x���g�o�^�E�Đ��E���[�v�E�O���[�o���^�ʃR�[���o�b�N�ɑΉ��B  
Visual Studio�Ή��EMIT���C�Z���X�B

### EN
A simple and fast event timeline library in C++.  
Lightweight C++ timeline engine for frame-based apps.  
Supports time-based event registration, playback, looping, and both global and per-event callbacks.  
Designed for Visual Studio. Released under the MIT License.

---

## ���� / Features
### JP
- `AddEvent(time, id, params, callback)` �ŃC�x���g�o�^  
- `Play/Pause/Stop/Reset`�A`SetLoop(true/false)`  
- `Update(deltaTimeSec)` �Ŏ��Ԑi�s����ԓ��C�x���g�����S�ɔ���  
- �ʃR�[���o�b�N�D�� �� ������΃O���[�o���R�[���o�b�N  

### EN
- Register events with `AddEvent(time, id, params, callback)`
- Control playback with `Play`, `Pause`, `Stop`, `Reset`, and `SetLoop(true/false)`
- Advance time using `Update(deltaTimeSec)` to safely trigger events within the interval
- Per-event callbacks take priority; if none are set, the global callback is used

---

## �p�r / Use Cases
### JP
- �Q�[�����o�iUI�t�F�[�h�ASE�A�J�b�g�V�[���̊ȈՐ���j
- AV/�c�[���̓����i���ʉ���G�t�F�N�g�̃^�C�~���O���킹�j
- GUI�A�j���[�V�����itooltip/indicator �̎�������j
- IoT/�V�~�����[�V�����̎����C�x���g�Ǘ�

### EN
- Game presentation (UI fades, SFX, simple cutscene control)
- AV/tools synchronization (aligning sound effects and visual effects timing)
- GUI animations (time-based control for tooltips/indicators)
- Timed event management for IoT/simulations

---

## �g�����i�T�v�j / Usage Example
```cpp
#include "timeline/timeline.h"
using namespace Modigs;

Timeline tl;
tl.SetGlobalCallback([](int id, const std::vector<double>& p){ /* ... */ });
tl.AddEvent(0.5,  1, {1.0, 2.0});
tl.AddEvent(1.0, 42, {}, [](int id, const auto&){ /* �ʏ��� / per-event callback */ });
tl.SetLoop(false);
tl.Play();
tl.Update(0.2); // �c�Q�[��/�A�v���̃t���[���ŌĂ� / call this per frame

tl.Pause(); // �c�ꎞ��~�APlay���ĂԂƎ~�߂��Ƃ��납��ĊJ����� / pause and resume
tl.Stop();  // �c��~�APlay���ĂԂƍŏ�����n�܂� / stop and restart from beginning
tl.Reset(); // �c�폜�A�^�C�����C�����폜����� / clear all events
```

---

## �r���h / Build
### JP
- Visual Studio 2022�iC++17 �ȏ�𐄏��j
- timeline.h / timeline.cpp ���v���W�F�N�g�ɒǉ�

### EN
- Visual Studio 2022 (C++17 or later recommended)
- Add timeline.h and timeline.cpp to your project

---

## ���C�Z���X / License
### JP
MIT License - �ڍׂ� LICENSE ���Q�ƁB

### EN
MIT License - see the LICENSE file for details.

---

## �T�|�[�g / Supports
### JP
- �{���|�W�g���� �����T�|�[�g�Ȃ� �ł��iIssues �̓o�O�񍐗p�̂݁j�B
- ���̃��|�W�g���� **�ŏ�API/����^�p** ��ړI�Ƃ��Ă��܂��B
- ����� **GitHub Issues �̂�** �Ŏ󂯕t���܂��i���[��/DM�͕s�j�B
- �o�O�񍐂� **�ŏ��Č��R�[�h** �� **�g�p��** ��Y�t���Ă��������B
- �V�@�\�̒�Ă͊��}�ł����A**�R�A�̓V���v���ێ�**��D�悵�܂��B

### EN
- This repository is provided with no official support (Issues are for bug reports only).
- The project aims for a minimal API and stable operation.
- Questions are accepted via GitHub Issues only (email/DMs are not supported).
- When reporting bugs, please include a minimal reproducible example and your environment details.
- Feature requests are welcome, but keeping the core simple has priority.

---

## FAQ (JP)

### Q1. �Ȃ��C�x���g�̔��΂́u(prev, now] ��ԁv�Ŕ��肷��̂ł����H
�t���[�����Ԃ͏�Ɉ��ł͂���܂���B�d���u�Ԃ� deltaTime ���L�т�ƁA���Ԃ� 1.00 �� 1.05 �̂悤�Ɂu��C�ɐi�ށv���Ƃ�����܂��B  
���̂Ƃ� **1.02�b** �̃C�x���g����肱�ڂ��Ȃ����߂ɁA**���O���� prev ���猻�ݎ��� now �܂ł̋�� (prev, now]** �ɓ������C�x���g�����ׂĎ��s���܂��B  
���l�ɁA**���[�v�I�[�ׂ̌�**�i��F2.9��3.2�b�j���A(2.9, 3.0] �� [0.0, 0.2] ��2��Ԃɕ������Ċm���ɏE���܂��B

### Q2. `SetTime()` �ŔC�ӎ����փW�����v������̋����́H
�����J�[�\�����Čv�Z���A**���� `Update()`** �� (prev, now] �ɓ���C�x���g�����ɔ��΂��܂��B  
�u�W�����v����ɉߋ��̃C�x���g������ɔ��΂���v���Ƃ͂���܂���B

### Q3. ���ꎞ���ɕ����C�x���g������ꍇ�̏����́H
**time �� id** �̏��Ń\�[�g���Ă���A���� time �ł� **id �̏���** �Ŕ��΂��܂��B

### Q4. ���ԁiduration�j�̈����́H
`SetDuration(d)` ���ݒ莞�́A**�ő�C�x���g����**�� duration �Ƃ��č̗p���܂��B  
`SetLoop(true)` �̂Ƃ��́A���� duration ��1���Ƃ��ďz���܂��B

### Q5. ���������_�̌덷�΍�́H
����� **(prev + EPS, now + EPS]** �̂悤�ɏ����� `EPS` ���������čs���܂��B  
�u1.0 ���傤�ǁv�𓙒l�ŏE��Ȃ��̂́A���������ł͌�����v�����҂ł��Ȃ����߂ł��B

### Q6. �X���b�h�Z�[�t�ł����H
**�����_�ł͔�Ή�**�ł��B�P��X���b�h�� `Update()` ����z��ł��B

### Q7. �}���`�g���b�N������t���C�x���g�́H
**v0.2+** �̃��[�h�}�b�v���ڂƂ��Č������ł��B�R�A�̃V���v������ۂ��g�����܂��B

---

## FAQ (EN)
### Q1. Why are events triggered based on the (prev, now] interval?
Frame delta times are not constant. When a frame takes longer, time can jump forward (for example from 1.00 to 1.05).  
To avoid missing an event at 1.02 seconds, we trigger all events whose times fall within the interval (prev, now].  
Similarly, when crossing a loop boundary (for example 2.9 �� 3.2), we split the check into two intervals?(2.9, 3.0] and [0.0, 0.2]?so events are not missed.

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