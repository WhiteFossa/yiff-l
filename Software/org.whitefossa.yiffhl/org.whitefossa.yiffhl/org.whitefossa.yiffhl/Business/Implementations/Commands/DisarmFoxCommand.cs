using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class DisarmFoxCommand : IDisarmFoxCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnDisarmFoxResponseDelegate _onDisarmFoxResponse;

        public DisarmFoxCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnDisarmFoxResponse(OnDisarmFoxResponse);
        }

        public void SetResponseDelegate(OnDisarmFoxResponseDelegate onDisarmFoxResponse)
        {
            _onDisarmFoxResponse = onDisarmFoxResponse;
        }

        public void SendDisarmFoxCommand()
        {
            _packetsProcessor.SendCommand(CommandType.DisarmFox, new List<byte>());
        }

        private void OnDisarmFoxResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onDisarmFoxResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onDisarmFoxResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
