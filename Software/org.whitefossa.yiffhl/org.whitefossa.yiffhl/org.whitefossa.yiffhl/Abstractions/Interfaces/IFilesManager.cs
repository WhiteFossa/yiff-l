using org.whitefossa.yiffhl.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Interface for saving files into the fox directory/reading them from anywhere
    /// </summary>
    public interface IFilesManager
    {
        Task<string> GetFirmwareDirectoryPath();

        Task SaveFileAsync(string filename, List<byte> content);

        Task OnSaveFilePermissionResultAsync(bool isGranted);

        Task<string> GenerateDumpFilename(MainModel mainModel);

        Task<List<byte>> ReadFileAsync(string fullPath);

        Task<string> GenerateBackupFilename(MainModel mainModel);
    }
}
