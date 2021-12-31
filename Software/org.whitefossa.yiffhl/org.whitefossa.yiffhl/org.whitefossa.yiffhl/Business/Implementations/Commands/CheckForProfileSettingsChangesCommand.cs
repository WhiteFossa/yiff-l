using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class CheckForProfileSettingsChangesCommand : ICheckForProfileSettingsChangesCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnCheckForProfileSettingsChangesDelegate _onCheckForProfileSettingsChanges;

        public CheckForProfileSettingsChangesCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnCheckForProfileSettingsChangesResponse(OnCheckForProfileSettingsChangesResponse);
        }

        public void SetResponseDelegate(OnCheckForProfileSettingsChangesDelegate onCheckForProfileSettingsChanges)
        {
            _onCheckForProfileSettingsChanges = onCheckForProfileSettingsChanges;
        }

        public void SendCheckForProfileSettingsChangesCommand()
        {
            _packetsProcessor.SendCommand(CommandType.CheckForProfileSettingsChanges, new List<byte>());
        }

        private void OnCheckForProfileSettingsChangesResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onCheckForProfileSettingsChanges == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onCheckForProfileSettingsChanges(CommandsHelper.ToBool(payload.ElementAt(0)));
        }
    }
}
