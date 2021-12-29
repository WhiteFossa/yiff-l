using org.whitefossa.yiffhl.Abstractions.Enums;
using System.Collections.Generic;

namespace org.whitefossa.yiffhl.Business.Implementations.PacketsProcessor
{
    public class CommandQueueElement
    {
        public CommandType Command { get; }

        public IReadOnlyCollection<byte> Payload { get; }

        public CommandQueueElement(CommandType command, IReadOnlyCollection<byte> payload)
        {
            Command = command;
            Payload = payload;
        }
    }
}
