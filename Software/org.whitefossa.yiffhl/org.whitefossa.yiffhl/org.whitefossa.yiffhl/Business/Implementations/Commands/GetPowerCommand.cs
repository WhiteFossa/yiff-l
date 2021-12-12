using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetPowerCommand : IGetPowerCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetPowerResponseDelegate _onGetPowerResponse;

        public GetPowerCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetFoxPowerResponse(OnGetPowerResponse);
        }

        public void SetResponseDelegate(OnGetPowerResponseDelegate onGetPowerResponse)
        {
            _onGetPowerResponse = onGetPowerResponse;
        }

        public void SendGetPowerCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetFoxPower, new List<byte>());
        }

        private void OnGetPowerResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetPowerResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var power = BitConverter.ToSingle(payload.ToArray(), 0);

            _onGetPowerResponse(power);
        }
    }
}
