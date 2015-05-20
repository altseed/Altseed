
#include "FBX2MDL.FBXExporter.h"

namespace FBX2MDL
{
	void FBXExporter::WriteHeader()
	{
		m_writer->Push((uint8_t)'M');
		m_writer->Push((uint8_t)'D');
		m_writer->Push((uint8_t)'L');
		m_writer->Push((uint8_t) 0);

		m_writer->Push(1);
	}

	void FBXExporter::WriteAnimationSource(AnimationSource& source)
	{
		m_writer->Push(source.Name);
		m_writer->Push((int32_t) source.keyFrameAnimations.size());

		for (auto& anim : source.keyFrameAnimations)
		{
			m_writer->Push(anim.TargetName);
			m_writer->Push((int32_t)anim.KeyFrames.size());

			for (auto& keyframe : anim.KeyFrames)
			{
				m_writer->Push(keyframe.KeyValue);
				m_writer->Push(keyframe.LeftPosition);
				m_writer->Push(keyframe.RightPosition);
				m_writer->Push(keyframe.Interpolation);
			}
		}
	}

	void FBXExporter::WriteAnimationSources(std::vector<AnimationSource>& animationSources)
	{
		m_writer->Push((int32_t) animationSources.size());
		for (auto& animationSource : animationSources)
		{
			WriteAnimationSource(animationSource);
		}
	}

	void FBXExporter::WriteMaterial(const Material& material)
	{
		int32_t external = 0;
		m_writer->Push(external);
		m_writer->Push(material.DiffuseTexturePath);
		m_writer->Push(ace::ToAString("").c_str());
		m_writer->Push(ace::ToAString("").c_str());
	}

	void FBXExporter::WriteMaterials(const std::vector<Material>& materials)
	{
		int32_t count = (int32_t)materials.size();
		m_writer->Push(count);

		for (const auto& material : materials)
		{
			WriteMaterial(material);
		}
	}

	void FBXExporter::WriteMesh(const ace::Model_IO::DividedMesh& mesh)
	{
		auto vcount = (int32_t)mesh.Vertices.size();
		m_writer->Push(vcount);
		for (const auto& v : mesh.Vertices)
		{
			m_writer->Push(v.Position);
			m_writer->Push(v.Normal);
			m_writer->Push(v.Binormal);
			m_writer->Push(v.UV1);
			m_writer->Push(v.UV2);
			m_writer->Push(v.VColor.R);
			m_writer->Push(v.VColor.G);
			m_writer->Push(v.VColor.B);
			m_writer->Push(v.VColor.A);
			
			for (const auto& w : v.BoneWeights)
			{
				m_writer->Push(w);
			}
			
			for (const auto& w : v.BoneIndexes)
			{
				m_writer->Push(w);
			}

			for (const auto& w : v.BoneIndexesOriginal)
			{
				m_writer->Push(w);
			}
		}

		auto fcount = (int32_t) mesh.Faces.size();
		m_writer->Push(fcount);

		for (const auto& f : mesh.Faces)
		{
			m_writer->Push(f.Indexes[0]);
			m_writer->Push(f.Indexes[1]);
			m_writer->Push(f.Indexes[2]);
		}
	}


	std::vector<std::shared_ptr<Mesh>> FBXExporter::GetMeshes(const std::shared_ptr<Node>& node)
	{
		std::vector<std::shared_ptr<Mesh>> dst;
		if (node->MeshParameter != nullptr)
		{
			dst.push_back(node->MeshParameter);
		}

		for (const auto& node_ : node->Children)
		{
			auto dst_ = GetMeshes(node_);
			dst.insert(dst.end(), dst_.begin(), dst_.end());
		}

		return dst;
	}

	std::vector<std::function<void()>> FBXExporter::GetWritingDeformerFuncs(const std::shared_ptr<Node>& node, int32_t parentIndex, int32_t& currentIndex)
	{
		std::vector<std::function<void()>> dst;

		auto func = [this, node, parentIndex]()-> void
		{
			m_writer->Push(node->Name);
			m_writer->Push(parentIndex);
			m_writer->Push((int32_t) node->RotationOrder);
			m_writer->Push(node->LclMatrix);
		};

		dst.push_back(func);

		auto current = currentIndex;
		for (const auto& node_ : node->Children)
		{
			currentIndex++;
			auto dst_ = GetWritingDeformerFuncs(node_, current, currentIndex);
			dst.insert(dst.end(), dst_.begin(), dst_.end());
		}

		return dst;
	}

	std::map<ace::astring, int32_t> FBXExporter::GetDeformerNameToIndexes(const std::shared_ptr<Node>& node, int32_t& currentIndex)
	{
		std::map<ace::astring, int32_t> dst;

		dst[node->Name] = currentIndex;

		auto current = currentIndex;
		for (const auto& node_ : node->Children)
		{
			currentIndex++;
			auto dst_ = GetDeformerNameToIndexes(node_, currentIndex);
			dst.insert(dst_.begin(), dst_.end());
		}

		return dst;
	}

