using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Interface to enumerate paired foxes
    /// </summary>
    public interface IPairedFoxesEnumerator
    {
        /// <summary>
        /// Enumerate paired foxes (it is possible that not-foxes will be enumerated too, so check on connect stage)
        /// </summary>
        Task<IReadOnlyCollection<PairedFoxDTO>> EnumerateAsync();
    }
}
