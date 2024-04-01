export module TR.Graphics.Shader;

import <string>;
import <vector>;
import <d3d12.h>;
import TR.Essentials.Proc;
import TR.Graphics.Exception;

export namespace TR {

	namespace Graphics {

		namespace Shader {

			struct _Shader {
				std::string data = {};
			};
			struct _Definition {
				std::string name = {};
				std::string value = {};
			};
			struct _Compiler {
				std::string profile = {};
				std::string entryPoint = "main";
				std::vector<_Definition> definitions = {};
				Proc<std::string&> sourceHandler = {};
			};

			struct _CompileException : public _D3D12Exception {
				std::string output;
				_CompileException(std::string msg = {}, HRESULT returnValue = 0, std::string output = "")
					: _D3D12Exception(msg, returnValue), output(output) {}
			};
			struct _FailedIncludeLoadException : public _D3D12Exception {
				std::string file = {};
				_FailedIncludeLoadException(HRESULT returnValue, std::string file = {}) : _D3D12Exception("Failed to load shader include file.", returnValue), file(file) {}
			};
			struct _FailedUtilsCreationException : public _D3D12Exception {
				_FailedUtilsCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create compiler utilities.", returnValue) {}
			};
			struct _FailedCompilerCreationException : public _D3D12Exception {
				_FailedCompilerCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create compiler.", returnValue) {}
			};
			struct _FailedArgumentCreationException : public _D3D12Exception {
				_FailedArgumentCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create compiler arguments.", returnValue) {}
			};
			struct _FailedCompilationException : public _D3D12Exception {
				std::string errors = {};
				_FailedCompilationException(HRESULT returnValue, std::string errors = {}) : _D3D12Exception("Failed to compile shader.", returnValue), errors(errors) {}
			};

			void LoadRaw(_Shader* shader, std::string path);
			void Compile(_Shader* shader, std::string path, _Compiler compiler);

			D3D12_SHADER_BYTECODE GetBytecode(_Shader* shader);

			struct _ShaderSet {
				_Shader vertex = {};
				_Shader pixel = {};
				_Shader domain = {};
				_Shader hull = {};
				_Shader geometry = {};
			};

		}
		using namespace Shader;

	}
	using namespace Graphics;

}