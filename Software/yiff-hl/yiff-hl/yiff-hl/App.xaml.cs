using Nancy.TinyIoc;
using Xamarin.Forms;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Pages;

namespace yiff_hl
{
    public partial class App : Application
    {
        public static TinyIoCContainer Container;

        public App()
        {
            InitializeComponent();

            MainPage = new NavigationPage(new MainPage(Container.Resolve<IBluetoothDevicesLister>(),
                Container.Resolve<IBluetoothCommunicator>()));
        }

        protected override void OnStart()
        {
        }

        protected override void OnSleep()
        {
        }

        protected override void OnResume()
        {
        }
    }
}
