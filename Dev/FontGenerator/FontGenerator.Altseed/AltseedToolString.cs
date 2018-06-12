using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive;
using System.Reactive.Subjects;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	class AltseedToolString
	{
		private sbyte[] bytes_;

		private Subject<Unit> OnValueChangedSubject { get; set; }

		public IEnumerable<sbyte> Bytes
		{
			get { return bytes_; }
			set
			{
				var input = value.ToArray();
				for (int i = 0; i < bytes_.Length; i++)
				{
					bytes_[i] = i < input.Length ? input[i] : (sbyte)0;
				}
				OnValueChangedSubject.OnNext(Unit.Default);
			}
		}
		public string String
		{
			get
			{
				var array = Bytes.Select(x => (byte)x)
					.TakeWhile(x => x != 0)
					.ToArray();
				return Encoding.UTF8.GetString(array);
			}
			set { Bytes = Encoding.UTF8.GetBytes(value).Select(x => (sbyte)x).ToArray(); }
		}
		public IObservable<Unit> OnValueChanged => OnValueChangedSubject;

		public AltseedToolString(int size)
		{
			bytes_ = new sbyte[size];
			OnValueChangedSubject = new Subject<Unit>();
		}
	}
}
