using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnIsFoxArmedResponseDelegate(bool isArmed);

    public class IsFoxArmedCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnIsFoxArmedResponseDelegate onIsFoxArmedResponse;

        private IsFoxArmedCommand()
        {

        }

        public IsFoxArmedCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnIsFoxArmedResponse(OnIsFoxArmedResponse);
        }

        public void SetResponseDelegate(OnIsFoxArmedResponseDelegate onIsFoxArmedResponse)
        {
            this.onIsFoxArmedResponse = onIsFoxArmedResponse;
        }

        public void SendIsFoxArmedCommand()
        {
            packetsProcessor.SendCommand(CommandType.IsFoxArmed, new List<byte>());
        }

        private void OnIsFoxArmedResponse(IReadOnlyCollection<byte> payload)
        {
            if (onIsFoxArmedResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onIsFoxArmedResponse(CommandsHelper.ToBool(payload.ElementAt(0)));
        }
    }
}
