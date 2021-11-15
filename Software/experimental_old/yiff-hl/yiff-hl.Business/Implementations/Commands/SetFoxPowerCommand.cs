using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetFoxPowerResponseDelegate(bool isSuccessfull);

    public class SetFoxPowerCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnSetFoxPowerResponseDelegate onSetFoxPowerResponse;

        private SetFoxPowerCommand()
        {

        }

        public SetFoxPowerCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetFoxPowerResponse(OnSetFoxPowerResponse);
        }

        public void SetResponseDelegate(OnSetFoxPowerResponseDelegate onSetFoxPowerResponse)
        {
            this.onSetFoxPowerResponse = onSetFoxPowerResponse;
        }

        public void SendSetFoxPowerCommand(float power)
        {
            var payload = new List<byte>();

            // 2th - 5th (from 0th) bytes - fox power
            payload.AddRange(BitConverter.GetBytes(power));

            packetsProcessor.SendCommand(CommandType.SetFoxPower, payload);
        }

        private void OnSetFoxPowerResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetFoxPowerResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetFoxPowerResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
