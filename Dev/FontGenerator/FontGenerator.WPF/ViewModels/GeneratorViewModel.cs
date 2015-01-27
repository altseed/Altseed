using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FontGenerator.Model;
using System.ComponentModel.DataAnnotations;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization;
using System.Reactive.Linq;
using System.Reactive;
using System.Windows.Media.Imaging;

namespace FontGenerator.WPF.ViewModels
{
	class GeneratorViewModel : INotifyPropertyChanged, IDataErrorInfo
	{
		public event PropertyChangedEventHandler PropertyChanged;

		#region Properties
		private GenerationConfig config;

		public GenerationConfig Config
		{
			get { return config; }
			set
			{
				config = value;
				PropertyChanged.Raise(this, () => FontIndex);
				PropertyChanged.Raise(this, () => TextPath);
				PropertyChanged.Raise(this, () => ExportPath);
				PropertyChanged.Raise(this, () => SheetName);
				PropertyChanged.Raise(this, () => FontSize);
				PropertyChanged.Raise(this, () => TextureSize);

				PropertyChanged.Raise(this, () => OutlineSize);
				PropertyChanged.Raise(this, () => OutlineSampling);

				PropertyChanged.Raise(this, () => Red);
				PropertyChanged.Raise(this, () => Green);
				PropertyChanged.Raise(this, () => Blue);
				PropertyChanged.Raise(this, () => Alpha);
				PropertyChanged.Raise(this, () => ColorString);

				PropertyChanged.Raise(this, () => OutlineRed);
				PropertyChanged.Raise(this, () => OutlineGreen);
				PropertyChanged.Raise(this, () => OutlineBlue);
				PropertyChanged.Raise(this, () => OutlineAlpha);
				PropertyChanged.Raise(this, () => OutlineColorString);
			}
		}


