using System.Collections.Generic;
using yiff_hl.Abstractions.Enums;

namespace yiff_hl.Abstractions.Interfaces
{
    /// <summary>
    /// Interface to send generic commands to the fox
    /// </summary>
    public interface IGenericCommandWriter
    {
        void SendCommand(CommandType command, IReadOnlyCollection<byte> commandPayload);
    }
}
