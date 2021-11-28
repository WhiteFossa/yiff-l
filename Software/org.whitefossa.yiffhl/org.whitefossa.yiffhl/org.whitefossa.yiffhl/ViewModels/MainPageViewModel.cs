using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Business.Implementations.Commands;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.ViewModels
{
    internal class MainPageViewModel : BindableObject
    {
        /// <summary>
        /// We support this version of fox protocol
        /// </summary>
        private const UInt16 SupportedProtocolVersion = 1;

        /// <summary>
        /// Minimal frequency for 80m range
        /// </summary>
        private const int MinFrequency80m = 3500000;

        /// <summary>
        /// Maximal frequency for 80m range
        /// </summary>
        private const int MaxFrequency80m = 3650000;

        /// <summary>
        /// Frequency step for 80m range
        /// </summary>
        private const int FrequencyStep80m = 5000;

        /// <summary>
        /// Default frequency for 80m range
        /// </summary>
        private const int DefaultFrequency80m = 3575000;

        /// <summary>
        /// Minimal frequency for 2m range
        /// </summary>
        private const int MinFrequency2m = 144000000;

        /// <summary>
        /// Maximal frequency for 2m range
        /// </summary>
        private const int MaxFrequency2m = 146000000;

        /// <summary>
        /// Frequency step for 2m range
        /// </summary>
        private const int FrequencyStep2m = 50000;

        /// <summary>
        /// Default frequency for 2m range
        /// </summary>
        private const int DefaultFrequency2m = 145000000;

        private readonly IFoxConnector _foxConnector;
        private readonly IPairedFoxesEnumerator _pairedFoxesEnumerator;
        private readonly IUserNotifier _userNotifier;
        private readonly IPacketsProcessor _packetsProcessor;
        private readonly IUserRequestor _userRequestor;
        private readonly IBluetoothManager _bluetoothManager;
        private readonly IFoxProfilesEnumerator _foxProfilesEnumerator;
        private readonly IFoxProfilesManager _foxProfilesManager;
        private readonly IFoxIdentificationManager _foxIdentificationManager;
        private readonly IFoxNameManager _foxNameManager;
        private readonly IFoxClockManager _foxClockManager;
        private readonly IProfileSettingsManager _profileSettingsManager;

        /// <summary>
        /// Main model
        /// </summary>
        private MainModel _mainModel = new MainModel();

        /// <summary>
        /// List of foxes, paired to a phone
        /// </summary>
        private ObservableCollection<PairedFoxDTO> _pairedFoxes = new ObservableCollection<PairedFoxDTO>();

        public ObservableCollection<PairedFoxDTO> PairedFoxes
        {
            get
            {
                return _pairedFoxes;
            }
            set
            {
                _pairedFoxes = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Selected fox
        /// </summary>
        private PairedFoxDTO _selectedFox = null;

        public PairedFoxDTO SelectedFox
        {
            get => _selectedFox;
            set
            {
                _selectedFox = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, executed on selected fox change
        /// </summary>
        public ICommand SelectedFoxChangedCommand { get; }

        /// <summary>
        /// True if connect button enabled
        /// </summary>
        private bool _isConnectButtonEnabled;

        public bool IsConnectButtonEnabled
        {
            get => _isConnectButtonEnabled;
            set
            {
                _isConnectButtonEnabled = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// True if connect-related controls are enabled
        /// </summary>
        private bool _isConnectRelatedControlsEnabled;

        public bool IsConnectRelatedControlsEnabled
        {
            get => _isConnectRelatedControlsEnabled;
            set
            {
                _isConnectRelatedControlsEnabled = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, executed on Connect button press
        /// </summary>
        public ICommand ConnectButtonClickedCommand { get; }

        /// <summary>
        /// Command, executed on Refresh Foxes List button press
        /// </summary>
        public ICommand RefreshFoxesListClickedCommand { get; }

        /// <summary>
        /// Is Disconnect button enabled
        /// </summary>
        private bool _isBtnDisconnectEnabled;

        public bool IsBtnDisconnectEnabled
        {
            get => _isBtnDisconnectEnabled;
            set
            {
                _isBtnDisconnectEnabled = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, executed on Disconnect button press
        /// </summary>
        public ICommand DisconnectButtonClickedCommand { get; }

        /// <summary>
        /// Fox name
        /// </summary>
        public string FoxName
        {
            get => _mainModel.FoxName;
            set
            {
                _mainModel.FoxName = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Fox hardware revision
        /// </summary>
        public uint FoxHardwareRevision
        {
            get => _mainModel.IdentificationData.HardwareRevision;
            set
            {
                _mainModel.IdentificationData.HardwareRevision = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Fox firmware version
        /// </summary>
        public uint FoxFirmwareVersion
        {
            get => _mainModel.IdentificationData.FirmwareVersion;
            set
            {
                _mainModel.IdentificationData.FirmwareVersion = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Fox serial number
        /// </summary>
        public uint FoxSerialNumber
        {
            get => _mainModel.IdentificationData.SerialNumber;
            set
            {
                _mainModel.IdentificationData.SerialNumber = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Is fox connected
        /// </summary>
        public bool IsFoxConnected
        {
            get => _mainModel.IsConnected;
            set
            {
                _mainModel.IsConnected = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, called when user tries to rename a fox
        /// </summary>
        public ICommand RenameFoxClickedCommand { get; }

        /// <summary>
        /// True if fox picker need to be enabled
        /// </summary>
        private bool _isFoxPickerEnabled;

        public bool IsFoxPickerEnabled
        {
            get => _isFoxPickerEnabled;
            set
            {
                _isFoxPickerEnabled = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// List of profiles in fox
        /// </summary>
        private ObservableCollection<Profile> _profiles = new ObservableCollection<Profile>();

        public ObservableCollection<Profile> Profiles
        {
            get => _profiles;
            set
            {
                _profiles = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(ProfilesCountText));
                IsAddProfileButtonEnabled = CanWeAddProfiles;
            }
        }

        /// <summary>
        /// Command, called when Refresh Profiles List button is clicked
        /// </summary>
        public ICommand RefreshProfilesListClickedCommand { get; }

        /// <summary>
        /// Can we add profiles
        /// </summary>
        public bool CanWeAddProfiles
        {
            get => Profiles.Count < Constants.MaxProfilesCount;
        }

        /// <summary>
        /// Profiles count
        /// </summary>
        public string ProfilesCountText
        {
            get
            {
                var countAsText = _mainModel.IsConnected ? $"{ Profiles.Count }" : "N/A";

                var maxProfilesWarning = CanWeAddProfiles ? String.Empty : ". No space left!";

                return $"Profiles count: { countAsText } {maxProfilesWarning}";
            }
        }

        /// <summary>
        /// Is add profile button enabled
        /// </summary>
        private bool _isAddProfileButtonEnabled;

        public bool IsAddProfileButtonEnabled
        {
            get => _isAddProfileButtonEnabled;
            set
            {
                _isAddProfileButtonEnabled = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, called when user clicks Add Profile button
        /// </summary>
        public ICommand AddProfileClickedCommand { get; }

        /// <summary>
        /// Selected profile
        /// </summary>
        public Profile SelectedProfile
        {
            get => _mainModel.CurrentProfile;
            set
            {
                _mainModel.CurrentProfile = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, executed on selected profile change
        /// </summary>
        public ICommand SelectedProfileChangedCommand { get; }

        /// <summary>
        /// Command, executed when Rename Profile button is clicked
        /// </summary>
        public ICommand RenameProfileClickedCommand { get; }

        /// <summary>
        /// Formatted frequency range
        /// </summary>
        public string FoxRangeFormatted
        {
            get
            {
                if (_mainModel.CurrentProfileSettings.FrequencySettings.Is2m)
                {
                    return "2m";
                }
                else
                {
                    return "80m";
                }
            }
        }

        /// <summary>
        /// Toggle fox frequency range
        /// </summary>
        public ICommand ToggleFoxFrequencyRangeCommand { get; }

        /// <summary>
        /// Formatted frequency for a label
        /// </summary>
        public string FrequencyFormatted
        {
            get
            {
                if (_mainModel.CurrentProfileSettings.FrequencySettings.Frequency == 0)
                {
                    return "N/A";
                }
                else
                {
                    return String.Format("{0:0.000 MHz}", _mainModel.CurrentProfileSettings.FrequencySettings.Frequency / 1000000.0);
                }
            }
        }

        public MainPageViewModel()
        {
            _foxConnector = App.Container.Resolve<IFoxConnector>();
            _pairedFoxesEnumerator = App.Container.Resolve<IPairedFoxesEnumerator>();
            _userNotifier = App.Container.Resolve<IUserNotifier>();
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _userRequestor = App.Container.Resolve<IUserRequestor>();
            _bluetoothManager = App.Container.Resolve<IBluetoothManager>();
            _foxProfilesEnumerator = App.Container.Resolve<IFoxProfilesEnumerator>();
            _foxProfilesManager = App.Container.Resolve<IFoxProfilesManager>();
            _foxIdentificationManager = App.Container.Resolve<IFoxIdentificationManager>();
            _foxNameManager = App.Container.Resolve<IFoxNameManager>();
            _foxClockManager = App.Container.Resolve<IFoxClockManager>();
            _profileSettingsManager = App.Container.Resolve<IProfileSettingsManager>();

            // Setting up fox connector delegates
            _mainModel.OnFoxConnectorNewByteRead += OnNewByteRead;
            _mainModel.OnFoxConnectorConnected += OnConnectedAsync;
            _mainModel.OnFoxConnectorDisconnected += OnDisconnected;
            _mainModel.OnFoxConnectorFailedToConnect += OnFailedToConnect;

            _foxConnector.SetupDelegates
            (
                _mainModel.OnFoxConnectorNewByteRead,
                _mainModel.OnFoxConnectorConnected,
                _mainModel.OnFoxConnectorDisconnected,
                _mainModel.OnFoxConnectorFailedToConnect
            );

            // Binding commands to handlers
            SelectedFoxChangedCommand = new Command<PairedFoxDTO>(async (f) => await OnSelectedFoxChangedAsync(f));
            ConnectButtonClickedCommand = new Command(async () => await OnConnectButtonCLickedAsync());
            RefreshFoxesListClickedCommand = new Command(async () => await OnRefreshFoxesListClickedAsync());
            DisconnectButtonClickedCommand = new Command(async () => await OnDisconnectButtonClickedAsync());
            RenameFoxClickedCommand = new Command(async () => await OnRenameFoxClickedAsync());
            RefreshProfilesListClickedCommand = new Command(async () => await OnRefreshProfilesListClickedAsync());
            AddProfileClickedCommand = new Command(async () => await OnAddProfileClickedAsync());
            SelectedProfileChangedCommand = new Command<Profile>(async (p) => await OnChangeSelectedProfileAsync(p));
            RenameProfileClickedCommand = new Command(async () => await OnRenameProfileClickedAsync());
            ToggleFoxFrequencyRangeCommand = new Command(async () => await OnToggleFoxFrequencyRangeAsync());

            // Initial state
            IsConnectButtonEnabled = false;
            IsFoxPickerEnabled = true;
            IsConnectRelatedControlsEnabled = true;
            IsBtnDisconnectEnabled = false;
            IsAddProfileButtonEnabled = false;

            ResetFoxRelatedData();

            // Requesting foxes list
            Task.WaitAll(OnRefreshFoxesListClickedAsync());
        }

        public async Task OnSelectedFoxChangedAsync(PairedFoxDTO selectedFox)
        {
            IsConnectRelatedControlsEnabled = selectedFox != null && !IsFoxConnected;
            IsConnectButtonEnabled = IsConnectRelatedControlsEnabled;
        }

        public async Task OnConnectButtonCLickedAsync()
        {
            IsConnectButtonEnabled = false;
            IsFoxPickerEnabled = false;
            IsConnectRelatedControlsEnabled = false;

            await _foxConnector.ConnectAsync(SelectedFox);
        }

        public async Task OnRefreshFoxesListClickedAsync()
        {
            PairedFoxes = await EnumerateFoxesAsync();
        }

        public async Task OnDisconnectButtonClickedAsync()
        {
            IsBtnDisconnectEnabled = false;

            await _foxConnector.DisconnectAsync();
        }

        private async Task<ObservableCollection<PairedFoxDTO>> EnumerateFoxesAsync()
        {
            try
            {
                return new ObservableCollection<PairedFoxDTO>(await _pairedFoxesEnumerator.EnumerateAsync());
            }
            catch (InvalidOperationException ex)
            {
                Device.BeginInvokeOnMainThread(async () =>
                {
                    await _userNotifier.ShowErrorMessageAsync("Bluetooth error:", ex.Message);
                });

                return new ObservableCollection<PairedFoxDTO>();
            }
        }

        private void OnNewByteRead(byte data)
        {
            _packetsProcessor.NewByteReceived(data);
        }

        private async void OnConnectedAsync(PairedFoxDTO connectedFox)
        {
            _mainModel.ConnectedFox = connectedFox;
            IsFoxConnected = true;

            IsBtnDisconnectEnabled = true;

            // Requesting fox identification
            await _foxIdentificationManager.IdentifyFoxAsync(async (isFox, pVer, hwRev, fwVer, sn)
                => await OnGetIdentificationDataResponseAsync(isFox, pVer, hwRev, fwVer, sn));
        }

        private void OnDisconnected()
        {
            IsFoxConnected = false;
            _mainModel.ConnectedFox = null;

            ResetFoxRelatedData();

            IsConnectButtonEnabled = true;
            IsFoxPickerEnabled = true;
            IsConnectRelatedControlsEnabled = true;
            IsAddProfileButtonEnabled = false;
        }

        private void OnFailedToConnect(Exception exception)
        {
            IsConnectButtonEnabled = true;
            IsFoxPickerEnabled = true;
            IsConnectRelatedControlsEnabled = true;
            IsAddProfileButtonEnabled = false;
        }

        /// <summary>
        /// Called when fox identification is received
        /// </summary>
        private async Task OnGetIdentificationDataResponseAsync
        (
            bool isFox,
            uint protocolVersion,
            uint hardwareRevision,
            uint firmwareVersion,
            uint serialNumber
        )
        {
            if (!isFox)
            {
                await OnDisconnectButtonClickedAsync();

                await _userNotifier.ShowErrorMessageAsync("Error:", "You've tried to connect to not a fox!");
                return;
            }

            if (protocolVersion != SupportedProtocolVersion)
            {
                await OnDisconnectButtonClickedAsync();

                await _userNotifier.ShowErrorMessageAsync("Error:",
                    $"Unsupported fox protocol version. Got { protocolVersion } while expecting { SupportedProtocolVersion }");

                return;
            }

            FoxHardwareRevision = hardwareRevision;
            FoxFirmwareVersion = firmwareVersion;
            FoxSerialNumber = serialNumber;

            // Setting fox time
            await _foxClockManager.SynchronizeClockAsync(async (isSuccessfull) => await OnSetFoxDateAndTimeResponseAsync(isSuccessfull));
        }

        /// <summary>
        /// Called when fox name is received
        /// </summary>
        private async Task OnGetFoxNameResponseAsync(string name)
        {
            FoxName = name;

            await EnumerateProfilesAsync();
        }

        /// <summary>
        /// Resets fox-related data (empties fox name, sets hardware version to zero and so on)
        /// </summary>
        private void ResetFoxRelatedData()
        {
            FoxName = "N/A";
            FoxHardwareRevision = 0;
            FoxFirmwareVersion = 0;
            FoxSerialNumber = 0;
        }

        private async Task OnSetFoxDateAndTimeResponseAsync(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                await OnDisconnectButtonClickedAsync();

                await _userNotifier.ShowErrorMessageAsync("Error:", "Failed to set fox date and time!");
                return;
            }

            // Requesting fox name
            await _foxNameManager.GetNameAsync(async (name) => await OnGetFoxNameResponseAsync(name));
        }

        private async Task OnRenameFoxClickedAsync()
        {
            var isConfirmed = await _userNotifier.ShowYesNoRequestAsync("Confirmation",
                @"Please take into account, that after fox rename the Bluetooth connection will be lost.
You will have to un-pair old device and pair with a fox under a new name in a phone settings, then reconnect to it.
Do you want to continue?");

            if (!isConfirmed)
            {
                return;
            }

            var newNameData = await _userRequestor.EnterStringAsync
            (
                "Fox name",
                "Enter new fox name",
                FoxName,
                SetFoxNameCommand.MaxNameLength,
                (args) =>
                {
                    args.IsValid = true;

                    // Minimal length
                    if (args.Value.Length < SetFoxNameCommand.MinNameLength)
                    {
                        args.IsValid = false;
                        return;
                    }

                    // Only digits and numbers
                    if (!args.Value.All(char.IsLetterOrDigit))
                    {
                        args.IsValid = false;
                        return;
                    }
                }
            );

            if (!newNameData.Item1)
            {
                return;
            }

            await _foxNameManager.SetNameAsync(newNameData.Item2, async (isSuccessfull) => await OnSetFoxNameResponseAsync(isSuccessfull));
        }

        private async Task OnSetFoxNameResponseAsync(bool isSuccessfull)
        {
            var fox = _mainModel.ConnectedFox;

            await OnDisconnectButtonClickedAsync();

            if (!isSuccessfull)
            {
                await _userNotifier.ShowErrorMessageAsync("Error", "Failed to change fox name!");
                return;
            }

            // Un-pairing bluetooth device
            _bluetoothManager.UnPair(fox.MAC);

            await OnRefreshFoxesListClickedAsync();

            await _userNotifier.ShowErrorMessageAsync("Success", "Fox name is changed successfully.");
        }

        private async Task EnumerateProfilesAsync()
        {
            await _foxProfilesEnumerator.EnumerateProfilesAsync(async (profiles) => await OnFoxProfilesEnumeratedAsync(profiles));
        }

        private async Task OnFoxProfilesEnumeratedAsync(IReadOnlyCollection<Profile> profiles)
        {
            Profiles = new ObservableCollection<Profile>(profiles);

            DetectActiveProfile();
        }

        private async Task OnRefreshProfilesListClickedAsync()
        {
            await EnumerateProfilesAsync();
        }

        private async Task OnAddProfileClickedAsync()
        {
            var profileNameData = await _userRequestor.EnterStringAsync
            (
                "New profile",
                "Enter new profile name",
                $"New profile { Profiles.Count + 1 }",
                SetProfileNameCommand.MaxNameLength,
                (args) =>
                {
                    args.IsValid = true;

                    // Minimal length
                    if (args.Value.Length < SetProfileNameCommand.MinNameLength)
                    {
                        args.IsValid = false;
                        return;
                    }

                    // Only digits and numbers
                    if (!args.Value.All(c => (char.IsWhiteSpace(c) || char.IsLetterOrDigit(c))))
                    {
                        args.IsValid = false;
                        return;
                    }
                }
            );

            if (!profileNameData.Item1)
            {
                return;
            }

            await _foxProfilesManager.AddProfileAsync(profileNameData.Item2, OnFoxProfileAdded);
        }

        private void OnFoxProfileAdded()
        {
            Task.WaitAll(EnumerateProfilesAsync());
        }

        public async Task OnChangeSelectedProfileAsync(Profile selectedProfile)
        {
            if (selectedProfile == null)
            {
                return;
            }

            _mainModel.CurrentProfile = selectedProfile;

            // Switching profile in the fox
            await _foxProfilesManager.SwitchProfileAsync(selectedProfile.Id, OnSelectedProfileChanged);
        }

        private void OnSelectedProfileChanged()
        {
            DetectActiveProfile();
        }

        private void OnGetCurrentProfileIdResponse(int profileId)
        {
            SelectedProfile = Profiles
                .FirstOrDefault(p => p.Id == profileId);

            Task.WhenAll(LoadProfileSettingsAsync());
        }

        private async Task OnRenameProfileClickedAsync()
        {
            // We are always renaming current profile, so there is no need to switch profile
            var profileNameData = await _userRequestor.EnterStringAsync
            (
                "Rename profile",
                $"Current name: { SelectedProfile.Name }",
                SelectedProfile.Name,
                SetProfileNameCommand.MaxNameLength,
                (args) =>
                {
                    args.IsValid = true;

                    // Minimal length
                    if (args.Value.Length < SetProfileNameCommand.MinNameLength)
                    {
                        args.IsValid = false;
                        return;
                    }

                    // Only digits and numbers
                    if (!args.Value.All(c => (char.IsWhiteSpace(c) || char.IsLetterOrDigit(c))))
                    {
                        args.IsValid = false;
                        return;
                    }
                }
            );

            if (!profileNameData.Item1)
            {
                return;
            }

            await _foxProfilesManager.RenameCurrentProfileAsync(profileNameData.Item2, async() => await OnProfileRenamedAsync());
        }

        private void DetectActiveProfile()
        {
            _foxProfilesManager.GetCurrentProfileId(OnGetCurrentProfileIdResponse);
        }

        private async Task OnProfileRenamedAsync()
        {
            await EnumerateProfilesAsync();
        }

        /// <summary>
        /// Entry point for profile data loading
        /// </summary>
        /// <returns></returns>
        private async Task LoadProfileSettingsAsync()
        {
            await LoadFrequencySettingsAsync();
        }

        private async Task LoadFrequencySettingsAsync()
        {
            await _profileSettingsManager.LoadFrequencySettingsAsync(OnGetFrequencySettings);
        }

        private void OnGetFrequencySettings(FrequencySettings settings)
        {
            _mainModel.CurrentProfileSettings.FrequencySettings = settings;
            OnPropertyChanged(nameof(FoxRangeFormatted));
            OnPropertyChanged(nameof(FrequencyFormatted));
        }

        private async Task SetFrequencyAsync()
        {
            await _profileSettingsManager.SetFrequencySettingsAsync(_mainModel.CurrentProfileSettings.FrequencySettings,
                async () => await OnSetFrequencySettingsAsync());
        }

        private async Task OnSetFrequencySettingsAsync()
        {
            // Re-reading data from fox
            await LoadFrequencySettingsAsync();
        }

        private async Task OnToggleFoxFrequencyRangeAsync()
        {
            _mainModel.CurrentProfileSettings.FrequencySettings.Is2m = !_mainModel.CurrentProfileSettings.FrequencySettings.Is2m;

            if (_mainModel.CurrentProfileSettings.FrequencySettings.Is2m)
            {
                _mainModel.CurrentProfileSettings.FrequencySettings.Frequency = DefaultFrequency2m;
            }
            else
            {
                _mainModel.CurrentProfileSettings.FrequencySettings.Frequency = DefaultFrequency80m;
            }

            await SetFrequencyAsync();
        }
    }
}
