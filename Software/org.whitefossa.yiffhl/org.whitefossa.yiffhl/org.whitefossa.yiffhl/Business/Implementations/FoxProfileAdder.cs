using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class FoxProfileAdder : IFoxProfileAdder
    {
        private MainModel _mainModel;
        private OnFoxProfileAddedDelegate _onFoxProfileAdded;

        private string _newProfileName;

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
            _mainModel.AddNewFoxProfile.SetResponseDelegate(OnAddNewProfileResponse);
            _mainModel.AddNewFoxProfile.SendAddNewProfileCommand();
        }

        private void OnAddNewProfileResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to add a new profile!");
            }

            // Getting profiles count, count - 1 will be the new profile ID
            _mainModel.GetFoxProfilesCount.SetResponseDelegate(OnGetProfilesCountResponse);
            _mainModel.GetFoxProfilesCount.SendGetProfilesCountCommand();
        }

        private void OnGetProfilesCountResponse(int count)
        {
            var newProfileId = count - 1;

            // Switching to a new profile
            _mainModel.SwitchFoxProfile.SetResponseDelegate(OnSwitchToProfileResponse);
            _mainModel.SwitchFoxProfile.SendSwitchToProfileCommand(newProfileId);
        }

        private void OnSwitchToProfileResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to switch to a new profile!");
            }

            // Setting profile name
            _mainModel.SetFoxProfileName.SetResponseDelegate(OnSetProfileNameResponse);
            _mainModel.SetFoxProfileName.SendSetProfileNameCommand(_newProfileName);
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
