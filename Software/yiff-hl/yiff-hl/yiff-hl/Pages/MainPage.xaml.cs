using Xamarin.Forms;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Pages
{
    public partial class MainPage : ContentPage
    {
        public MainPage(IBluetoothDevicesLister bluetoothDevicesLister,
            IBluetoothCommunicator bluetoothCommunicator)
        {
            InitializeComponent();

            btnConnectToFox.Clicked += (s, e) => Navigation.PushModalAsync(new ConnectToFoxPage(bluetoothDevicesLister, bluetoothCommunicator));

            btnFoxSettings.Clicked += (s, e) => Navigation.PushModalAsync(new FoxSettingsPage(bluetoothCommunicator));
        }
    }
}
