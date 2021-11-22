using Acr.UserDialogs;
using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Business.Implementations.Commands;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Threading;
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

        private IFoxConnector _foxConnector;
        private IPairedFoxesEnumerator _pairedFoxesEnumerator;
        private IUserNotifier _userNotifier;
        private IPacketsProcessor _packetsProcessor;
        private IUserRequestor _userRequestor;
        private IBluetoothManager _bluetoothManager;

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
                return EnumerateFoxesAsync().Result;
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
        public UInt16 FoxHardwareRevision
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
        public UInt16 FoxFirmwareVersion
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
        public UInt32 FoxSerialNumber
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

        public MainPageViewModel()
        {
            _foxConnector = App.Container.Resolve<IFoxConnector>();
            _pairedFoxesEnumerator = App.Container.Resolve<IPairedFoxesEnumerator>();
            _userNotifier = App.Container.Resolve<IUserNotifier>();
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _userRequestor = App.Container.Resolve<IUserRequestor>();
            _bluetoothManager = App.Container.Resolve<IBluetoothManager>();

            // Setting up fox connector delegates
            _mainModel.OnFoxConnectorNewByteRead += OnNewByteRead;
            _mainModel.OnFoxConnectorConnected += OnConnected;
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
            RenameFoxClickedCommand = new Command(async() => await OnRenameFoxClickedCommandAsync());

            // Setting up fox commands
            _mainModel.GetIdentificationDataCommand.SetResponseDelegate(async (isFox, pVer, hwRev, fwVer, sn)
                => await OnGetIdentificationDataResponseAsync(isFox, pVer, hwRev, fwVer, sn));

            _mainModel.GetFoxNameCommand.SetResponseDelegate(async (name) => await OnGetFoxNameResponseAsync(name));

            _mainModel.SetFoxDateAndTimeCommand.SetResponseDelegate(async(isSuccessfull) => await OnSetFoxDateAndTimeResponseAsync(isSuccessfull));

            _mainModel.SetFoxNameCommand.SetResponseDelegate(async (isSuccessfull) => await OnSetFoxNameResponse(isSuccessfull));

            // Initial state
            IsConnectButtonEnabled = false;
            IsFoxPickerEnabled = true;
            IsConnectRelatedControlsEnabled = true;
            IsBtnDisconnectEnabled = false;

            ResetFoxRelatedData();
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

        private void OnConnected(PairedFoxDTO connectedFox)
        {
            _mainModel.ConnectedFox = connectedFox;
            IsFoxConnected = true;

            IsBtnDisconnectEnabled = true;

            // Requesting fox identification
            _mainModel.GetIdentificationDataCommand.SendGetIdentificationDataCommand();
        }

        private void OnDisconnected()
        {
            IsFoxConnected = false;
            _mainModel.ConnectedFox = null;

            ResetFoxRelatedData();

            IsConnectButtonEnabled = true;
            IsFoxPickerEnabled = true;
            IsConnectRelatedControlsEnabled = true;
        }

        private void OnFailedToConnect(Exception exception)
        {
            IsConnectButtonEnabled = true;
            IsFoxPickerEnabled = true;
            IsConnectRelatedControlsEnabled = true;
        }

        /// <summary>
        /// Called when fox identification is received
        /// </summary>
        private async Task OnGetIdentificationDataResponseAsync
        (
            bool isFox,
            UInt16 protocolVersion,
            UInt16 hardwareRevision,
            UInt16 firmwareVersion,
            UInt32 serialNumber
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
            _mainModel.SetFoxDateAndTimeCommand.SendSetDateAndTimeCommand(DateTime.Now);
        }

        /// <summary>
        /// Called when fox name is received
        /// </summary>
        private async Task OnGetFoxNameResponseAsync(string name)
        {
            FoxName = name;
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
            _mainModel.GetFoxNameCommand.SendGetFoxNameCommand();
        }

        private async Task OnRenameFoxClickedCommandAsync()
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

            _mainModel.SetFoxNameCommand.SendSetFoxNameCommand(newNameData.Item2);
        }

        private async Task OnSetFoxNameResponse(bool isSuccessfull)
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
    }
}