	FBXExporter::FBXExporter()
	{
	}

	FBXExporter::~FBXExporter()
	{
	}


	std::shared_ptr<ace::BinaryWriter> FBXExporter::Export(std::shared_ptr<Scene> scene)
	{
		auto writer = std::make_shared<ace::BinaryWriter>();
		m_writer = writer;

		WriteHeader();

		// デフォーマー出力
		{
			int32_t currentIndex = 0;
			auto deformerFuncs = GetWritingDeformerFuncs(scene->Root, -1, currentIndex);
			m_writer->Push((int32_t)deformerFuncs.size());
			for (auto& func : deformerFuncs)
			{
				func();
			}
		}

		// デフォーマー情報取得
		std::map<ace::astring, int32_t> deformer_name2ind;
		{
			int32_t currentIndex = 0;
			deformer_name2ind = GetDeformerNameToIndexes(scene->Root, currentIndex);
		}

		// メッシュ編集
		auto meshes = GetMeshes(scene->Root);
		m_writer->Push((int32_t) meshes.size());
		for (auto& mesh : meshes)
		{
			// ボーンが設定されていない場合はメッシュをボーンとして指定
			if (mesh->BoneConnectors.size() == 0)
			{
				BoneConnector bc;
				bc.Name = mesh->Name;
				mesh->BoneConnectors.push_back(bc);
			}

			// 上下方向に面ソート(将来的にonoffつける)
#if 1
			std::sort(mesh->Faces.begin(), mesh->Faces.end(), [mesh](const Face& f1, const Face& f2) -> float {
				return
					(mesh->Vertexes[f1.Index[0]].Position.Y +
					mesh->Vertexes[f1.Index[1]].Position.Y +
					mesh->Vertexes[f1.Index[2]].Position.Y) < 
					(mesh->Vertexes[f2.Index[0]].Position.Y +
					mesh->Vertexes[f2.Index[1]].Position.Y +
					mesh->Vertexes[f2.Index[2]].Position.Y);
			});
#endif

			// 材質ソート
			std::sort(mesh->Faces.begin(), mesh->Faces.end(), [](const Face& f1, const Face& f2) -> int32_t { return f1.MaterialIndex > f2.MaterialIndex; });

			// 不要マテリアル削除
			{
				std::vector<int32_t> materialCounts;
				std::vector<int32_t> newIndexes;

				materialCounts.resize(mesh->Materials.size());
				newIndexes.resize(mesh->Materials.size());

				for (auto& f : mesh->Faces)
				{
					materialCounts[f.MaterialIndex]++;
				}

				auto nind = 0;
				for (size_t i = 0; i < materialCounts.size(); i++)
				{
					if (materialCounts[i] > 0)
					{
						if (mesh->Materials[i].Name == ace::astring())
						{
							mesh->Materials[i].Name = ace::ToAString("Noname");
						}

						newIndexes[i] = nind;
						nind++;
					}
					else
					{
						mesh->Materials[i].Name = ace::astring();
					}
				}

				for (auto& f : mesh->Faces)
				{
					f.MaterialIndex = newIndexes[f.MaterialIndex];
				}

				{
					auto it = std::remove_if(mesh->Materials.begin(), mesh->Materials.end(), 
						[](Material mat)->bool { 
						return mat.Name == ace::astring(); 
					});

					mesh->Materials.erase(it, mesh->Materials.end());
				}

			}

			// ボーン数が一定ごとになるように分割
			std::vector<ace::Model_IO::DividedMesh> dividedMeshes;

			{
				dividedMeshes.push_back(ace::Model_IO::DividedMesh());
				std::set<int32_t> connectors;
				std::set<int32_t> newConnectors;
				std::map<int32_t, int32_t> oldV2newV;
				std::map<int32_t, int32_t> oldC2newC;

				int32_t materialIndex = -1;
				int32_t materialCount = 0;

				auto finishMesh = [&dividedMeshes, &connectors, &oldV2newV, &oldC2newC,&materialIndex,&materialCount]() -> void
				{
					auto& m = dividedMeshes[dividedMeshes.size() - 1];
					for (auto& v : m.Vertices)
					{
						for (auto w = 0; w < 4; w++)
						{
							v.BoneIndexes[w] = oldC2newC[v.BoneIndexesOriginal[w]];
						}
					}

					connectors.clear();
					oldV2newV.clear();
					oldC2newC.clear();

					if (materialCount > 0)
					{
						ace::Model_IO::MaterialOffset mo;
						mo.MaterialIndex = materialIndex;
						mo.FaceOffset = materialCount;
						m.MaterialOffsets.push_back(mo);
						materialCount = 0;
					}
				};

				for (auto& face : mesh->Faces)
				{
					newConnectors.clear();

					for (auto fp = 0; fp < 3; fp++)
					{
						auto v = mesh->Vertexes[face.Index[fp]];
						for (auto w = 0; w < 4; w++)
						{
							if (v.BoneWeights[w] == 0.0f) continue;
							if (connectors.count(v.BoneIndexesOriginal[w]) == 0)
							{
								newConnectors.insert(v.BoneIndexesOriginal[w]);
							}
						}
					}

					if (connectors.size() + newConnectors.size() > 32)
					{
						// 新規メッシュを生成する際に登録済みのボーンの情報も別途必要なため
						for (auto fp = 0; fp < 3; fp++)
						{
							auto v = mesh->Vertexes[face.Index[fp]];
							for (auto w = 0; w < 4; w++)
							{
								if (v.BoneWeights[w] == 0.0f) continue;
								newConnectors.insert(v.BoneIndexesOriginal[w]);
							}
						}

						finishMesh();

						// 新しいメッシュを生成
						dividedMeshes.push_back(ace::Model_IO::DividedMesh());
						dividedMeshes[dividedMeshes.size() - 1];
					}
					ace::Model_IO::DividedMesh& dmesh = dividedMeshes[dividedMeshes.size() - 1];

					for (auto it = newConnectors.begin(); it != newConnectors.end(); it++)
					{
						connectors.insert(*it);
					}

					auto face_ = ace::Model_IO::Face();
					face_.Indexes[0] = face.Index[0];
					face_.Indexes[1] = face.Index[1];
					face_.Indexes[2] = face.Index[2];

					for (auto fp = 0; fp < 3; fp++)
					{
						auto ind = face.Index[fp];
						if (oldV2newV.count(ind) == 0)
						{
							auto v = mesh->Vertexes[ind];
							dmesh.Vertices.push_back(v);
							oldV2newV[ind] = dmesh.Vertices.size() - 1;
						}

						face_.Indexes[fp] = oldV2newV[ind];
					}

					dmesh.Faces.push_back(face_);

					if (materialIndex == face.MaterialIndex)
					{
						materialCount++;
					}
					else
					{
						if (materialCount > 0)
						{
							ace::Model_IO::MaterialOffset mo;
							mo.MaterialIndex = materialIndex;
							mo.FaceOffset = materialCount;
							dmesh.MaterialOffsets.push_back(mo);
							materialCount = 0;
						}

						materialCount++;
						materialIndex = face.MaterialIndex;
					}

					for (auto& newC : newConnectors)
					{
						ace::Model_IO::BoneConnector c;
						c.TargetIndex = deformer_name2ind[mesh->BoneConnectors[newC].Name];
						c.OffsetMatrix = mesh->BoneConnectors[newC].OffsetMatrix;
						dmesh.BoneConnectors.push_back(c);
						oldC2newC[newC] = dmesh.BoneConnectors.size() - 1;
					}
				}

				finishMesh();

				if ((dividedMeshes.end() - 1)->Vertices.size() == 0)
				{
					dividedMeshes.resize(dividedMeshes.size() - 1);
				}
			}
			

			// 分割済
			m_writer->Push((int32_t)dividedMeshes.size());
			for (auto& dmesh : dividedMeshes)
			{
				// メッシュ情報出力
				WriteMesh(dmesh);

				// 材質面出力
				
				m_writer->Push((int32_t) dmesh.MaterialOffsets.size());
				for (auto& mf : dmesh.MaterialOffsets)
				{
					m_writer->Push(mf.MaterialIndex);
					m_writer->Push(mf.FaceOffset);
				}
				

				// ボーン情報出力
				m_writer->Push((int32_t) dmesh.BoneConnectors.size());
				for (auto& bone : dmesh.BoneConnectors)
				{
					m_writer->Push((int32_t) bone.TargetIndex);
					m_writer->Push(bone.OffsetMatrix);
				}
			}

			// 材質出力
			WriteMaterials(mesh->Materials);
		}

		// アニメーション出力
		std::remove_if(
			scene->AnimationSources.begin(), 
			scene->AnimationSources.end(),
			[](const AnimationSource& source) {
			return source.keyFrameAnimations.size() == 0;
		});

		// アニメーションソース出力
		WriteAnimationSources(scene->AnimationSources);

		// アニメーションクリップ出力
		{
			m_writer->Push((int32_t) scene->AnimationSources.size());
			for (auto i = 0; i < scene->AnimationSources.size(); i++)
			{
				m_writer->Push(scene->AnimationSources[i].Name);
				m_writer->Push(i);
			}
		}
		m_writer.reset();
		return writer;
	}
}