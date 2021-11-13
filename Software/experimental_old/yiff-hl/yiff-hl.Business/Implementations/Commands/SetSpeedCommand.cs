using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetSpeedResponseDelegate(bool isSuccessfull);

    public class SetSpeedCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnSetSpeedResponseDelegate onSetSpeedResponse;

        private SetSpeedCommand()
        {

        }

        public SetSpeedCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetSpeedResponse(OnSetSpeedResponse);
        }

        public void SetResponseDelegate(OnSetSpeedResponseDelegate onSetSpeedResponse)
        {
            this.onSetSpeedResponse = onSetSpeedResponse;
        }

        public void SendSetSpeedCommand(bool isFast)
        {
            var payload = new List<byte>();
            payload.Add(CommandsHelper.FromBool(isFast));

            packetsProcessor.SendCommand(CommandType.SetSpeed, payload);
        }

        private void OnSetSpeedResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetSpeedResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetSpeedResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
