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

        #region Set speed

        private void OnSetSpeedClicked(object sender, EventArgs e)
        {
            SetSpeed(cbIsFast.IsChecked);
        }

        private void SetSpeed(bool isFast)
        {
            var command = new SetSpeedCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetSpeedResponse);
            command.SendSetSpeedCommand(isFast);
        }

        private void OnSetSpeedResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Speed set" : "Failed to set speed";
                DisplayAlert("Set speed", message, "OK");
            });
        }

        #endregion

        #region Get cycle

        private void OnGetCycleClicked(object sender, EventArgs e)
        {
            GetCycle();
        }

        private void GetCycle()
        {
            var command = new GetCycleCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetCycleResponse);
            command.SendGetCycleCommand();
        }

        private void OnGetCycleResponse(bool isContinuous, TimeSpan txTime, TimeSpan pauseTime)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                cbIsContinuous.IsChecked = isContinuous;
                edTxTime.Text = $"{ txTime.TotalSeconds }";
                edPauseTime.Text = $"{ pauseTime.TotalSeconds }";
            });
        }

        #endregion

        #region Set cycle

        private void OnSetCycleClicked(object sender, EventArgs e)
        {
            SetCycle(cbIsContinuous.IsChecked, new TimeSpan(0, 0, int.Parse(edTxTime.Text)), new TimeSpan(0, 0, int.Parse(edPauseTime.Text)));
        }

        private void SetCycle(bool isContinuous, TimeSpan txTime, TimeSpan pauseTime)
        {
            var command = new SetCycleCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetCycleResponse);
            command.SendSetCycleCommand(isContinuous, txTime, pauseTime);
        }

        private void OnSetCycleResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Cycle set" : "Failed to set cycle";
                DisplayAlert("Set cycle", message, "OK");
            });
        }

        #endregion

        #region Get ending tone duration

        private void OnGetEndingToneDurationClicked(object sender, EventArgs e)
        {
            GetEndingToneDuration();
        }

        private void GetEndingToneDuration()
        {
            var command = new GetEndingToneDurationCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetEndingToneDurationResponse);
            command.SendGetEndingToneDurationCommand();
        }

        private void OnGetEndingToneDurationResponse(TimeSpan duration)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                edEndingTone.Text = $"{ duration.TotalSeconds }";
            });
        }

        #endregion

        #region Set ending tone duration

        private void OnSetEndingToneDurationClicked(object sender, EventArgs e)
        {
            SetEndingToneDuration(new TimeSpan(0, 0, int.Parse(edEndingTone.Text)));
        }

        private void SetEndingToneDuration(TimeSpan endingToneDuration)
        {
            var command = new SetEndingToneDurationCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetEndingToneDurationResponse);
            command.SendSetEndingToneResponseDurationCommand(endingToneDuration);
        }

        private void OnSetEndingToneDurationResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Ending tone duration set" : "Failed to set Ending tone duration";
                DisplayAlert("Set ending tone duration", message, "OK");
            });
        }

        #endregion

        #region Get begin and end times

        private void OnGetBeginAndEndTimeClicked(object sender, EventArgs e)
        {
            GetBeginAndEndTimes();
        }

        private void GetBeginAndEndTimes()
        {
            var command = new GetBeginAndEndTimesCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetBeginAndEndTimesResponse);
            command.SendGetBeginAndEndTimesCommand();
        }

        private void OnGetBeginAndEndTimesResponse(DateTime beginTime, DateTime endTime)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                tpBeginTime.Time = beginTime - GetBeginAndEndTimesCommand.BaseDate;
                tpEndTime.Time = endTime - GetBeginAndEndTimesCommand.BaseDate;
            });
        }

        #endregion

        #region Is fox armed

        private void OnIsFoxArmedClicked(object sender, EventArgs e)
        {
            IsFoxArmed();
        }

        private void IsFoxArmed()
        {
            var command = new IsFoxArmedCommand(packetsProcessor);
            command.SetResponseDelegate(OnIsFoxArmedResponse);
            command.SendIsFoxArmedCommand();
        }

        private void OnIsFoxArmedResponse(bool isArmed)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                cbIsArmed.IsChecked = isArmed;
            });
        }

        #endregion

        #region Arm fox

        private void OnArmFoxClicked(object sender, EventArgs e)
        {
            ArmFox();
        }

        private void ArmFox()
        {
            var command = new ArmFoxCommand(packetsProcessor);
            command.SetResponseDelegate(OnArmFoxResponse);
            command.SendArmFoxCommand();
        }

        private void OnArmFoxResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Fox armed" : "Failed to arm fox";
                DisplayAlert("Arm fox", message, "OK");
            });
        }

        #endregion

        #region Disarm fox

        private void OnDisarmFoxClicked(object sender, EventArgs e)
        {
            DisarmFox();
        }

        private void DisarmFox()
        {
            var command = new DisarmFoxCommand(packetsProcessor);
            command.SetResponseDelegate(OnDisarmFoxResponse);
            command.SendDisarmFoxCommand();
        }

        private void OnDisarmFoxResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Fox disarmed" : "Failed to disarm fox";
                DisplayAlert("Disarm fox", message, "OK");
            });
        }

        #endregion

        #region Set begin and end times

        private void OnSetBeginAndEndTimeClicked(object sender, EventArgs e)
        {
            SetBeginAndEndTimes(new DateTime(1970, 1, 1) + tpBeginTime.Time, new DateTime(1970, 1, 1) + tpEndTime.Time);
        }

        private void SetBeginAndEndTimes(DateTime beginTime, DateTime endTime)
        {
            var command = new SetBeginAndEndTimesCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetBeginAndEndTimesResponse);
            command.SendSetBeginAndEndTimesCommand(beginTime, endTime);
        }

        private void OnSetBeginAndEndTimesResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Times set" : "Failed to set times";
                DisplayAlert("Set begin and end times", message, "OK");
            });
        }

        #endregion

        #region Get fox power

        private void OnGetFoxPowerClicked(object sender, EventArgs e)
        {
            GetFoxPower();
        }

        private void GetFoxPower()
        {
            var command = new GetFoxPowerCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetFoxPowerResponse);
            command.SendGetFoxPowerCommand();
        }

        private void OnGetFoxPowerResponse(float power)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                edFoxPower.Text = $"{power}";
            });
        }

        #endregion

        #region Set fox power

        private void OnSetFoxPowerClicked(object sender, EventArgs e)
        {
            var power = float.Parse(edFoxPower.Text);

            SetFoxPower(power);
        }

        private void SetFoxPower(float power)
        {
            var command = new SetFoxPowerCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetFoxPowerResponse);
            command.SendSetFoxPowerCommand(power);
        }

        private void OnSetFoxPowerResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Power set" : "Failed to set power";
                DisplayAlert("Set fox power", message, "OK");
            });
        }

        #endregion

        #region Get battery level

        private void OnGetBatteryLevelClicked(object sender, EventArgs e)
        {
            GetBatteryLevel();
        }

        private void GetBatteryLevel()
        {
            var command = new GetBatteryLevelCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetBatteryLevelrResponse);
            command.SendGetBatteryLevelCommand();
        }

        private void OnGetBatteryLevelrResponse(float level)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                DisplayAlert("Check battery", $"Battery level: { level }", "OK");
            });
        }

        #endregion

        #region Get UAnt (ADC)

        private void OnGetUAntADCClicked(object sender, EventArgs e)
        {
            GetUAntADC();
        }

        private void GetUAntADC()
        {
            var command = new GetUAntADCCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetUAntADCResponse);
            command.SendGetUAntADCCommand();
        }

        private void OnGetUAntADCResponse(float uAnt)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                edUAntADC.Text = $"{uAnt}";
            });
        }

        #endregion
    }
}