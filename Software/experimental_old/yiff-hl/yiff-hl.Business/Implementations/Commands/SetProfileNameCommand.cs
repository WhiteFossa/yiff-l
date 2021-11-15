using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetProfileNameResponseDelegate(bool isSuccessfull);

    public class SetProfileNameCommand
    {
        private const int MinNameLength = 1;
        private const int MaxNameLength = 16;

        private readonly IPacketsProcessor packetsProcessor;
        private OnSetProfileNameResponseDelegate onSetProfileNameResponse;

        private SetProfileNameCommand()
        {

        }

        public SetProfileNameCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetProfileNameResponse(OnSetProfileNameResponse);
        }

        public void SetResponseDelegate(OnSetProfileNameResponseDelegate onSetProfileNameResponse)
        {
            this.onSetProfileNameResponse = onSetProfileNameResponse;
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

            packetsProcessor.SendCommand(CommandType.SetProfileName, payload);
        }

        private void OnSetProfileNameResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetProfileNameResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetProfileNameResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
