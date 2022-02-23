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

        private readonly IUserNotifier _userNotifier;

        /// <summary>
        /// Save file with this name
        /// </summary>
        private string _saveFilename;

        /// <summary>
        /// File content to save
        /// </summary>
        private List<byte> _saveContent;

        public FilesManager(IUserNotifier userNotifier)
        {
            _userNotifier = userNotifier;
        }

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

            _saveFilename = "firmware.bin";
            _saveContent = content;
            
            // Requesting permission to write a file
            if (ContextCompat.CheckSelfPermission(Android.App.Application.Context, Manifest.Permission.WriteExternalStorage)
                != (int)Permission.Granted)
            {
                ActivityCompat.RequestPermissions(Platform.CurrentActivity,
                    new String[] { Manifest.Permission.WriteExternalStorage }, Constants.FileManagerWriteExternalStoragePermission);
            }
            else
            {
                await OnSaveFilePermissionResultAsync(true);
            }
        }

        public async Task OnSaveFilePermissionResultAsync(bool isGranted)
        {
            if (!isGranted)
            {
                await _userNotifier.ShowErrorMessageAsync("Permission required", $"Permission required to save file {_saveFilename}");
                return;
            }

            var foxDirectory = await GetFirmwareDirectoryPath();
            Directory.CreateDirectory(foxDirectory);

            var fullPath = Path.Combine(foxDirectory, _saveFilename);

            using (var writer = File.Create(fullPath))
            {
                await writer.WriteAsync(_saveContent.ToArray());
            }
        }
    }
}