using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Droid.Business.Implementations
{
    public class FilesManager : IFilesManager
    {
        public async Task SaveFileAsync(List<byte> content)
        {
            var isWriteable = Android.OS.Environment.MediaMounted.Equals(Android.OS.Environment.ExternalStorageState);
            if (!isWriteable)
            {
                throw new InvalidOperationException("External storage is unavailable");
            }

            var path = Path.Combine(
                Android.OS.Environment.GetExternalStoragePublicDirectory(Android.OS.Environment.DirectoryDocuments).AbsolutePath,
                "Firmware.bin");

            using (var writer = File.Create(path))
            {
                await writer.WriteAsync(content.ToArray());
            }
        }
    }
}