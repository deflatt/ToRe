export module TR.Graphics.VertexLayout;

import <d3d12.h>;
import <string>;
import <vector>;

export namespace TR {

	namespace Graphics {

		namespace VertexLayout {

			struct _VertexElement {
				std::string name = {};
				DXGI_FORMAT format = {};
			};
			
			struct _VertexLayout {
				std::vector<_VertexElement> elements = {};
				std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};
				D3D12_INPUT_LAYOUT_DESC layoutDesc = {};
			};

			void CreateDesc(_VertexLayout* vertexLayout);
			void Init(_VertexLayout* vertexLayout, std::vector<_VertexElement> elements);

		}
		using namespace VertexLayout;

	}
	using namespace Graphics;

}