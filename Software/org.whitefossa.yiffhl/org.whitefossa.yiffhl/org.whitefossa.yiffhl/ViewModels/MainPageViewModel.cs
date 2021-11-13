using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Collections.ObjectModel;
using System.Windows.Input;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.ViewModels
{
    internal class MainPageViewModel : BindableObject
    {
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

        public MainPageViewModel()
        {
            // Binding commands to handlers
            SelectedFoxChangedCommand = new Command<PairedFoxDTO>(OnSelectedFoxChanged);

            // Initial state
            IsBtnConnectEnabled = false;

            _pairedFoxes.Add(new PairedFoxDTO("Yiffy foxy", "11:22:33:44:55"));
            _pairedFoxes.Add(new PairedFoxDTO("Yerfy foxy", "11:22:33:44:55"));
            _pairedFoxes.Add(new PairedFoxDTO("Yuffy foxy", "11:22:33:44:55"));

            //_selectedFox = _pairedFoxes.First();
        }

        public void OnSelectedFoxChanged(PairedFoxDTO selectedFox)
        {
            IsBtnConnectEnabled = selectedFox != null;
        }
    }
}
