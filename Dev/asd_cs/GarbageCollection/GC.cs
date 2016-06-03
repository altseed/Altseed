using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// C++のインスタンスの生成及び取得方法
	/// </summary>
	internal enum GenerationType
	{
		/// <summary>
		/// ファイルパス等からC++のインスタンスが生成された場合
		/// 生成時に使い回しをしている場合、カウンタを増やしているので、
		/// 使い回しと判定した場合、カウンタを減らす
		/// </summary>
		Create = 0,

		/// <summary>
		/// C++のインスタンスを取得した場合、
		/// 新規の場合はC#側でC++のインスタンスを保持するのでカウンタを増やす
		/// </summary>
		Get = 1,
	}

    static class GC
    {
        internal static GarbageCollector Collector { get; private set; }

        internal static IDObjectContainer<SoundSource> SoundSources { get; private set; }

        internal static IDObjectContainer<Cursor> Cursors { get; private set; }

        internal static IDObjectContainer<Texture2D> Texture2Ds { get; private set; }
        internal static IDObjectContainer<CubemapTexture> CubemapTextures { get; private set; }
        internal static IDObjectContainer<Font> Fonts { get; private set; }
        internal static IDObjectContainer<Chip2D> Chip2Ds { get; private set; }

        internal static IDObjectContainer<Shader2D> Shader2Ds { get; private set; }
        internal static IDObjectContainer<Shader3D> Shader3Ds { get; private set; }
        internal static IDObjectContainer<Material2D> Material2Ds { get; private set; }
        internal static IDObjectContainer<Material3D> Material3Ds { get; private set; }
        internal static IDObjectContainer<MaterialPropertyBlock> MaterialPropertyBlocks { get; private set; }

        internal static IDObjectContainer<ImagePackage> ImagePackages { get; private set; }

        internal static IDObjectContainer<Effect> Effects { get; private set; }

        internal static IDObjectContainer<Mesh> Meshs { get; private set; }
        internal static IDObjectContainer<Deformer> Deformers { get; private set; }
        internal static IDObjectContainer<Model> Models { get; private set; }
        internal static IDObjectContainer<MassModel> MassModels { get; private set; }
        internal static IDObjectContainer<Terrain3D> Terrain3Ds { get; private set; }

        internal static IDObjectContainer<KeyframeAnimation> KeyframeAnimations { get; private set; }
        internal static IDObjectContainer<AnimationSource> AnimationSources { get; private set; }
        internal static IDObjectContainer<AnimationClip> AnimationClips { get; private set; }

        internal static IDObjectContainer<Scene> Scenes { get; private set; }

        internal static IDObjectContainer<Layer2D> Layer2Ds { get; private set; }
        internal static IDObjectContainer<Object2D> Object2Ds { get; private set; }

        internal static IDObjectContainer<Shape> Shapes { get; private set; }

        internal static IDObjectContainer<Layer3D> Layer3Ds { get; private set; }
        internal static IDObjectContainer<Object3D> Object3Ds { get; private set; }

        internal static IDObjectContainer<PostEffect> PostEffects { get; private set; }

        internal static IDObjectContainer<Transition> Transitions { get; private set; }

        internal static IDObjectContainer<StaticFile> StaticFiles { get; private set; }
        internal static IDObjectContainer<StreamFile> StreamFiles { get; private set; }

        internal static void Initialize()
        {
            Collector = new GarbageCollector();

            SoundSources = new IDObjectContainer<SoundSource>();

            Cursors = new IDObjectContainer<Cursor>();

            Texture2Ds = new IDObjectContainer<Texture2D>();
            CubemapTextures = new IDObjectContainer<CubemapTexture>();
            Fonts = new IDObjectContainer<Font>();
            Chip2Ds = new IDObjectContainer<Chip2D>();

            Shader2Ds = new IDObjectContainer<Shader2D>();
            Shader3Ds = new IDObjectContainer<Shader3D>();
            Material2Ds = new IDObjectContainer<Material2D>();
            Material3Ds = new IDObjectContainer<Material3D>();
            MaterialPropertyBlocks = new IDObjectContainer<MaterialPropertyBlock>();

            ImagePackages = new IDObjectContainer<ImagePackage>();

            Effects = new IDObjectContainer<Effect>();

            Meshs = new IDObjectContainer<Mesh>();
            Deformers = new IDObjectContainer<Deformer>();
            Models = new IDObjectContainer<Model>();
            MassModels = new IDObjectContainer<MassModel>();
            Terrain3Ds = new IDObjectContainer<Terrain3D>();

            KeyframeAnimations = new IDObjectContainer<KeyframeAnimation>();
            AnimationSources = new IDObjectContainer<AnimationSource>();
            AnimationClips = new IDObjectContainer<AnimationClip>();

            Scenes = new IDObjectContainer<Scene>();

            Layer2Ds = new IDObjectContainer<Layer2D>();
            Object2Ds = new IDObjectContainer<Object2D>();

            Layer3Ds = new IDObjectContainer<Layer3D>();
            Object3Ds = new IDObjectContainer<Object3D>();

            PostEffects = new IDObjectContainer<PostEffect>();

            Transitions = new IDObjectContainer<Transition>();

            StaticFiles = new IDObjectContainer<StaticFile>();
            StreamFiles = new IDObjectContainer<StreamFile>();

            Shapes = new IDObjectContainer<Shape>();
        }

        internal static void Update()
        {
            if (Collector.Collect())
            {
                SoundSources.Collect();

                Cursors.Collect();

                Texture2Ds.Collect();
                CubemapTextures.Collect();
                Fonts.Collect();
                Chip2Ds.Collect();

                Shader2Ds.Collect();
                Shader3Ds.Collect();
                Material2Ds.Collect();
                Material3Ds.Collect();
                MaterialPropertyBlocks.Collect();

                ImagePackages.Collect();

                Effects.Collect();

                Meshs.Collect();
                Deformers.Collect();
                Models.Collect();
                MassModels.Collect();
                Terrain3Ds.Collect();

                KeyframeAnimations.Collect();
                AnimationSources.Collect();
                AnimationClips.Collect();

                Scenes.Collect();

                Layer2Ds.Collect();
                Object2Ds.Collect();

                Layer3Ds.Collect();
                Object3Ds.Collect();

                PostEffects.Collect();

                Transitions.Collect();

                StaticFiles.Collect();
                StreamFiles.Collect();

                Shapes.Collect(); 
            }
        }

        internal static void Terminate()
        {
            for (int loop = 0; loop < 3; loop++)
            {
                SoundSources.DestroyAll();

                Cursors.DestroyAll();

                Texture2Ds.DestroyAll();
                CubemapTextures.DestroyAll();
                Fonts.DestroyAll();
                Chip2Ds.DestroyAll();

                Shader2Ds.DestroyAll();
                Shader3Ds.DestroyAll();
                Material2Ds.DestroyAll();
                Material3Ds.DestroyAll();
                MaterialPropertyBlocks.DestroyAll();

                ImagePackages.DestroyAll();

                Effects.DestroyAll();

                Meshs.DestroyAll();
                Deformers.DestroyAll();
                Models.DestroyAll();
                MassModels.DestroyAll();
                Terrain3Ds.DestroyAll();

                KeyframeAnimations.DestroyAll();
                AnimationSources.DestroyAll();

                Scenes.DestroyAll();

                Layer2Ds.DestroyAll();
                Object2Ds.DestroyAll();

                Layer3Ds.DestroyAll();
                Object3Ds.DestroyAll();

                PostEffects.DestroyAll();
                Transitions.DestroyAll();

                StaticFiles.DestroyAll();
                StreamFiles.DestroyAll();

                Shapes.DestroyAll();

                //Profilers.DestroyAll();
				Collector.Collect();
				Particular.GC.CollectAll();
				Collector.Collect();
            }
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static SoundSource GenerateSoundSource(swig.SoundSource o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.SoundSources.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new SoundSource(o);
            GC.SoundSources.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static Cursor GenerateCursor(swig.Cursor o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Cursors.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Cursor(o);
            GC.Cursors.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static Texture2D GenerateTexture2D(swig.Texture2D o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Texture2Ds.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Texture2D(o);
            GC.Texture2Ds.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static RenderTexture2D GenerateRenderTexture2D(swig.RenderTexture2D o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Texture2Ds.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return (RenderTexture2D)existing;

            var ret = new RenderTexture2D(o);
            GC.Texture2Ds.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static CubemapTexture GenerateCubemapTexture(swig.CubemapTexture o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.CubemapTextures.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new CubemapTexture(o);
            GC.CubemapTextures.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static Effect GenerateEffect(swig.Effect o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Effects.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Effect(o);
            GC.Effects.AddObject(p, ret);
            return ret;
        }

        internal static Shader3D GenerateShader3D(swig.Shader3D o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Shader3Ds.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Shader3D(o);
            GC.Shader3Ds.AddObject(p, ret);
            return ret;
        }

        internal static Material3D GenerateMaterial3D(swig.Material3D o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Material3Ds.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Material3D(o);
            GC.Material3Ds.AddObject(p, ret);
            return ret;
        }

        internal static MaterialPropertyBlock GenerateMaterialPropertyBlock(swig.MaterialPropertyBlock o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.MaterialPropertyBlocks.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new MaterialPropertyBlock(o);
            GC.MaterialPropertyBlocks.AddObject(p, ret);
            return ret;
        }

        internal static ImagePackage GenerateImagePackage(swig.ImagePackage o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.ImagePackages.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new ImagePackage(o);
            GC.ImagePackages.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static Font GenerateFont(swig.Font o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Fonts.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Font(o);
            GC.Fonts.AddObject(p, ret);
            return ret;
        }

        internal static StaticFile GenerateStaticFile(swig.StaticFile o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.StaticFiles.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new StaticFile(o);
            GC.StaticFiles.AddObject(p, ret);
            return ret;
        }

        internal static StreamFile GenerateStreamFile(swig.StreamFile o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.StreamFiles.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new StreamFile(o);
            GC.StreamFiles.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static Mesh GenerateMesh(swig.Mesh o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Meshs.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Mesh(o);
            GC.Meshs.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static Deformer GenerateDeformer(swig.Deformer o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Deformers.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Deformer(o);
            GC.Deformers.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static Model GenerateModel(swig.Model o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Models.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Model(o);
            GC.Models.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static MassModel GenerateMassModel(swig.MassModel o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.MassModels.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new MassModel(o);
            GC.MassModels.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static Terrain3D GenerateTerrain3D(swig.Terrain3D o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.Terrain3Ds.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new Terrain3D(o);
            GC.Terrain3Ds.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static KeyframeAnimation GenerateKeyframeAnimation(swig.KeyframeAnimation o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.KeyframeAnimations.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new KeyframeAnimation(o);
            GC.KeyframeAnimations.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static AnimationSource GenerateAnimationSource(swig.AnimationSource o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.AnimationSources.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new AnimationSource(o);
            GC.AnimationSources.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
        /// </summary>
        /// <param name="o"></param>
        /// <param name="type"></param>
        internal static AnimationClip GenerateAnimationClip(swig.AnimationClip o, GenerationType type)
        {
            if (o == null) return null;
            var p = o.GetPtr();

            var existing = GC.AnimationClips.GetObject(p);
            existing = GenerateInternal(existing, o, type);
            if (existing != null) return existing;

            var ret = new AnimationClip(o);
            GC.AnimationClips.AddObject(p, ret);
            return ret;
        }

        /// <summary>
        /// Generate*メソッドの内部処理
        /// </summary>
        /// <typeparam name="E"></typeparam>
        /// <typeparam name="T"></typeparam>
        /// <param name="existing"></param>
        /// <param name="o"></param>
        /// <param name="type"></param>
        /// <returns></returns>
        static E GenerateInternal<E, T>(E existing, T o, GenerationType type)
            where E : class
            where T : swig.IReference
        {
            if (existing != null)
            {
                if (type == GenerationType.Create)
                {
                    o.Release();
                }

                return existing;
            }

            if (type == GenerationType.Get)
            {
                o.AddRef();
            }

            return null;
        }
    }
}
