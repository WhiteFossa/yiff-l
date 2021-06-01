using System;
using System.Collections.Generic;
using System.Linq;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands;

namespace yiff_hl.Pages
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class FoxSettingsPage : ContentPage
    {
        private readonly IBluetoothCommunicator bluetoothCommunicator;
        private readonly IPacketsProcessor packetsProcessor;

        public FoxSettingsPage(IBluetoothCommunicator bluetoothCommunicator,
            IPacketsProcessor packetsProcessor)
        {
            this.bluetoothCommunicator = bluetoothCommunicator;
            this.packetsProcessor = packetsProcessor;

            App.NewByteReadDelegate = packetsProcessor.NewByteReceived;

            InitializeComponent();
        }

        private void OnDisconnectClicked(object sender, EventArgs e)
        {
            Disconnect();
        }

        protected override bool OnBackButtonPressed()
        {
            Disconnect();
            return base.OnBackButtonPressed();
        }

        private void Disconnect()
        {
            bluetoothCommunicator.Disconnect();
            Navigation.PopModalAsync();
        }

        #region Set date and time

        private void OnSetCurrentDateAndTimeClicked(object sender, EventArgs e)
        {
            SetCurrentDateAndTime();
        }

        private void SetCurrentDateAndTime()
        {
            var command = new SetDateAndTimeCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetCurrentDateAndTimeResponse);
            command.SendSetDateAndTimeCommand(DateTime.Now);
        }

        private void OnSetCurrentDateAndTimeResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Fox clock synchronized" : "Failed to synchronize fox clock";
                DisplayAlert("Clock synchronization", message, "OK");
            });
        }

        #endregion

        #region Set fox name

        private void OnSetFoxNameClicked(object sender, EventArgs e)
        {
            SetFoxName();
        }

        private void SetFoxName()
        {
            var command = new SetFoxNameCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetFoxNameResponse);
            command.SendSetFoxNameCommand(edFoxName.Text);
        }

        private void OnSetFoxNameResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Fox name set" : "Failed to set fox name";
                DisplayAlert("Name change", message, "OK");
            });
        }

        #endregion

        #region Get fox name

        private void OnGetFoxNameClicked(object sender, EventArgs e)
        {
            GetFoxName();
        }

        private void GetFoxName()
        {
            var command = new GetFoxNameCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetFoxNameResponse);
            command.SendGetFoxNameCommand();
        }

        private void OnGetFoxNameResponse(string name)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                DisplayAlert("Fox name", $"Fox name: {name}", "OK");
            });
        }

        #endregion

        #region Get profiles count

        private void OnGetProfilesCountClicked(object sender, EventArgs e)
        {
            GetProfilesCount();
        }

        private void GetProfilesCount()
        {
            var command = new GetProfilesCountCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetProfilesCountResponse);
            command.SendGetProfilesCountCommand();
        }

        private void OnGetProfilesCountResponse(int count)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                DisplayAlert("Profiles count", $"Profiles count: {count}", "OK");
            });
        }

        #endregion

        #region Get profile name

        private void OnGetProfileNameClicked(object sender, EventArgs e)
        {
            GetProfileName(int.Parse(edProfileId.Text));
        }

        private void GetProfileName(int profileId)
        {
            var command = new GetProfileNameCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetProfileNameResponse);
            command.SendGetProfileNameCommand(profileId);
        }

        private void OnGetProfileNameResponse(bool isSuccessful, int profileId, string name)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                DisplayAlert("Profile name", $"Is successful: {isSuccessful}, ID: {profileId}, Name: {name}", "OK");
            });
        }

        #endregion

        #region Enumerate profiles

        private void OnEnumerateProfilesClicked(object sender, EventArgs e)
        {
            EnumerateProfiles();
        }

        private void EnumerateProfiles()
        {
            pkProfiles.Items.Clear();

            // Profiles count
            var command = new GetProfilesCountCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetProfilesCountInEnumerateProfilesResponse);
            command.SendGetProfilesCountCommand();
        }

        private void OnGetProfilesCountInEnumerateProfilesResponse(int count)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var profilesCount = count;

                // Getting profiles names
                for (var profileId = 0; profileId < profilesCount; profileId ++)
                {
                    var command = new GetProfileNameCommand(packetsProcessor);
                    command.SetResponseDelegate(OnGetProfileNameResponseInEnumerateProfiles);
                    command.SendGetProfileNameCommand(profileId);
                }
            });
        }

        private void OnGetProfileNameResponseInEnumerateProfiles(bool isSuccessful, int profileId, string name)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                pkProfiles.Items.Add(name);

                pkProfiles.SelectedIndex = 0;
            });
        }

        #endregion

        #region Add new profile

        private void OnAddNewProfileClicked(object sender, EventArgs e)
        {
            AddNewProfile();
        }

        private void AddNewProfile()
        {
            var command = new AddNewProfileCommand(packetsProcessor);
            command.SetResponseDelegate(OnAddNewProfileResponse);
            command.SendAddNewProfileCommand();
        }

        private void OnAddNewProfileResponse(bool isSuccessful)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                if (!isSuccessful)
                {
                    DisplayAlert("Add new profile", "Failed to add new profile", "OK");
                    return;
                }

                EnumerateProfiles();
                DisplayAlert("Add new profile", "New profile added", "OK");
            });
        }

        #endregion
    }
}