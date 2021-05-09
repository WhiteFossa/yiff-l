using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

            App.NewByteReadDelegate = OnNewByteReceived;

            InitializeComponent();
        }

        private void OnDisconnectClicked(object sender, EventArgs e)
        {
            bluetoothCommunicator.Disconnect();
        }

        private void OnSendMessageClicked(object sender, EventArgs e)
        {
            var message = "Yiffy yiff yerf"
                .ToCharArray()
                .Select(ch => (byte)ch)
                .ToList();

            packetsProcessor.SendPacket(message);
        }

        public void OnNewByteReceived(byte data)
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                edMessagesFromFox.Text += ((char)data).ToString();
            });
        }
    }
}