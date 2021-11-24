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
        private readonly IFoxProfileSwitcher _foxProfileSwitcher;
        private readonly IGetCurrentProfileIdCommand _getCurrentProfileIdCommand;

        private OnProfileAddedDelegate _onProfileAdded;

        private string _newProfileName;

        private OnGetCurrentProfileIdDelegate _onGetCurrentProfileId;

        public FoxProfilesManager(IAddNewProfileCommand addNewProfileCommand,
            IGetProfilesCountCommand getProfilesCountCommand,
            ISetProfileNameCommand setProfileNameCommand,
            IFoxProfileSwitcher foxProfileSwitcher,
            IGetCurrentProfileIdCommand getCurrentProfileIdCommand)
        {
            _addNewProfileCommand = addNewProfileCommand;
            _getProfilesCountCommand = getProfilesCountCommand;
            _setProfileNameCommand = setProfileNameCommand;
            _foxProfileSwitcher = foxProfileSwitcher;
            _getCurrentProfileIdCommand = getCurrentProfileIdCommand;
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
            _addNewProfileCommand.SetResponseDelegate(OnAddNewProfileResponse);
            _addNewProfileCommand.SendAddNewProfileCommand();
        }

        private void OnAddNewProfileResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to add a new profile!");
            }

            // Getting profiles count, count - 1 will be the new profile ID
            _getProfilesCountCommand.SetResponseDelegate(OnGetProfilesCountResponse);
            _getProfilesCountCommand.SendGetProfilesCountCommand();
        }

        private void OnGetProfilesCountResponse(int count)
        {
            var newProfileId = count - 1;

            // Switching to a new profile
            _foxProfileSwitcher.SwitchProfileAsync(newProfileId, OnSwitchToProfile);
        }

        private void OnSwitchToProfile()
        {
            // Setting profile name
            _setProfileNameCommand.SetResponseDelegate(OnSetProfileNameResponse);
            _setProfileNameCommand.SendSetProfileNameCommand(_newProfileName);
        }

        private void OnSetProfileNameResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to rename a new profile!");
            }

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
    }
}
