using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Implementations.Commands;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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

        private readonly IFoxConnector _foxConnector;
        private readonly IPairedFoxesEnumerator _pairedFoxesEnumerator;
        private readonly IUserNotifier _userNotifier;
        private readonly IPacketsProcessor _packetsProcessor;
        private readonly IUserRequestor _userRequestor;
        private readonly IBluetoothManager _bluetoothManager;
        private readonly IFoxProfilesEnumerator _foxProfilesEnumerator;
        private readonly IFoxProfileAdder _foxProfileAdder;
        private readonly ISetDateAndTimeCommand _setDateAndTimeCommand;
        private readonly IGetCurrentProfileIdCommand _getCurrentProfileIdCommand;
        private readonly IFoxProfileSwitcher _foxProfileSwitcher;
        private readonly IFoxIdentificationManager _foxIdentificationManager;
        private readonly IFoxNameManager _foxNameManager;

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
            get => _mainModel.SelectedProfile;
            set
            {
                _mainModel.SelectedProfile = value;
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

        public MainPageViewModel()
        {
            _foxConnector = App.Container.Resolve<IFoxConnector>();
            _pairedFoxesEnumerator = App.Container.Resolve<IPairedFoxesEnumerator>();
            _userNotifier = App.Container.Resolve<IUserNotifier>();
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _userRequestor = App.Container.Resolve<IUserRequestor>();
            _bluetoothManager = App.Container.Resolve<IBluetoothManager>();
            _foxProfilesEnumerator = App.Container.Resolve<IFoxProfilesEnumerator>();
            _foxProfileAdder = App.Container.Resolve<IFoxProfileAdder>();
            _setDateAndTimeCommand = App.Container.Resolve<ISetDateAndTimeCommand>();
            _getCurrentProfileIdCommand = App.Container.Resolve<IGetCurrentProfileIdCommand>();
            _foxProfileSwitcher = App.Container.Resolve<IFoxProfileSwitcher>();
            _foxIdentificationManager = App.Container.Resolve<IFoxIdentificationManager>();
            _foxNameManager = App.Container.Resolve<IFoxNameManager>();

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
            _setDateAndTimeCommand.SetResponseDelegate(async (isSuccessfull) => await OnSetFoxDateAndTimeResponseAsync(isSuccessfull));
            _setDateAndTimeCommand.SendSetDateAndTimeCommand(DateTime.Now);
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
            await _foxProfilesEnumerator.EnumerateProfilesAsync(OnFoxProfilesEnumerated);
        }

        private void OnFoxProfilesEnumerated(IReadOnlyCollection<Profile> profiles)
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

            await _foxProfileAdder.AddProfileAsync(profileNameData.Item2, OnFoxProfileAdded);
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

            _mainModel.SelectedProfile = selectedProfile;

            // Switching profile in the fox
            await _foxProfileSwitcher.SwitchProfileAsync(selectedProfile.Id, OnSelectedProfileChanged);
        }

        private void OnSelectedProfileChanged()
        {
            DetectActiveProfile();
        }

        private async Task OnGetCurrentProfileIdResponseAsync(int profileId)
        {
            SelectedProfile = Profiles
                .FirstOrDefault(p => p.Id == profileId);
        }

        private async Task OnRenameProfileClickedAsync()
        {
            // We are always renaming current profile, so there is no need to switch profile
        }

        private void DetectActiveProfile()
        {
            _getCurrentProfileIdCommand.SetResponseDelegate(async (profileId) => await OnGetCurrentProfileIdResponseAsync(profileId));
            _getCurrentProfileIdCommand.SendGetCurrentProfileIdCommand();
        }
    }
}
