using org.whitefossa.yiffhl.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Interface for saving files into the fox directory
    /// </summary>
    public interface IFilesManager
    {
        Task<string> GetFirmwareDirectoryPath();

        Task SaveFileAsync(string filename, List<byte> content);

        Task OnSaveFilePermissionResultAsync(bool isGranted);

        Task<string> GenerateDumpFilename(MainModel mainModel);
    }
}
