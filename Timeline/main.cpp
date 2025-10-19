
// MIT License
// Copyright (c) 2025 ����
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
	// �f�t�H���g�̃R�[���o�b�N�ݒ�
	//-------------------------------------------

	tl.SetGlobalCallback([](int id, const std::vector<double>& params) {
		// �C�x���gID�ƃp�����[�^���o�͂���
		std::cout << "[GLOBAL] id=" << id << " params=[";
		for (size_t i = 0; i < params.size(); ++i) {
			std::cout << std::fixed << std::setprecision(2) << params[i];
			if (i + 1 < params.size()) std::cout << ", ";
		}
		std::cout << "]\n";
	});
	


	//-------------------------------------------
	// �C�x���g�o�^
	// �����̃p�^�[��������
	//-------------------------------------------

	// Case 1. 
	//    ���Ύ���: 1.0�b
	//    �֐��w��: �O���[�o���R�[���o�b�N
	//    ����:     �L
	tl.AddEvent(1.0, 100, {1.0});


	// Case 2.
	//    ���Ύ���: 2.0�b
	//    �֐��w��: �ʃR�[���o�b�N
	//    ����:     �L
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
	//    ���Ύ���: 4.5�b
	//    �֐��w��: �O���[�o���R�[���o�b�N
	//    ����:     ��
	tl.AddEvent(4.5, 200);


	// Case 4.
	//    ���Ύ���: 3.2�b
	//    �֐��w��: �ʃR�[���o�b�N
	//    ����:     ��
	// �� �O�̎��Ԃ��w�肵�Ă������Ń\�[�g�����
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
	// �^�C�����C���̊���
	// ���w��Ȃ�ő�C�x���g����=4.5�b
	//-------------------------------------------
	//tl.SetDuration(4.0);



	//-------------------------------------------
	// ���[�v�w��
	//-------------------------------------------
	tl.SetLoop(true);// 3.0�b��؂�Ń��[�v



	//-------------------------------------------
	// �Đ��J�n
	//-------------------------------------------
	tl.Play();



	//-------------------------------------------
	// �Đ�
	// 0.1 x 70 = 7�b���s���� 
	//-------------------------------------------
	for (int i = 0; i < 70; ++i) {
	    tl.Update(0.1);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}