using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Models;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Interface to connect to fox
    /// </summary>
    public interface IFoxConnector
    {
        /// <summary>
        /// Connect to fox
        /// </summary>
        Task ConnectAsync(MainModel model, PairedFoxDTO foxToConnect);

        /// <summary>
        /// Disconnect from a fox
        /// </summary>
        Task DisconnectAsync(MainModel model);
    }
}
