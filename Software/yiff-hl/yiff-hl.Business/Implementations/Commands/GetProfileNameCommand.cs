using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetProfileNameResponseDelegate(bool isSuccessful, int profileId, string name);

    public class GetProfileNameCommand
    {
        private const int MinNameLength = 1;
        private const int MaxNameLength = 16;

        private readonly IPacketsProcessor packetsProcessor;
        private OnGetProfileNameResponseDelegate onGetProfileNameResponse;

        public GetProfileNameCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetProfileNameResponse(OnGetProfileNameResponse);
        }

        public void SetResponseDelegate(OnGetProfileNameResponseDelegate onGetProfileNameResponse)
        {
            this.onGetProfileNameResponse = onGetProfileNameResponse;
        }

        public void SendGetProfileNameCommand(int id)
        {
            if (id < Constants.MinProfileId || id > Constants.MaxProfileId)
            {
                throw new ArgumentException("Invalid profile ID", nameof(id));
            }

            var payload = new List<byte>();

            // 2th (from 0th) byte - profile ID
            payload.Add((byte)(id));

            packetsProcessor.SendCommand(CommandType.GetProfileName, payload);
        }

        private void OnGetProfileNameResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetProfileNameResponse == null)
            {
                return;
            }

            if (!CommandsHelper.IsSuccessful(payload.ElementAt(0)))
            {
                onGetProfileNameResponse(false, 0, String.Empty);
                return;
            }

            var profileId = (int)payload.ElementAt(1);

            var expectedNameLength = payload.ElementAt(2);

            if (expectedNameLength < MinNameLength || expectedNameLength > MaxNameLength)
            {
                return;
            }

            if (payload.Count != expectedNameLength + 3)
            {
                return;
            }

            var name = Encoding.ASCII.GetString(payload
                .ToList()
                .GetRange(3, payload.Count - 3)
                .ToArray());

            onGetProfileNameResponse(true, profileId, name);
        }
    }
}
