using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetProfileNameCommand : IGetProfileNameCommand
    {
        public const int MinNameLength = 1;
        public const int MaxNameLength = 16;

        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetProfileNameResponseDelegate _onGetProfileNameResponse;

        public GetProfileNameCommand()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _packetsProcessor.SetOnGetProfileNameResponse(OnGetProfileNameResponse);
        }

        public void SetResponseDelegate(OnGetProfileNameResponseDelegate onGetProfileNameResponse)
        {
            _onGetProfileNameResponse = onGetProfileNameResponse;
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

            _packetsProcessor.SendCommand(CommandType.GetProfileName, payload);
        }

        private void OnGetProfileNameResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetProfileNameResponse == null)
            {
                return;
            }

            if (!CommandsHelper.IsSuccessful(payload.ElementAt(0)))
            {
                _onGetProfileNameResponse(false, 0, String.Empty);
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

            _onGetProfileNameResponse(true, profileId, name);
        }
    }
}
