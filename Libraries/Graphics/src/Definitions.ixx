export module TR.Graphics.Definitions;

export import <d3d12.h>;
export import <d3dx12.h>;
export import <wrl.h>;

export namespace TR {

	namespace Graphics {

		template <typename Type>
		using ComPtr = Microsoft::WRL::ComPtr<Type>;

	}
	using namespace Graphics;

}