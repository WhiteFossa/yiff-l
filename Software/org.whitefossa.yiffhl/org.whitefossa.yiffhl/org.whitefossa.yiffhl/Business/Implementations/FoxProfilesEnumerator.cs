using Acr.UserDialogs;
using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class FoxProfilesEnumerator : IFoxProfilesEnumerator
    {
        private IGetProfilesCountCommand _getProfilesCountCommand;
        private IGetProfileNameCommand _getProfileNameCommand;

        private MainModel _mainModel;
        private OnFoxProfilesEnumeratedDelegate _onFoxProfilesEnumerated;

        private int _profilesCount;
        private List<Profile> _enumeratedProfiles = new List<Profile>();

        private IProgressDialog _progressDialog;

        public FoxProfilesEnumerator(IGetProfilesCountCommand getProfilesCountCommand,
            IGetProfileNameCommand getProfileNameCommand)
        {
            _getProfilesCountCommand = getProfilesCountCommand;
            _getProfileNameCommand = getProfileNameCommand;
        }

        public async Task EnumerateProfilesAsync(MainModel mainModel, OnFoxProfilesEnumeratedDelegate onProfilesEnumerated)
        {
            _mainModel = mainModel ?? throw new ArgumentNullException(nameof(mainModel));
            _onFoxProfilesEnumerated = onProfilesEnumerated ?? throw new ArgumentNullException(nameof(onProfilesEnumerated));

            _getProfilesCountCommand.SetResponseDelegate(OnGetProfilesCountResponse);
            _getProfileNameCommand.SetResponseDelegate(OnNewProfileRead);

            _enumeratedProfiles.Clear();

            // How many profiles do we have?
            _getProfilesCountCommand.SendGetProfilesCountCommand();
        }

        private void OnGetProfilesCountResponse(int count)
        {
            _profilesCount = count;

            // Starting enumeration
            _progressDialog = UserDialogs.Instance.Progress("Enumerating profiles...", null, null, true, MaskType.Clear);
            _getProfileNameCommand.SendGetProfileNameCommand(0);
        }

        void OnNewProfileRead(bool isSuccessful, int profileId, string name)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to get profile name!");
            }

            _progressDialog.PercentComplete = (int)Math.Round(100 * profileId / (double)_profilesCount);

            _enumeratedProfiles.Add(new Profile() { Id = profileId, Name = name });

            var nextProfileId = profileId + 1;
            if (nextProfileId < _profilesCount)
            {
                // Continuing enumeration
                _getProfileNameCommand.SendGetProfileNameCommand(nextProfileId);
                return;
            }

            // Enumeration completed
            _progressDialog.Dispose();
            _onFoxProfilesEnumerated(_enumeratedProfiles);
        }
    }
}
