using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Models;
using System;
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
        private IFoxConnector _foxConnector;
        private IPairedFoxesEnumerator _pairedFoxesEnumerator;
        private IUserNotifier _userNotifier;

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
        /// True if Connect button is enabled
        /// </summary>
        private bool _isBtnConnectEnabled;

        public bool IsBtnConnectEnabled
        {
            get => _isBtnConnectEnabled;
            set
            {
                _isBtnConnectEnabled = value;
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
        /// Is Refresh Foxes List button enabled
        /// </summary>
        public bool IsRefreshFoxesListButtonEnabled
        {
            get => !_mainModel.IsConnected;
        }

        /// <summary>
        /// Is Select Fox picker enabled
        /// </summary>
        public bool IsFoxSelectorEnabled
        {
            get => !_mainModel.IsConnected;
        }

        /// <summary>
        /// Is Disconnect button enabled
        /// </summary>
        public bool IsBtnDisconnectEnabled
        {
            get => _mainModel.IsConnected;
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

            // Model initialization
            _mainModel = new MainModel();

            // Binding commands to handlers
            SelectedFoxChangedCommand = new Command<PairedFoxDTO>(async (f) => await OnSelectedFoxChangedAsync(f));
            ConnectButtonClickedCommand = new Command(async () => await OnConnectButtonCLickedAsync());
            RefreshFoxesListClickedCommand = new Command(async () => await OnRefreshFoxesListClickedAsync());
            DisconnectButtonClickedCommand = new Command(async () => await OnDisconnectButtonClickedAsync());

            // Initial state
            IsBtnConnectEnabled = false;
        }

        public async Task OnSelectedFoxChangedAsync(PairedFoxDTO selectedFox)
        {
            IsBtnConnectEnabled = selectedFox != null && !_mainModel.IsConnected;
        }

        public async Task OnConnectButtonCLickedAsync()
        {
            await _foxConnector.ConnectAsync(_mainModel, SelectedFox);

            IsBtnConnectEnabled = CalculateBtnConnectEnabledState();

            OnPropertyChanged(nameof(IsRefreshFoxesListButtonEnabled));
            OnPropertyChanged(nameof(IsFoxSelectorEnabled));
            OnPropertyChanged(nameof(IsBtnConnectEnabled));
            OnPropertyChanged(nameof(IsBtnDisconnectEnabled));
        }

        public async Task OnRefreshFoxesListClickedAsync()
        {
            PairedFoxes = await EnumerateFoxesAsync();
        }

        public async Task OnDisconnectButtonClickedAsync()
        {
            await _foxConnector.DisconnectAsync(_mainModel);

            IsBtnConnectEnabled = CalculateBtnConnectEnabledState();

            OnPropertyChanged(nameof(IsRefreshFoxesListButtonEnabled));
            OnPropertyChanged(nameof(IsFoxSelectorEnabled));
            OnPropertyChanged(nameof(IsBtnConnectEnabled));
            OnPropertyChanged(nameof(IsBtnDisconnectEnabled));
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
    }
}
