using System.Collections.Generic;
using System.Linq;
using System.Text;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetFoxNameResponseDelegate(string name);

    public class GetFoxNameCommand
    {
        private const int MinNameLength = 1;
        private const int MaxNameLength = 32;

        private readonly IPacketsProcessor packetsProcessor;
        private OnGetFoxNameResponseDelegate onGetFoxNameResponse;

        public GetFoxNameCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor;

            packetsProcessor.SetOnGetFoxNameResponse(OnGetFoxNameResponse);
        }

        public void SetResponseDelegate(OnGetFoxNameResponseDelegate onGetFoxNameResponse)
        {
            this.onGetFoxNameResponse = onGetFoxNameResponse;
        }

        public void SendGetFoxNameCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetFoxName, new List<byte>());
        }

        private void OnGetFoxNameResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetFoxNameResponse == null)
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

            onGetFoxNameResponse(name);
        }
    }
}
