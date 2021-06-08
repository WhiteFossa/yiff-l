using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnArmFoxResponseDelegate(bool isSuccessful);

    public class ArmFoxCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnArmFoxResponseDelegate onArmFoxResponse;

        private ArmFoxCommand()
        {

        }

        public ArmFoxCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnArmFoxResponse(OnArmFoxResponse);
        }

        public void SetResponseDelegate(OnArmFoxResponseDelegate onArmFoxResponse)
        {
            this.onArmFoxResponse = onArmFoxResponse;
        }

        public void SendArmFoxCommand()
        {
            packetsProcessor.SendCommand(CommandType.ArmFox, new List<byte>());
        }

        private void OnArmFoxResponse(IReadOnlyCollection<byte> payload)
        {
            if (onArmFoxResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onArmFoxResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
