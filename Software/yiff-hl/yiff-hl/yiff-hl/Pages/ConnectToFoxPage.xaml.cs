using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using yiff_hl.Data;

namespace yiff_hl.Pages
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ConnectToFoxPage : ContentPage
    {
        private ObservableCollection<BluetoothDevice> bluetoothDevices = new ObservableCollection<BluetoothDevice>();

        public ObservableCollection<BluetoothDevice> BluetoothDevices { get { return bluetoothDevices; } }

        public ConnectToFoxPage()
        {
            InitializeComponent();

            BindingContext = this;
        }

        public async void OnOkClicked(object sender, EventArgs args)
        {
            await Navigation.PopModalAsync();
        }

        public async void OnCancelClicked(object sender, EventArgs args)
        {
            await Navigation.PopModalAsync();
        }

        protected override bool OnBackButtonPressed()
        {
            return base.OnBackButtonPressed();
        }

        public async void OnRefreshClicked(object sender, EventArgs args)
        {
            bluetoothDevices.Add(new BluetoothDevice() { Name = "Test device", MAC ="00:01:02:03:04:05" });
        }
    }
}