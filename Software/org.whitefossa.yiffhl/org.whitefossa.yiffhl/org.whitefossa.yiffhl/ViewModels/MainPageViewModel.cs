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
        private IFoxConnector _foxConnector;
        private IPairedFoxesEnumerator _pairedFoxesEnumerator;
        private IUserNotifier _userNotifier;
        private IBluetoothCommunicator _bluetoothCommunicator;

        /// <summary>
        /// Main model
        /// </summary>
        private MainModel _mainModel;

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

        public MainPageViewModel()
        {
            _foxConnector = App.Container.Resolve<IFoxConnector>();
            _pairedFoxesEnumerator = App.Container.Resolve<IPairedFoxesEnumerator>();
            _userNotifier = App.Container.Resolve<IUserNotifier>();
            _bluetoothCommunicator = App.Container.Resolve<IBluetoothCommunicator>();

            // Model initialization
            _mainModel = new MainModel();

            // Setting up BT communicator delegates
            _mainModel.OnBTCommunicatorNewByteRead += OnNewByteRead;
            _mainModel.OnBTCommunicatorConnect += OnConnect;
            _mainModel.OnBTCommunicatorDisconnect += OnDisconnect;

            _bluetoothCommunicator.SetupDelegates
             (
                _mainModel.OnBTCommunicatorConnect,
                _mainModel.OnBTCommunicatorDisconnect,
                _mainModel.OnBTCommunicatorNewByteRead
             );

            // Binding commands to handlers
            SelectedFoxChangedCommand = new Command<PairedFoxDTO>(async (f) => await OnSelectedFoxChangedAsync(f));
            ConnectButtonClickedCommand = new Command(async () => await OnConnectButtonCLickedAsync());
            RefreshFoxesListClickedCommand = new Command(async () => await OnRefreshFoxesListClickedAsync());
            DisconnectButtonClickedCommand = new Command(async () => await OnDisconnectButtonClickedAsync());

            // Initial state
            IsConnectRelatedControlsEnabled = true;
            IsBtnDisconnectEnabled = false;
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
            Debug.WriteLine($"Byte: {data}");
        }

        private void OnConnect(PairedFoxDTO connectedFox)
        {
            _mainModel.ConnectedFox = connectedFox;
            _mainModel.IsConnected = true;

            IsBtnDisconnectEnabled = true;
        }

        private void OnDisconnect()
        {
            _mainModel.IsConnected = false;
            _mainModel.ConnectedFox = null;

            IsConnectRelatedControlsEnabled = true;
        }
    }
}
