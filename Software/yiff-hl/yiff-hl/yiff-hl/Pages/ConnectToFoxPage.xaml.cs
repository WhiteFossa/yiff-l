using System;
using System.Linq;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using yiff_hl.Abstractions;
using yiff_hl.Data;

namespace yiff_hl.Pages
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ConnectToFoxPage : ContentPage
    {
        private readonly IBluetoothDevicesLister bluetoothDevicesLister;

        private ConnectToFoxModel connectToFoxModel = new ConnectToFoxModel();

        public ConnectToFoxPage(IBluetoothDevicesLister bluetoothDevicesLister)
        {
            this.bluetoothDevicesLister = bluetoothDevicesLister;

            InitializeComponent();

            BindingContext = connectToFoxModel;

            UpdateDevicesList();
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
            UpdateDevicesList();
        }

        public void UpdateDevicesList()
        {
            var devices = bluetoothDevicesLister.ListPairedDevices();

            connectToFoxModel.BluetoothDevices.Clear();
            foreach (var device in devices)
            {
                connectToFoxModel.BluetoothDevices.Add(new BluetoothDevice() { Name = device.Name, MAC = device.MAC });
            }
        }
    }
}