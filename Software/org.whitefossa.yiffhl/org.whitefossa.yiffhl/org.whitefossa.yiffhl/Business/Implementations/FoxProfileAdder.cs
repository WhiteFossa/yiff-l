using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Models;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class FoxProfileAdder : IFoxProfileAdder
    {
        private readonly IAddNewProfileCommand _addNewProfileCommand;
        private readonly IGetProfilesCountCommand _getProfilesCountCommand;
        private readonly ISetProfileNameCommand _setProfileNameCommand;
        private readonly ISwitchToProfileCommand _switchToProfileCommand;

        private MainModel _mainModel;
        private OnFoxProfileAddedDelegate _onFoxProfileAdded;

        private string _newProfileName;

        public FoxProfileAdder(IAddNewProfileCommand addNewProfileCommand,
            IGetProfilesCountCommand getProfilesCountCommand,
            ISetProfileNameCommand setProfileNameCommand,
            ISwitchToProfileCommand switchToProfileCommand)
        {
            _addNewProfileCommand = addNewProfileCommand;
            _getProfilesCountCommand = getProfilesCountCommand;
            _setProfileNameCommand = setProfileNameCommand;
            _switchToProfileCommand = switchToProfileCommand;
        }

        public async Task AddProfileAsync(MainModel mainModel, string newProfileName, OnFoxProfileAddedDelegate onFoxProfileAdded)
        {
            _mainModel = mainModel ?? throw new ArgumentNullException(nameof(mainModel));
            _onFoxProfileAdded = onFoxProfileAdded ?? throw new ArgumentException(nameof(onFoxProfileAdded));

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
            _switchToProfileCommand.SetResponseDelegate(OnSwitchToProfileResponse);
            _switchToProfileCommand.SendSwitchToProfileCommand(newProfileId);
        }

        private void OnSwitchToProfileResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to switch to a new profile!");
            }

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

            _onFoxProfileAdded();
        }
    }
}
