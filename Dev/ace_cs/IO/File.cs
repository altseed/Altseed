﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public partial class File
    {
        internal File(swig.File swig)
		{
			coreInstance = swig;
		}

        public StaticFile CreateStaticFile(string path)
        {
            var file_ = swig.Accessor.File_CreateStaticFile(coreInstance, path);
            if (file_ == null) return null;
            return GC.GenerateStaticFile(file_, GC.GenerationType.Create);
        }

		public StreamFile CreateStreamFile(string path)
		{
			var file_ = swig.Accessor.File_CreateStreamFile(coreInstance, path);
			if (file_ == null) return null;
			return GC.GenerateStreamFile(file_, GC.GenerationType.Create);
		}
    }
}
