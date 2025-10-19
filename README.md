# Modigs Timeline (C++)

�V���v���ō����ȃC�x���g�^�C�����C���iC++�j�B  
�����w��̃C�x���g�o�^�E�Đ��E���[�v�E�O���[�o���^�ʃR�[���o�b�N�ɑΉ��B  
Visual Studio�Ή��EMIT���C�Z���X�B

A simple and fast event timeline library in C++.  
Supports time-based event registration, playback, looping, and both global and per-event callbacks.  
Designed for Visual Studio. Released under the MIT License.

---

## ���� / Features
- `AddEvent(time, id, params, callback)` �ŃC�x���g�o�^  
  Register events with `AddEvent(time, id, params, callback)`
- `Play/Pause/Stop/Reset`�A`SetLoop(true/false)`  
  Control playback with `Play`, `Pause`, `Stop`, `Reset`, and `SetLoop(true/false)`
- `Update(deltaTimeSec)` �Ŏ��Ԑi�s����ԓ��C�x���g�����S�ɔ���  
  Advance time using `Update(deltaTimeSec)` to safely trigger events within the interval
- �ʃR�[���o�b�N�D�� �� ������΃O���[�o���R�[���o�b�N  
  Per-event callbacks take priority; if none are set, the global callback is used

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



## ����� / Environment
- Visual Studio 2022�iC++17 �ȏ�𐄏��j
  Visual Studio 2022 (C++17 or later recommended)
- timeline.h / timeline.cpp ���v���W�F�N�g�ɒǉ�
  Add timeline.h and timeline.cpp to your project

## ���C�Z���X / License
MIT License - �ڍׂ� LICENSE ���Q�ƁB
MIT License - see the LICENSE file for details.

## ���̑� / Notes
- �{���|�W�g���� �����T�|�[�g�Ȃ� �ł��iIssues �̓o�O�񍐗p�̂݁j�B
This repository is provided as-is, with no official support. Please use Issues for bug reports only.

