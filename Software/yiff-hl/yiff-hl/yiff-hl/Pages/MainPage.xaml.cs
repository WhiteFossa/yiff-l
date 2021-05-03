using Xamarin.Forms;

namespace yiff_hl.Pages
{
    public partial class MainPage : ContentPage
    {
        public MainPage()
        {
            InitializeComponent();

            btnConnectToFox.Clicked += (s, e) => Navigation.PushModalAsync(new ConnectToFoxPage());

            btnFoxSettings.Clicked += (s, e) => Navigation.PushModalAsync(new FoxSettingsPage());
        }
    }
}
