using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SwitchToProfileCommand : ISwitchToProfileCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSwitchToProfileResponseDelegate _onSwitchToProfileResponse;

        public SwitchToProfileCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSwitchToProfileResponse(OnSwitchToProfileResponse);
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

            _packetsProcessor.SendCommand(CommandType.SwitchToProfile, payload);
        }

        private void OnSwitchToProfileResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSwitchToProfileResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSwitchToProfileResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }

        public void SetResponseDelegate(OnSwitchToProfileResponseDelegate onSwitchToProfileResponse)
        {
            _onSwitchToProfileResponse = onSwitchToProfileResponse;
        }
    }
}
