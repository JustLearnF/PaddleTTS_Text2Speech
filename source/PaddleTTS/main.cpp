// Copyright (c) 2023 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <paddle_inference_api.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <Tools.hpp>
#include "Predictor.hpp"

using namespace paddle_infer;

#define acoustic_model "./models/fastspeech2_csmsc"
#define vocoder "./models/pwgan_csmsc"
#define output_wav "./output/tts.wav"
#define wav_bit_depth "16"
#define wav_sample_rate "24000"
#define cpu_thread "4"

int main(int argc, char *argv[]) {
	PredictorInterface *predictor;
	if (wav_bit_depth == "16") {
		predictor = new TPredictor<int16_t>();
	} else if (wav_bit_depth == "32") {
		predictor = new TPredictor<float>();
	} else {
		return -1;
	}
	/////////////////////////// 后端：音素转音频 ///////////////////////////

	// WAV采样率（必须与模型输出匹配）
	// 如果播放速度和音调异常，请修改采样率
	// 常见采样率：16000, 24000, 32000, 44100, 48000, 96000
	const uint32_t wavSampleRate = std::stoul(wav_sample_rate);

	// CPU线程数
	const int cpuThreadNum = std::stol(cpu_thread);
	std::cout << "Instance TPredictor sucessfully" << std::endl;
	if (!predictor->Init(acoustic_model,
						 vocoder,
						 cpuThreadNum,
						 wavSampleRate)) {
		std::cout << "Init models failed!" << std::endl;
		return -1;
	}
	std::cout << "init predictor sucessfully" << std::endl;
	//从二进制文件中读取phoneids
	std::vector<int> phoneids;
	phoneids = readData2Phoneids();
	if (phoneids.empty()) {
		std::cout << "Cant read phoneids file,please check!" << std::endl;
	}
	std::vector<int64_t> phones(phoneids.size());
	std::transform(phoneids.begin(), phoneids.end(), phones.begin(), [](int x) {
		return static_cast<int64_t>(x);
	});

	std::cout << phones.size()<<std::endl;


	if (!predictor->RunModel(phones)) {
		return -1;
	}

	if (!predictor->WriteWavToFile(output_wav)) {
		return -1;
	}

	delete predictor;

	return 0;
}
