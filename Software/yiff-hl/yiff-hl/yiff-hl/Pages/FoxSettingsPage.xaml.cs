using System;
using System.Collections.Generic;
using System.Linq;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands;

namespace yiff_hl.Pages
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class FoxSettingsPage : ContentPage
    {
        private readonly IBluetoothCommunicator bluetoothCommunicator;
        private readonly IPacketsProcessor packetsProcessor;

        public FoxSettingsPage(IBluetoothCommunicator bluetoothCommunicator,
            IPacketsProcessor packetsProcessor)
        {
            this.bluetoothCommunicator = bluetoothCommunicator;
            this.packetsProcessor = packetsProcessor;

            App.NewByteReadDelegate = packetsProcessor.NewByteReceived;

            InitializeComponent();
        }

        private void OnDisconnectClicked(object sender, EventArgs e)
        {
            Disconnect();
        }

        protected override bool OnBackButtonPressed()
        {
            Disconnect();
            return base.OnBackButtonPressed();
        }

        private void Disconnect()
        {
            bluetoothCommunicator.Disconnect();
            Navigation.PopModalAsync();
        }

        private void OnSetCurrentDateAndTimeClicked(object sender, EventArgs e)
        {
            SetCurrentDateAndTime();
        }

        private void SetCurrentDateAndTime()
        {
            var command = new SetDateAndTimeCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetCurrentDateAndTimeResponse);
            command.SendSetDateAndTimeCommand(DateTime.Now);
        }

        private void OnSetCurrentDateAndTimeResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Fox clock synchronized" : "Failed to synchronize fox clock";
                DisplayAlert("Clock synchronization", message, "OK");
            });
        }

        private void OnSetFoxNameClicked(object sender, EventArgs e)
        {
            SetFoxName();
        }

        private void SetFoxName()
        {
            var command = new SetFoxNameCommand(packetsProcessor);
            command.SetResponseDelegate(OnSetFoxNameResponse);
            command.SendSetFoxNameCommand(edFoxName.Text);
        }

        private void OnSetFoxNameResponse(bool isSuccess)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                var message = isSuccess ? "Fox name set" : "Failed to set fox name";
                DisplayAlert("Name change", message, "OK");
            });
        }

        private void OnGetFoxNameClicked(object sender, EventArgs e)
        {
            GetFoxName();
        }

        private void GetFoxName()
        {
            var command = new GetFoxNameCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetFoxNameResponse);
            command.SendGetFoxNameCommand();
        }

        private void OnGetFoxNameResponse(string name)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                DisplayAlert("Fox name", $"Fox name: {name}", "OK");
            });
        }

        private void OnGetProfilesCountClicked(object sender, EventArgs e)
        {
            GetProfilesCount();
        }

        private void GetProfilesCount()
        {
            var command = new GetProfilesCountCommand(packetsProcessor);
            command.SetResponseDelegate(OnGetProfilesCountResponse);
            command.SendGetProfilesCountCommand();
        }

        private void OnGetProfilesCountResponse(int count)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                DisplayAlert("Profiles count", $"Profiles count: {count}", "OK");
            });
        }
    }
}