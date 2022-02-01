using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class FoxProfilesManager : IFoxProfilesManager
    {
        private readonly IAddNewProfileCommand _addNewProfileCommand;
        private readonly IGetProfilesCountCommand _getProfilesCountCommand;
        private readonly ISetProfileNameCommand _setProfileNameCommand;
        private readonly IGetCurrentProfileIdCommand _getCurrentProfileIdCommand;
        private readonly ISwitchToProfileCommand _switchToProfileCommand;

        private OnProfileAddedDelegate _onProfileAdded;

        private string _newProfileName;

        private OnGetCurrentProfileIdDelegate _onGetCurrentProfileId;

        private OnProfileSwitchedDelegate _onProfileSwitched;

        private int _newProfileId;

        private OnProfileRenamedDelegate _onProfileRenamed;

        public FoxProfilesManager(IAddNewProfileCommand addNewProfileCommand,
            IGetProfilesCountCommand getProfilesCountCommand,
            ISetProfileNameCommand setProfileNameCommand,
            IGetCurrentProfileIdCommand getCurrentProfileIdCommand,
            ISwitchToProfileCommand switchToProfileCommand)
        {
            _addNewProfileCommand = addNewProfileCommand;
            _getProfilesCountCommand = getProfilesCountCommand;
            _setProfileNameCommand = setProfileNameCommand;
            _getCurrentProfileIdCommand = getCurrentProfileIdCommand;
            _switchToProfileCommand = switchToProfileCommand;
        }

        public async Task AddProfileAsync(string newProfileName, OnProfileAddedDelegate onProfileAdded)
        {
            _onProfileAdded = onProfileAdded ?? throw new ArgumentException(nameof(onProfileAdded));

            if (string.IsNullOrWhiteSpace(newProfileName))
            {
                throw new ArgumentException(nameof(newProfileName));
            }
            _newProfileName = newProfileName;

            // Adding a new profile
            _addNewProfileCommand.SetResponseDelegate(OnAddNewProfileResponse_AddPathway);
            _addNewProfileCommand.SendAddNewProfileCommand();
        }

        private void OnAddNewProfileResponse_AddPathway(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to add a new profile!");
            }

            // Getting profiles count, count - 1 will be the new profile ID
            _getProfilesCountCommand.SetResponseDelegate(OnGetProfilesCountResponse_AddPathway);
            _getProfilesCountCommand.SendGetProfilesCountCommand();
        }

        private async void OnGetProfilesCountResponse_AddPathway(int count)
        {
            var newProfileId = count - 1;

            // Switching to a new profile
            await SwitchProfileAsync(newProfileId, async () => await OnSwitchToProfile_AddPathwayAsync());
        }

        private async Task OnSwitchToProfile_AddPathwayAsync()
        {
            // Setting profile name
            await RenameCurrentProfileAsync(_newProfileName, OnSetProfileNameResponse_AddPathway);
        }

        private void OnSetProfileNameResponse_AddPathway()
        {
            _onProfileAdded();
        }

        public async Task GetCurrentProfileId(OnGetCurrentProfileIdDelegate onGetCurrentProfileId)
        {
            _onGetCurrentProfileId = onGetCurrentProfileId ?? throw new ArgumentNullException(nameof(onGetCurrentProfileId));

            _getCurrentProfileIdCommand.SetResponseDelegate(OnGetCurrentProfileIdResponse);
            _getCurrentProfileIdCommand.SendGetCurrentProfileIdCommand();
        }

        private void OnGetCurrentProfileIdResponse(int profileId)
        {
            _onGetCurrentProfileId(profileId);
        }

        public async Task SwitchProfileAsync(int profileId, OnProfileSwitchedDelegate onProfileSwitched)
        {
            _onProfileSwitched = onProfileSwitched ?? throw new ArgumentNullException(nameof(onProfileSwitched));
            _newProfileId = profileId;
            
            await GetCurrentProfileId(OnGetCurrentProfileId_SwitchPathway);
        }

        private void OnGetCurrentProfileId_SwitchPathway(int profileId)
        {
            if (_newProfileId == profileId)
            {
                // New profile == current profile
                _onProfileSwitched();
                return;
            }

            _switchToProfileCommand.SetResponseDelegate(OnSwitchToProfileResponse_SwitchPathway);
            _switchToProfileCommand.SendSwitchToProfileCommand(_newProfileId);
        }

        private void OnSwitchToProfileResponse_SwitchPathway(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to switch to a new profile!");
            }

            _onProfileSwitched();
        }

        public async Task RenameCurrentProfileAsync(string newName, OnProfileRenamedDelegate onProfileRenamed)
        {
            _onProfileRenamed = onProfileRenamed ?? throw new ArgumentNullException(nameof(onProfileRenamed));

            _setProfileNameCommand.SetResponseDelegate(OnProfileRenamedResponse_RenamePathway);
            _setProfileNameCommand.SendSetProfileNameCommand(newName);
        }

        private void OnProfileRenamedResponse_RenamePathway(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to rename profile!");
            }

            _onProfileRenamed();
        }
    }
}
