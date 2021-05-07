using System;
using System.Threading;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Data;

namespace yiff_hl.Pages
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ConnectToFoxPage : ContentPage
    {
        private readonly IBluetoothDevicesLister bluetoothDevicesLister;

        private ConnectToFoxModel connectToFoxModel = new ConnectToFoxModel();

        public ConnectToFoxPage(IBluetoothDevicesLister bluetoothDevicesLister,
            IBluetoothCommunicator bluetoothCommunicator)
        {
            this.bluetoothDevicesLister = bluetoothDevicesLister;
            connectToFoxModel.BluetoothCommunicator = bluetoothCommunicator;

            InitializeComponent();

            BindingContext = connectToFoxModel;

            UpdateDevicesList();
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

        public async void OnConnectClicked(object sender, EventArgs args)
        {
            var deviceName = ((BluetoothDevice)((Button)sender).CommandParameter).Name;
            connectToFoxModel.SelectedDevice = deviceName;

            await Navigation.PopModalAsync();

            // Connecting
            connectToFoxModel.BluetoothCommunicator.SetDeviceName(connectToFoxModel.SelectedDevice);
            connectToFoxModel.BluetoothCommunicator.SetReadDelegate(OnNewByte);
            var communicatorThread = new Thread(new ThreadStart(connectToFoxModel.BluetoothCommunicator.Connect));
            communicatorThread.Start();
        }
        public void OnNewByte(byte data)
        {
            int a = 10;
        }
    }
}