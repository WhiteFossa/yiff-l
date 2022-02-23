using Android;
using Android.Content.PM;
using AndroidX.Core.App;
using AndroidX.Core.Content;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using Xamarin.Essentials;

namespace org.whitefossa.yiffhl.Droid.Business.Implementations
{
    public class FilesManager : IFilesManager
    {
        private const string FoxDirectory = "Yiff HL";

        public async Task<string> GetFirmwareDirectoryPath()
        {
            return Path.Combine
                (
                    Android.OS.Environment.GetExternalStoragePublicDirectory(Android.OS.Environment.DirectoryDownloads).AbsolutePath,
                    FoxDirectory
                );
        }

        public async Task SaveFileAsync(List<byte> content)
        {
            var isWriteable = Android.OS.Environment.MediaMounted.Equals(Android.OS.Environment.ExternalStorageState);
            if (!isWriteable)
            {
                throw new InvalidOperationException("External storage is unavailable");
            }

            var directory = await GetFirmwareDirectoryPath();
            
            // Requesting permission to write a file
            if (ContextCompat.CheckSelfPermission(Android.App.Application.Context, Manifest.Permission.WriteExternalStorage)
                != (int)Permission.Granted)
            {
                ActivityCompat.RequestPermissions(Platform.CurrentActivity,
                    new String[] { Manifest.Permission.WriteExternalStorage }, 1337);
            }

            Directory.CreateDirectory(directory);

            var fullPath = Path.Combine(directory, "Firmware.bin");

            using (var writer = File.Create(fullPath))
            {
                await writer.WriteAsync(content.ToArray());
            }
        }
    }
}