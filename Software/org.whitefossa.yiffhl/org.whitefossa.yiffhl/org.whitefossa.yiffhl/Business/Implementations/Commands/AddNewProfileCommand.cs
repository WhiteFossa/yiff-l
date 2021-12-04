using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class AddNewProfileCommand : IAddNewProfileCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnAddNewProfileResponseDelegate _onAddNewProfileResponse;

        public AddNewProfileCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnAddNewProfileResponse(OnAddNewProfileResponse);
        }

        public void SetResponseDelegate(OnAddNewProfileResponseDelegate onAddNewProfileResponse)
        {
            _onAddNewProfileResponse = onAddNewProfileResponse;
        }

        public void SendAddNewProfileCommand()
        {
            var payload = new List<byte>();
            _packetsProcessor.SendCommand(CommandType.AddNewProfile, payload);
        }

        private void OnAddNewProfileResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onAddNewProfileResponse == null)
            {
                return;
            }

            _onAddNewProfileResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
