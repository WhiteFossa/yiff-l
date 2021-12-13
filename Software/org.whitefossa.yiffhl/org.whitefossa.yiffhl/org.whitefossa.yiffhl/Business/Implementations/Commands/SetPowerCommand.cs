using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetPowerCommand : ISetPowerCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetPowerResponseDelegate _onSetPowerResponse;

        public SetPowerCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetFoxPowerResponse(OnSetPowerResponse);
        }

        public void SetResponseDelegate(OnSetPowerResponseDelegate onSetFoxPowerResponse)
        {
            _onSetPowerResponse = onSetFoxPowerResponse;
        }

        public void SendSetPowerCommand(float power)
        {
            var payload = new List<byte>();

            // 2th - 5th (from 0th) bytes - fox power
            payload.AddRange(BitConverter.GetBytes(power));

            _packetsProcessor.SendCommand(CommandType.SetFoxPower, payload);
        }

        private void OnSetPowerResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetPowerResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetPowerResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
