using Xamarin.Forms;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Pages
{
    public partial class MainPage : ContentPage
    {
        public MainPage(IBluetoothDevicesLister bluetoothDevicesLister,
            IBluetoothCommunicator bluetoothCommunicator,
            IPacketsProcessor packetsProcessor,
            IGenericCommandWriter genericCommandWriter)
        {
            InitializeComponent();

            var connectToFoxPage = new ConnectToFoxPage(bluetoothDevicesLister,
                bluetoothCommunicator);

            var foxSettingsPage = new FoxSettingsPage(bluetoothCommunicator,
                packetsProcessor,
                genericCommandWriter);

            btnConnectToFox.Clicked += (s, e) => Navigation.PushModalAsync(connectToFoxPage);
            btnFoxSettings.Clicked += (s, e) => Navigation.PushModalAsync(foxSettingsPage);
        }
    }
}
