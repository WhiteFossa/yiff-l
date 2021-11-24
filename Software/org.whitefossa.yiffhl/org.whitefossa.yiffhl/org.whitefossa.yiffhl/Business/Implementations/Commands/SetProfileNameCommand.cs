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
    public class SetProfileNameCommand : ISetProfileNameCommand
    {
        public const int MinNameLength = 1;
        public const int MaxNameLength = 16;

        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetProfileNameResponseDelegate _onSetProfileNameResponse;

        public SetProfileNameCommand()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _packetsProcessor.SetOnSetProfileNameResponse(OnSetProfileNameResponse);
        }

        public void SetResponseDelegate(OnSetProfileNameResponseDelegate onSetProfileNameResponse)
        {
            _onSetProfileNameResponse = onSetProfileNameResponse;
        }

        public void SendSetProfileNameCommand(string name)
        {
            if (name.Length < MinNameLength || name.Length > MaxNameLength)
            {
                throw new ArgumentException("Invalid name length", nameof(name));
            }

            var payload = new List<byte>();

            // 2th (from 0th) byte - name length
            payload.Add((byte)(name.Length));

            // 3th - 18th bytes - name
            var nameBytes = Encoding.ASCII.GetBytes(name);
            payload.AddRange(nameBytes);

            _packetsProcessor.SendCommand(CommandType.SetProfileName, payload);
        }

        private void OnSetProfileNameResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetProfileNameResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetProfileNameResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
