using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Models;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows.Input;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.ViewModels
{
    internal class MainPageViewModel : BindableObject
    {
        private IFoxConnector _foxConnector;

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
            get => _pairedFoxes;
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

        public MainPageViewModel()
        {
            _foxConnector = App.Container.Resolve<IFoxConnector>();

            // Model initialization
            _mainModel = new MainModel();

            // Binding commands to handlers
            SelectedFoxChangedCommand = new Command<PairedFoxDTO>(async (f) => await OnSelectedFoxChangedAsync(f));
            ConnectButtonClickedCommand = new Command(async () => await OnConnectButtonCLickedAsync());

            // Initial state
            IsBtnConnectEnabled = false;

            _pairedFoxes.Add(new PairedFoxDTO("Yiffy foxy", "11:22:33:44:55"));
            _pairedFoxes.Add(new PairedFoxDTO("Yerfy foxy", "11:22:33:44:55"));
            _pairedFoxes.Add(new PairedFoxDTO("Yuffy foxy", "11:22:33:44:55"));
        }

        public Task OnSelectedFoxChangedAsync(PairedFoxDTO selectedFox)
        {
            IsBtnConnectEnabled = selectedFox != null;

            return Task.CompletedTask;
        }

        public Task OnConnectButtonCLickedAsync()
        {
            return _foxConnector.ConnectAsync(_mainModel, SelectedFox);
        }
    }
}
