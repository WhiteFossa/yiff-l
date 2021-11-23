
using Acr.UserDialogs;
using Android.App;
using Android.Content.PM;
using Android.OS;
using Android.Runtime;
using Nancy.TinyIoc;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Business.Implementations;
using org.whitefossa.yiffhl.Droid.Business.Implementations;

namespace org.whitefossa.yiffhl.Droid
{
    [Activity(Label = "Project YIFF: HL", Icon = "@mipmap/icon", Theme = "@style/MainTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize )]
    public class MainActivity : global::Xamarin.Forms.Platform.Android.FormsAppCompatActivity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            // Registering IoC stuff
            App.Container = new TinyIoCContainer();
            App.Container.Register<IFoxConnector, FoxConnector>().AsSingleton();
            App.Container.Register<IPairedFoxesEnumerator, PairedFoxesEnumerator>().AsSingleton();
            App.Container.Register<IUserNotifier, UserNotifier>().AsSingleton();
            App.Container.Register<IBluetoothCommunicator, BluetoothCommunicator>().AsSingleton();
            App.Container.Register<IPacketsProcessor, PacketsProcessor>().AsSingleton();
            App.Container.Register<IUserRequestor, UserRequestor>().AsSingleton();
            App.Container.Register<IBluetoothManager, BluetoothManager>().AsSingleton();
            App.Container.Register<IFoxProfilesEnumerator, FoxProfilesEnumerator>().AsSingleton();
            App.Container.Register<IFoxProfileAdder, FoxProfileAdder>().AsSingleton();

            base.OnCreate(savedInstanceState);

            Xamarin.Essentials.Platform.Init(this, savedInstanceState);
            global::Xamarin.Forms.Forms.Init(this, savedInstanceState);
            LoadApplication(new App());

            UserDialogs.Init(this);
        }
        public override void OnRequestPermissionsResult(int requestCode, string[] permissions, [GeneratedEnum] Android.Content.PM.Permission[] grantResults)
        {
            Xamarin.Essentials.Platform.OnRequestPermissionsResult(requestCode, permissions, grantResults);

            base.OnRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }
}