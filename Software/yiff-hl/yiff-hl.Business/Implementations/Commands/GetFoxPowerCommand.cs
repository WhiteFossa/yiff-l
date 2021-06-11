using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetFoxPowerResponseDelegate(float power);

    public class GetFoxPowerCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetFoxPowerResponseDelegate onGetFoxPowerResponse;

        private GetFoxPowerCommand()
        {

        }

        public GetFoxPowerCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetFoxPowerResponse(OnGetFoxPowerResponse);
        }

        public void SetResponseDelegate(OnGetFoxPowerResponseDelegate onGetFoxPowerResponse)
        {
            this.onGetFoxPowerResponse = onGetFoxPowerResponse;
        }

        public void SendGetFoxPowerCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetFoxPower, new List<byte>());
        }

        private void OnGetFoxPowerResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetFoxPowerResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var power = BitConverter.ToSingle(payload.ToArray(), 0);

            onGetFoxPowerResponse(power);
        }

    }
}
