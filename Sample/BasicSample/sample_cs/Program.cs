
class Program
{
	static void Main(string[] args)
	{
		ISample[] samples =
			{
				new Basic_Empty(),
				//new Basic_FullScreen(), 

                new TextureObject2D_Basic(),
                new TextureObject2D_Transform(),
                new TextureObject2D_Turn(),
                new TextureObject2D_Src(),

                new Mouse_Position(),
                new Mouse_Click(),
                new Mouse_State(),
                new Mouse_Wheel(),

                new Keyboard_Basic(),
                new Keyboard_SingularKey(),
                new Keyboard_PuralKeys(),

                new Joystick_Basic(),
                new Joystick_Axis(),
                new Joystick_GamePad(),

                new Sound_SE(),
				new Sound_BGM(),
				new Sound_Loop(),

                new TextObject2D_Basic(),
                new TextObject2D_DynamicFont(),
                new TextObject2D_Rotate(),
                new TextObject2D_Filtered(),
				new TextObject2D_Centering(),

                new CameraObject2D_Basic(),
                new CameraObject2D_Magnify(),
				new CameraObject2D_Group(),
				new CameraObject2D_Texture(),

                new EffectObject2D_Basic(),

                new GeometryObject2D_CircleShape(),
                new GeometryObject2D_ArcShape(),
                new GeometryObject2D_RectangleShape(),
                new GeometryObject2D_PolygonShape(),
                new GeometryObject2D_PolygonShape_Textured(),

				new Object2DComponent_Custom(),

                new Collision2D_Basic(),
                new Collision2D_Geometries(),

                new File_RootDirectory(),
                new File_PackFile(),
                new File_StaticFile(),
                new File_StreamFile(),

                new SceneAndLayer_Basic(),
				new SceneAndLayer_CustomLayer(),
				new SceneAndLayer_CustomScene(),

                new Dispose_Object2D(),
                new Dispose_Layer2D(),

				new PostEffect_GrayScale(),
				new PostEffect_Sepia(),
				new PostEffect_GaussianBlur(),
				new PostEffect_LightBloom(),

				new Transition_Fade(),

                new EngineMisc_ShowFps(),

                new Texture_Basic(),
                new Texture_Edit(),

				new Log_Basic(), 
				new Log_OutputLevel(), 
				new Log_Table(), 

				new MediaPlayer_Basic(),
            };

		var browser = new SampleBrowser(samples);
		browser.Run();
	}
}