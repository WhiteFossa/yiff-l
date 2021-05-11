using System;
using System.Collections.Generic;
using System.Linq;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using yiff_hl.Abstractions.Interfaces;

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
            packetsProcessor.SetNewPacketReceived(OnNewPacketReceived);

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

        private void OnSendMessageClicked(object sender, EventArgs e)
        {
            var message = "Yiff yiff yerf!!!"
                .ToCharArray()
                .Select(ch => (byte)ch)
                .ToList();

            packetsProcessor.SendPacket(message);
        }

        private void OnNewPacketReceived(IReadOnlyCollection<byte> payload)
        {
            var message = new string(payload
                .Select(b => (char)b)
                .ToArray());

            Device.BeginInvokeOnMainThread(() =>
            {
                edMessagesFromFox.Text += message;
            });
        }
    }
}