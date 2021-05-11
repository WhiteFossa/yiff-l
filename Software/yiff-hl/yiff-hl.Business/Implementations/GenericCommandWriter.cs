using System.Collections.Generic;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations
{
    public class GenericCommandWriter : IGenericCommandWriter
    {
        private readonly IPacketsProcessor packetsProcessor;

        public GenericCommandWriter(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor;
        }

        public void SendCommand(CommandType command, IReadOnlyCollection<byte> commandPayload)
        {
            var resultPayload = new List<byte>();
            resultPayload.Add((byte)PayloadType.CommandToFox);
            resultPayload.Add((byte)command);
            resultPayload.AddRange(commandPayload);

            packetsProcessor.SendPacket(resultPayload);
        }
    }
}
