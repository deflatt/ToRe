module;

#include "GraphicsHeader.h"

module TR.Graphics.VertexLayout;

import TR.Graphics.Formats;

namespace TR::Graphics {

	namespace VertexLayout {

		namespace Element {

			void Init(_Context* context, std::string name, DXGI_FORMAT format)
			{
				context->name = name;
				context->format = format;
			}

		}

		void Init(_Context* context, std::vector<Element::_Context> elements)
		{
			context->elements = elements;
		}

		void CreateDesc(_Context* context)
		{
			auto& [elements, elementDescs, layoutDesc] = *context;

			layoutDesc.NumElements = (UINT)elements.size();
			elementDescs.resize(elements.size());

			UINT bytes = 0;
			for (UINT i = 0; i < elements.size(); i++) {
				elementDescs[i] = {
					&elements[i].name[0], 0, elements[i].format, 0, bytes
						, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
				};
				bytes += (UINT)BitsPerPixel(elements[i].format) / 8U;
			}

			layoutDesc.pInputElementDescs = &elementDescs[0];
		}

	}

	void _VertexLayout::Init(std::vector<VertexLayout::Element::_Context> elements)
	{
		VertexLayout::Init(&context, elements);
	}

	void _VertexLayout::CreateDesc()
	{
		VertexLayout::CreateDesc(&context);
	}

	VertexLayout::_Context* _VertexLayout::GetContext() noexcept
	{
		return &context;
	}



}