module;

#include "GraphicsHeader.h"

export module TR.Graphics.Shader;

export import TR.Graphics.Device;
export import TR.Essentials.Procedure;

export namespace TR::Graphics {

	namespace Shader {

		struct _CompileException : public _D3D12Exception {
			std::string output;
			_CompileException(std::string msg = {}, HRESULT returnValue = 0, std::string output = "")
				: _D3D12Exception(msg, returnValue), output(output) {}
		};
		struct E_FailedIncludeLoad : public _D3D12Exception {
			std::string file = {};
			E_FailedIncludeLoad(HRESULT returnValue, std::string file = {}) : _D3D12Exception("Failed to load shader include file.", returnValue), file(file) {}
		};
		struct E_FailedUtilsCreation : public _D3D12Exception {
			E_FailedUtilsCreation(HRESULT returnValue) : _D3D12Exception("Failed to create compiler utilities.", returnValue) {}
		};
		struct E_FailedCompilerCreation : public _D3D12Exception {
			E_FailedCompilerCreation(HRESULT returnValue) : _D3D12Exception("Failed to create compiler.", returnValue) {}
		};
		struct E_FailedArgumentCreation : public _D3D12Exception {
			E_FailedArgumentCreation(HRESULT returnValue) : _D3D12Exception("Failed to create compiler arguments.", returnValue) {}
		};
		struct E_FailedCompilation : public _D3D12Exception {
			std::string errors = {};
			E_FailedCompilation(HRESULT returnValue, std::string errors = {}) : _D3D12Exception("Failed to compile shader.", returnValue), errors(errors) {}
		};

		struct _Context {
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
			Procedure<std::string&> sourceHandler = {};
		};

		void LoadRaw(_Context* shader, std::string path);
		void Compile(_Context* shader, std::string path, _Compiler compiler);

		_NODISCARD D3D12_SHADER_BYTECODE GetBytecode(_Context* context);

	}

	struct _Shader {

		Shader::_Context shader = {};

		void LoadRaw(std::string path);
		void Compile(std::string path);

		Shader::_Compiler compiler = {};

		operator D3D12_SHADER_BYTECODE() noexcept;

	};

	namespace Shader {

		struct _Set {
			D3D12_SHADER_BYTECODE vertex = {};
			D3D12_SHADER_BYTECODE pixel = {};
			D3D12_SHADER_BYTECODE domain = {};
			D3D12_SHADER_BYTECODE hull = {};
			D3D12_SHADER_BYTECODE geometry = {};
		};

	}

}