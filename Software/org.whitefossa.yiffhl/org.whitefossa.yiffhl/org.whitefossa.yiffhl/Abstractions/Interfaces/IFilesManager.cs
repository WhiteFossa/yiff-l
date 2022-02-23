using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Interface for saving files
    /// </summary>
    public interface IFilesManager
    {
        Task<string> GetFirmwareDirectoryPath();

        Task SaveFileAsync(List<byte> content);
    }
}
