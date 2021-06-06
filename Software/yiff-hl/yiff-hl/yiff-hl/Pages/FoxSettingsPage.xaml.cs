using System;
using System.Collections.Generic;
using System.Linq;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands;

namespace yiff_hl.Pages
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class FoxSettingsPage : ContentPage
    {
        private readonly IBluetoothCommunicator bluetoothCommunicator;
        private readonly IPacketsProcessor packetsProcessor;

        private class Profile
        {
            public int Id { get; set; }

            public int Index { get; set; }

            public string Name { get; set; }
        }

        private List<Profile> profiles = new List<Profile>();

        public FoxSettingsPage(IBluetoothCommunicator bluetoothCommunicator,
            IPacketsProcessor packetsProcessor)
        {
            this.bluetoothCommunicator = bluetoothCommunicator;
            this.packetsProcessor = packetsProcessor;

            App.NewByteReadDelegate = packetsProcessor.NewByteReceived;

            InitializeComponent();

            // Fox codes
            pkCode.Items.Add("Finish");
            pkCode.Items.Add("Fox #1");
            pkCode.Items.Add("Fox #2");
            pkCode.Items.Add("Fox #3");
            pkCode.Items.Add("Fox #4");
            pkCode.Items.Add("Fox #5");
            pkCode.Items.Add("Beacon");
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
            profiles.Clear();

            // Getting first profile name
            var command = new GetProfileNameCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetProfileNameResponseInEnumerateProfiles);
            command.SendGetProfileNameCommand(0);
        }

        private void OnGetProfileNameResponseInEnumerateProfiles(bool isSuccessful, int profileId, string name)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                if (isSuccessful)
                {
                    profiles.Add(new Profile() { Index = pkProfiles.Items.Count, Id = profileId, Name = name });
                    pkProfiles.Items.Add(name);

                    var command = new GetProfileNameCommand(packetsProcessor);
                    command.SetResponseDelegate(OnGetProfileNameResponseInEnumerateProfiles);
                    command.SendGetProfileNameCommand(profileId + 1);
                }
                else
                {
                    // Enumeration completed, asking for active profile
                    var command = new GetCurrentProfileIdCommand(packetsProcessor);
                    command.SetResponseDelegate(OnGetCurrentProfileIdResponseInEnumerateProfiles);
                    command.SendGetCurrentProfileIdCommand();
                }
            });
        }

        private void OnGetCurrentProfileIdResponseInEnumerateProfiles(int profileId)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var index = profiles
                    .First(p => p.Id == profileId)
                    .Index;

                pkProfiles.SelectedIndex = index;
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

        #region Get current profile ID

        private void OnGetCurrentProfileIdClicked(object sender, EventArgs e)
        {
            GetCurrentProfileId();
        }

        private void GetCurrentProfileId()
        {
            var command = new GetCurrentProfileIdCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetCurrentProfileIdResponse);
            command.SendGetCurrentProfileIdCommand();
        }

        private void OnGetCurrentProfileIdResponse(int profileId)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                DisplayAlert("Current profile ID", $"Current profile ID: {profileId}", "OK");
            });
        }

        #endregion

        #region Switch to profile

        private void OnSwitchToProfileClicked(object sender, EventArgs e)
        {
            var profileId = profiles
                .First(p => p.Index == pkProfiles.SelectedIndex)
                .Id;

            SwitchProfile(profileId);
        }

        private void SwitchProfile(int profileId)
        {
            var command = new SwitchToProfileCommand(packetsProcessor);
            command.SetResponseDelegate(OnSwitchToProfileResponse);
            command.SendSwitchToProfileCommand(profileId);
        }

        private void OnSwitchToProfileResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Switched successfully" : "Failed to switch profile";
                DisplayAlert("Switch profile", message, "OK");
            });
        }

        #endregion

        #region Set profile name

        private void OnSetProfileNameClicked(object sender, EventArgs e)
        {
            SetProfileName(edProfileName.Text);
        }

        private void SetProfileName(string name)
        {
            var command = new SetProfileNameCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetProfileNameResponse);
            command.SendSetProfileNameCommand(name);
        }

        private void OnSetProfileNameResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Profile name set" : "Failed to set profile name";
                DisplayAlert("Set profile name", message, "OK");
            });
        }

        #endregion

        #region Get frequency

        private void OnGetFrequencyClicked(object sender, EventArgs e)
        {
            GetFrequency();
        }

        private void GetFrequency()
        {
            var command = new GetFrequencyCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetFrequencyResponse);
            command.SendGetFrequencyCommand();
        }

        private void OnGetFrequencyResponse(bool is144Mhz, int frequency)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                cbIs144.IsChecked = is144Mhz;
                edFrequency.Text = $"{frequency}";
            });
        }

        #endregion

        #region Set frequency

        private void OnSetFrequencyClicked(object sender, EventArgs e)
        {
            SetFrequency(cbIs144.IsChecked, int.Parse(edFrequency.Text));
        }

        private void SetFrequency(bool is144MHz, int frequency)
        {
            var command = new SetFrequencyCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetFrequencyResponse);
            command.SendSetFrequencyCommand(is144MHz, frequency);
        }

        private void OnSetFrequencyResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Frequency set" : "Failed to set frequency";
                DisplayAlert("Set frequency", message, "OK");
            });
        }

        #endregion

        #region Get code

        private void OnGetCodeClicked(object sender, EventArgs e)
        {
            GetCode();
        }

        private void GetCode()
        {
            var command = new GetCodeCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetCodeResponse);
            command.SendGetCodeCommand();
        }

        private void OnGetCodeResponse(FoxCode code)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                pkCode.SelectedIndex = (int)code;
            });
        }

        #endregion

        #region Set code

        private void OnSetCodeClicked(object sender, EventArgs e)
        {
            SetCode((FoxCode)pkCode.SelectedIndex);
        }

        private void SetCode(FoxCode code)
        {
            var command = new SetCodeCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetCodeResponse);
            command.SendSetCodeCommand(code);
        }

        private void OnSetCodeResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Code set" : "Failed to set code";
                DisplayAlert("Set code", message, "OK");
            });
        }

        #endregion

        #region Get speed

        private void OnGetSpeedClicked(object sender, EventArgs e)
        {
            GetSpeed();
        }

        private void GetSpeed()
        {
            var command = new GetSpeedCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetSpeedResponse);
            command.SendGetSpeedCommand();
        }

        private void OnGetSpeedResponse(bool isFast)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                cbIsFast.IsChecked = isFast;
            });
        }

        #endregion
    }
}