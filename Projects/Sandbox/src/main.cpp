#include <iostream>
#include <Windows.h>
#include <fstream>
#undef CreateWindow

import TR.Windows.Window;
import TR.Windows.KeyboardListener;
import TR.Essentials.Procedure;
import TR.Graphics.WinGraphics;
import TR.Graphics.ConstantResource;
import TR.Graphics.ArrayResource;
import TR.Graphics.Shader;
import TR.Graphics.Renderer;
import TR.Graphics.VertexBuffer;
import TR.Graphics.RWArrayResource;
import TR.Windows.MouseListener;
import TR.Windows.RawMouseListener;
import TR.Windows.RawKeyboardListener;
import TR.Media.FileDecoder;
import TR.Media.Encoder;

using namespace TR;

int main() {

	try {

		Media::Encoder::_Context encoder = {};
		Media::Encoder::Init(&encoder, AV_CODEC_ID_H264, AV_PIX_FMT_YUV420P, { 1280, 720 }, 60, 10'000'000);

		std::ofstream ofile("out.mp4", std::ios::binary);
		Procedure<const byte*, int> packetHandler = [&ofile](const byte* data, int length) {
			ofile.write((const char*)data, length);
		};
		Media::Frame::_Context frame = {};
		Media::Frame::Allocate(&frame, { 1280, 720 }, AV_PIX_FMT_RGBA);
		for (int y = 0; y < 720; y++) {
			for (int x = 0; x < 1280; x++) {
				frame.data[(y * 1280 + x) * 4] = 0;
				frame.data[(y * 1280 + x) * 4 + 1] = 255;
				frame.data[(y * 1280 + x) * 4 + 2] = 255;
				frame.data[(y * 1280 + x) * 4 + 3] = 255;
			}
		}

		for (int i = 0; i < 240; i++)
			Media::Encoder::EncodeFrame(&encoder, &frame, packetHandler);
		Media::Encoder::Flush(&encoder, packetHandler);
	
	}
	catch (Graphics::_D3D12Exception e) {
		std::cout << e.msg << " ret: " << (void*)e.returnValue << " code: " << (void*)e.d3d12ErrorCode << std::endl;
	}
	catch (_Exception e) {
		std::cout << e.msg << std::endl;
	}

	return 0;
}