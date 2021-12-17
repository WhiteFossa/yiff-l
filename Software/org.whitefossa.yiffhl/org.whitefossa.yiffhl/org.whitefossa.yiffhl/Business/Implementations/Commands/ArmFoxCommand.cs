using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class ArmFoxCommand : IArmFoxCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnArmFoxResponseDelegate _onArmFoxResponse;

        public ArmFoxCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnArmFoxResponse(OnArmFoxResponse);
        }

        public void SetResponseDelegate(OnArmFoxResponseDelegate onArmFoxResponse)
        {
            _onArmFoxResponse = onArmFoxResponse;
        }

        public void SendArmFoxCommand()
        {
            _packetsProcessor.SendCommand(CommandType.ArmFox, new List<byte>());
        }

        private void OnArmFoxResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onArmFoxResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onArmFoxResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
