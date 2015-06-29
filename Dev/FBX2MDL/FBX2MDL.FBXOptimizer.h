
#include "FBX2MDL.Base.h"

namespace FBX2MDL
{
	class FBXOptimizer
	{
	private:
		bool HasEnabledBone(std::map<asd::astring, int32_t>& boneCounts, std::vector<std::shared_ptr<Node>>& children);
		void RemoveDisabledBone(std::map<asd::astring, int32_t>& boneCounts, std::shared_ptr<Node>& bone);

		void OptimizeArmature(std::shared_ptr<Scene> scene, std::shared_ptr<Node> armature, std::vector<std::shared_ptr<Node>> meshes);

	public:
		void Optimize(std::shared_ptr<Scene> scene);
	};
}