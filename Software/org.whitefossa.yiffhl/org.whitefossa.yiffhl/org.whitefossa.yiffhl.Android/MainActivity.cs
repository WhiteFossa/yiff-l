using System;

using Android.App;
using Android.Content.PM;
using Android.Runtime;
using Android.OS;
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