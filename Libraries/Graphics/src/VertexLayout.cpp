module TR.Graphics.VertexLayout;

import TR.Graphics.Formats;

namespace TR {

	namespace Graphics {

		namespace VertexLayout {

			void CreateDesc(_VertexLayout* vertexLayout)
			{
				auto& [elements, elementDescs, layoutDesc] = *vertexLayout;

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

			void Init(_VertexLayout* vertexLayout, std::vector<_VertexElement> elements)
			{
				vertexLayout->elements = elements;
				CreateDesc(vertexLayout);
			}

		}

	}

}