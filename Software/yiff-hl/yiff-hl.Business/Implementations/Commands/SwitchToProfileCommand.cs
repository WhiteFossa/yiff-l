using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSwitchToProfileResponseDelegate(bool isSuccessfull);

    public class SwitchToProfileCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnSwitchToProfileResponseDelegate onSwitchToProfileResponse;

        public SwitchToProfileCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor;
            packetsProcessor.SetOnSwitchToProfileResponse(OnSwitchToProfileResponse);
        }

        public void SendSwitchToProfileCommand(int id)
        {
            if (id < Constants.MinProfileId || id > Constants.MaxProfileId)
            {
                throw new ArgumentException("Invalid profile ID", nameof(id));
            }

            var payload = new List<byte>();

            // 2th (from 0th) byte - profile ID
            payload.Add((byte)(id));

            packetsProcessor.SendCommand(CommandType.SwitchToProfile, payload);
        }

        private void OnSwitchToProfileResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSwitchToProfileResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSwitchToProfileResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }

        public void SetResponseDelegate(OnSwitchToProfileResponseDelegate onSwitchToProfileResponse)
        {
            this.onSwitchToProfileResponse = onSwitchToProfileResponse;
        }
    }
}
