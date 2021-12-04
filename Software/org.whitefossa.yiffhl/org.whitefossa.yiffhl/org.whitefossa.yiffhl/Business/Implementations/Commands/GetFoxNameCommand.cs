using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetFoxNameCommand : IGetFoxNameCommand
    {
        private const int MinNameLength = 1;
        private const int MaxNameLength = 32;

        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetFoxNameResponseDelegate _onGetFoxNameResponse;

        public GetFoxNameCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetFoxNameResponse(OnGetFoxNameResponse);
        }

        public void SetResponseDelegate(OnGetFoxNameResponseDelegate onGetFoxNameResponse)
        {
            _onGetFoxNameResponse = onGetFoxNameResponse;
        }

        public void SendGetFoxNameCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetFoxName, new List<byte>());
        }

        private void OnGetFoxNameResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetFoxNameResponse == null)
            {
                return;
            }

            var expectedNameLength = payload.ElementAt(0);

            if (expectedNameLength < MinNameLength || expectedNameLength > MaxNameLength)
            {
                return;
            }

            if (payload.Count != expectedNameLength + 1)
            {
                return;
            }

            var name = Encoding.ASCII.GetString(payload
                .ToList()
                .GetRange(1, payload.Count - 1)
                .ToArray());

            _onGetFoxNameResponse(name);
        }
    }
}
