module;

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <d3d12.h>
#include <dxcapi.h>
#include <filesystem>
#include <fstream>
#include <atlbase.h>
#include <atlconv.h>
#include <codecvt>

module TR.Graphics.Shader;

import TR.Graphics.Definitions;

namespace TR {

	namespace Graphics {

		namespace Shader {

			void LoadFile(std::string path, std::string& output)
			{
				size_t size = std::filesystem::file_size(path);
				std::ifstream ifile(path, std::ios::binary);
				output.resize(size);
				ifile.read(&output[0], size);
				ifile.close();
			}

			void LoadSource(std::string path, std::string& output, Proc<std::string&> sourceHandler)
			{
				LoadFile(path, output);
				if (sourceHandler != nullptr)
					sourceHandler(output);
			}

			void LoadRaw(_Shader* shader, std::string path)
			{
				LoadFile(path, shader->data);
			}

			struct _IncludeHandler : public IDxcIncludeHandler {

				IDxcUtils* utils = nullptr;
				Proc<std::string&>* sourceHandler = nullptr;

				HRESULT QueryInterface(REFIID riid, void** ppvObject) override {
					return 0;
				}
				ULONG AddRef() override {
					return 0;
				}
				ULONG Release() override {
					return 0;
				}

				HRESULT LoadSource(LPCWSTR path, IDxcBlob** outBlob) override
				{
					std::string srcPath = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(path);
					if (!std::filesystem::is_regular_file(std::filesystem::path(srcPath))) {
						return -1;
					}
					std::string content = {};
					Shader::LoadSource(srcPath, content, *sourceHandler);

					HRESULT ret = utils->CreateBlob(&content[0], (UINT)content.size(), CP_UTF8, (IDxcBlobEncoding**)outBlob);
					if (ret != 0) {
						return -1;
					}
					return 0;
				}

			};

			void Compile(_Shader* shader, std::string path, _Compiler compiler)
			{
				HRESULT ret;

				std::filesystem::path oldPath = std::filesystem::current_path();
				std::filesystem::current_path(std::filesystem::path(path).parent_path());

				path = std::filesystem::path(path).filename().string();

				ComPtr<IDxcUtils> utils = {};
				ret = DxcCreateInstance(CLSID_DxcUtils, __uuidof(IDxcUtils), &utils);
				if (ret != 0) {
					throw _FailedUtilsCreationException(ret);
				}

				ComPtr<IDxcCompiler3> dxCompiler = {};
				ret = DxcCreateInstance(CLSID_DxcCompiler, __uuidof(IDxcCompiler3), &dxCompiler);
				if (ret != 0) {
					throw _FailedCompilerCreationException(ret);
				}

				std::wstring wPath = std::wstring(CA2W(path.c_str()));
				std::wstring wEntryPoint = std::wstring(CA2W(compiler.entryPoint.c_str()));
				std::wstring wProfile = std::wstring(CA2W(compiler.profile.c_str()));

				std::vector<std::pair<std::wstring, std::wstring>> wDefinitions(compiler.definitions.size());
				std::vector<DxcDefine> defines(compiler.definitions.size());

				for (UINT i = 0; i < compiler.definitions.size(); i++) {
					wDefinitions[i] = { std::wstring(CA2W(compiler.definitions[i].name.c_str()))
						, std::wstring(CA2W(compiler.definitions[i].value.c_str())) };
					defines[i] = { wDefinitions[i].first.c_str(), wDefinitions[i].second.c_str() };
				}

				ComPtr<IDxcCompilerArgs> args = {};
				ret = utils->BuildArguments(wPath.c_str(), wEntryPoint.c_str(), wProfile.c_str(), nullptr, 0, &defines[0], (UINT)defines.size(), &args);
				if (ret != 0) {
					throw _FailedArgumentCreationException(ret);
				}

				std::string source = {};
				LoadSource(path, source, compiler.sourceHandler);
				DxcBuffer sourceBuffer = {};
				sourceBuffer.Encoding = CP_UTF8;
				sourceBuffer.Ptr = &source[0];
				sourceBuffer.Size = source.size();

				_IncludeHandler includeHandler = {};
				includeHandler.utils = utils.Get();
				includeHandler.sourceHandler = &compiler.sourceHandler;

				ComPtr<IDxcResult> result = {};
				ret = dxCompiler->Compile(&sourceBuffer, args->GetArguments(), args->GetCount(), &includeHandler, __uuidof(IDxcResult), &result);

				if (result->HasOutput(DXC_OUT_ERRORS)) {
					std::string errors = {};
					ComPtr<IDxcBlobUtf8> errorBlob = {};
					HRESULT errorRet = result->GetOutput(DXC_OUT_ERRORS, __uuidof(IDxcBlobUtf8), &errorBlob, nullptr);
					if (errorRet == 0)
						errors = std::string(errorBlob->GetStringPointer(), errorBlob->GetStringLength());
					if (errors.size())
						throw _FailedCompilationException(ret, errors);
				}

				ComPtr<IDxcBlob> resultBlob = {};
				ret = result->GetOutput(DXC_OUT_OBJECT, __uuidof(IDxcBlob), &resultBlob, nullptr);
				if (ret != 0) {
					throw _FailedCompilationException(ret);
				}

				shader->data.resize(resultBlob->GetBufferSize());
				memcpy(&shader->data[0], resultBlob->GetBufferPointer(), resultBlob->GetBufferSize());

				std::filesystem::current_path(oldPath);
			}

			D3D12_SHADER_BYTECODE GetBytecode(_Shader* shader)
			{
				if (shader->data.size())
					return { &shader->data[0], shader->data.size() };
				else
					return {};
			}

		}

	}

}