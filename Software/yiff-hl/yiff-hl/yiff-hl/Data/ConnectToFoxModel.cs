using System.Collections.ObjectModel;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Data
{
    /// <summary>
    /// Model for Connect to fox page
    /// </summary>
    public class ConnectToFoxModel
    {
        private ObservableCollection<BluetoothDevice> bluetoothDevices = new ObservableCollection<BluetoothDevice>();

        public ObservableCollection<BluetoothDevice> BluetoothDevices { get { return bluetoothDevices; } }

        /// <summary>
        /// Name of selected bluetooth device
        /// </summary>
        public string SelectedDevice { get; set; }

        /// <summary>
        /// Bluetooth communicator, it will live here, at least temporarily
        /// </summary>
        public IBluetoothCommunicator BluetoothCommunicator { get; set; }
    }
}
