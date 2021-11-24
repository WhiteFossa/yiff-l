using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class FoxProfileSwitcher : IFoxProfileSwitcher
    {
        private ISwitchToProfileCommand _switchToProfileCommand;
        private IGetCurrentProfileIdCommand _getCurrentProfileIdCommand;

        private OnProfileSwitchedDelegate _onProfileSwitchedDelegate;

        private int _newProfile;

        public FoxProfileSwitcher(ISwitchToProfileCommand switchToProfileCommand,
            IGetCurrentProfileIdCommand getCurrentProfileIdCommand)
        {
            _switchToProfileCommand = switchToProfileCommand;
            _getCurrentProfileIdCommand = getCurrentProfileIdCommand;
        }

        public async Task SwitchProfileAsync(int profileId, OnProfileSwitchedDelegate onProfileSwitchedDelegate)
        {
            _onProfileSwitchedDelegate = onProfileSwitchedDelegate ?? throw new ArgumentNullException(nameof(onProfileSwitchedDelegate));

            // If current profile is equal to new profile we will do nothing
            _newProfile = profileId;

            _getCurrentProfileIdCommand.SetResponseDelegate(OnGetCurrentProfileIdResponse);
            _getCurrentProfileIdCommand.SendGetCurrentProfileIdCommand();
        }

        private void OnGetCurrentProfileIdResponse(int profileId)
        {
            if (profileId == _newProfile)
            {
                _onProfileSwitchedDelegate();
                return;
            }

            // Actually changing
            _switchToProfileCommand.SetResponseDelegate(OnSwitchToProfileResponse);
            _switchToProfileCommand.SendSwitchToProfileCommand(_newProfile);
        }

        private void OnSwitchToProfileResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to switch to a new profile!");
            }

            _onProfileSwitchedDelegate();
        }
    }
}
