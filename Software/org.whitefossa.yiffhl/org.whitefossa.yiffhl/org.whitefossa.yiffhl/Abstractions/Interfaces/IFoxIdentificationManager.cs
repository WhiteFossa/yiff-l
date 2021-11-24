using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnFoxIdentificationDelegate
    (
        bool isFox,
        uint protocolVersion,
        uint hardwareRevision,
        uint softwareVersion,
        uint serialNumber
     );

    /// <summary>
    /// Interface to get fox identification data
    /// </summary>
    public interface IFoxIdentificationManager
    {
        Task IdentifyFoxAsync(OnFoxIdentificationDelegate onFoxIdentification);
    }
}
