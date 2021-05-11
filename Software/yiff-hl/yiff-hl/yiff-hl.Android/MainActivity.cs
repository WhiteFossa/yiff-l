
using Android.App;
using Android.Content.PM;
using Android.OS;
using Android.Runtime;
using Nancy.TinyIoc;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations;
using yiff_hl.Droid.Implementations;

namespace yiff_hl.Droid
{
    [Activity(Label = "yiff_hl", Icon = "@mipmap/icon", Theme = "@style/MainTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize )]
    public class MainActivity : global::Xamarin.Forms.Platform.Android.FormsAppCompatActivity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            // Registering IoC stuff
            App.Container = new TinyIoCContainer();
            App.Container.Register<IBluetoothDevicesLister, BluetoothDevicesLister>();
            App.Container.Register<IBluetoothCommunicator, BluetoothCommunicator>();
            App.Container.Register<IPacketsProcessor, PacketsProcessor>();
            App.Container.Register<IGenericCommandWriter, GenericCommandWriter>();

            base.OnCreate(savedInstanceState);

            Xamarin.Essentials.Platform.Init(this, savedInstanceState);
            global::Xamarin.Forms.Forms.Init(this, savedInstanceState);
            LoadApplication(new App());
        }
        public override void OnRequestPermissionsResult(int requestCode, string[] permissions, [GeneratedEnum] Android.Content.PM.Permission[] grantResults)
        {
            Xamarin.Essentials.Platform.OnRequestPermissionsResult(requestCode, permissions, grantResults);

            base.OnRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }
}