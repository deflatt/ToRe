#include <iostream>
#include <Windows.h>
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

using namespace TR;

int main() {

	try {

		Media::FileDecoder::_Context fileDecoder = {};
		Media::FileDecoder::Init(&fileDecoder, "pic.png");
		Media::Frame::_Context frame = {};
		Media::FileDecoder::DecodeFrame(&fileDecoder, &frame, true);
		std::cout << frame.size.ToString() << std::endl;
		for (int i = 0; i < 4; i++)
			std::cout << (int)frame.data[i] << " ";

	
	}
	catch (Graphics::_D3D12Exception e) {
		std::cout << e.msg << " ret: " << (void*)e.returnValue << " code: " << (void*)e.d3d12ErrorCode << std::endl;
	}
	catch (_Exception e) {
		std::cout << e.msg << std::endl;
	}

	return 0;
}