		[Required]
		public int FontIndex
		{
			get { return config.FontIndex; }
			set
			{
				if(!value.Equals(config.FontIndex))
				{
					config.FontIndex = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public string TextPath
		{
			get { return config.TextPath; }
			set
			{
				if(!value.Equals(config.TextPath))
				{
					config.TextPath = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public string ExportPath
		{
			get { return config.ExportPath; }
			set
			{
				if(!value.Equals(config.ExportPath))
				{
					config.ExportPath = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public string SheetName
		{
			get { return config.SheetName; }
			set
			{
				if(!value.Equals(config.SheetName))
				{
					config.SheetName = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public int FontSize
		{
			get { return config.FontSize; }
			set
			{
				if(!value.Equals(config.FontSize))
				{
					config.FontSize = value;
					PropertyChanged.Raise(this);
					GeneratePreviewAsync();
				}
			}
		}

		[Required]
		public int TextureSize
		{
			get { return config.TextureSize; }
			set
			{
				if(!value.Equals(config.TextureSize))
				{
					config.TextureSize = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public int OutlineSize
		{
			get { return config.OutlineSize; }
			set
			{
				if(!value.Equals(config.OutlineSize))
				{
					config.OutlineSize = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		[Required]
		public int OutlineSampling
		{
			get { return config.OutlineSampling; }
			set
			{
				if(!value.Equals(config.OutlineSampling))
				{
					config.OutlineSampling = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		public byte Red
		{
			get { return config.FontColor.Red; }
			set
			{
				if(!value.Equals(config.FontColor.Red))
				{
					config.FontColor.Red = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => ColorString);
				}
			}
		}

		public byte Green
		{
			get { return config.FontColor.Green; }
			set
			{
				if(!value.Equals(config.FontColor.Green))
				{
					config.FontColor.Green = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => ColorString);
				}
			}
		}

		public byte Blue
		{
			get { return config.FontColor.Blue; }
			set
			{
				if(!value.Equals(config.FontColor.Blue))
				{
					config.FontColor.Blue = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => ColorString);
				}
			}
		}

		public byte Alpha
		{
			get { return config.FontColor.Alpha; }
			set
			{
				if(!value.Equals(config.FontColor.Alpha))
				{
					config.FontColor.Alpha = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => ColorString);
				}
			}
		}

		public string ColorString
		{
			get { return string.Format("#{0:X2}{1:X2}{2:X2}{3:X2}", Alpha, Red, Green, Blue); }
		}


		public byte OutlineRed
		{
			get { return config.OutlineColor.Red; }
			set
			{
				if(!value.Equals(config.OutlineColor.Red))
				{
					config.OutlineColor.Red = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => OutlineColorString);
				}
			}
		}

		public byte OutlineGreen
		{
			get { return config.OutlineColor.Green; }
			set
			{
				if(!value.Equals(config.OutlineColor.Green))
				{
					config.OutlineColor.Green = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => OutlineColorString);
				}
			}
		}

		public byte OutlineBlue
		{
			get { return config.OutlineColor.Blue; }
			set
			{
				if(!value.Equals(config.OutlineColor.Blue))
				{
					config.OutlineColor.Blue = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => OutlineColorString);
				}
			}
		}

		public byte OutlineAlpha
		{
			get { return config.OutlineColor.Alpha; }
			set
			{
				if(!value.Equals(config.OutlineColor.Alpha))
				{
					config.OutlineColor.Alpha = value;
					PropertyChanged.Raise(this);
					PropertyChanged.Raise(this, () => OutlineColorString);
				}
			}
		}

		public string OutlineColorString
		{
			get { return string.Format("#{0:X2}{1:X2}{2:X2}{3:X2}", OutlineAlpha, OutlineRed, OutlineGreen, OutlineBlue); }
		}

		BitmapImage PreviewImage_;

		public BitmapImage PreviewImage
		{
			get { return PreviewImage_; }
			set
			{
				if(!value.Equals(PreviewImage_))
				{
					PreviewImage_ = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		private string statusString_;

		public string StatusString
		{
			get { return statusString_; }
			set
			{
				if(!value.Equals(statusString_))
				{
					statusString_ = value;
					PropertyChanged.Raise(this);
				}
			}
		}

		private bool canGenerate_;

		public bool CanGenerate
		{
			get { return canGenerate_; }
			set
			{
				if(!value.Equals(canGenerate_))
				{
					canGenerate_ = value;
					PropertyChanged.Raise(this);
				}
			}
		}


		#endregion

		public string Error
		{
			get { throw new NotImplementedException(); }
		}

		public string this[string columnName]
		{
			get
			{
				var errors = new List<ValidationResult>();
				var validation = Validator.TryValidateProperty(
					GetType().GetProperty(columnName).GetValue(this, null),
					new ValidationContext(this) { MemberName = columnName },
					errors);

				return validation ? null : errors.FirstOrDefault().ErrorMessage;
			}
		}

		public DelegateCommand OpenFontFileCommand { get; private set; }
		public DelegateCommand OpenTextFileCommand { get; private set; }
		public DelegateCommand OpenExportPathCommand { get; private set; }
		public DelegateCommand SaveConfigurationCommand { get; private set; }
		public DelegateCommand LoadConfigurationCommand { get; private set; }

		public GeneratorViewModel()
		{
			config = new GenerationConfig();

			Red = 255;
			Green = 255;
			Blue = 255;
			Alpha = 255;

			FontSize = 14;
			TextureSize = 1024;
			OpenFontFileCommand = new DelegateCommand { CommandHandler = OpenFontFile };
			OpenTextFileCommand = new DelegateCommand { CommandHandler = OpenTextFile };
			OpenExportPathCommand = new DelegateCommand { CommandHandler = OpenExportPath };
			SaveConfigurationCommand = new DelegateCommand { CommandHandler = SaveConfiguration };
			LoadConfigurationCommand = new DelegateCommand { CommandHandler = LoadConfiguration };

			OutlineAlpha = 255;
			OutlineSize = 0;
			OutlineSampling = 1;

			Observable.FromEventPattern<PropertyChangedEventArgs>(this, "PropertyChanged")
				.Where(x => x.EventArgs.PropertyName != "PreviewImage")
				.Throttle(TimeSpan.FromMilliseconds(500))
				.Subscribe(x => GeneratePreviewAsync());

			CanGenerate = true;
		}

		private void LoadConfiguration(object obj)
		{
			var dialog = new OpenFileDialog()
			{
				Filter = "Aceフォント設定ファイル|*.afcfg",
			};

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				try
				{
					Config = ConfigurationFile.Load(dialog.FileName);
				}
				catch (SerializationException)
				{
					MessageBox.Show("選択されたファイルは設定ファイルではないか、壊れています。", "ロード中止", MessageBoxButtons.OK);
				}
			}
		}

		private void SaveConfiguration(object obj)
		{
			var dialog = new SaveFileDialog()
			{
				DefaultExt = "afcfg",
				FileName = "FontSetting",
				Filter = "Aceフォント設定ファイル|*.afcfg",
			};

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				ConfigurationFile.Save(config, dialog.FileName);
			}
		}

		private void OpenExportPath(object obj)
		{
			var dialog = new FolderBrowserDialog();

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				ExportPath = dialog.SelectedPath;
			}
		}

		private void OpenTextFile(object obj)
		{
			var dialog = new OpenFileDialog()
			{
				Filter = "テキスト ファイル(*.txt)|*.txt|すべてのファイル(*.*)|*.*",
			};

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				TextPath = dialog.FileName;
			}
		}

		private void OpenFontFile(object obj)
		{
			var dialog = new FontDialog();

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				var fontDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Fonts);
				//FontPath = Path.Combine(fontDirectory, );
			}
		}

		public async Task GenerateAsync()
		{
			CanGenerate = false;
			StatusString = "フォントを生成中…";

			try
			{
				await Generator.GenerateAsync(config);
			}
			catch (Exception)
			{
				CanGenerate = true;
				StatusString = "生成エラー";
				throw;
			}

			CanGenerate = true;
			StatusString = "生成完了";
		}

		public async Task GeneratePreviewAsync()
		{
			var imagePath = await Generator.GeneratePreviewAsync(config);

			var image = new BitmapImage();
			using (var file = new FileStream(imagePath, FileMode.Open, FileAccess.Read))
			{
				image.BeginInit();
				image.CacheOption = BitmapCacheOption.OnLoad;
				image.StreamSource = file;
				image.EndInit();
				image.Freeze();
			}

			PreviewImage = image;

			File.Delete(imagePath);
		}
	}
}
