module;

#include "GraphicsHeader.h"

export module TR.Graphics.VertexLayout;

export namespace TR::Graphics {

	namespace VertexLayout {

		namespace Element {

			struct _Context {
				std::string name = {};
				DXGI_FORMAT format = {};
			};

			void Init(_Context* element, std::string name, DXGI_FORMAT format);

		}

		struct _Context {
			std::vector<Element::_Context> elements = {};
			std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};
			D3D12_INPUT_LAYOUT_DESC layoutDesc = {};
		};

		void Init(_Context* vertexLayout, std::vector<Element::_Context> elements);

		void CreateDesc(_Context* vertexLayout);

	}

	struct _VertexLayout {

		VertexLayout::_Context vertexLayout = {};

		void Init(std::vector<VertexLayout::Element::_Context> elements);
		void CreateDesc();

	};

}