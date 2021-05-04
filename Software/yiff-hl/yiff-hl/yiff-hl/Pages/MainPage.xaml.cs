using Xamarin.Forms;
using yiff_hl.Abstractions;

namespace yiff_hl.Pages
{
    public partial class MainPage : ContentPage
    {
        public MainPage(IBluetoothDevicesLister bluetoothDevicesLister)
        {
            InitializeComponent();

            btnConnectToFox.Clicked += (s, e) => Navigation.PushModalAsync(new ConnectToFoxPage(bluetoothDevicesLister));

            btnFoxSettings.Clicked += (s, e) => Navigation.PushModalAsync(new FoxSettingsPage());
        }
    }
}
