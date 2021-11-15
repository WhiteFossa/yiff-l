using Nancy.TinyIoc;
using org.whitefossa.yiffhl.Views;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl
{
    public partial class App : Application
    {
        public static TinyIoCContainer Container;

        public App()
        {
            InitializeComponent();

            MainPage = new MainPageView();
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
