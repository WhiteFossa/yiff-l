using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnAddNewProfileResponseDelegate(bool isSuccessful);

    public class AddNewProfileCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnAddNewProfileResponseDelegate onAddNewProfileResponse;

        public AddNewProfileCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor;
            packetsProcessor.SetOnAddNewProfileResponse(OnAddNewProfileResponse);
        }

        public void SetResponseDelegate(OnAddNewProfileResponseDelegate onAddNewProfileResponse)
        {
            this.onAddNewProfileResponse = onAddNewProfileResponse;
        }

        public void SendAddNewProfileCommand()
        {
            var payload = new List<byte>();
            packetsProcessor.SendCommand(CommandType.AddNewProfile, payload);
        }

        private void OnAddNewProfileResponse(IReadOnlyCollection<byte> payload)
        {
            if (onAddNewProfileResponse == null)
            {
                return;
            }

            onAddNewProfileResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
