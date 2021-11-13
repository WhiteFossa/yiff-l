using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Collections.ObjectModel;
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
            get { return _pairedFoxes; }
            set { _pairedFoxes = value; }
        }

        public MainPageViewModel()
        {
            _pairedFoxes.Add(new PairedFoxDTO("Yiffy foxy", "11:22:33:44:55"));
            _pairedFoxes.Add(new PairedFoxDTO("Yerfy foxy", "11:22:33:44:55"));
            _pairedFoxes.Add(new PairedFoxDTO("Yuffy foxy", "11:22:33:44:55"));
        }
    }
}
