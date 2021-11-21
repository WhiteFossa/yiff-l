using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public delegate void OnSetFoxNameResponseDelegate(bool isSuccessfull);

    public class SetFoxNameCommand
    {
        private const int MinNameLength = 1;
        private const int MaxNameLength = 16;

        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetFoxNameResponseDelegate _onSetFoxNameResponse;

        public SetFoxNameCommand()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _packetsProcessor.SetOnSetFoxNameResponse(OnSetFoxNameResponse);
        }

        public void SetResponseDelegate(OnSetFoxNameResponseDelegate onSetFoxNameResponse)
        {
            _onSetFoxNameResponse = onSetFoxNameResponse;
        }

        public void SendSetFoxNameCommand(string name)
        {
            if (name.Length < MinNameLength || name.Length > MaxNameLength)
            {
                throw new ArgumentException("Invalid name length", nameof(name));
            }

            var payload = new List<byte>();

            // 2th (from 0th) byte - name length
            payload.Add((byte)(name.Length));

            // 3th - 35th bytes - name
            var nameBytes = Encoding.ASCII.GetBytes(name);
            payload.AddRange(nameBytes);

            _packetsProcessor.SendCommand(CommandType.SetFoxName, payload);
        }

        private void OnSetFoxNameResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetFoxNameResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetFoxNameResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
