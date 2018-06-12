using FontGenerator.Altseed;
using Reactive.Bindings;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive;
using System.Reactive.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	class ConfigViewModel
	{
		public static readonly int TextSize = 2048;
		
		public ReactiveProperty<string> FontName = new ReactiveProperty<string>("");
		public AltseedToolString TextPath = new AltseedToolString(TextSize);
		public AltseedToolString ExportPath = new AltseedToolString(TextSize);
		public AltseedToolString SheetName = new AltseedToolString(TextSize);
		public ReactiveProperty<int> FontSize = new ReactiveProperty<int>(14);
		public ReactiveProperty<int> TextureSize = new ReactiveProperty<int>(256);
		public ReactiveProperty<int> OutlineSize = new ReactiveProperty<int>(0);
		public ReactiveProperty<int> OutlineSampling = new ReactiveProperty<int>(1);
		public ReactiveProperty<asd.Color> FontColor = new ReactiveProperty<asd.Color>(new asd.Color(255, 255, 255, 255));
		public ReactiveProperty<asd.Color> OutlineColor = new ReactiveProperty<asd.Color>(new asd.Color(0, 0, 0, 255));
		public IObservable<Unit> OnNeedToUpdatePreview { get; private set; }

		public ConfigViewModel()
		{
			OnNeedToUpdatePreview = FontName.Select(x => Unit.Default)
				.MergeUnit(FontSize)
				.MergeUnit(TextureSize)
				.MergeUnit(OutlineSize)
				.MergeUnit(OutlineSampling)
				.MergeUnit(FontColor)
				.MergeUnit(OutlineColor);
		}
	}
}
