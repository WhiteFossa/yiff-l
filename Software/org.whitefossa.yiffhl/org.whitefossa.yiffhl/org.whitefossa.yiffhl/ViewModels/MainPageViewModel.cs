using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
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

        public MainPageViewModel()
        {
            _foxConnector = App.Container.Resolve<IFoxConnector>();
            _pairedFoxesEnumerator = App.Container.Resolve<IPairedFoxesEnumerator>();
            _userNotifier = App.Container.Resolve<IUserNotifier>();
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();

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

            // Setting up fox commands
            _mainModel.GetIdentificationDataCommand.SetResponseDelegate(async (isFox, pVer, hwRev, fwVer, sn)
                => await OnGetIdentificationDataResponseAsync(isFox, pVer, hwRev, fwVer, sn));

            // Initial state
            IsConnectRelatedControlsEnabled = true;
            IsBtnDisconnectEnabled = false;

            _mainModel.FoxName = "N/A";
            _mainModel.IdentificationData.HardwareRevision = 0;
            _mainModel.IdentificationData.FirmwareVersion = 0;
            _mainModel.IdentificationData.SerialNumber = 0;
        }

        public async Task OnSelectedFoxChangedAsync(PairedFoxDTO selectedFox)
        {
            IsConnectRelatedControlsEnabled = selectedFox != null && !_mainModel.IsConnected;
        }

        public async Task OnConnectButtonCLickedAsync()
        {
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

        private bool CalculateBtnConnectEnabledState()
        {
            return SelectedFox != null && !_mainModel.IsConnected;
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
            _mainModel.IsConnected = true;

            IsBtnDisconnectEnabled = true;

            // Requesting fox identification
            _mainModel.GetIdentificationDataCommand.SendGetIdentificationDataCommand();
        }

        private void OnDisconnected()
        {
            _mainModel.IsConnected = false;
            _mainModel.ConnectedFox = null;

            IsConnectRelatedControlsEnabled = true;
        }

        private void OnFailedToConnect(Exception exception)
        {
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
            }

            if (protocolVersion != SupportedProtocolVersion)
            {
                await OnDisconnectButtonClickedAsync();

                await _userNotifier.ShowErrorMessageAsync("Error:",
                    $"Unsupported fox protocol version. Got { protocolVersion } while expecting { SupportedProtocolVersion }");
            }

            FoxHardwareRevision = hardwareRevision;
            FoxFirmwareVersion = firmwareVersion;
            FoxSerialNumber = serialNumber;
        }
    }
}
