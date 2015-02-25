
import aceutils

aceutils.cdToScript()

'''
aceutils.cd(r'../')
aceutils.rm(r'bullet-2.82-r2704.zip')
aceutils.rmdir(r'bullet-2.82-r2704')

aceutils.wget(r'https://bullet.googlecode.com/files/bullet-2.82-r2704.zip')
aceutils.unzip(r'bullet-2.82-r2704.zip')

aceutils.editCmakeForACE(r'bullet-2.82-r2704/CMakeLists.txt','cp932')

aceutils.mkdir(r"bullet_bin")
aceutils.cd(r"bullet_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../bullet-2.82-r2704/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" BULLET_PHYSICS.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" BULLET_PHYSICS.sln /p:configuration=Release')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../bullet-2.82-r2704/')
	aceutils.call(r'make')


'''

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.mkdir(r'Dev/lib/Debug')
	aceutils.mkdir(r'Dev/lib/Release')

	aceutils.mkdir(r'Dev/include/BulletCollision')
	aceutils.mkdir(r'Dev/include/BulletCollision/CollisionDispatch/')
	aceutils.mkdir(r'Dev/include/BulletCollision/BroadphaseCollision/')
	aceutils.mkdir(r'Dev/include/BulletCollision/NarrowPhaseCollision/')

	aceutils.mkdir(r'Dev/include/LinearMath')

	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/CollisionDispatch/btCollisionWorld.h', r'Dev/include/BulletCollision/CollisionDispatch/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/CollisionDispatch/btCollisionObject.h', r'Dev/include/BulletCollision/CollisionDispatch/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.h', r'Dev/include/BulletCollision/CollisionDispatch/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/CollisionDispatch/btCollisionCreateFunc.h', r'Dev/include/BulletCollision/CollisionDispatch/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/CollisionDispatch/btManifoldResult.h', r'Dev/include/BulletCollision/CollisionDispatch/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h', r'Dev/include/BulletCollision/CollisionDispatch/')

	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/BroadphaseCollision/btDispatcher.h', r'Dev/include/BulletCollision/BroadphaseCollision/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h', r'Dev/include/BulletCollision/BroadphaseCollision/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h', r'Dev/include/BulletCollision/BroadphaseCollision/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h', r'Dev/include/BulletCollision/BroadphaseCollision/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h', r'Dev/include/BulletCollision/BroadphaseCollision/')

	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h', r'Dev/include/BulletCollision/NarrowPhaseCollision/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h', r'Dev/include/BulletCollision/NarrowPhaseCollision/')
	aceutils.copy(r'bullet-2.82-r2704/src/BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h', r'Dev/include/BulletCollision/NarrowPhaseCollision/')

	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btVector3.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btAlignedObjectArray.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btTransform.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btScalar.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btMinMax.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btAlignedAllocator.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btMatrix3x3.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btQuaternion.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btQuadWord.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btMotionState.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btAlignedAllocator.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btAlignedObjectArray.h', r'Dev/include/LinearMath/')
	aceutils.copy(r'bullet-2.82-r2704/src/LinearMath/btTransformUtil.h', r'Dev/include/LinearMath/')

	aceutils.copy(r'bullet_bin/lib/Debug/BulletCollision_Debug.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'bullet_bin/lib/Debug/LinearMath_Debug.lib', r'Dev/lib/Debug/')

	aceutils.copy(r'bullet_bin/lib/Release/BulletCollision.lib', r'Dev/lib/Release/')
	aceutils.copy(r'bullet_bin/lib/Release/LinearMath.lib', r'Dev/lib/Release/')

else:
	aceutils.copy(r'Culling3D-master/Culling3D/Culling3D.h', r'Dev/include/')
	aceutils.copy(r'culling3d_bin/libCulling3D.a', r'Dev/lib/')


