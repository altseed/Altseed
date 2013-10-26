
#pragma once

#include <ace.common.Base.h>

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Graphics/ace.Color.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

#include "../../../ace.ReferenceObject.h"

namespace ace
{
	class Mesh
		: public ReferenceObject
	{
	private:

		struct Vertex
		{
			Vector3DF	Position;
			Vector3DF	Normal;
			Vector3DF	Binormal;
			Vector3DF	Tangent;
			Vector2DF	UV1;
			Vector2DF	UV2;
			Color		VColor;
			int32_t		BoneIndexes;
			int32_t		BoneWeights;
		};

		struct Face
		{
			int32_t		Index1;
			int32_t		Index2;
			int32_t		Index3;
		};

		struct MaterialOffset
		{
			int32_t		MaterialIndex;
			int32_t		FaceOffset;
		};

		Graphics_Imp*	m_graphics;

		std::shared_ptr<VertexBuffer_Imp>	m_vertexBuffer;
		std::shared_ptr<IndexBuffer_Imp>	m_indexBuffer;

		std::vector<Vertex>	m_vertexBufferOnMM;
		std::vector<Face>	m_faceBufferOnMM;

		std::vector<MaterialOffset>	m_materialOffsets;

	public:

		Mesh(Graphics* graphics);
		virtual ~Mesh();

		void AddVertex(
			const Vector3DF& position,
			const Vector3DF& normal,
			const Vector3DF& binormal,
			const Vector3DF& tangent,
			const Vector2DF& uv1,
			const Vector2DF& uv2,
			const Color& color,
			int32_t boneIndexes,
			int32_t boneWeights);

		void AddFace(int32_t index1, int32_t index2, int32_t index3);

		void AddMaterialOffset(int32_t materialIndex, int32_t faceOffset);
		void SendToGPUMemory();

		std::shared_ptr<VertexBuffer_Imp>& GetVertexBuffer() { return m_vertexBuffer; }
		std::shared_ptr<IndexBuffer_Imp>& GetIndexBuffer() { return m_indexBuffer; }

	};
};