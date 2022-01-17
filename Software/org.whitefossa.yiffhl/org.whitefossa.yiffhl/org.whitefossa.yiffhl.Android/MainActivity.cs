
using Acr.UserDialogs;
using Android.App;
using Android.Content.PM;
using Android.OS;
using Android.Runtime;
using Nancy.TinyIoc;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Models;
using org.whitefossa.yiffhl.Business.Implementations;
using org.whitefossa.yiffhl.Business.Implementations.Commands;
using org.whitefossa.yiffhl.Business.Implementations.PacketsProcessor;
using org.whitefossa.yiffhl.Droid.Business.Implementations;
using org.whitefossa.yiffhl.Models;

namespace org.whitefossa.yiffhl.Droid
{
    [Activity(Label = "Project YIFF: HL", Icon = "@mipmap/icon", Theme = "@style/MainTheme", MainLauncher = true, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize )]
    public class MainActivity : global::Xamarin.Forms.Platform.Android.FormsAppCompatActivity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            // Registering IoC stuff
            App.Container = new TinyIoCContainer();
            App.Container.Register<IMainModel, MainModel>().AsSingleton();
            App.Container.Register<IFoxConnector, FoxConnector>().AsSingleton();
            App.Container.Register<IPairedFoxesEnumerator, PairedFoxesEnumerator>().AsSingleton();
            App.Container.Register<IUserNotifier, UserNotifier>().AsSingleton();
            App.Container.Register<IBluetoothCommunicator, BluetoothCommunicator>().AsSingleton();
            App.Container.Register<IPacketsProcessor, PacketsProcessor>().AsSingleton();
            App.Container.Register<IUserRequestor, UserRequestor>().AsSingleton();
            App.Container.Register<IBluetoothManager, BluetoothManager>().AsSingleton();
            App.Container.Register<IFoxProfilesEnumerator, FoxProfilesEnumerator>().AsSingleton();
            App.Container.Register<IFoxProfilesManager, FoxProfilesManager>().AsSingleton();
            App.Container.Register<IFoxIdentificationManager, FoxIdentificationManager>().AsSingleton();
            App.Container.Register<IFoxNameManager, FoxNameManager>().AsSingleton();
            App.Container.Register<IFoxClockManager, FoxClockManager>().AsSingleton();
            App.Container.Register<IProfileSettingsManager, ProfileSettingsManager>().AsSingleton();
            App.Container.Register<IDynamicFoxStatusManager, DynamicFoxStatusManager>().AsSingleton();
            App.Container.Register<IStaticFoxStatusManager, StaticFoxStatusManager>().AsSingleton();
            App.Container.Register<IColorsFactory, ColorsFactory>().AsSingleton();
            App.Container.Register<IAntennaMatchingManager, AntennaMatchingManager>().AsSingleton();
            App.Container.Register<IEventsGenerator, EventsGenerator>().AsSingleton();
            App.Container.Register<IServiceCommandsManager, ServiceCommandsManager>().AsSingleton();

            #region Commands

            App.Container.Register<IAddNewProfileCommand, AddNewProfileCommand>().AsSingleton();
            App.Container.Register<IGetIdentificationDataCommand, GetIdentificationDataCommand>().AsSingleton();
            App.Container.Register<IGetFoxNameCommand, GetFoxNameCommand>().AsSingleton();
            App.Container.Register<ISetDateAndTimeCommand, SetDateAndTimeCommand>().AsSingleton();
            App.Container.Register<ISetFoxNameCommand, SetFoxNameCommand>().AsSingleton();
            App.Container.Register<IGetProfilesCountCommand, GetProfilesCountCommand>().AsSingleton();
            App.Container.Register<IGetProfileNameCommand, GetProfileNameCommand>().AsSingleton();
            App.Container.Register<ISetProfileNameCommand, SetProfileNameCommand>().AsSingleton();
            App.Container.Register<ISwitchToProfileCommand, SwitchToProfileCommand>().AsSingleton();
            App.Container.Register<IGetCurrentProfileIdCommand, GetCurrentProfileIdCommand>().AsSingleton();
            App.Container.Register<IGetFrequencyCommand, GetFrequencyCommand>().AsSingleton();
            App.Container.Register<ISetFrequencyCommand, SetFrequencyCommand>().AsSingleton();
            App.Container.Register<IGetCodeCommand, GetCodeCommand>().AsSingleton();
            App.Container.Register<IGetSpeedCommand, GetSpeedCommand>().AsSingleton();
            App.Container.Register<ISetSpeedCommand, SetSpeedCommand>().AsSingleton();
            App.Container.Register<ISetCodeCommand, SetCodeCommand>().AsSingleton();
            App.Container.Register<IGetCycleCommand, GetCycleCommand>().AsSingleton();
            App.Container.Register<ISetCycleCommand, SetCycleCommand>().AsSingleton();
            App.Container.Register<IGetEndingToneDurationCommand, GetEndingToneDurationCommand>().AsSingleton();
            App.Container.Register<ISetEndingToneDurationCommand, SetEndingToneDurationCommand>().AsSingleton();
            App.Container.Register<IGetBeginAndEndTimesCommand, GetBeginAndEndTimesCommand>().AsSingleton();
            App.Container.Register<ISetBeginAndEndTimesCommand, SetBeginAndEndTimesCommand>().AsSingleton();
            App.Container.Register<IGetPowerCommand, GetPowerCommand>().AsSingleton();
            App.Container.Register<ISetPowerCommand, SetPowerCommand>().AsSingleton();
            App.Container.Register<IGetBatteryLevelCommand, GetBatteryLevelCommand>().AsSingleton();
            App.Container.Register<IIsFoxArmedCommand, IsFoxArmedCommand>().AsSingleton();
            App.Container.Register<IArmFoxCommand, ArmFoxCommand>().AsSingleton();
            App.Container.Register<IDisarmFoxCommand, DisarmFoxCommand>().AsSingleton();
            App.Container.Register<IGetAntennaMatchingStatusCommand, GetAntennaMatchingStatusCommand>().AsSingleton();
            App.Container.Register<IMarkMatchingAsSeenCommand, MarkMatchingAsSeenCommand>().AsSingleton();
            App.Container.Register<IGetAntennaMatchingDataCommand, GetAntennaMatchingDataCommand>().AsSingleton();
            App.Container.Register<ICheckForProfileSettingsChangesCommand, CheckForProfileSettingsChangesCommand>().AsSingleton();
            App.Container.Register<IGetLastErrorCodeCommand, GetLastErrorCodeCommand>().AsSingleton();
            App.Container.Register<IResetLastErrorCodeCommand, ResetLastErrorCodeCommand>().AsSingleton();
            App.Container.Register<IUpdateSerialNumberCommand, UpdateSerialNumberCommand>().AsSingleton();
            App.Container.Register<IGetUbattADCCommand, GetUbattADCCommand>().AsSingleton();
            App.Container.Register<IGetUbattVoltsCommand, GetUbattVoltsCommand>().AsSingleton();

            #endregion

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